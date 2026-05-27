import "ifj25" for Ifj

// ERROR: Mixed type operations with identifiers
// Expected error: SEMANTIC_TYPE_EXPR_ERROR (6)

class Program {
	static main() {
		var num_var
		num_var = 10
		
		var str_var
		str_var = "text"
		
		// This should fail - can't add Num identifier to String identifier
		var wrong
		wrong = num_var + str_var  // Num identifier + String identifier → ERROR
	}
}

