"Error 51 - sending multi-parameter message that no method handles"
class Foo : Object {
    greet [|
        _ := 'Hello' print.
    ]
}

class Main : Object {
    run [|
        f := Foo new.
        _ := f greet: 'World'.
    ]
}
