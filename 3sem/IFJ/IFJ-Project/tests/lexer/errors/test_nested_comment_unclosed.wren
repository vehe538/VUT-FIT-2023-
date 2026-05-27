// Test: Nested block comment not closed
// Expected: Lexical error (exit code 1)
// Reason: All nested block comments must be properly closed

import "ifj25" for Ifj

class Program {
    /* Outer comment
       /* Inner comment
       Inner closed */
       // Outer not closed
    static main() {
        var x
        x = 10
    }
}

