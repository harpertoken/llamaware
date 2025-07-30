#!/bin/bash
# Llamaware Agent - Binary Installation Script

set -e

INSTALL_DIR="/usr/local/bin"
CONFIG_DIR="$HOME/.llamaware"
VERSION="2.0.0"

echo "Installing Llamaware Agent v${VERSION}..."

# Check system
if [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="macos"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="linux"
else
    echo "ERROR: Unsupported platform: $OSTYPE"
    exit 1
fi

# Create config directory
mkdir -p "$CONFIG_DIR"

# Copy binary
if [ -f "llamaware-agent-${PLATFORM}" ]; then
    sudo cp "llamaware-agent-${PLATFORM}" "$INSTALL_DIR/llamaware-agent"
    sudo chmod +x "$INSTALL_DIR/llamaware-agent"
else
    echo "ERROR: Binary not found for platform: $PLATFORM"
    exit 1
fi

# Copy example config
if [ ! -f "$CONFIG_DIR/.env" ]; then
    cp .env.example "$CONFIG_DIR/.env"
    echo "Configuration template created at $CONFIG_DIR/.env"
fi

# Create data directory
mkdir -p "$CONFIG_DIR/data"

echo "Llamaware Agent installed successfully!"
echo "Edit $CONFIG_DIR/.env with your API keys"
echo "Run with: llamaware-agent"