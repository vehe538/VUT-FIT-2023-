"isNumber returns true for Integer instances, false for others - spec section 2"
class Main : Object {
    run [|
        _ := ((42 isNumber) asString) print.
        _ := ((0 isNumber) asString) print.
        _ := ((-5 isNumber) asString) print.
        _ := (('hello' isNumber) asString) print.
        _ := ((true isNumber) asString) print.
        _ := ((nil isNumber) asString) print.
        _ := (((Object new) isNumber) asString) print.
    ]
}
