"From spec section 1.2.12 - closures demonstration"
"Assumes stdin: 1\n2\n3\n4\n"
class Main : Object {
    giveObjectWithBlock [|
        x := ((String read) asInteger).
        b := [| y := ((String read) asInteger) plus: x. ].
        x := 7.
        z := ((Object new) block: b).
        x := 9.
        _ := z.
    ]
    run [|
        o := self giveObjectWithBlock.
        _ := String read.
        r1 := ((o block) value).
        _ := (r1 asString) print.
        r2 := ((o block) value).
        _ := (r2 asString) print.
    ]
}
