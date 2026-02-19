import "ifj25" for Ifj

// ERROR: Num identifier * String is not allowed
// Expected error: SEMANTIC_TYPE_EXPR_ERROR (6)

class Program {
	static main() {
		var num
		num = 5
		
		var wrong
		wrong = num * "text"  // Num identifier * String → ERROR
	}
}

