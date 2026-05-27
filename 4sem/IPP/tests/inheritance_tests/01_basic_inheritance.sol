"Basic method inheritance - child uses parent method"
class Animal : Object {
    speak [|
        _ := 'Some animal sound' print.
    ]
    describe [|
        _ := 'I am an animal' print.
    ]
}

class Dog : Animal {
    speak [|
        _ := 'Woof!' print.
    ]
}

class Cat : Animal {
    speak [|
        _ := 'Meow!' print.
    ]
}

class Main : Object {
    run [|
        d := Dog new.
        c := Cat new.
        _ := d speak.
        _ := d describe.
        _ := c speak.
        _ := c describe.
    ]
}
