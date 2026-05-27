import "ifj25" for Ifj

// ERROR: String * String is not allowed
// Only String * Num is valid
// Expected error: SEMANTIC_TYPE_EXPR_ERROR (6)

class Program {
	static main() {
		var wrong
		wrong = "abc" * "def"
	}
}

