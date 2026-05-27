import "ifj25" for Ifj

// Test: User-defined function calls in expressions
// Expected: COMPILATION_OK (0)

class Program {
	static add(a, b) {
		return a + b
	}
	
	static multiply(x, y) {
		return x * y
	}
	
	static main() {
		// Simple user function call
		var result1
		result1 = add(5, 3)

		// User function in arithmetic
		var result2
		result2 = add(10, 20) + 5
		
		var result3
		result3 = multiply(2, 3) * 4
		
		// Nested user function calls
		var result4
		result4 = add(multiply(2, 3), 5)
		
		// User function with Ifj function
		var result5
		result5 = add(Ifj.length("test"), 10)
		
		// Complex expression
		var result6
		result6 = multiply(add(1, 2), add(3, 4))
		
		// User function in comparison
		if (add(5, 5) == 10) {
			var x
			x = 1
			__dummy = Ifj.write(x)      // Expected: 1
			__dummy = Ifj.write("\n")
		} else {
			var y
			y = 0
			__dummy = Ifj.write(y)
			__dummy = Ifj.write("\n")
		}

		__dummy = Ifj.write(result1)    // Expected: 8 (5+3)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result2)    // Expected: 35 (add(10,20)+5 = 30+5)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result3)    // Expected: 24 (multiply(2,3)*4 = 6*4)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result4)    // Expected: 11 (add(6,5))
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result5)    // Expected: 14 (add(4,10))
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result6)    // Expected: 21 (multiply(3,7))
		__dummy = Ifj.write("\n")
	}
}

