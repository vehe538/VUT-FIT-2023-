import "ifj25" for Ifj

// ERROR: String identifier * String is not allowed
// Expected error: SEMANTIC_TYPE_EXPR_ERROR (6)

class Program {
	static main() {
		var str1
		str1 = "abc"
		
		var str2
		str2 = "def"
		
		var wrong
		wrong = str1 * str2  // String identifier * String identifier → ERROR
	}
}

