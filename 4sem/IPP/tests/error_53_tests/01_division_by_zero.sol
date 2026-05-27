"Error 53 - division by zero"
class Main : Object {
    run [|
        x := 10.
        y := 0.
        _ := x divBy: y.
    ]
}
