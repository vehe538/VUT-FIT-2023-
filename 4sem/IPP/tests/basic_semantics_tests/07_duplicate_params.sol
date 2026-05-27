"Duplicate parameter names in same block - should fail with error 35"
class Main : Object {
    run [|
        b := [ :x :x | y := x plus: x. ].
        _ := b value: 1 value: 2.
    ]
}
