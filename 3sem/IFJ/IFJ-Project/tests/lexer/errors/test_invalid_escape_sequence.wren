// Test: Invalid escape sequence in string
// Expected: Lexical error (exit code 1)
// Reason: \x with invalid hex digits

import "ifj25" for Ifj

class Program {
    static main() {
        var x
        x = "Invalid escape: \xGG"
        __dummy = Ifj.write(x)
    }
}

