"Inheriting from undefined class - should fail with error 32"
class Foo : UndefinedParent {
    run [|
        x := 1.
    ]
}

class Main : Object {
    run [|
        f := Foo new.
    ]
}
