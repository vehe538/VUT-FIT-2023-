"asString for true returns 'true', false returns 'false' - spec section 2"
class Main : Object {
    run [|
        _ := (true asString) print.
        _ := (false asString) print.
        _ := (((true asString) equalTo: 'true') asString) print.
        _ := (((false asString) equalTo: 'false') asString) print.
        _ := (((true asString) length) asString) print.
        _ := (((false asString) length) asString) print.
    ]
}
