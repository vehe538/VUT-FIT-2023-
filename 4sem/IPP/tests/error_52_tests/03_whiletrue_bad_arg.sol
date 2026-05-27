"Error 52 - whileTrue: given argument that does not understand value"
class Main : Object {
    run [|
        _ := [| r := true. ] whileTrue: 42.
    ]
}
