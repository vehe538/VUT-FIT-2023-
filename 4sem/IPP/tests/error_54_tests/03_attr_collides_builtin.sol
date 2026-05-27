"Error 54 - trying to create attribute 'asString' which is a built-in method"
class Main : Object {
    run [|
        x := Object new.
        _ := x asString: 'hello'.
    ]
}
