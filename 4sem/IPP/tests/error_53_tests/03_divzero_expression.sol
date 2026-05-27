"Error 53 - division by zero produced via expression result"
class Main : Object {
    run [|
        a := 10.
        b := 5.
        divisor := a minus: b.
        divisor := divisor minus: 5.
        _ := 100 divBy: divisor.
    ]
}
