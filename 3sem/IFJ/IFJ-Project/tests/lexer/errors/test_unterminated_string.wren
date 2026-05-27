// Test: Unterminated string literal
// Expected: Lexical error (exit code 1)
// Reason: String is not closed before end of line

import "ifj25" for Ifj

class Program {
    static main() {
        var x
        x = "This string is not terminated
        __dummy = Ifj.write(x)
    }
}

