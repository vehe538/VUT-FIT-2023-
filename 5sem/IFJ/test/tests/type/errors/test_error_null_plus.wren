import "ifj25" for Ifj

// ERROR: null + Num is not allowed
// null cannot be used in arithmetic operations
// Expected error: SEMANTIC_TYPE_EXPR_ERROR (6)

class Program {
	static main() {
		var wrong
		wrong = null + 5
	}
}

