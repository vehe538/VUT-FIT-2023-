// Test: Identifier cannot start with number
// Expected: Lexical error (exit code 1)
// Reason: Identifiers must start with letter or underscore

import "ifj25" for Ifj

class Program {
    static main() {
        var 123abc
        123abc = 10
    }
}

