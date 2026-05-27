"Negative integer literals and arithmetic with negatives"
class Main : Object {
    run [|
        a := -5.
        b := -10.
        _ := (a asString) print.
        _ := (b asString) print.
        sum := a plus: b.
        _ := (sum asString) print.
        diff := a minus: b.
        _ := (diff asString) print.
        _ := ((a greaterThan: b) asString) print.
        _ := ((b greaterThan: a) asString) print.
    ]
}
