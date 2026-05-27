// Test: null in expression
import "ifj25" for Ifj

class Program {
    static main() {
        var x
        x = null
        __dummy = Ifj.write(x)  // Expected output: null
        __dummy = Ifj.write("\n")
    }
}

