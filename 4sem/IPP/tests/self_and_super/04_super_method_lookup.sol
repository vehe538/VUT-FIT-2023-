"super forces method lookup to start in superclass - spec section 1.2.8"
class A : Object {
    hello [|
        r := 'Hello from A'.
    ]
}

class B : A {
    hello [|
        r := 'Hello from B'.
    ]
    run [|
        _ := (self hello) print.
        _ := (super hello) print.
    ]
}

class Main : Object {
    run [|
        b := B new.
        _ := b run.
    ]
}
