#!/bin/bash
# Bump version for Llamaware
# Usage: ./scripts/bump-version.sh [major|minor|patch]

set -e

BUMP_TYPE=${1:-patch}
VERSION_FILE="VERSION"
CURRENT_VERSION=$(<"$VERSION_FILE")

if [[ ! $CURRENT_VERSION =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
  echo "Error: Invalid version format in $VERSION_FILE: $CURRENT_VERSION" >&2
  exit 1
fi

IFS='.' read -r MAJOR MINOR PATCH <<< "$CURRENT_VERSION"

case $BUMP_TYPE in
  major) NEW_VERSION="$((MAJOR + 1)).0.0" ;;
  minor) NEW_VERSION="$MAJOR.$((MINOR + 1)).0" ;;
  patch) NEW_VERSION="$MAJOR.$MINOR.$((PATCH + 1))" ;;
  *) echo "Usage: $0 [major|minor|patch]" >&2; exit 1 ;;
esac

echo "Bumping version: $CURRENT_VERSION → $NEW_VERSION"
echo "$NEW_VERSION" > "$VERSION_FILE"

# Update package.json if it exists
[[ -f package.json ]] && sed -i '' "s/\"version\": \"$CURRENT_VERSION\"/\"version\": \"$NEW_VERSION\"/" package.json

cat <<EOF

✅ Version updated to $NEW_VERSION

Next steps:
1. git add .
2. git commit -m "chore: bump version to $NEW_VERSION"
3. git push origin main
EOF
