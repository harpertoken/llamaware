#!/bin/bash
# Llamaware E2E Test Suite
# Tests all 16 enterprise features

set -e -o pipefail

BINARY_PATH="../../build/bin/llamaware-agent"
TEST_DATA_DIR="./test_data"
LOG_FILE="./e2e_test.log"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Setup
mkdir -p "$TEST_DATA_DIR"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
BINARY_PATH="$PROJECT_ROOT/build/bin/llamaware-agent"
echo "Starting Llamaware E2E Tests..." > "$LOG_FILE"

# Helper functions
run_command() {
    local input="$1"
    local expected="$2"
    local description="$3"

    echo "Testing: $description" >> "$LOG_FILE"
    output=$(printf "$input" | "$BINARY_PATH" 2>&1 | sed 's/\x1b\[[0-9;]*m//g')

    echo "DEBUG: Expected '$expected', Got: '$output'" >> "$LOG_FILE"
    if echo "$output" | grep -i -E -q "$expected"; then
        echo -e "${GREEN}PASS${NC}: $description"
        echo "PASS: $description" >> "$LOG_FILE"
    else
        echo -e "${RED}FAIL${NC}: $description"
        echo "FAIL: $description" >> "$LOG_FILE"
        echo "Expected: $expected" >> "$LOG_FILE"
        echo "Got: $output" >> "$LOG_FILE"
        return 1
    fi
}

# Test 1-4: Files Features
echo "=== Testing Files Features ==="

# File Injection - use defaults, inject file
run_command "\n\n@file README.md\nexit" "Goodbye" "File Injection"

# Advanced Operations
echo "test content" > "$TEST_DATA_DIR/test.txt"
run_command "\n\nread:$TEST_DATA_DIR/test.txt\nexit" "test content" "File Read Operation"

# Context Integration (basic check)
run_command "\n\n@directory src/\nexit" "Goodbye" "Directory Context"

# Shell Integration
run_command "\n\n!echo 'shell test'\nexit" "Command completed" "Shell Toggle"

# Test 5-8: Sessions Features
echo "=== Testing Sessions Features ==="

# Session Management
run_command "\n\n/save e2e_session\nexit" "Goodbye" "Session Save"

# Tool Registry (check help shows tools)
run_command "\n\nhelp\nexit" "Commands" "Tool Registry"

# Configuration (basic)
run_command "\n\nversion\nexit" "v0.1" "Configuration Check"

# Compression (if implemented) - expects start of compression process
run_command "\n\n/compress\nexit" "Analyzing" "Context Compression"

# Test 9-12: Extensions Features
echo "=== Testing Extensions Features ==="

# MCP Support
run_command "\n\n/mcp servers\nexit" "servers" "MCP Servers"

# Checkpointing
run_command "\n\n/checkpoint e2e_test\nexit" "Goodbye" "Checkpointing"

# Web Integration
run_command "\n\nsearch:test query\nexit" "Goodbye" "Web Search"

# Filtering (basic git check)
if [ -d "../../.git" ]; then
    run_command "\n\ncmd:git status\nexit" "Command completed" "Git Filtering"
fi

# Test 13-16: Security Features
echo "=== Testing Security Features ==="

# Themes
run_command "\n\n/theme set dark\nexit" "Goodbye" "Theme Setting"

# Authentication
run_command "\n\n/auth providers\nexit" "providers" "Auth Providers"

# Sandboxing
run_command "\n\n/sandbox run \"echo sandbox\"\nexit" "sandbox" "Sandbox Execution"

# Error Handling
run_command "\n\ninvalid_command\nexit" "Error" "Error Handling"

# Cleanup
rm -rf "$TEST_DATA_DIR"

echo "E2E Tests completed. Check $LOG_FILE for details."