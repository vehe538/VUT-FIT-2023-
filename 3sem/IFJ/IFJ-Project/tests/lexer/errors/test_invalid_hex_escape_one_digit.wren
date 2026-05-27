// Test: Hex escape with only one digit
// Expected: Lexical error (exit code 1)
// Reason: \xdd requires exactly 2 hex digits

import "ifj25" for Ifj

class Program {
    static main() {
        var str
        str = "Invalid: \xA"
        __dummy = Ifj.write(str)
    }
}

