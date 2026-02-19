import "ifj25" for Ifj

// ERROR: String - String is not allowed
// Strings only support + (concatenation) and * (with Num)
// Expected error: SEMANTIC_TYPE_EXPR_ERROR (6)

class Program {
	static main() {
		var wrong
		wrong = "hello" - "world"
	}
}

