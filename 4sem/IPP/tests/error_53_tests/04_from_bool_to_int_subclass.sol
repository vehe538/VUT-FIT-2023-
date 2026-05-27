"Error 53 - from: on Integer subclass with a True object"
class MyInt : Integer {
    doubled [|
        r := self multiplyBy: 2.
    ]
}

class Main : Object {
    run [|
        x := MyInt from: true.
    ]
}
