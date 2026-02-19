import "ifj25" for Ifj

// ERROR: Num identifier + String is not allowed
// Expected error: SEMANTIC_TYPE_EXPR_ERROR (6)

class Program {
	static main() {
		var a
		a = 42
		
		var wrong
		wrong = a + "text"  // Num identifier + String → ERROR
	}
}

