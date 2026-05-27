"Basic integer arithmetic using plus:, minus:, multiplyBy:"
class Main : Object {
    run [|
        a := 10.
        b := 3.
        sum := a plus: b.
        diff := a minus: b.
        prod := a multiplyBy: b.
        _ := (sum asString) print.
        _ := (diff asString) print.
        _ := (prod asString) print.
    ]
}
