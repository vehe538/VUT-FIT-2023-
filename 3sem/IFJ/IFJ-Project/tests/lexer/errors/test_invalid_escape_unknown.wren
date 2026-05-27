// Test: Unknown escape sequence
// Expected: Lexical error (exit code 1)
// Reason: \q is not a valid escape sequence

import "ifj25" for Ifj

class Program {
    static main() {
        var message
        message = "Hello\qWorld"
        __dummy = Ifj.write(message)
    }
}

