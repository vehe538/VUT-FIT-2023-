"Error 53 - from: with incompatible argument type (from spec section 1.2.10)"
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
        y := Factorial from: 'str'.
    ]
}
