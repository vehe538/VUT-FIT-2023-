"Empty block body - result should be nil per spec section 1.2.6"
class Main : Object {
    run [|
        b := [| ].
        result := b value.
        _ := (result asString) print.
    ]
}
