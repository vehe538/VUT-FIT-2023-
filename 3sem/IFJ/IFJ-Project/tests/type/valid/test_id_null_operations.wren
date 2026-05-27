import "ifj25" for Ifj

// Valid: Null identifiers with correct operations (only == and !=)
class Program {
	static main() {
		// Null comparisons
		var nullable
		nullable = null
		
		var is_null
		is_null = nullable == null  // OK: Null == Null
		
		var not_null
		not_null = nullable != 5  // OK: Null != Num
		
		var compare_with_string
		compare_with_string = nullable == "text"  // OK: Null == String
		
		// Global null
		__global_null = null
		var global_cmp
		global_cmp = __global_null == null  // OK: Global Null == Null
		
		__dummy = Ifj.write(nullable)           // Expected: null
		__dummy = Ifj.write(is_null)            // Expected: true
		__dummy = Ifj.write(not_null)           // Expected: true
		__dummy = Ifj.write(compare_with_string) // Expected: false
		__dummy = Ifj.write(global_cmp)         // Expected: true
	}
}

