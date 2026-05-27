"Error 51 / 32 - sending unknown class-level message"
class Main : Object {
    run [|
        x := Integer unknownClassMethod.
    ]
}
