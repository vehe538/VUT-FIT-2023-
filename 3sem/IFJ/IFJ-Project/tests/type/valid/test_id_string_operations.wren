import "ifj25" for Ifj

// Valid: String identifiers with correct operations
class Program {
	static main() {
		// String + String
		var str1
		str1 = "Hello"
		
		var str2
		str2 = " World"
		
		var result1
		result1 = str1 + str2  // OK: String + String
		
		// String * Num
		var str3
		str3 = "x"
		
		var result2
		result2 = str3 * 3  // OK: String * Num
		
		// Global string operations
		__global_str = "Test"
		__result = __global_str + "ing"  // OK: Global String + String
		
		// String comparisons (only == and !=)
		var cmp1
		cmp1 = str1 == str2  // OK: String == String
		
		var cmp2
		cmp2 = str1 != "Hello"  // OK: String != String
		
		__dummy = Ifj.write(result1)   // Expected: Hello World
		__dummy = Ifj.write(result2)   // Expected: xxx
		__dummy = Ifj.write(__result)  // Expected: Testing
	}
}

