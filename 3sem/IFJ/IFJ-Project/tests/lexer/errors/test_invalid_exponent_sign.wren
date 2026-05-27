// Test: Invalid exponent with just sign
// Expected: Lexical error (exit code 1)
// Reason: Exponent must have digits after sign

import "ifj25" for Ifj

class Program {
    static main() {
        var num
        num = 1.5e+
        __dummy = Ifj.write(num)
    }
}

