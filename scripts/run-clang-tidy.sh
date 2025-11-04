#!/bin/bash

# Script to run clang-tidy on the project
# Usage: ./scripts/run-clang-tidy.sh [file_pattern]

set -e

CLANG_TIDY="/opt/homebrew/Cellar/llvm/21.1.4/bin/clang-tidy"
BUILD_DIR="build"
EXTRA_ARGS="--extra-arg=-isystem/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1 --extra-arg=-isystem/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include"

# Check if clang-tidy exists
if [ ! -f "$CLANG_TIDY" ]; then
    echo "Error: clang-tidy not found at $CLANG_TIDY"
    echo "Please install LLVM via: brew install llvm"
    exit 1
fi

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Error: Build directory not found. Please run cmake first."
    exit 1
fi

# Default to all cpp files if no pattern provided
PATTERN=${1:-"src/**/*.cpp"}

echo "Running clang-tidy on files matching: $PATTERN"
echo "Using configuration from .clang-tidy"
echo

# Find and check files
find src -name "*.cpp" -type f | while read -r file; do
    echo "Checking: $file"
    $CLANG_TIDY "$file" -p "$BUILD_DIR" $EXTRA_ARGS
    if [ $? -eq 0 ]; then
        echo "✅ $file - OK"
    else
        echo "❌ $file - Issues found"
    fi
    echo
done

echo "Clang-tidy analysis complete!"
