"Using undefined class NonExistent - should fail with error 32"
class Main : Object {
    run [|
        x := NonExistent new.
    ]
}
