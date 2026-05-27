"Passing blocks as arguments to user-defined methods"
class Main : Object {
    run [|
	result := self applyTwice: [ :x | r := x plus: 10. ] to: 5.
        _ := (result asString) print.
    ]
    applyTwice:to: [ :block :val |
        first := block value: val.
        result := block value: first.
    ]
}
