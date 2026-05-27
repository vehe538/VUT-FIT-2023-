"super assigned to variable - behaves as self per spec section 1.2.8"
class Base : Object {
    greet [|
        r := 'Hello from Base'.
    ]
}

class Child : Base {
    greet [|
        r := 'Hello from Child'.
    ]
    run [|
        s := super.
        _ := (s greet) print.
        _ := ((s identicalTo: self) asString) print.
    ]
}

class Main : Object {
    run [|
        c := Child new.
        _ := c run.
    ]
}
