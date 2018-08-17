#!/usr/bin/expect
spawn iRunner
expect "Please select one detector to continue('Q' to exit):"
send "1\r"
expect "1 - Acquire\n2 - Detector\n3 - SDK\n4 - Calibration\nQ - Press Q to Exit"
send "3\r"
expect "1 - Set Log Level\n2 - List Configuration\n3 - Set Configuration\nQ - Back to Super Menu\nPlease input your selection:"
send "2\r"
expect "Q - Back to Super Menu"
send "Q\r"
expect "Q - Press Q to Exit"
send "Q\r"
expect "Press <Enter> to exit..."
send "\r"
expect eof
exit
