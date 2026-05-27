"Error 52 - sending plus: to nil"
class Main : Object {
    run [|
        x := nil.
        _ := x plus: 1.
    ]
}
