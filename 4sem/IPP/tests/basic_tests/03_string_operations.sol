"Basic string operations: concatenateWith:, length, asString"
class Main : Object {
    run [|
        a := 'Hello'.
        b := ' World'.
        c := a concatenateWith: b.
        _ := c print.
        len := c length.
        _ := (len asString) print.
    ]
}
