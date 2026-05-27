"No Main class defined - should fail with error 31"
class Foo : Object {
    run [|
        x := 1.
    ]
}
