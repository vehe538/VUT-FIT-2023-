// Test: Global variable with single underscore
// Expected: Lexical error (exit code 1)
// Reason: Global variable must start with TWO underscores (__), not one

import "ifj25" for Ifj

class Program {
    static main() {
        _single = 10
    }
}

