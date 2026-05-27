"Error 51 - sending unknown message to an object"
class Main : Object {
    run [|
        x := 42.
        _ := x fly.
    ]
}
