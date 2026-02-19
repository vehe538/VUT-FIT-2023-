import "ifj25" for Ifj

// ERROR: Num * String is not allowed
// Only String * Num (in that order!)
// Expected error: SEMANTIC_TYPE_EXPR_ERROR (6)

class Program {
	static main() {
		var wrong
		wrong = 5 * "hello"
	}
}

