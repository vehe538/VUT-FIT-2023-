import "ifj25" for Ifj

// Valid: Chained operations with identifiers
class Program {
	static main() {
		// Type propagation through identifiers
		var x
		x = 5
		
		var y
		y = x + 10  // y inherits Num type from x
		
		var z
		z = y * 2  // z uses y's Num type
		
		// String type propagation
		var str1
		str1 = "Hello"
		
		var str2
		str2 = str1 + " World"  // str2 inherits String type
		
		var str3
		str3 = str2 * 2  // str3 uses str2's String type
		
		// Mixed with globals
		__global_num = 100
		var local_num
		local_num = __global_num + 50  // local_num inherits from global
		
		var final_num
		final_num = local_num - 20  // final_num inherits from local_num
		
		__dummy = Ifj.write(z)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(str3)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(local_num)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(final_num)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(__global_num)
		__dummy = Ifj.write("\n")
	}
}

