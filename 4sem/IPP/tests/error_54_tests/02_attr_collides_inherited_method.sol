"Error 54 - attribute name collides with an inherited method"
class Base : Object {
    greet [|
        _ := 'Hello from Base' print.
    ]
}

class Child : Base {
    run [|
        _ := self greet: 'World'.
    ]
}

class Main : Object {
    run [|
        c := Child new.
        _ := c run.
    ]
}
