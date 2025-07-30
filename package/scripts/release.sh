#!/bin/bash
# Llamaware Agent - Release Script

set -e

VERSION="2.0.0"
RELEASE_DIR="release/v${VERSION}"

echo "Creating release v${VERSION}..."

# Clean and build
make clean
make build

# Create release directory
mkdir -p "$RELEASE_DIR"

# Create packages
make package
cp -r package/dist/* "$RELEASE_DIR/"

# Create source archive
git archive --format=tar.gz --prefix="llamaware-agent-${VERSION}/" HEAD > "$RELEASE_DIR/llamaware-agent-${VERSION}-source.tar.gz"

# Create checksums
cd "$RELEASE_DIR"
shasum -a 256 * > checksums.sha256

echo "Release v${VERSION} created in $RELEASE_DIR"
echo "Contents:"
ls -la "$RELEASE_DIR"