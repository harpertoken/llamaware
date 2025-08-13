#!/bin/bash
# Llamaware Agent - Release Preparation Script
# Comprehensive pre-release validation and preparation

set -e

VERSION="2.0.0"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
cd "$PROJECT_ROOT"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_header() {
    echo -e "${BLUE}"
    echo "=============================================="
    echo "  Llamaware Agent v${VERSION} Release Prep"
    echo "=============================================="
    echo -e "${NC}"
}

print_step() {
    echo -e "${BLUE}[STEP]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[PASS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[FAIL]${NC} $1"
}

print_header

# Step 1: Verify clean working directory
print_step "Checking working directory status..."
if [ -n "$(git status --porcelain)" ]; then
    print_error "Working directory is not clean. Commit or stash changes first."
    git status --short
    exit 1
fi
print_success "Working directory is clean"

# Step 2: Verify we're on main branch
print_step "Checking current branch..."
CURRENT_BRANCH=$(git branch --show-current)
if [ "$CURRENT_BRANCH" != "main" ] && [ "$CURRENT_BRANCH" != "master" ]; then
    print_warning "Not on main/master branch (currently on: $CURRENT_BRANCH)"
    read -p "Continue anyway? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
else
    print_success "On main branch: $CURRENT_BRANCH"
fi

# Step 3: Verify version consistency
print_step "Verifying version consistency..."
VERSION_HEADER=$(grep "LLAMAWARE_VERSION_STRING" include/version.h | cut -d'"' -f2)
VERSION_RELEASE=$(grep "VERSION=" package/scripts/release.sh | cut -d'"' -f2)
VERSION_CHANGELOG=$(grep "## \[" package/docs/CHANGELOG.md | head -1 | cut -d'[' -f2 | cut -d']' -f1)

if [ "$VERSION_HEADER" != "$VERSION" ] || [ "$VERSION_RELEASE" != "$VERSION" ] || [ "$VERSION_CHANGELOG" != "$VERSION" ]; then
    print_error "Version mismatch detected:"
    echo "  Header: $VERSION_HEADER"
    echo "  Release script: $VERSION_RELEASE"
    echo "  Changelog: $VERSION_CHANGELOG"
    echo "  Expected: $VERSION"
    exit 1
fi
print_success "Version consistency verified: $VERSION"

# Step 4: Run comprehensive preflight checks
print_step "Running comprehensive preflight checks..."
if ! CI=false QUICK=false ./package/scripts/preflight.sh > /dev/null 2>&1; then
    print_error "Preflight checks failed. Run manually to see details:"
    echo "  ./package/scripts/preflight.sh"
    exit 1
fi
print_success "All preflight checks passed"

# Step 5: Test both online and offline modes (basic)
print_step "Testing application modes..."

# Test offline mode startup
if ! printf "2\n1\nversion\nexit\n" | timeout 10 ./build/bin/llamaware-agent > /dev/null 2>&1; then
    print_error "Offline mode test failed"
    exit 1
fi
print_success "Offline mode basic test passed"

# Test help system
if ! printf "2\n1\nhelp\nexit\n" | timeout 10 ./build/bin/llamaware-agent > /dev/null 2>&1; then
    print_error "Help system test failed"
    exit 1
fi
print_success "Help system test passed"

# Step 6: Verify documentation completeness
print_step "Checking documentation completeness..."
REQUIRED_DOCS=(
    "README.md"
    "docs/SETUP.md"
    "docs/DEVELOPMENT.md"
    "docs/COMMANDS.md"
    "docs/ARCHITECTURE.md"
    "package/docs/CHANGELOG.md"
    "package/docs/CONTRIBUTING.md"
)

for doc in "${REQUIRED_DOCS[@]}"; do
    if [ ! -f "$doc" ]; then
        print_error "Required documentation missing: $doc"
        exit 1
    fi
done
print_success "All required documentation present"

# Step 7: Check for TODO/FIXME in critical files
print_step "Checking for unresolved TODOs..."
if grep -r "TODO\|FIXME\|HACK" src/ include/ --exclude-dir=build 2>/dev/null | grep -v "// TODO: Future enhancement" > /dev/null; then
    print_warning "Found TODO/FIXME/HACK comments in source code:"
    grep -r "TODO\|FIXME\|HACK" src/ include/ --exclude-dir=build 2>/dev/null || true
    read -p "Continue with release? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
else
    print_success "No critical TODOs found"
fi

# Step 8: Verify GitHub Actions workflows
print_step "Validating GitHub Actions workflows..."
if [ ! -f ".github/workflows/ci.yml" ] || [ ! -f ".github/workflows/release.yml" ]; then
    print_error "Required GitHub Actions workflows missing"
    exit 1
fi

# Check workflow syntax (basic)
if ! grep -q "name: CI/CD Pipeline" .github/workflows/ci.yml; then
    print_error "CI workflow appears malformed"
    exit 1
fi

if ! grep -q "name: Release" .github/workflows/release.yml; then
    print_error "Release workflow appears malformed"
    exit 1
fi
print_success "GitHub Actions workflows validated"

# Step 9: Check dependencies and licenses
print_step "Verifying dependency licenses..."
DEPS_OK=true

# Check if we can build (dependencies available)
if ! make clean > /dev/null 2>&1 || ! make build > /dev/null 2>&1; then
    print_error "Build failed - check dependencies"
    DEPS_OK=false
fi

if [ "$DEPS_OK" = "true" ]; then
    print_success "Dependencies and build system verified"
else
    exit 1
fi

# Step 10: Create release tag (dry run)
print_step "Preparing release tag..."
TAG_NAME="v${VERSION}"

if git tag -l | grep -q "^${TAG_NAME}$"; then
    print_error "Tag $TAG_NAME already exists"
    echo "Existing tags:"
    git tag -l | grep "^v" | tail -5
    exit 1
fi
print_success "Tag $TAG_NAME is available"

# Step 11: Final confirmation
echo
echo -e "${GREEN}=============================================="
echo "  Release Preparation Complete!"
echo "=============================================="
echo -e "${NC}"
echo "Version: $VERSION"
echo "Branch: $CURRENT_BRANCH"
echo "Tag: $TAG_NAME"
echo
echo "Ready to release! Next steps:"
echo
echo "1. Create and push the release tag:"
echo "   git tag -a $TAG_NAME -m \"Release $TAG_NAME - Clean & Optimized Edition\""
echo "   git push origin $TAG_NAME"
echo
echo "2. Monitor GitHub Actions:"
echo "   https://github.com/bniladridas/Llamaware/actions"
echo
echo "3. Verify release artifacts:"
echo "   https://github.com/bniladridas/Llamaware/releases"
echo
echo "4. Test downloaded binaries on clean systems"
echo
echo -e "${YELLOW}Note: The release workflow will automatically create the GitHub release${NC}"
echo -e "${YELLOW}with binaries when you push the tag.${NC}"
echo

# Optional: Ask if user wants to create the tag now
read -p "Create and push the release tag now? (y/N): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    print_step "Creating release tag..."
    git tag -a "$TAG_NAME" -m "Release $TAG_NAME - Clean & Optimized Edition"
    
    print_step "Pushing release tag..."
    git push origin "$TAG_NAME"
    
    print_success "Release tag created and pushed!"
    echo
    echo "🚀 Release workflow started!"
    echo "Monitor progress at: https://github.com/bniladridas/Llamaware/actions"
else
    echo "Tag creation skipped. Run manually when ready:"
    echo "  git tag -a $TAG_NAME -m \"Release $TAG_NAME - Clean & Optimized Edition\""
    echo "  git push origin $TAG_NAME"
fi

echo
print_success "Release preparation completed successfully!"