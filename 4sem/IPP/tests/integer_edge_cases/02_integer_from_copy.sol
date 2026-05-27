"Integer from: copies the internal numeric value - spec section 2"
class Main : Object {
    run [|
        a := 42.
        b := Integer from: a.
        _ := (b asString) print.
        _ := ((b equalTo: 42) asString) print.
        _ := ((b equalTo: a) asString) print.
    ]
}
