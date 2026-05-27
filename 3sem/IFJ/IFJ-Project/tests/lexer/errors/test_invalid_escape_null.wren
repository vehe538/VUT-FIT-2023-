// Test: Invalid escape \0 (not supported)
// Expected: Lexical error (exit code 1)
// Reason: \0 is not a valid escape sequence in IFJ25

import "ifj25" for Ifj

class Program {
    static main() {
        var str
        str = "Null\0char"
        __dummy = Ifj.write(str)
    }
}

