"""SOL26 block (closure) type."""

from typing import Any

from builtin_classes.builtin_nil import Nil
from builtin_classes.builtin_object import Object, Result
from frame_objects.frames import BlockFrame
from helper_objects.assign import Assign
from helper_objects.interfaces.block_interface import Block
from interpreter.error_codes import ErrorCode
from interpreter.exceptions import InterpreterError


class BlockBuiltin(Block):
    """SOL26 block object that can be executed as a closure."""

    def __init__(self, root: Any, frame_stack: Any, class_frame: Any, program_frame: Any):
        super().__init__(root, frame_stack, class_frame, program_frame)
        # fill assign_arr attribute with assings
        # fill args attribute with arguments values
        self._handle_args()
        self._handle_assigns()

    def _handle_args(self) -> None:
        for parameter in self.current_root[0]:
            params_names = {arg[0] for arg in self.args}
            if parameter.tag == "parameter":
                pname = parameter.get("name")
                porder = parameter.get("order")
                if pname in params_names:
                    raise InterpreterError(
                        ErrorCode.SEM_ERROR,
                        f"Formal block parameter '{pname}' collision detected."
                    )
                # each item in args array is list - ['name', order]
                self.args.append([pname, porder])

    def _handle_assigns(self) -> None:
        params_names = {arg[0] for arg in self.args}
        for assign in self.current_root[0]:

            if assign.tag == "assign":
                new_assign = Assign(
                    assign, self.frame_stack, self.class_frame, self.program_frame
                )
                self.assign_arr.append(new_assign)
                if new_assign.name in params_names:
                    n = new_assign.name
                    raise InterpreterError(
                        ErrorCode.SEM_COLLISION, f"Attempt to write into formal parameter '{n}'."
                    )

    def value(self, args: list[Any] | None = None, frame: BlockFrame | None = None) -> Result:
        """Execute this block with the given arguments and optional frame."""
        if args is None:
            args = []


        new_frame = BlockFrame()

        if frame is not None:
            for k, v in frame.obj_dict.items():
                new_frame.write(k, v)
        try:
            for arg in self.args:
                #new_frame.write(arg[0], args[int(arg[1]) - 1])
                new_frame.obj_dict[arg[0]] = args[int(arg[1]) - 1]
        except IndexError as err:
            raise InterpreterError(ErrorCode.INT_DNU, "Bad block arity.") from err

        self.frame_stack.push(new_frame)
        self.current_frame = new_frame

        try:
            last = None
            for ass in self.assign_arr:
                last = ass.exec()
        finally:
            self.frame_stack.pop()

        if last is None:
            self.result.value = Nil
            return self.result

        self.block_value = last.obj_value
        self.result.value = last

        return self.result

    def while_true(self, args: list[Block]) -> Result | Object:
        """Repeatedly execute arg block while this block's value is true."""
        try:
            self.value()
            while self.block_value:
                args[0].value()
                self.value()

            return Object(self.value().value)

        except AttributeError:
            self.result.error = ErrorCode.INT_INVALID_ARG
            self.result.err_msg = "Method 'whitetrue' received invalid args."
        return self.result
