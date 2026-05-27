"Block new creates empty parameterless block - spec section 2"
class Main : Object {
    run [|
        b := Block new.
        result := b value.
        _ := (result asString) print.
        _ := ((b isBlock) asString) print.
    ]
}
