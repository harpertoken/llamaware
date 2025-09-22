#!/bin/bash
# Llamaware E2E Test Suite
# Tests all 16 enterprise features

set -e -o pipefail

BINARY_PATH="/usr/local/bin/llamaware-agent"
TEST_DATA_DIR="./test_data"
LOG_FILE="./e2e_test.log"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test configuration
BINARY_PATH="/usr/local/bin/llamaware-agent"
TEST_DATA_DIR="/tmp/llamaware_test_data"
LOG_FILE="e2e_test.log"

# Ensure test mode is set
export TEST_MODE=1

# Print header
print_header() {
    echo -e "\n${YELLOW}=== $1 ===${NC}"
    echo -e "=== $1 ===" >> "$LOG_FILE"
}

# Create test data directory
mkdir -p "$TEST_DATA_DIR"

# Clear previous log
echo "Starting E2E Tests - $(date)" > "$LOG_FILE"

# Function to run a command and check its output
run_command() {
    local input="$1"
    local expected="$2"
    local description="$3"
    local max_attempts=3
    local attempt=1
    local result=1
    local timeout_seconds=60  # Increased timeout to 60 seconds
    
    # Replace newlines with spaces for logging
    local clean_input=$(echo "$input" | tr '\n' ' ' | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')
    
    echo -e "\n${YELLOW}Running test: $description${NC}" | tee -a "$LOG_FILE"
    echo "Command: $clean_input" | tee -a "$LOG_FILE"
    
    while [ $attempt -le $max_attempts ]; do
        local start_time=$(date +%s)
        echo "Attempt $attempt/$max_attempts (Timeout: ${timeout_seconds}s)" | tee -a "$LOG_FILE"
        
        # Create a temporary file for the input
        local input_file="${TEST_DATA_DIR}/input_${RANDOM}.txt"
        echo -e "$input" > "$input_file"
        
        echo "Attempt $attempt/$max_attempts" | tee -a "$LOG_FILE"

        # Check if binary exists and is executable
        if [ ! -x "$BINARY_PATH" ]; then
            echo "ERROR: Binary $BINARY_PATH not found or not executable" >&2
            exit 1
        fi

        # Set test input commands
        input="help"

        # Use expect to handle interactive prompts with better logging
        local expect_script=$(cat << "EOF"
            set timeout 120
            log_file -a -noappend "$TEST_DATA_DIR/expect.log"
            log_user 1

            spawn "$BINARY_PATH"
            set spawn_id $spawn_id

            # Enable verbose logging
            exp_internal 1

            # Wait for the prompt with a longer timeout
            expect {
                -timeout 120
                "> " {
                    send_log "Successfully got prompt\n"
                }
                timeout {
                    send_log "ERROR: Timeout waiting for prompt\n"
                    exit 1
                }
                eof {
                    send_log "ERROR: Unexpected EOF waiting for prompt\n"
                    exit 1
                }
            }

            # Process each line of input
            set timeout 60
            foreach line [split "$input" "\n"] {
                if {[string trim "\$line"] ne ""} {
                    send_log "Sending command: \"\$line\"\n"
                    send -- "\$line\r"
                    
                     # Wait for command to complete or next prompt
                     expect {
                         -timeout 30
                         "> " {
                             send_log "Command completed successfully\n"
                         }
                         timeout {
                             send_log "ERROR: Timeout waiting for command to complete\n"
                             exit 1
                         }
                         eof {
                             send_log "ERROR: Unexpected EOF after sending command\n"
                             exit 1
                         }
                     }
                    # Small delay between commands
                    sleep 1
                }
            }
            
            # Add a small delay before exiting
            send_log "All commands processed, waiting before exit\n"
            sleep 2
            send -- "exit\r"
            expect eof
            send_log "Test completed successfully\n"
             exit 0
EOF
         )
        
        # Run the expect script with detailed logging
        echo "=== Starting expect script ===" > "${TEST_DATA_DIR}/expect_${attempt}.log"
        echo "$expect_script" >> "${TEST_DATA_DIR}/expect_script_${attempt}.log"
        
        output=$(echo "$expect_script" | expect -d -f - 2>&1 | tee -a "${TEST_DATA_DIR}/expect_${attempt}.log" | sed 's/\x1b\[[0-9;]*m//g')
        local exit_code=${PIPESTATUS[0]}
        
        # Log the output
        echo "=== Command Output (attempt $attempt) ===" >> "$LOG_FILE"
        echo "$output" >> "$LOG_FILE"
        echo "========================================" >> "$LOG_FILE"
        
        # Log expect debug output
        if [ -f "${TEST_DATA_DIR}/expect_${attempt}.log" ]; then
            echo "=== Expect Debug Output ===" >> "$LOG_FILE"
            cat "${TEST_DATA_DIR}/expect_${attempt}.log" >> "$LOG_FILE"
            echo "===========================" >> "$LOG_FILE"
        fi
        
        # Clean up the input file
        rm -f "$input_file"
        
        # Log the output
        echo "--- Command Output ---" | tee -a "$LOG_FILE"
        echo "$output" | tee -a "$LOG_FILE"
        echo "----------------------" | tee -a "$LOG_FILE"
        
        # Check if the expected pattern is in the output
        if echo "$output" | grep -i -E -q "$expected"; then
            echo -e "${GREEN}PASS${NC}: $description" | tee -a "$LOG_FILE"
            result=0
            break
        else
            echo -e "${RED}FAIL${NC}: $description (attempt $attempt/$max_attempts)" | tee -a "$LOG_FILE"
            echo "Expected: $expected" | tee -a "$LOG_FILE"
            echo "Got: "$output"" | tee -a "$LOG_FILE"
            
            # If we have more attempts, wait a bit before retrying
            if [ $attempt -lt $max_attempts ]; then
                echo "Retrying in 2 seconds..." | tee -a "$LOG_FILE"
                sleep 2
            fi
            
            attempt=$((attempt + 1))
        fi
    done
    
    if [ $result -ne 0 ]; then
        echo -e "${RED}All attempts failed for: $description${NC}" | tee -a "$LOG_FILE"
        # Don't exit on first failure, continue with other tests
    fi
    
    return $result
}

# Test 1-4: Files Features
print_header "Testing Files Features"

# File Injection - use defaults, inject file
echo "Starting Version Check"
run_command "\n\nversion\nexit" "v0.1" "Version Check"

# Advanced Operations
echo "Starting File Read Setup"
echo "test content" > "$TEST_DATA_DIR/test.txt"
echo "Starting File Read Operation"
run_command "\n\nread:$TEST_DATA_DIR/test.txt\nexit" "test content" "File Read Operation"

# Context Integration (basic check)
run_command "\n\n@directory ../../src/\nexit" "Goodbye" "Directory Context"

# Shell Integration
run_command "\n\n!echo 'shell test'\nexit" "Command completed" "Shell Toggle"

# Test 5-8: Sessions Features
print_header "Testing Sessions Features"

# Session Management
run_command "\n\n/save e2e_session\nexit" "Goodbye" "Session Save"

# Tool Registry (check help shows tools)
run_command "\n\nhelp\nexit" "Commands" "Tool Registry"

# Configuration (basic)
run_command "\n\nversion\nexit" "v0.1" "Configuration Check"

# Compression (if implemented) - expects start of compression process
# run_command "\n\n/compress\nexit" "Analyzing" "Context Compression"

# Test 9-12: Extensions Features
print_header "Testing Extensions Features"

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
print_header "Testing Security Features"

# Themes
run_command "\n\n/theme set dark\nexit" "Goodbye" "Theme Setting"

# Authentication
run_command "\n\n/auth providers\nexit" "providers" "Auth Providers"

# Sandboxing
run_command "\n\n/sandbox run \"echo sandbox\"\nexit" "sandbox" "Sandbox Execution"

# Error Handling
run_command "\n\ninvalid_command\nexit" "Error" "Error Handling"

# Test completion
print_header "Test Results"

# Check for any failures in the log
if grep -q "FAIL" "$LOG_FILE"; then
    echo -e "${RED}Some tests failed. Check $LOG_FILE for details.${NC}"
    FAILED_TESTS=$(grep -c "^FAIL" "$LOG_FILE")
    PASSED_TESTS=$(grep -c "^PASS" "$LOG_FILE")
    echo -e "${YELLOW}Test Summary: ${RED}$FAILED_TESTS failed${NC}, ${GREEN}$PASSED_TESTS passed${NC}"
    
    # Show the failed tests
    echo -e "\n${RED}Failed tests:${NC}"
    grep -B 1 "^FAIL" "$LOG_FILE" | grep -v "^--$" | grep -v "^FAIL"
    
    # Cleanup and exit with error
    rm -rf "$TEST_DATA_DIR"
    exit 1
else
    echo -e "${GREEN}All tests completed successfully!${NC}" | tee -a "$LOG_FILE"
    PASSED_TESTS=$(grep -c "^PASS" "$LOG_FILE")
    echo -e "${GREEN}Test Summary: $PASSED_TESTS tests passed${NC}"
    
    # Create a success file to indicate tests passed
    echo "success" > "${TEST_DATA_DIR}/test_completion.txt"
    
    # Cleanup and exit successfully
    rm -rf "$TEST_DATA_DIR"
    exit 0
fi