"Testing instance attributes - setting and reading via messages"
class Main : Object {
    run [|
        _ := self name: 'Alice'.
        _ := self age: 30.
        _ := (self name) print.
        _ := ((self age) asString) print.

        "Modify existing attribute"
        _ := self age: 31.
        _ := ((self age) asString) print.
    ]
}
