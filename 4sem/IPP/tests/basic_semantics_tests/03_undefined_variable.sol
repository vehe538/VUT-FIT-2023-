"Reading an uninitialized variable - should fail with error 32"
class Main : Object {
    run [|
        x := y.
    ]
}
