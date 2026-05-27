import "ifj25" for Ifj

// Test: Function calls in return statements
// Expected: COMPILATION_OK (0)

class Program {
	static getNumber() {
		return 42
	}
	
	static double(x) {
		// Return with function call
		return getNumber() * 2
	}
	
	static addNumbers(a, b) {
		// Return with expression containing function calls
		return double(a) + double(b)
	}
	
	static complexReturn(x) {
		// Return with nested function calls in expression
		return double(getNumber()) + x * 2
	}
	
	static conditionalReturn(flag) {
		if (flag > 0) {
			return getNumber()
		} else {
			return double(21)
		}
	}
	
	static chainedCalls() {
		// Return with deeply nested calls
		return double(double(double(5)))
	}
	
	static mixedReturn(str) {
		// Return with Ifj and user functions
		return Ifj.length(str) + getNumber()
	}
	
	static main() {
		var x
		x = double(5)
		
		var y
		y = addNumbers(3, 7)
		
		var z
		z = complexReturn(10)
		
		var w
		w = conditionalReturn(1)
		
		var v
		v = chainedCalls()
		
		var u
		u = mixedReturn("test")

		__dummy = Ifj.write(x)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(y)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(z)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(w)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(v)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(u)
		__dummy = Ifj.write("\n")
	}
}

