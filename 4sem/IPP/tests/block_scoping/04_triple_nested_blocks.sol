"Three levels of nested blocks, all share outer variables"
class Main : Object {
    run [|
        x := 'outer'.
        level1 := [|
            y := 'level1'.
            level2 := [|
                z := 'level2'.
                level3 := [|
                    msg := x concatenateWith: y.
                    msg := msg concatenateWith: z.
                    _ := msg print.
                ].
                _ := level3 value.
            ].
            _ := level2 value.
        ].
        _ := level1 value.
    ]
}
