// Test: Invalid escape \b
// Expected: Lexical error (exit code 1)
// Reason: \b is not a valid escape sequence in IFJ25

import "ifj25" for Ifj

class Program {
    static main() {
        var str
        str = "Back\bspace"
        __dummy = Ifj.write(str)
    }
}

