// Test: Invalid escape with unknown character
// Expected: Lexical error (exit code 1)
// Reason: \a is not a valid escape sequence (only \", \n, \r, \t, \\, \xdd allowed)

import "ifj25" for Ifj

class Program {
    static main() {
        var str
        str = "Test \a string"
        __dummy = Ifj.write(str)
    }
}

