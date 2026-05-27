"super used to access attribute shadowed by method in subclass - spec 1.2.11"
class Base : Object {
    setup [|
        _ := self color: 'red'.
    ]
}

class Child : Base {
    color [|
        r := 'blue method result'.
    ]
    run [|
        _ := self setup.
        _ := (self color) print.
        _ := (super color) print.
    ]
}

class Main : Object {
    run [|
        c := Child new.
        _ := c run.
    ]
}
