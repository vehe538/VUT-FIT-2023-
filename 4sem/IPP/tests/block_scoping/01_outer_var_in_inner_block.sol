"Variable from outer block visible in lexically nested block - spec section 1.2.4"
class Main : Object {
    run [|
        x := 10.
        inner := [|
            y := x plus: 5.
            _ := (y asString) print.
        ].
        _ := inner value.
    ]
}
