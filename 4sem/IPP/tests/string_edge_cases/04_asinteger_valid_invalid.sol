"asInteger on valid/invalid strings - spec section 2"
class Main : Object {
    run [|
        _ := (('42' asInteger) asString) print.
        _ := (('-7' asInteger) asString) print.
        _ := (('0' asInteger) asString) print.
        _ := ((('abc' asInteger) isNil) asString) print.
        _ := ((('12abc' asInteger) isNil) asString) print.
        _ := ((('' asInteger) isNil) asString) print.
    ]
}
