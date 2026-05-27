import "ifj25" for Ifj

// ERROR: Global String identifier + Num is not allowed
// Expected error: SEMANTIC_TYPE_EXPR_ERROR (6)

class Program {
	static main() {
		__global_str = "hello"
		
		var wrong
		wrong = __global_str + 42  // Global String identifier + Num → ERROR
	}
}

