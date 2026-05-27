"equalTo: can be redefined in subclass for custom comparison - spec section 2"
class Point : Object {
    initX:y: [ :px :py |
        _ := self x: px.
        _ := self y: py.
    ]
    equalTo: [ :other |
        xEq := (self x) equalTo: (other x).
        yEq := (self y) equalTo: (other y).
        r := xEq and: [| r := yEq. ].
    ]
}

class Main : Object {
    run [|
        p1 := Point new.
        _ := p1 initX: 3 y: 4.
        p2 := Point new.
        _ := p2 initX: 3 y: 4.
        p3 := Point new.
        _ := p3 initX: 1 y: 2.
        _ := ((p1 equalTo: p2) asString) print.
        _ := ((p1 equalTo: p3) asString) print.
        _ := ((p1 identicalTo: p2) asString) print.
    ]
}
