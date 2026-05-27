import "ifj25" for Ifj

class Program {
	static main() {
		// === Test 1: Local identifiers with String operations ===
		var a
		a = "Hello"
		
		var b
		b = a + " World"  // String + String via identifier
		
		var c
		c = a + a  // identifier + identifier
		
		// === Test 2: Local identifiers with String multiplication ===
		var str1
		str1 = "x"
		
		var rep1
		rep1 = str1 * 3  // identifier * Num
		
		var rep2
		rep2 = "y" * 5
		
		// === Test 3: Local identifiers with Num operations ===
		var num1
		num1 = 10
		
		var num2
		num2 = 20
		
		var sum
		sum = num1 + num2  // identifier + identifier (Num)
		
		var diff
		diff = num1 - 5  // identifier - literal
		
		var mult
		mult = num1 * num2  // identifier * identifier
		
		var div
		div = num1 / 2  // identifier / literal
		
		// === Test 4: Global identifiers ===
		__global1 = "Test"
		__global2 = __global1 + " String"  // global + literal
		
		__num_global = 42
		__result = __num_global + 8  // global + literal
		
		// === Test 5: Mixed local and global ===
		var local1
		local1 = 100
		
		__global3 = local1 + 50  // local + literal assigned to global
		
		var local2
		local2 = __num_global * 2  // global used in local expression
		
		// === Test 6: Chained identifier operations ===
		var x
		x = 5
		
		var y
		y = x + 10
		
		var z
		z = y + x  // y depends on x, z depends on both
		
		// === Test 7: Identifier comparisons ===
		var val1
		val1 = 10
		
		var val2
		val2 = 20
		
		var cmp1
		cmp1 = val1 < val2  // identifier < identifier
		
		var cmp2
		cmp2 = val1 == 10  // identifier == literal
		
		var cmp3
		cmp3 = val2 != val1  // identifier != identifier
		
		// === Test 8: String identifier comparisons ===
		var str_a
		str_a = "abc"
		
		var str_b
		str_b = "def"
		
		var str_eq
		str_eq = str_a == str_b  // String identifier comparison
		
		var str_neq
		str_neq = str_a != "xyz"  // identifier != literal
		
		// === Test 9: Complex expressions with identifiers ===
		var base
		base = 10
		
		var complex1
		complex1 = base + 5 * 2  // identifier in complex expression
		
		var complex2
		complex2 = (base + 10) * 2  // identifier with parentheses
		
		// === Test 10: Null identifier operations ===
		var nullable
		nullable = null
		
		var null_eq
		null_eq = nullable == null  // identifier == null
		
		var null_neq
		null_neq = nullable != 5  // identifier != literal

		__dummy = Ifj.write(b)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(rep1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(rep2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(sum)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(diff)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(mult)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(div)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(__global2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(__result)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(__global3)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(local2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(z)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(cmp1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(cmp2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(cmp3)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(str_eq)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(str_neq)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(complex1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(complex2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(null_eq)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(null_neq)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(nullable)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(null_eq)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(null_neq)
		__dummy = Ifj.write("\n")
	}
}

