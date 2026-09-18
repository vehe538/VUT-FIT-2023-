"""SOL26 block (closure) type."""

from typing import Any

from builtin_classes.builtin_object import Object, Result
from builtin_classes.singletons.builtin_nil import Nil
from frame_objects.frames import BlockFrame
from helper_objects.assign import AssignXml
from helper_objects.interfaces.block_interface import Block
from interpreter.error_codes import ErrorCode
from interpreter.exceptions import InterpreterError


class BlockBuiltin(Block):
    """SOL26 block object that can be executed as a closure."""

    def __init__(self, root: Any, frame_stack: Any, class_frame: Any, program_frame: Any):
        super().__init__(root, frame_stack, class_frame, program_frame)
        # fill assign_arr attribute with assings
        self._handle_assigns()
        # fill args attribute with arguments values
        self._handle_args()
        self.frame_stack = frame_stack
        self.class_frame = class_frame
        self.program_frame = program_frame

    def _handle_args(self) -> None:
        for parameter in self.current_root[0]:
            if parameter.tag == "parameter":
                # each item in args array is list - ['name', order]
                self.args.append([parameter.get("name"), parameter.get("order")])

    def _handle_assigns(self) -> None:
        for assign in self.current_root[0]:
            if assign.tag == "assign":
                new_assign = AssignXml(
                    assign, self.frame_stack, self.class_frame, self.program_frame
                )
                self.assign_arr.append(new_assign)

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
