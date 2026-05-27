// Test: Hex escape with non-hex characters
// Expected: Lexical error (exit code 1)
// Reason: \xdd must contain exactly 2 hex digits (0-9, a-f, A-F)

import "ifj25" for Ifj

class Program {
    static main() {
        var str
        str = "Bad hex: \xZZ"
        __dummy = Ifj.write(str)
    }
}

