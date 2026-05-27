"identicalTo: - same object vs different objects - spec section 2"
class Main : Object {
    run [|
        a := Object new.
        b := Object new.
        c := a.
        _ := ((a identicalTo: a) asString) print.
        _ := ((a identicalTo: c) asString) print.
        _ := ((a identicalTo: b) asString) print.
        _ := ((a identicalTo: nil) asString) print.
    ]
}
