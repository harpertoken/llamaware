#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}=== Starting Debug Test ===${NC}"

# Set up test directory
TEST_DIR="/tmp/llamaware_debug"
mkdir -p "$TEST_DIR"
echo -e "${GREEN}✓ Created test directory: $TEST_DIR${NC}"

# Create a simple test file
TEST_FILE="$TEST_DIR/test.txt"
echo "This is a test file" > "$TEST_FILE"
echo -e "${GREEN}✓ Created test file: $TEST_FILE${NC}"

# Create expect script
EXPECT_SCRIPT="$TEST_DIR/test.exp"
cat > "$EXPECT_SCRIPT" << 'EOL'
#!/usr/bin/expect -f

set timeout 30

# Start the llamaware-agent
spawn /usr/local/bin/llamaware-agent

# First, wait for any output to appear
expect {
    timeout {
        puts "ERROR: No output from application after 30 seconds"
        exit 1
    }
    eof {
        puts "ERROR: Application exited unexpectedly"
        exit 1
    }
    -re ".+" {
        # We got some output, check if it's the mode prompt
        if {[regexp {Mode.*1=Online / 2=Offline} $expect_out(buffer)]} {
            puts "Found mode selection prompt"
            send "2\r"
            exp_continue
        } elseif {[regexp {Ready - Type a command:} $expect_out(buffer)]} {
            puts "Successfully got to command prompt"
        } else {
            # If we get here, we got output but it's not what we expected
            puts "Unexpected output: $expect_out(buffer)"
            exp_continue
        }
    }
    timeout {
        puts "ERROR: Timeout waiting for initial prompt"
        exit 1
    }
    eof {
        puts "ERROR: Unexpected EOF waiting for initial prompt"
        exit 1
    }
}

# Send version command
send "version\r"

# Wait for version output
expect {
    -re "Llamaware v" {
        puts "SUCCESS: Version check passed"
    }
    timeout {
        puts "ERROR: Timeout waiting for version output"
        exit 1
    }
    eof {
        puts "ERROR: Unexpected EOF after version command"
        exit 1
    }
}

# Exit the application
send "exit\r"

expect eof
puts "Test completed successfully"
EOL

# Make the expect script executable
chmod +x "$EXPECT_SCRIPT"

# Run the expect script
echo -e "${YELLOW}=== Running Test ===${NC}"
expect -d -f "$EXPECT_SCRIPT" 2>&1 | tee "$TEST_DIR/test_output.log"

# Check the exit status
if [ ${PIPESTATUS[0]} -eq 0 ]; then
    echo -e "${GREEN}✓ Test completed successfully${NC}"
else
    echo -e "${RED}✗ Test failed. Check $TEST_DIR/test_output.log for details.${NC}"
    exit 1
fi
