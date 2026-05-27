// Test: Invalid hexadecimal literal
// Expected: Lexical error (exit code 1)
// Reason: 0x must be followed by hex digits

import "ifj25" for Ifj

class Program {
    static main() {
        var num
        num = 0xGHIJ
        __dummy = Ifj.write(num)
    }
}

