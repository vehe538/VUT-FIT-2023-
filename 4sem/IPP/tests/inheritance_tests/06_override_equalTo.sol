"Overriding equalTo: in a subclass for custom comparison"
class NamedObject : Object {
    initName: [ :n |
        _ := self name: n.
    ]
    equalTo: [ :other |
        r := (self name) equalTo: (other name).
    ]
}

class Main : Object {
    run [|
        a := NamedObject new.
        _ := a initName: 'Alice'.
        b := NamedObject new.
        _ := b initName: 'Alice'.
        c := NamedObject new.
        _ := c initName: 'Bob'.
        _ := ((a equalTo: b) asString) print.
        _ := ((a equalTo: c) asString) print.
    ]
}
