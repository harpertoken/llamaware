#!/bin/bash

# Version bump script for Llamaware
# Usage: ./scripts/bump-version.sh [major|minor|patch]

set -e

BUMP_TYPE=${1:-patch}
CURRENT_VERSION=$(cat VERSION)

if [[ ! "$CURRENT_VERSION" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    echo "Error: Invalid version format in VERSION file: $CURRENT_VERSION"
    exit 1
fi

IFS='.' read -r MAJOR MINOR PATCH <<< "$CURRENT_VERSION"

case $BUMP_TYPE in
    major)
        NEW_VERSION="$((MAJOR + 1)).0.0"
        ;;
    minor)
        NEW_VERSION="$MAJOR.$((MINOR + 1)).0"
        ;;
    patch)
        NEW_VERSION="$MAJOR.$MINOR.$((PATCH + 1))"
        ;;
    *)
        echo "Error: Invalid bump type. Use: major, minor, or patch"
        exit 1
        ;;
esac

echo "Bumping version from $CURRENT_VERSION to $NEW_VERSION"

# Update VERSION file
echo "$NEW_VERSION" > VERSION

# Update any other version references if needed
if [ -f "package.json" ]; then
    sed -i '' "s/\"version\": \"$CURRENT_VERSION\"/\"version\": \"$NEW_VERSION\"/" package.json
fi

echo "Version bumped to $NEW_VERSION"
echo "Don't forget to:"
echo "1. Commit changes: git add . && git commit -m 'chore: bump version to $NEW_VERSION'"
echo "2. Create release: git commit -m 'feat: release $NEW_VERSION [release]'"
echo "3. Push to trigger release: git push origin main"
