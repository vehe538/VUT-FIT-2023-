"Zero as integer literal and edge arithmetic"
class Main : Object {
    run [|
        z := 0.
        _ := (z asString) print.
        _ := ((z plus: 5) asString) print.
        _ := ((z minus: 3) asString) print.
        _ := ((z multiplyBy: 100) asString) print.
        _ := ((z greaterThan: -1) asString) print.
        _ := ((z greaterThan: 1) asString) print.
        _ := ((z equalTo: 0) asString) print.
    ]
}
