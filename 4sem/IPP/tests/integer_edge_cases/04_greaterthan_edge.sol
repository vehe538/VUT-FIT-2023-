"greaterThan: edge cases - equal values, negative values"
class Main : Object {
    run [|
        _ := ((5 greaterThan: 5) asString) print.
        _ := ((5 greaterThan: 4) asString) print.
        _ := ((4 greaterThan: 5) asString) print.
        _ := ((-1 greaterThan: -2) asString) print.
        _ := ((-2 greaterThan: -1) asString) print.
        _ := ((0 greaterThan: -1) asString) print.
        _ := ((-1 greaterThan: 0) asString) print.
    ]
}
