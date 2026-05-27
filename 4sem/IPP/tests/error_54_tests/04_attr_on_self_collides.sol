"Error 54 - setting attribute on self whose name matches a defined method"
class Counter : Object {
    increment [|
        _ := 'incrementing' print.
    ]
    run [|
        _ := self increment: 5.
    ]
}

class Main : Object {
    run [|
        c := Counter new.
        _ := c run.
    ]
}
