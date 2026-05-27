import "ifj25" for Ifj

// ERROR: Null identifier * anything is not allowed
// Expected error: SEMANTIC_TYPE_EXPR_ERROR (6)

class Program {
	static main() {
		var nullable
		nullable = null
		
		var wrong
		wrong = nullable * 3  // Null identifier * Num → ERROR
	}
}

