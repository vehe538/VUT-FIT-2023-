"nil is a singleton - identicalTo: must return true - spec section 2"
class Main : Object {
    run [|
        a := nil.
        b := nil.
        _ := ((a identicalTo: b) asString) print.
        _ := ((a identicalTo: nil) asString) print.
        _ := ((nil identicalTo: nil) asString) print.
    ]
}
