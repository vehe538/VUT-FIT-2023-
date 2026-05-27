"String new has empty string as default value - spec section 2"
class Main : Object {
    run [|
        s := String new.
        _ := s print.
        _ := ((s length) asString) print.
        _ := ((s equalTo: '') asString) print.
        _ := ((s isString) asString) print.
    ]
}
