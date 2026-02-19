// Test: Hex escape with space
// Expected: Lexical error (exit code 1)
// Reason: \xdd must have exactly 2 hex digits with no spaces

import "ifj25" for Ifj

class Program {
    static main() {
        var str
        str = "Bad: \x 22"
        __dummy = Ifj.write(str)
    }
}

