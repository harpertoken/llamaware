#!/usr/bin/expect -f
set timeout 30

# Start the application
spawn /usr/local/bin/llamaware-agent

expect {
    # Handle mode selection
    -re {mode.*\[?1=Online\s*/\s*2=Offline\]?.*:?\s*$} {
        send "2\r"
        exp_continue
    }
    # Look for command prompt
    -re {>\s*$} {
        # Send read command
        send "read:/tmp/llamaware_test_data/test_read.txt\r"
        expect {
            -re "test content for read" {
                puts "TEST_PASSED: File read successfully"
                send "exit\r"
                exit 0
            }
            timeout {
                puts "ERROR: Timeout waiting for file content"
                puts "Current buffer: $expect_out(buffer)"
                exit 1
            }
        }
    }
    timeout {
        puts "ERROR: Timeout waiting for command prompt"
        exit 1
    }
    eof {
        puts "ERROR: Application exited unexpectedly"
        exit 1
    }
}
