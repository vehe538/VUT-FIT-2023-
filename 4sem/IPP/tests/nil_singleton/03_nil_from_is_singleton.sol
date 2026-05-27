"Nil from: nil returns the same singleton - spec section 2"
class Main : Object {
    run [|
        a := Nil from: nil.
        _ := ((a identicalTo: nil) asString) print.
        _ := (a asString) print.
    ]
}
