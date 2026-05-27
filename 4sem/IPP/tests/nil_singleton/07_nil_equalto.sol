"nil equalTo: nil - since Nil has no internal attrs, falls back to identicalTo:"
class Main : Object {
    run [|
        _ := ((nil equalTo: nil) asString) print.
        _ := ((nil equalTo: false) asString) print.
        _ := ((nil equalTo: 0) asString) print.
    ]
}
