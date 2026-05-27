"Empty string literal - length 0, print nothing"
class Main : Object {
    run [|
        s := ''.
        _ := s print.
        _ := ((s length) asString) print.
        s2 := s concatenateWith: 'hello'.
        _ := s2 print.
    ]
}
