"Wildcard _ used multiple times in same block - each _ is independent discard"
class Main : Object {
    run [|
        _ := self compute: 3.
        _ := self compute: 7.
        _ := 'done' print.
    ]
    compute: [ :n |
        r := n multiplyBy: n.
        _ := (r asString) print.
    ]
}
