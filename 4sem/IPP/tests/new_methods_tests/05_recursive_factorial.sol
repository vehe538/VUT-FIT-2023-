"Recursive factorial using user-defined method"
class Main : Object {
    run [|
        r := self factorial: 5.
        _ := (r asString) print.
        r2 := self factorial: 0.
        _ := (r2 asString) print.
    ]
    factorial: [ :n |
        result := (n equalTo: 0)
            ifTrue: [| r := 1. ]
            ifFalse: [| r := n multiplyBy: (self factorial: (n minus: 1)). ].
    ]
}
