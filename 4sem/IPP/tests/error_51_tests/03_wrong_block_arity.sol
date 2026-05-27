"Error 51 - calling block with wrong arity (2-param block with value)"
class Main : Object {
    run [|
        b := [ :x :y | r := x plus: y. ].
        _ := b value.
    ]
}
