"not method on true and false - spec section 2"
class Main : Object {
    run [|
        _ := ((true not) asString) print.
        _ := ((false not) asString) print.
        _ := (((true not) not) asString) print.
        _ := (((true not) identicalTo: false) asString) print.
        _ := (((false not) identicalTo: true) asString) print.
    ]
}
