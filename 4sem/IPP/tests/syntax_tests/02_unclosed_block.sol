"Unclosed block bracket - block literal must be closed with ]"
class Main : Object {
    run [|
        b := [| x := 1.
        _ := b value.
    ]
}
