// Test: Exponent with invalid character
// Expected: Lexical error (exit code 1)
// Reason: Exponent must contain only digits after e/E and optional sign

import "ifj25" for Ifj

class Program {
    static main() {
        var num
        num = 1.5eX10
        __dummy = Ifj.write(num)
    }
}

