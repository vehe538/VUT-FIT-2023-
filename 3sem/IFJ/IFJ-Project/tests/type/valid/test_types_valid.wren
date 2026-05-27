import "ifj25" for Ifj

class Program {
	static main() {
		// === Test 1: String operations ===
		// String concatenation (String + String = String)
		var concat1
		concat1 = "hello" + "world"
		
		var concat2
		concat2 = "abc" + "def"
		
		// String iteration (String * Num = String)
		var repeat1
		repeat1 = "x" * 5
		
		var repeat2
		repeat2 = "hello" * 3
		
		// === Test 2: Numeric operations ===
		// Arithmetic (Num + Num = Num)
		var sum1
		sum1 = 10 + 20
		
		var sum2
		sum2 = 42 + 3.14
		
		var diff
		diff = 100 - 50
		
		var mult
		mult = 5 * 10
		
		// Division (Num / Num = Num, float only)
		var div1
		div1 = 10.0 / 2.0
		
		var div2
		div2 = 42 / 3.14
		
		// === Test 3: Relational operations with Num ===
		// Num relop Num = Bool
		var less
		less = 5 < 10
		
		var greater
		greater = 100 > 50
		
		var lessEq
		lessEq = 5 <= 10
		
		var greaterEq
		greaterEq = 100 >= 50
		
		// === Test 4: Equality operations ===
		// Num == Num = Bool
		var numEq
		numEq = 5 == 5
		
		var numNeq
		numNeq = 10 != 20
		
		// String == String = Bool
		var strEq
		strEq = "abc" == "abc"
		
		var strNeq
		strNeq = "hello" != "world"
		
		// === Test 5: null operations ===
		// null == null = true
		var nullEqNull
		nullEqNull = null == null
		
		// null != null = false
		var nullNeqNull
		nullNeqNull = null != null
		
		// null == other_type = false (not error!)
		var nullEqNum
		nullEqNum = null == 5
		
		var nullEqStr
		nullEqStr = null == "text"
		
		var nullNeqNum
		nullNeqNum = null != 10
		
		// === Test 6: Complex expressions ===
		var complex1
		complex1 = 5 + 10 * 2
		
		var complex2
		complex2 = "a" + "b" + "c"
		
		var complex3
		complex3 = "x" * 2 + "y" * 3
		
		var complex4
		complex4 = 10 + 20 - 5 * 2
		
		// === Test 7: Parentheses ===
		var paren1
		paren1 = (5 + 3) * 2
		
		var paren2
		paren2 = ("hello" + " ") + "world"
		
		// === Test 8: Chained comparisons ===
		var chain1
		chain1 = 5 < 10
		
		var chain2
		chain2 = 3 <= 3
		
		// === Test 9: Hex and different number formats ===
		var hex1
		hex1 = 0xFF + 1
		
		var hex2
		hex2 = 0x10 * 2
		
		var float1
		float1 = 1.5e10 / 2.0
		
		var float2
		float2 = 2.0e-3 + 1.0

		__dummy = Ifj.write(concat1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(concat2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(repeat1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(repeat2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(sum1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(sum2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(diff)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(mult)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(div1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(div2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(less)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(greater)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(lessEq)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(greaterEq)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(numEq)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(numNeq)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(strEq)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(strNeq)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(nullEqNull)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(nullNeqNull)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(nullEqNum)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(nullEqStr)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(nullNeqNum)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(complex1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(complex2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(complex3)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(complex4)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(paren1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(paren2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(chain1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(chain2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(hex1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(hex2)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(float1)
		__dummy = Ifj.write("\n")
		__dummy = Ifj.write(float2)
		__dummy = Ifj.write("\n")
	}
}
