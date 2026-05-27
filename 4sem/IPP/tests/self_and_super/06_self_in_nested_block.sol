"self in nested block retains original method context - spec section 1.2.7"
class Main : Object {
    run [|
        _ := self label: 'outer'.
        b := [|
            inner := [|
                _ := self label: 'inner nested'.
            ].
            _ := inner value.
        ].
        _ := b value.
    ]
    label: [ :s |
        _ := s print.
    ]
}
