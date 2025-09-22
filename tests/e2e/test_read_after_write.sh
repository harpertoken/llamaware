#!/usr/bin/expect -f
set timeout 30
set test_file [lindex $argv 0]
set expected_content [lindex $argv 1]

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
        send "read:$test_file\r"
        expect {
            -re "$expected_content" {
                puts "TEST_PASSED: File read successful"
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
