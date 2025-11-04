#!/bin/bash

set -e

echo "Setting up pre-commit hooks..."

# Check if pre-commit is installed
if ! command -v pre-commit &> /dev/null; then
    echo "Installing pre-commit..."
    if command -v pip3 &> /dev/null; then
        pip3 install pre-commit
    elif command -v pip &> /dev/null; then
        pip install pre-commit
    elif command -v brew &> /dev/null; then
        brew install pre-commit
    else
        echo "Error: Could not find pip or brew to install pre-commit"
        echo "Please install pre-commit manually: https://pre-commit.com/#installation"
        exit 1
    fi
fi

# Check if yamllint is installed
if ! command -v yamllint &> /dev/null; then
    echo "Installing yamllint..."
    if command -v pip3 &> /dev/null; then
        pip3 install yamllint
    elif command -v pip &> /dev/null; then
        pip install yamllint
    elif command -v brew &> /dev/null; then
        brew install yamllint
    else
        echo "Error: Could not find pip or brew to install yamllint"
        echo "Please install yamllint manually: pip install yamllint"
        exit 1
    fi
fi

# Install pre-commit hooks
echo "Installing pre-commit hooks..."
pre-commit install

# Backup existing commit-msg hook if it exists
if [ -f .git/hooks/commit-msg ]; then
    echo "Backing up existing commit-msg hook..."
    cp .git/hooks/commit-msg .git/hooks/commit-msg.backup
fi

# Install our custom commit-msg hook
echo "Installing custom commit-msg hook..."
cp scripts/commit-msg .git/hooks/commit-msg
chmod +x .git/hooks/commit-msg

echo "Pre-commit setup complete!"
echo ""
echo "To run pre-commit on all files: pre-commit run --all-files"
echo "To run yamllint manually: yamllint ."
