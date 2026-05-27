"Selector foo: needs 1 param block but block has 0 - error 33"
class Main : Object {
    run [|
        _ := self foo: 1.
    ]
    foo: [|
        x := 1.
    ]
}
