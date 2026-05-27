"Error 52 - concatenateWith: with non-string returns nil, then length on nil"
class Main : Object {
    run [|
        s := 'Hello'.
        result := s concatenateWith: 42.
        _ := result length.
    ]
}
