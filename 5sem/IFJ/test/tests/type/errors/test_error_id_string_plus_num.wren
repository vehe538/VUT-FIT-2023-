import "ifj25" for Ifj

// ERROR: String identifier + Num is not allowed
// Expected error: SEMANTIC_TYPE_EXPR_ERROR (6)

class Program {
	static main() {
		var str
		str = "text"
		
		var wrong
		wrong = str + 42  // String identifier + Num → ERROR
	}
}

