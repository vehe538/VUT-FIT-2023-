"Nil subclass new does NOT guarantee singleton - spec section 2"
class MyNil : Nil {
    describe [|
        r := 'I am MyNil'.
    ]
}

class Main : Object {
    run [|
        a := MyNil new.
        b := MyNil new.
        _ := (a describe) print.
        _ := ((a identicalTo: nil) asString) print.
        _ := ((a identicalTo: b) asString) print.
    ]
}
