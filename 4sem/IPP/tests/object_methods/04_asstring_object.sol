"asString on plain Object returns '' - spec section 2"
class Main : Object {
    run [|
        o := Object new.
        s := o asString.
        _ := s print.
        _ := ((s equalTo: '') asString) print.
        _ := ((s length) asString) print.
    ]
}
