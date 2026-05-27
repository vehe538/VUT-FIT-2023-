"Error 54 - setting attribute on a regular reference whose name collides with a method"
class Foo : Object {
    bar [|
        _ := 'bar method' print.
    ]
}

class Main : Object {
    run [|
        f := Foo new.
        _ := f bar: 42.
    ]
}
