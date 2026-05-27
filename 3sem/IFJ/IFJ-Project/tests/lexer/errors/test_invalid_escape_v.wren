// Test: Invalid escape \v
// Expected: Lexical error (exit code 1)
// Reason: Only \", \n, \r, \t, \\, \xdd are valid

import "ifj25" for Ifj

class Program {
    static main() {
        var str
        str = "Test\vstring"
        __dummy = Ifj.write(str)
    }
}

