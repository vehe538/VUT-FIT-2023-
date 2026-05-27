"True new and False new return the singletons - spec section 2"
class Main : Object {
    run [|
        t := True new.
        f := False new.
        _ := ((t identicalTo: true) asString) print.
        _ := ((f identicalTo: false) asString) print.
        _ := (t asString) print.
        _ := (f asString) print.
    ]
}
