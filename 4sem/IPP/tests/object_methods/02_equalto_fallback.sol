"equalTo: on Object without internal attrs falls back to identicalTo: - spec section 2"
class Main : Object {
    run [|
        a := Object new.
        b := Object new.
        c := a.
        _ := ((a equalTo: a) asString) print.
        _ := ((a equalTo: c) asString) print.
        _ := ((a equalTo: b) asString) print.
    ]
}
