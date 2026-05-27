"Selector has 2 params but block has 1 - should fail with error 33"
class Main : Object {
    run [|
        x := self foo: 1 bar: 2.
    ]
    foo:bar: [ :x |
        y := x plus: 1.
    ]
}
