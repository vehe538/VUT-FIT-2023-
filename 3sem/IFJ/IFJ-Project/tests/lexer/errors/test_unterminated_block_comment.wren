// Test: Unterminated block comment
// Expected: Lexical error (exit code 1)
// Reason: /* comment is never closed

import "ifj25" for Ifj

class Program {
    static main() {
        /* This comment is never closed
        var x
        x = 10
    }
}

