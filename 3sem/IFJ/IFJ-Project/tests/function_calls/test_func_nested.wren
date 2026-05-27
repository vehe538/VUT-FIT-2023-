import "ifj25" for Ifj

// Test: Nested and complex function calls
// Expected: COMPILATION_OK (0)

class Program {
	static calc(x) {
		return x * 2
	}
	
	static add(a, b) {
		return a + b
	}
	
	static main() {
		var x
		x = 5
		
		var y
		y = 10
		
		// User function with expression as argument
		var result1
		result1 = calc(x + y)
		
		var result5
		result5 = calc(x + calc(y))
		
		// Mixed Ifj and user functions
		var str
		str = "hello"
		
		var result6
		result6 = calc(Ifj.length(str))
		
		var result7
		result7 = add(Ifj.length("abc"), Ifj.length("xyz"))

		__dummy = Ifj.write(result1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result5)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result6)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result7)
		__dummy = Ifj.write("\n")
	}
}

