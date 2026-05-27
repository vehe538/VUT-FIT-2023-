import "ifj25" for Ifj

// Test: Function calls in comparison operations
// Expected: COMPILATION_OK (0)

class Program {
	static main() {
		var str
		str = "hello"
		
		// Function call in if condition
		if (Ifj.length(str) > 3) {
			var x
			x = 1
		} else {
			var y
			y = 0
		}
		
		// Function call comparison with literal
		if (Ifj.length("test") == 4) {
			var a
			a = 1
		} else {
			var b
			b = 0
		}
		
		// Function call in while
		while (Ifj.length("x") < 10) {
			var c
			c = 1
		}
		
		// Assignment with comparison containing function call
		var cmp1
		cmp1 = Ifj.length("abc") < 5
		
		var cmp2
		cmp2 = 10 > Ifj.length(str)

		__dummy = Ifj.write(cmp1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(cmp2)
		__dummy = Ifj.write("\n")
	}
}

