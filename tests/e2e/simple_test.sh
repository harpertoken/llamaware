#!/usr/bin/expect -f
set timeout 10

# Start the application
spawn /usr/local/bin/llamaware-agent

expect {
    # Look for the mode selection prompt
    -re {mode.*\[?1=Online\s*/\s*2=Offline\]?.*:?\s*$} {
        # Select offline mode (2)
        send "2\r"
        exp_continue
    }
    # Look for the command prompt
    -re {>\s*$} {
        # Send version command and exit
        send "version\r"
        expect {
            -re {Llamaware v([0-9.]+)} {
                puts "$expect_out(1,string)"
                send "exit\r"
                exit 0
            }
            timeout {
                puts "ERROR: Timeout waiting for version output"
                exit 1
            }
        }
    }
    timeout {
        puts "ERROR: Timeout waiting for version information"
        exit 1
    }
    eof {
        puts "ERROR: Application exited unexpectedly"
        exit 1
    }
}
