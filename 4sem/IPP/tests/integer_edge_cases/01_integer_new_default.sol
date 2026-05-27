"Integer new default internal value is 0 - spec section 2"
class Main : Object {
    run [|
        x := Integer new.
        _ := (x asString) print.
        _ := ((x equalTo: 0) asString) print.
        _ := ((x isNumber) asString) print.
    ]
}
