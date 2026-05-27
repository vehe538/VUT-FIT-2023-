"from: copies all instance attributes shallowly - spec section 1.2.10"
class Animal : Object {
    initName:sound: [ :n :s |
        _ := self name: n.
        _ := self sound: s.
    ]
    describe [|
        msg := (self name) concatenateWith: ' says '.
        msg := msg concatenateWith: (self sound).
        r := msg.
    ]
}

class Main : Object {
    run [|
        a := Animal new.
        _ := a initName: 'Dog' sound: 'Woof'.
        b := Animal from: a.
        _ := (b describe) print.
        _ := (((a name) equalTo: (b name)) asString) print.
        _ := (((a name) identicalTo: (b name)) asString) print.
        _ := a name: 'Cat'.
        _ := (a name) print.
        _ := (b name) print.
    ]
}
