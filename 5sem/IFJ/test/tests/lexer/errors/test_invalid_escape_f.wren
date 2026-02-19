// Test: Invalid escape \f
// Expected: Lexical error (exit code 1)
// Reason: \f is not a valid escape sequence in IFJ25

import "ifj25" for Ifj

class Program {
    static main() {
        var str
        str = "Form\ffeed"
        __dummy = Ifj.write(str)
    }
}

