#!/bin/bash

# Llamaware Agent - Dependency Installation Script
# This script installs all required dependencies for building the agent

set -e

echo "🚀 Installing Llamaware Agent dependencies..."

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "📦 Detected Linux - installing dependencies with apt"

    # Update package list
    sudo apt update

    # Install basic dependencies
    sudo apt install -y nlohmann-json3-dev cmake build-essential libcurl4-openssl-dev git

    # Check if CPR is already installed
    if ! pkg-config --exists cpr; then
        echo "🔧 Building CPR from source..."

        # Create temp directory
        CPR_DIR=$(mktemp -d)
        cd "$CPR_DIR"

        # Clone and build CPR
        git clone https://github.com/libcpr/cpr.git
        cd cpr
        cmake -S . -B build -DCPR_USE_SYSTEM_CURL=ON -DCMAKE_BUILD_TYPE=Release
        cmake --build build --parallel
        sudo cmake --install build

        # Cleanup
        cd /
        rm -rf "$CPR_DIR"

        echo "✅ CPR installed successfully"
    else
        echo "✅ CPR already installed"
    fi

elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "📦 Detected macOS - installing dependencies with Homebrew"

    # Check if Homebrew is installed
    if ! command -v brew &> /dev/null; then
        echo "❌ Homebrew not found. Please install Homebrew first:"
        echo "   /bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
        exit 1
    fi

    # Install dependencies
    brew install cpr nlohmann-json cmake

else
    echo "❌ Unsupported operating system: $OSTYPE"
    echo "Please install dependencies manually:"
    echo "  - CMake 3.15+"
    echo "  - nlohmann/json"
    echo "  - CPR (C++ Requests)"
    echo "  - libcurl development headers"
    exit 1
fi

echo "🎉 All dependencies installed successfully!"
echo ""
echo "Next steps:"
echo "  1. Configure your API keys in .env file"
echo "  2. Run: cmake -S . -B build"
echo "  3. Run: cmake --build build"
echo "  4. Run: ./build/bin/llamaware-agent"
