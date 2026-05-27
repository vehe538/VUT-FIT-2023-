"Redefining a class - should fail with error 35"
class Main : Object {
    run [|
        x := 1.
    ]
}

class Main : Object {
    run [|
        x := 2.
    ]
}
