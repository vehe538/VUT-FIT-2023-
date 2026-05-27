"or: short circuit - if receiver is true, block not evaluated"
class Main : Object {
    run [|
        executed := false.
        result := true or: [|
            executed := true.
            r := false.
        ].
        _ := (result asString) print.
        _ := (executed asString) print.
        executed2 := false.
        result2 := false or: [|
            executed2 := true.
            r := true.
        ].
        _ := (result2 asString) print.
        _ := (executed2 asString) print.
    ]
}
