"length of empty string is 0, escape sequence counts as 1 - spec section 2"
class Main : Object {
    run [|
        _ := (('' length) asString) print.
        _ := (('a' length) asString) print.
        _ := (('hello' length) asString) print.
        _ := (('\n' length) asString) print.
        _ := (('\\' length) asString) print.
        _ := (('\'' length) asString) print.
        _ := (('a\nb' length) asString) print.
    ]
}
