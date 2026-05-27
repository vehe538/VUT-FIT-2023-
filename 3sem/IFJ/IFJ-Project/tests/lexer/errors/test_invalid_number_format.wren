// Test: Invalid number format
// Expected: Lexical error (exit code 1)
// Reason: Multiple decimal points

import "ifj25" for Ifj

class Program {
    static main() {
        var num
        num = 10.5.3
        __dummy = Ifj.write(num)
    }
}

