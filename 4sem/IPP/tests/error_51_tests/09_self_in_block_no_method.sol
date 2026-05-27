"Using self inside a standalone block not attached to a method
 - should produce a semantic error"
class Main : Object {
    run [|
        b := [| x := self foo. ].
        _ := b value.
    ]
}
