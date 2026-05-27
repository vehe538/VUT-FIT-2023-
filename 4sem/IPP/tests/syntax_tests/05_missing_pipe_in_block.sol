"Block literal missing mandatory | separator - spec 1.2.2"
class Main : Object {
    run [|
        b := [ :x x plus: 1. ].
        _ := b value: 5.
    ]
}
