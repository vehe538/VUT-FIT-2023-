"concatenateWith: non-String argument returns nil - spec section 2"
class Main : Object {
    run [|
        s := 'hello'.
        r1 := s concatenateWith: nil.
        _ := ((r1 isNil) asString) print.
        r2 := s concatenateWith: 42.
        _ := ((r2 isNil) asString) print.
        r3 := s concatenateWith: true.
        _ := ((r3 isNil) asString) print.
        sub := String from: ' world'.
        r4 := s concatenateWith: sub.
        _ := r4 print.
    ]
}
