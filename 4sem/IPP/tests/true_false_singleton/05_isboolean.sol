"isBoolean returns true for true and false, false for others - spec section 2"
class Main : Object {
    run [|
        _ := ((true isBoolean) asString) print.
        _ := ((false isBoolean) asString) print.
        _ := ((nil isBoolean) asString) print.
        _ := ((0 isBoolean) asString) print.
        _ := (('true' isBoolean) asString) print.
        _ := (((Object new) isBoolean) asString) print.
    ]
}
