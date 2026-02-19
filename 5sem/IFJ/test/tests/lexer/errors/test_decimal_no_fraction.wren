// Test: Invalid decimal number format
// Expected: Lexical error (exit code 1)
// Reason: Decimal point must be followed by digits

import "ifj25" for Ifj

class Program {
    static main() {
        var num
        num = 10.
        __dummy = Ifj.write(num)
    }
}

