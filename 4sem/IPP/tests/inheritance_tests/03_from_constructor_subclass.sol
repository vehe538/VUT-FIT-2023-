"Testing from: constructor with subclass - from spec Factorial example"
class Factorial : Integer {
    factorial [|
        r := (self equalTo: 0)
            ifTrue: [| r := Factorial from: 1. ]
            ifFalse: [|
                r := self multiplyBy:
                    ((Factorial from: (self plus: -1)) factorial).
            ].
    ]
}

class Main : Object {
    run [|
        x := Factorial from: 5.
        result := x factorial.
        _ := (result asString) print.
        y := Factorial from: 0.
        _ := ((y factorial) asString) print.
    ]
}
