import "ifj25" for Ifj

// Valid: Num identifiers with correct operations
class Program {
	static main() {
		// Num arithmetic
		var a
		a = 10
		
		var b
		b = 5
		
		var sum
		sum = a + b  // OK: Num + Num
		
		var diff
		diff = a - b  // OK: Num - Num
		
		var mult
		mult = a * b  // OK: Num * Num
		
		var div
		div = a / b  // OK: Num / Num
		
		// Num relational operators
		var lt
		lt = a < b  // OK: Num < Num
		
		var gt
		gt = a > b  // OK: Num > Num
		
		var le
		le = a <= b  // OK: Num <= Num
		
		var ge
		ge = a >= b  // OK: Num >= Num
		
		var eq
		eq = a == b  // OK: Num == Num
		
		var neq
		neq = a != b  // OK: Num != Num
		
		// Global num operations
		__global_num = 42
		__result = __global_num + 8  // OK: Global Num + Num
		
		__dummy = Ifj.write(sum)      // Expected: 15
		__dummy = Ifj.write(diff)     // Expected: 5
		__dummy = Ifj.write(mult)     // Expected: 50
		__dummy = Ifj.write(div)      // Expected: 2 (or 2.0)
		__dummy = Ifj.write(__result) // Expected: 50
	}
}

