// Test: Leading zeros in integer (not allowed except for 0x prefix)
// Expected: Lexical error (exit code 1)
// Reason: Přebytečné počáteční číslice 0 jsou zakázány v celočíselné části

import "ifj25" for Ifj

class Program {
    static main() {
        var num
        num = 0123
        __dummy = Ifj.write(num)
    }
}

