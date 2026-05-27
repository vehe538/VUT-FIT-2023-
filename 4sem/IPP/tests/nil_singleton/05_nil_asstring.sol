"nil asString returns the string 'nil' - spec section 2 Nil class"
class Main : Object {
    run [|
        s := nil asString.
        _ := s print.
        _ := ((s equalTo: 'nil') asString) print.
        _ := ((s length) asString) print.
    ]
}
