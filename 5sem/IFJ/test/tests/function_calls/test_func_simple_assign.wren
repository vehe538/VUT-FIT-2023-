import "ifj25" for Ifj

// Test: Simple function call in assignment
// Expected: COMPILATION_OK (0)

class Program {
	static main() {
		// Ifj.write returns null (void)
		var result
		result = Ifj.write(5)
		
		// Ifj.length returns Num
		var len
		len = Ifj.length("hello")

		__dummy = Ifj.write(result)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(len)
		__dummy = Ifj.write("\n")
	}
}

