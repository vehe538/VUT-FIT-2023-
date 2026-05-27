"Two closures sharing same variable - modifying via one affects the other"
class Main : Object {
    run [|
        shared := 0.
        adder := [| shared := shared plus: 1. ].
        reader := [| r := shared. ].
        _ := adder value.
        _ := adder value.
        _ := adder value.
        result := reader value.
        _ := (result asString) print.
    ]
}
