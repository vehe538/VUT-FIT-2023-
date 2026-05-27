"and: short circuit - if receiver is false, block not evaluated"
class Main : Object {
    run [|
        executed := false.
        result := false and: [|
            executed := true.
            r := true.
        ].
        _ := (result asString) print.
        _ := (executed asString) print.
        executed2 := false.
        result2 := true and: [|
            executed2 := true.
            r := true.
        ].
        _ := (result2 asString) print.
        _ := (executed2 asString) print.
    ]
}
