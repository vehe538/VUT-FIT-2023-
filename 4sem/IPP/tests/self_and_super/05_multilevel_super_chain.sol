"Multi-level super chain - A -> B -> C, each calls super"
class A : Object {
    greet [|
        r := 'A'.
    ]
}

class B : A {
    greet [|
        base := super greet.
        r := base concatenateWith: 'B'.
    ]
}

class C : B {
    greet [|
        base := super greet.
        r := base concatenateWith: 'C'.
    ]
}

class Main : Object {
    run [|
        c := C new.
        _ := (c greet) print.
    ]
}
