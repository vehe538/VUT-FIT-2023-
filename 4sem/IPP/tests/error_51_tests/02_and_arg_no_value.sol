"Error 51 - and: argument doesn't understand value message"
class Main : Object {
    run [|
        _ := true and: 'not a block'.
    ]
}
