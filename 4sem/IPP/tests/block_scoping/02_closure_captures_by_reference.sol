"Closure captures variable by reference not by value - spec section 1.2.4"
class Main : Object {
    run [|
        x := 1.
        b := [| r := (x asString) print. ].
        x := 2.
        _ := b value.
        x := 3.
        _ := b value.
    ]
}
