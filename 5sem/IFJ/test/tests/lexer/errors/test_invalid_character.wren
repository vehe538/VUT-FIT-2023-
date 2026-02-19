// Test: Invalid character in source code
// Expected: Lexical error (exit code 1)
// Reason: @ is not allowed outside of specific contexts

import "ifj25" for Ifj

class Program {
    static main() {
        var x @ 10
    }
}

