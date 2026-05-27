"Testing timesRepeat: and divBy:"
class Main : Object {
    run [|
        _ := 5 timesRepeat: [ :i |
            _ := (i asString) print.
        ].
        q := 17 divBy: 3.
        _ := (q asString) print.
    ]
}
