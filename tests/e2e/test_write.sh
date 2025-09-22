#!/usr/bin/expect -f
set timeout 30
set test_file [lindex $argv 0]
set test_content [lindex $argv 1]

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
        # Send write command
        send "write:$test_file $test_content\r"
        expect {
            -re {File.*written successfully|File.*saved} {
                # Small delay to ensure file is written
                after 1000
                
                # Verify the file was created
                if {[file exists $test_file]} {
                    set fh [open $test_file r]
                    set content [read $fh]
                    close $fh
                    
                    # Trim whitespace for comparison
                    set content [string trim $content]
                    set expected [string trim "$test_content"]
                    
                    if {$content == $expected} {
                        puts "TEST_PASSED: File write successful"
                        send "exit\r"
                        exit 0
                    } else {
                        puts "ERROR: File content does not match expected"
                        puts "Expected: '$expected'"
                        puts "Got: '$content'"
                        exit 1
                    }
                } else {
                    puts "ERROR: File was not created at $test_file"
                    exit 1
                }
            }
            timeout {
                puts "ERROR: Timeout waiting for write confirmation"
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
