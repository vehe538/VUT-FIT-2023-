"Missing dot terminator after assignment - every statement must end with dot"
class Main : Object {
    run [|
        x := 42
        y := x plus: 1.
    ]
}
