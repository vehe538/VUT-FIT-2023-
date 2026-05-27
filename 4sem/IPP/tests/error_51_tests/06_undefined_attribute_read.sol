"Error 51 - reading instance attribute that has not been set"
class Main : Object {
    run [|
        x := Object new.
        _ := x color.
    ]
}
