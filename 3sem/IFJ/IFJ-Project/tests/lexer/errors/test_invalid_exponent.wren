// Test: Invalid exponent format
// Expected: Lexical error (exit code 1)
// Reason: Exponent must have digits after e

import "ifj25" for Ifj

class Program {
    static main() {
        var num
        num = 10.5e
        __dummy = Ifj.write(num)
    }
}

