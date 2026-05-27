// Test: Empty hex literal (0x without digits)
// Expected: Lexical error (exit code 1)
// Reason: 0x must be followed by at least one hex digit

import "ifj25" for Ifj

class Program {
    static main() {
        var num
        num = 0x
        __dummy = Ifj.write(num)
    }
}

