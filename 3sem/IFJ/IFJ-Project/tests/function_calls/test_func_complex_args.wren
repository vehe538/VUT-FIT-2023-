import "ifj25" for Ifj

// Test: Complex expressions as function arguments
// Expected: COMPILATION_OK (0)

class Program {
	static process(a, b, c) {
		return a + b + c
	}
	
	static calc(x) {
		return x * 2
	}
	
	static main() {
		var x
		x = 5
		var y
		y = 10
		var z
		z = 3
		
		// Test 1: All arguments are expressions
		var result1
		result1 = process(x + y, y - z, z * 2)
		
		// Test 2: Arguments with division and modulo
		var result2
		result2 = process(x / z, y / 2, x + y / z)
		
		// Test 3: Arguments with nested function calls
		var result3
		result3 = process(calc(x), calc(y), calc(z))
		
		// Test 4: Arguments with mixed operations
		var result4
		result4 = process(x * y + z, calc(x) - y, z + calc(y))
		
		// Test 5: Arguments with comparison expressions
		var result5
		result5 = process(x + 1, y + 2, z + 3)
		
		// Test 6: Arguments with parenthesized expressions
		var result6
		result6 = process((x + y) * z, x * (y + z), (x * y) + z)
		
		// Test 7: String concatenation as argument
		var str1
		str1 = "hello"
		var str2
		str2 = "world"
		var result7
		result7 = Ifj.length(str1 + str2)
		
		// Test 8: Multiple nested function calls in single argument
		var result8
		result8 = calc(calc(x) + calc(y))
		
		// Test 9: Ifj functions with complex arguments
		var result9
		result9 = Ifj.substring("hello world", calc(1), calc(4))
		
		// Test 10: Chain of function calls with different argument counts
		var result10
		result10 = process(calc(x), process(1, 2, 3), calc(y))

		__dummy = Ifj.write(result1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result3)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result4)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result5)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result6)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result7)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result8)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result9)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result10)
		__dummy = Ifj.write("\n")
	}
}

