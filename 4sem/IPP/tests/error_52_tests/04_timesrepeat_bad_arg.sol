"Error 52 - timesRepeat: given non-block argument that does not understand value:"
class Main : Object {
    run [|
        _ := 3 timesRepeat: 'not a block'.
    ]
}
