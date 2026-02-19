// Test: String with actual newline inside (not escaped)
// Expected: Lexical error (exit code 1)
// Reason: Regular strings cannot span multiple lines

import "ifj25" for Ifj

class Program {
    static main() {
        var str
        str = "This is line one
and this is line two"
        __dummy = Ifj.write(str)
    }
}

