"""Class record for tracking SOL26 class definitions and their method blocks."""

from typing import Any, cast

from builtin_classes.builtin_block import BlockBuiltin
from frame_objects.frame_stack import BlockFrameStack
from frame_objects.frames import ClassFrame, ProgramFrame
from interpreter.error_codes import ErrorCode
from interpreter.exceptions import InterpreterError


class ClassRecord:
    """Holds the class hierarchy information and method blocks for a SOL26 class."""

    def __init__(self, root: Any,
        frame_stack: BlockFrameStack,
        class_frame: ClassFrame,
        program_frame: ProgramFrame):

        self.current_class_frame = class_frame  # class frame -> {"selector": Block}
        self.current_root = root
        self.program_frame = program_frame
        self.frame_stack = frame_stack
        self._base_types = ["Object", "Integer", "String", "TrueC", "FalseC", "NilC"]
        # print("Created new Class Record with name: ", self.current_root.get("name"))

        self.base_obj = self._base_lookup()
        self.parent_class = self._parent_lookup()
        self._parse_methods()

    def __str__(self) -> str:
        tmp_methods = self.current_class_frame.obj_dict
        return (
            f"ClassRecord '{self.current_class_frame.current_class}' "
            f"with base object '{self.base_obj}', "
            f"parent class '{self.parent_class}' and methods '{tmp_methods}'"
        )

    # this method defines base object for this class by recursively searching up the program frame
    def _base_lookup(self) -> str | None:
        if self.current_class_frame.parent_class in self._base_types:
            return self.current_class_frame.parent_class

        if self.current_class_frame.parent_class in ["Nil", "True", "False"]:
            # if parent class is Nil/True/False, its Subclass is created so it can have
            # a bit different behavior than its singleton instance
            return self.current_class_frame.parent_class + "Subclass"

        for record in self.program_frame.obj_dict:
            tmp_parent = self.current_class_frame.parent_class
            if tmp_parent not in self.program_frame.obj_dict:
                raise InterpreterError(
                    ErrorCode.SEM_UNDEF, f"Parent class '{tmp_parent}' is undefined."
                )
            if record == tmp_parent:
                return cast(str | None, self.program_frame.obj_dict[record]._base_lookup())

        raise InterpreterError(ErrorCode.SEM_ERROR, "Circular inheritance detected.")

    def _parent_lookup(self) -> str:
        tmp = self.current_root.get("parent")
        if tmp in ["Nil", "True", "False"]:
            tmp += "C"
        if tmp in self._base_types:
            return cast(str, tmp)

        for c in self.program_frame.obj_dict:
            if tmp == c:
                return cast(str, tmp)

        raise InterpreterError(ErrorCode.SEM_UNDEF, f"Parent class '{tmp}' is undefined.")

    def _parse_methods(self) -> None:
        #frame_stack = BlockFrameStack()
        for method in self.current_root:
            # for each method: key=method selector, value=block

            selector = method.get("selector")
            # check for arity mismatch
            if selector.count(":") != int(method[0].get("arity")):
                raise InterpreterError(
                    ErrorCode.SEM_ARITY,
                    f"Method '{selector}' has wrong arity of '{method[0].get("arity")}'."
                )

            block = BlockBuiltin(
                method, self.frame_stack, self.current_class_frame, self.program_frame
            )
            self.current_class_frame.write(selector, block)

    def methods_extension(self, methods: dict[str, Any] | None = None,
            rec: ClassRecord | None = None) -> dict[str, Any]:
        """Return a merged dict of all method blocks from this class and its ancestors."""
        if methods is None:
            methods = self.current_class_frame.obj_dict
            rec = self

        if rec is not None:
            if rec.parent_class in self._base_types:
                return methods

            parent_methods = self.program_frame.read(rec.parent_class).current_class_frame.obj_dict
            # methods from current frame have precedence over same-name methods from parent
            methods = parent_methods | methods

            rec = self.program_frame.read(rec.parent_class)
            return self.methods_extension(methods, rec)
        return {}
