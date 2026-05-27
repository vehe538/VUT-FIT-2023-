"asInteger on Integer returns self - spec section 2"
class Main : Object {
    run [|
        x := 42.
        y := x asInteger.
        _ := ((x equalTo: y) asString) print.
        _ := ((x identicalTo: y) asString) print.
        _ := (y asString) print.
    ]
}
