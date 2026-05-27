"Variable defined in outer block visible in inner even if assigned after block literal"
class Main : Object {
    run [|
        b := [| _ := (x asString) print. ].
        x := 99.
        _ := b value.
    ]
}
