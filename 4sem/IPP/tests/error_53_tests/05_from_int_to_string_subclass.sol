"Error 53 - from: on String subclass with Integer argument"
class MyString : String {
    shout [|
        _ := self print.
    ]
}

class Main : Object {
    run [|
        x := MyString from: 99.
    ]
}
