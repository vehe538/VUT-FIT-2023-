"Assigning to a formal parameter - should fail with error 34"
class Main : Object {
    run [|
        x := self double: 5.
    ]
    double: [ :x |
        x := x plus: x.
    ]
}
