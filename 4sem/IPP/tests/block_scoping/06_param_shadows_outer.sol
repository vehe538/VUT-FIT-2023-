"Block parameter with same name as outer variable - parameter takes precedence"
class Main : Object {
    run [|
        x := 'outer'.
        b := [ :x |
            _ := x print.
        ].
        _ := b value: 'inner'.
        _ := x print.
    ]
}
