"Error 54 - from spec 1.2.10: attribute named 'foo' collides with method foo"
class Main : Object {
    run [|
        _ := self foo: 10.
    ]
    foo [|
        _ := 'foo method' print.
    ]
}
