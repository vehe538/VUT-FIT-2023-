// Test: Incomplete hex escape sequence
// Expected: Lexical error (exit code 1)
// Reason: \x must be followed by exactly 2 hex digits

import "ifj25" for Ifj

class Program {
    static main() {
        var str
        str = "Bad hex: \x1"
        __dummy = Ifj.write(str)
    }
}

