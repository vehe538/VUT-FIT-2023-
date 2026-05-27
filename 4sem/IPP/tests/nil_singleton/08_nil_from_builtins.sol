"nil returned from builtin methods with invalid arguments"
class Main : Object {
    run [|
        x := 'abc' asInteger.
        _ := ((x isNil) asString) print.
        y := 'hello' concatenateWith: 42.
        _ := ((y isNil) asString) print.
        z := 'hello' startsWith: -1 endsBefore: 3.
        _ := ((z isNil) asString) print.
    ]
}
