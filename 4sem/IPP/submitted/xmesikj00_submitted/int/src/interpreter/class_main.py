"""Main class handler for the SOL26 interpreter."""

from typing import Any

from builtin_classes.builtin_block import BlockBuiltin
from frame_objects.frame_stack import BlockFrameStack
from frame_objects.frames import Frame
from interpreter.error_codes import ErrorCode
from interpreter.exceptions import InterpreterError


class ClassMain:
    """Manages methods of the Main class and provides the program entry point."""

    def __init__(
            self, root: Any, frame_stack: BlockFrameStack, class_frame: Frame, program_frame: Frame
        ):
        self.current_class_frame = class_frame  # class frame format -> {"selector": Block}
        self.current_root = root
        self.program_frame = program_frame
        self.frame_stack = frame_stack
        self.has_run = False
        # print("Created ClassMain singleton")
        self._parse_methods()

    def _parse_methods(self) -> None:
        #frame_stack = BlockFrameStack()
        for method in self.current_root:
            # for each method: key=method selector, value=block
            selector = method.get("selector")
            block = BlockBuiltin(
                method, self.frame_stack, self.current_class_frame, self.program_frame
            )
            self.current_class_frame.write(selector, block)
            if selector == "run":
                self.has_run = True

    def run(self) -> None:
        """Execute the 'run' method of the Main class."""
        if not self.has_run:
            raise InterpreterError(ErrorCode.SEM_MAIN, "Missing main()")
        # print("===================")
        # print("Called run method")
        method_block = self.current_class_frame.read("run")
        method_block.value()
