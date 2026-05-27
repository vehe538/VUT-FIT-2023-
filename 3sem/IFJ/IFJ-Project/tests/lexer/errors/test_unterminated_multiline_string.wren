// Test: Unterminated multiline string
// Expected: Lexical error (exit code 1)
// Reason: """ string is never closed

import "ifj25" for Ifj

class Program {
    static main() {
        var x
        x = """This is
        a multiline string
        but it is never closed
    }
}

