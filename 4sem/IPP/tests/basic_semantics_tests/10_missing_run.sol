"Main class exists but has no run method - should fail with error 31"
class Main : Object {
    foo [|
        x := 1.
    ]
}
