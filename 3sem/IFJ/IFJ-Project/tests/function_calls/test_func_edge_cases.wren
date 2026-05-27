import "ifj25" for Ifj

// Test: Edge cases for function calls
// Expected: COMPILATION_OK (0)

class Program {
	static calc(x) {
		return x * 2
	}
	
	static add(a, b) {
		return a + b
	}
	
	static nested3(x, y, z) {
		return x + y + z
	}
	
	static main() {
		// Edge case 1: Multiple nested function calls (3 levels deep)
		var result1
		result1 = calc(calc(calc(5)))
		
		// Edge case 2: Function call with complex expression in multiple arguments
		var result2
		result2 = add(calc(3) + 5, calc(7) - 2)
		
		// Edge case 3: Function with 3 arguments and nested calls
		var result3
		result3 = nested3(calc(1), calc(2), calc(3))
		
		// Edge case 4: Multiple operations with function calls
		var result4
		result4 = calc(5) + calc(10) - calc(3) * 2
		
		// Edge case 5: Function call as operand in division
		var result5
		result5 = 100 / calc(5)
		
		// Edge case 6: Mixed user and Ifj functions in same expression
		var str
		str = "test"
		var result6
		result6 = calc(Ifj.length(str)) + Ifj.length("hello")
		
		// Edge case 7: Function call in condition with comparison
		if (calc(5) > 8) {
			var x
			x = 1
		}
		
		// Edge case 8: Function call in while condition
		var counter
		counter = 0
		while (counter < calc(2)) {
			counter = counter + 1
		}
		
		// Edge case 9: Function call in return statement with expression
		var result9
		result9 = calc(5 + 3)
		
		// Edge case 10: Empty function call (no arguments) with expression
		// Note: This tests if parser handles () correctly
		
		// Edge case 11: Nested function calls with string operations
		var result11
		result11 = Ifj.length(Ifj.substring("hello", 0, calc(2)))
		
		// Edge case 12: Function call with parenthesized expression
		var result12
		result12 = calc((5 + 3) * 2)

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
		__dummy = Ifj.write(result9)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result11)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(result12)
		__dummy = Ifj.write("\n")
	}
}

