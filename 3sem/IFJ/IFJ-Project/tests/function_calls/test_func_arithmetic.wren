import "ifj25" for Ifj

// Test: Function calls in arithmetic operations
// Expected: COMPILATION_OK (0)

class Program {
	static main() {
		var str1
		str1 = "test"
		
		// Function call + literal
		var result1
		result1 = Ifj.length(str1) + 5
		
		// Literal + function call
		var result2
		result2 = 10 + Ifj.length("hello")
		
		// Function call + function call
		var result3
		result3 = Ifj.length("abc") + Ifj.length("xyz")
		
		// Complex expression with function call
		var result4
		result4 = Ifj.length("test") * 2 + 3
		
		// Function call in multiplication
		var result5
		result5 = "x" * Ifj.length("hello")

		__dummy = Ifj.write(result1)  // Expected: 9 (length("test")=4 + 5)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result2)  // Expected: 15 (10 + length("hello")=5)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result3)  // Expected: 6 (length("abc")=3 + length("xyz")=3)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result4)  // Expected: 11 (length("test")=4 * 2 + 3)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result5)  // Expected: xxxxx ("x" * 5)
		__dummy = Ifj.write("\n")
	}
}

