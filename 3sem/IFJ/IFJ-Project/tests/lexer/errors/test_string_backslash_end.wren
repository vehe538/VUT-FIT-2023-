// Test: String with backslash at end
// Expected: Lexical error (exit code 1)
// Reason: Backslash must be followed by valid escape sequence

import "ifj25" for Ifj

class Program {
    static main() {
        var str
        str = "Test string\"
        __dummy = Ifj.write(str)
    }
}

