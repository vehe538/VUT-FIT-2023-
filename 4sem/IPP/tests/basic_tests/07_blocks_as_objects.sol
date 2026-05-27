"Testing blocks as objects, sending value and value:"
class Main : Object {
    run [|
        b1 := [| r := 'block executed'. ].
        _ := b1 value.
        b2 := [ :x | r := x asString. ].
        r := b2 value: 42.
        _ := r print.
        b3 := [ :x :y | r := x plus: y. ].
        sum := b3 value: 10 value: 20.
        _ := (sum asString) print.
    ]
}
