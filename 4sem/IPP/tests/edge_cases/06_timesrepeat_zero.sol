"timesRepeat: with 0 - body never executes, returns nil per spec"
class Main : Object {
    run [|
        result := 0 timesRepeat: [ :i |
            _ := 'should not print' print.
            r := i.
        ].
        _ := (result asString) print.
    ]
}
