"print returns self - allowing chained calls - spec section 2"
class Main : Object {
    run [|
        s := 'hello'.
        result := s print.
        _ := ((result identicalTo: s) asString) print.
        r := 'a' print.
        r2 := r concatenateWith: 'b'.
        _ := r2 print.
    ]
}
