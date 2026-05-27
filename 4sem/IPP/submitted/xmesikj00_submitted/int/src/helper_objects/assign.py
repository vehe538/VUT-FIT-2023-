"""Assignment and expression classes for SOL26 interpretation."""

import re
from typing import Any, cast

from builtin_classes.builtin_integer import Integer
from builtin_classes.builtin_object import Object
from builtin_classes.builtin_string import String
from builtin_classes.singletons.builtin_false import FalseSubclass, false
from builtin_classes.singletons.builtin_nil import Nil, NilSubclass
from builtin_classes.singletons.builtin_true import TrueSubclass, true
from frame_objects.frames import BlockFrame
from helper_objects.empty_block import EmptyBlock
from helper_objects.interfaces.assign_interface import Assign
from helper_objects.interfaces.block_interface import Block
from helper_objects.interfaces.expression_interface import Expression
from helper_objects.wrappers.result_wrapper import Result
from interpreter.error_codes import ErrorCode
from interpreter.exceptions import InterpreterError

# Mapping of special SOL selectors to their Python method names
_BUILTIN_SELECTORS: dict[str, str] = {
    "from:": "constructor_from",
    "new": "constructor_new",
    "or:": "or_method",
    "and:": "and_method",
    "not": "not_method",
    "ifTrue:ifFalse:": "if_true_if_false",
    "startsWith:endsBefore:": "starts_with_ends_before",
    "print": "method_print",
}


class AssignXml(Assign):
    """Concrete assignment that parses and executes an XML assign node."""

    def __init__(self, root: Any, frame_stack: Any, class_frame: Any, program_frame: Any) -> None:
        super().__init__(root, frame_stack, class_frame, program_frame)
        self.expr: Any = None

        match root[1][0].tag:
            case "literal":
                # create literal object from builin class based on literal's 'class' attribute
                self.expr = ExprLiteral(root[1], frame_stack, class_frame, program_frame)
            case "var":
                self.expr = ExprVariable(root[1], frame_stack, class_frame, program_frame)
            case "send":
                self.expr = ExprMessage(root[1], frame_stack, class_frame, program_frame)
            case "block":
                self.expr = ExprBlock(root[1], frame_stack, class_frame, program_frame)

    # execution of assign means get value of right side of the assignment
    # and writing it into a frame to variable on the right side of the assignment
    def exec(self) -> Object:
        """Evaluate the right-hand expression and store the result in the current frame."""
        self.current_frame = self.frame_stack.top()
        # first, whole expression is evaluated
        result_obj = self.expr.get_value()  # vracia Objekt
        # then result of expression is written into current frame
        # print(f"writing '{self.name}' to frame")
        self.current_frame.write(self.name, result_obj)
        #print("---")
        for _ in self.frame_stack.stack:
            pass
            #_.print()
        #print("---")

        return cast(Object, result_obj)


class ExprLiteral(Expression):
    """
    <literal class='' value=0/>
    """

    def __init__(self, root: Any, frame_stack: Any, class_frame: Any, program_frame: Any) -> None:
        super().__init__(root, frame_stack, class_frame, program_frame)

    def _resolve_class_object(self, literal_value: str) -> Object:
        """Instantiate the SOL26 object for a class-typed literal."""
        tmp = self.program_frame.find_object(literal_value)
        if tmp is None:
            try:
                if literal_value == "Block":
                    return EmptyBlock(
                        self.root, self.frame_stack, self.class_frame, self.program_frame
                    )
                if literal_value == "True":
                    return true
                if literal_value == "False":
                    return false
                if literal_value == "Nil":
                    return Nil
                return cast(Object, globals()[literal_value]())
            except KeyError:
                raise InterpreterError(
                    ErrorCode.SEM_UNDEF, f"Class '{literal_value}' is undefined."
                ) from None
        new: Object
        match tmp.base_obj:
            case "NilSubclass":
                new = NilSubclass()
            case "TrueSubclass":
                new = TrueSubclass()
            case "FalseSubclass":
                new = FalseSubclass()
            case _:
                new = cast(Object, globals()[tmp.base_obj]())
        new_methods = tmp.methods_extension()
        new_methods.update({"self": new})
        new.methods = new_methods
        return new

    def get_value(self) -> Object:
        """Instantiate and return the SOL26 object described by this literal."""
        literal_class = self.root[0].get("class")
        literal_value = self.root[0].get("value")

        new = Object()
        match literal_class:
            case "class":
                new = self._resolve_class_object(literal_value)
            case "Integer":
                new = Integer(int(literal_value))
            case "String":
                new = String(literal_value)
            case "True":
                new = true
            case "False":
                new = false
            case "Nil":
                new = Nil

        return new


class ExprVariable(Expression):
    """
    <expr>
        <var name=''/>
    </expr>
    """

    def __init__(
        self,
        root: Any,
        frame_stack: Any,
        class_frame: Any,
        program_frame: Any,
        is_receiver: bool = False,
    ) -> None:
        super().__init__(root, frame_stack, class_frame, program_frame, is_receiver)
        # print("Created new Variable Expression object")

    def get_value(self) -> Object:
        """Look up and return the object bound to this variable name."""
        self.current_frame = self.frame_stack.top()

        var_name = self.root[0].get("name")

        if var_name == "self" or var_name == "super":
            # create an Object only if 'self' is not yet created inside current class frame
            try:
                new = self.current_frame.read(var_name)
                self.class_frame.write(var_name, new)
                return cast(Object, new)

            except KeyError:
                if var_name == "self":
                    tmp = self.program_frame.read(self.class_frame.current_class)
                    try:
                        copy = self.class_frame.read("self")
                    except KeyError:
                        copy = globals()[tmp.base_obj]()
                        new_methods = tmp.methods_extension()
                        copy.methods = new_methods

                    #new_methods = tmp.methods_extension()
                    #copy.methods = new_methods
                    # print(f"Setting methods '{new_methods}' for '{copy}'")
                    # setattr(copy, "methods", new_methods)

                elif var_name == "super":
                    if not self.is_receiver:
                        # if super is not receiver it acts like 'self'
                        try:
                            copy = self.current_frame.read("self")
                        except KeyError:
                            tmp = self.program_frame.read(self.class_frame.current_class)
                            copy = globals()[tmp.base_obj]()

                        self.class_frame.write("self", copy)
                        self.current_frame.write("self", copy)
                        return cast(Object, copy)

                    tmp = self.program_frame.read(self.class_frame.parent_class)
                    copy = globals()[tmp.base_obj]()

                    # print("Creating SUPER of class: ", self.class_frame.current_class)
                    # print(tmp)
                    new_methods = tmp.methods_extension()
                    copy.methods = new_methods
                    # this try-except block is for encoutering "super" while "self"
                    # for current class frame wasn't yet created.

                    # "super" allows access to parent class, but it needs
                    # to know correct value of "self" - that of current class.
                    # print(f"Setting methods '{new_methods}' for '{copy}'")

                self.class_frame.write(var_name, copy)
                self.current_frame.write(var_name, copy)
                return cast(Object, copy)
        else:
            # make a COPY of the object that has 'var_name' as key in current frame
            try:
                copy = self.current_frame.read(var_name)
            except KeyError:
                # if not, search all other frames of the stack (block frames only)
                copy = None
                for f in self.frame_stack.stack:
                    if var_name in f.obj_dict:
                        copy = f.read(var_name)
                        break
                    #copy = None

                if copy is None:
                    raise InterpreterError(
                        ErrorCode.SEM_UNDEF, f"Unknown variable '{var_name}'"
                    ) from None

        return cast(Object, copy)


class ExprMessage(Expression):
    """
    <expr>
       <send selector=''/>
    </expr>
    """

    def __init__(self, root: Any, frame_stack: Any, class_frame: Any, program_frame: Any) -> None:
        super().__init__(root, frame_stack, class_frame, program_frame)
        # print("Created new Message Sending Expression object")

    # helper function for overloading check
    def _check_collision(self, sel: str, new: Object) -> None:
        getter = sel.rstrip(":")
        try:
            self.class_frame.read(getter)
            raise InterpreterError(
                ErrorCode.INT_INST_ATTR,
                f"Attribute colliding with method selector: '{getter}'",
            )
        except (KeyError):
            pass
        try:
            if sel.rstrip(":") in new.methods:
                raise InterpreterError(
                    ErrorCode.INT_INST_ATTR,
                    f"Attribute colliding with method selector: '{getter}'",
                )
        except AttributeError:
            pass

    def _get_receiver(self) -> list[Any]:
        send_root = self.root[0]
        receiver_copy: Any = None
        copy_name = ""
        for c in send_root:
            if c.tag == "expr":
                # determine receiver based on XML node tag
                match c[0].tag:
                    case "var":
                        copy_name = c[0].get("name")
                        receiver_copy = ExprVariable(
                            send_root[0],
                            self.frame_stack,
                            self.class_frame,
                            self.program_frame,
                            is_receiver=True,
                        )
                    case "literal":
                        copy_name = c[0].get("class")
                        receiver_copy = ExprLiteral(
                            send_root[0], self.frame_stack, self.class_frame, self.program_frame
                        )
                    case "send":
                        copy_name = c[0].get("selector")
                        receiver_copy = ExprMessage(
                            send_root[0], self.frame_stack, self.class_frame, self.program_frame
                        )
                    case "block":
                        copy_name = f"Block with arity ${c[0].get('arity')})"
                        receiver_copy = ExprBlock(
                            send_root[0], self.frame_stack, self.class_frame, self.program_frame
                        )
        return [copy_name, receiver_copy]

    def _get_args(self) -> list[Any]:
        send_root = self.root[0]
        args: list[Any] = []
        for a in send_root:
            if a.tag == "arg":
                # keep correct order of arguments
                order = int(a.get("order"))
                match a[0][0].tag:
                    case "var":
                        args.insert(
                            order - 1,
                            ExprVariable(
                                a[0], self.frame_stack, self.class_frame, self.program_frame
                            ).get_value(),
                        )
                    case "literal":
                        args.insert(
                            order - 1,
                            ExprLiteral(
                                a[0], self.frame_stack, self.class_frame, self.program_frame
                            ).get_value(),
                        )
                    case "send":
                        args.insert(
                            order - 1,
                            ExprMessage(
                                a[0], self.frame_stack, self.class_frame, self.program_frame
                            ).get_value(),
                        )
                    case "block":
                        args.insert(
                            order - 1,
                            ExprBlock(
                                a[0], self.frame_stack, self.class_frame, self.program_frame
                            ).get_value(),
                        )
        return args

    def _selector_to_msg(self, selector: str, new: Object) -> str:
        """Map a SOL selector string to the Python method name."""
        if selector in _BUILTIN_SELECTORS:
            return _BUILTIN_SELECTORS[selector]
        if hasattr(new, "methods") and selector != "self" and selector in new.methods:
            return selector
        if re.match(r"^value:?", selector):
            return "value"
        # convert camelCase selector to snake_case, stripping ':' separators
        tmp = selector.replace(":", "")
        return re.sub(r"([A-Z])", r"_\1", tmp).lower()

    def _handle_unknown_selector(
        self, msg: str, selector: str, new: Object, copy_name: str, args: list[Object]
    ) -> Object | None:
        """Check collision and handle attribute-setting for non-builtin selectors.

        Returns the object if an attribute was set (caller should return it immediately),
        or None to continue with normal message sending.
        """
        #
        # check if method is one of builtin Object methods
        if hasattr(new, msg) and callable(getattr(new, msg)):
            return None  # msg is already set correctly
        if selector in new.attributes:

            return cast(Object, new.attributes[selector])
        try:
            # current class check
            self.class_frame.read(selector)

        except KeyError:
            try:
                # check if the message is from inherited methods
                new.methods[selector]
            except (KeyError, AttributeError):
                self._check_collision(selector, new)

                # if it's not even a user-defined method
                # attribute of an object is defined
                if copy_name not in self.frame_stack.top().obj_dict:
                    new = Object()
                else:
                    new = self.frame_stack.top().read(copy_name)
                if len(args) == 1:
                    if isinstance(args[0], ExprBlock):
                        args[0].current_frame = self.frame_stack.top()
                    # print(f"Setting attribute '{msg}' for object '{new}'")
                    # attribute contains one argument - setting attribute
                    new._set_attribute(msg, args[0])
                    return new
        return None

    def _send_message(
        self, new: Object, msg: str, selector: str, copy_name: str, args: list[Object]
    ) -> Result | Object:
        """Send message to receiver; fall back through user-defined, inherited, and attributes."""
        # try to send message to new copy
        try:
            if args == []:
                return cast(Result | Object, getattr(new, msg)())
            return cast(Result | Object, getattr(new, msg)(args))
        except TypeError as err:
            raise InterpreterError(
                ErrorCode.INT_INST_ATTR,
                f"Attribute name '{selector}' is colliding with method '{msg}'",
            ) from err
        except AttributeError:
            # if it's user-defined method
            # send 'value' message to its corresponding block
            try:
                # first check receiver's current class
                # print("Before before calling", copy_name, new)
                tmp_block = new.methods[selector]
                new_block_frame = BlockFrame()
                new.methods["self"] = new
                new_block_frame.write("self", new)
                # print(
                #    f"Before calling '{selector}' of '{copy_name}' object",
                #    f"Setting 'self' to '{new}'",
                #    f"methods of new: '{new.methods}'\n "
                # )
                #print(
                #        f"Sending message: '{selector}' to copy of '{copy_name}' "
                #        f"with frame: '{new_block_frame.obj_dict}' and args '{args}'"
                #    )
                #tmp_block = self.class_frame.read(selector)
                return cast(Result | Object, tmp_block.value(args, new_block_frame))
            except (KeyError, AttributeError):
                try:
                    # then check current class
                    tmp_block = self.class_frame.read(selector)
                    return cast(Result | Object, tmp_block.value(args))

                except (AttributeError, KeyError):
                    # if not, we are finally reading the object's attribute
                    try:
                        #print(new.attributes)
                        #print(
                        #    f"Reading attribute of '{new}' called '{msg}', "
                        #    f"its value is ''"
                        #)
                        return cast(Result | Object, new.attributes[msg])

                    except KeyError as err:
                        # if message isn't builtin method, user-defined method nor custom
                        # attribute, message is not recognized - errors 32/51
                        if copy_name == "class":
                            raise InterpreterError (
                                ErrorCode.SEM_UNDEF,
                                f"Undefined class method '{msg}' for '{type(new)}.'"
                            ) from err
                        raise InterpreterError(
                            ErrorCode.INT_DNU,
                            f"There's no attribute of '{copy_name}' or method called '{msg}'.",
                        ) from err

    def _process_result(self, result: Result, new: Object) -> Object:
        """Process a method's Result into the final returned Object."""
        # result of the operation has 'value' and 'error' fields to
        # distinguish when the message returns integer value and when error value
        if result.ok():
            match result.value:
                case True:
                    return true
                case False:
                    return false
                case _:
                    if isinstance(result.value, str):
                        new = String(result.value)
                    elif isinstance(result.value, int):
                        new = Integer(result.value)
                    elif result.value is Nil:
                        return Nil
                    elif isinstance(result.value, Object):
                        return result.value

            new.obj_value = result.value

        else:
            raise InterpreterError(cast(ErrorCode, result.error), result.err_msg)
        return new

    def get_value(self) -> Object:
        """Send a message to the receiver and return the resulting object."""
        selector = self.root[0].get("selector")

        copy_name, receiver = self._get_receiver()
        receiver_copy = Object()

        # make a copy of the receiver, so new object with new value is written to the frame
        if isinstance(receiver, ExprBlock):
            receiver_copy = type(receiver)(
                receiver.current_root, self.frame_stack, receiver.class_frame, self.program_frame
            )
        elif type(receiver) in [Object, String, Integer]:
            receiver_copy = type(receiver)(receiver.obj_value)
        elif receiver is false or receiver is true or receiver is Nil:
            receiver_copy = receiver
        else:
            # print(copy_name)
            receiver_copy = receiver

        args = self._get_args()

        # this returns Object()
        new = receiver_copy.get_value()

        # convert selector to method name
        msg = self._selector_to_msg(selector, new)

        # print(f"Sending message: '{selector}' to '{copy_name}' with args")
        # for non-builtin selectors: check collision and handle attribute-setting
        if selector not in _BUILTIN_SELECTORS:
            early = self._handle_unknown_selector(msg, selector, new, copy_name, args)
            if early is not None:
                return early

        # send message and get result
        outcome = self._send_message(new, msg, selector, copy_name, args)
        # print("Recevier from send_message: ", new)

        # if outcome is not a Result, it was an attribute read — return directly
        if not isinstance(outcome, Result):
            if isinstance(outcome, String):
                return outcome
            if isinstance(outcome, Integer):
                return outcome
            if isinstance(outcome.obj_value, str):
                outcome = String(outcome.obj_value)
            elif isinstance(outcome.obj_value, int):
                outcome = Integer(outcome.obj_value)
            return outcome
        return self._process_result(outcome, new)


class ExprBlock(Block):
    """
    <expr>
        <block arity="">
           ...
        </block>
    </expr>
    """

    def __init__(self, root: Any, frame_stack: Any, class_frame: Any, program_frame: Any) -> None:
        super().__init__(root, frame_stack, class_frame, program_frame)
        # print("Created new Block Expression object ", self)
        self.result = Result()
        self._handle_args()
        self._handle_assigns()

    def _handle_assigns(self) -> None:
        for assign in self.current_root[0]:
            if assign.tag == "assign":
                new_assign = AssignXml(
                    assign, self.frame_stack, self.class_frame, self.program_frame
                )
                self.assign_arr.append(new_assign)

    def _handle_args(self) -> None:
        for parameter in self.current_root[0]:
            if parameter.tag == "parameter":
                # each item in args array is list - ['name', order]
                self.args.append([parameter.get("name"), parameter.get("order")])

    def value(self, args: list[Object] | None = None, frame: Any = None) -> Result:
        """Execute this block with the given arguments and return the last result."""
        if args is None:
            args = []

        new_frame = BlockFrame()
        # from actual frame write to new frame - enabling closures
        if self.current_frame is not None:
            for k,v in self.current_frame.obj_dict.items():
                if k == "self":
                    continue
                new_frame.write(k,v)

        # store parameter names
        params_names = {arg[0] for arg in self.args}
        for arg in self.args:
            try:
                new_frame.write(arg[0], args[int(arg[1]) - 1])
            except IndexError as err:
                raise InterpreterError(ErrorCode.INT_DNU, "Bad block arity.") from err

        self.frame_stack.push(new_frame)

        last = None
        for a in self.assign_arr:
            last = a.exec()

        if self.current_frame is not None:
            for k in self.current_frame.obj_dict:
                # writing to current frame everything but param_names-named data
                # this way if parameters' names match var names from
                # outer frame - parameters have precedence
                if k in new_frame.obj_dict and k not in params_names:
                    self.current_frame.write(k, new_frame.obj_dict[k])

        self.frame_stack.pop()
        if last is None:
            self.block_value = Nil
            self.result.value = Nil
            return self.result

        if isinstance(last, ExprBlock):
            self.block_value = last
            self.result.value = last
            return self.result

        self.block_value = last.obj_value
        self.result.value = last.obj_value

        return self.result

    def get_value(self) -> Object:
        """
        In case a block is expression, get_value returns this block object intself.
        """
        if self.current_frame is None:
            self.current_frame = self.frame_stack.top()
        return self

    def while_true(self, args: list[Block]) -> Result | Object:
        """Repeatedly execute arg block while this block's value is true."""
        try:
            self.value()
            last: Result | Object | None = Nil
            while self.block_value:
                last = args[0].value()
                self.value()

            if not isinstance(last, Result):
                return Nil
            return Object(last.value)

        except AttributeError:
            self.result.error = ErrorCode.INT_INVALID_ARG
            self.result.err_msg = "Method 'whileTrue' received invalid args."
        return self.result
