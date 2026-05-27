"isString returns true for String and its subclasses - spec section 2"
class MyStr : String {
    shout [|
        _ := self print.
    ]
}

class Main : Object {
    run [|
        _ := (('hello' isString) asString) print.
        _ := (((String new) isString) asString) print.
        _ := (((MyStr new) isString) asString) print.
        _ := ((42 isString) asString) print.
        _ := ((nil isString) asString) print.
        _ := ((true isString) asString) print.
    ]
}
