"""emtpy block for the SOL26 interpreter."""

from typing import Any

from builtin_classes.builtin_object import Object, Result
from builtin_classes.singletons.builtin_nil import Nil
from frame_objects.frames import BlockFrame
from helper_objects.interfaces.block_interface import Block


class EmptyBlock(Block):
    """Emtpy block when receiving message 'new'"""

    def __init__(self, root: Any, frame_stack: Any, class_frame: Any, program_frame: Any):
        # print(f"Creating new block object for {root.get('selector')} method.", self)
        self.current_root = root
        self.block_value = None
        self.assign_arr = []
        self.args = []
        self.result = Result()
        self.attributes: dict[str, Any] = {}

        # create new frame and push it onto a stack
        self.frame_stack = frame_stack
        self.class_frame = class_frame
        self.program_frame = program_frame
        self.current_frame = None

    def _handle_assigns(self) -> None:
        pass

    def _handle_args(self) -> None:
        pass

    def constructor_new(self, args: Any = None) -> Result:
        """Return self wrapped in a Result."""
        self.result.value = self
        return self.result

    def value(self, args: list[Object] | None = None, frame: BlockFrame | None = None) -> Result:
        """Return Nil — empty block always evaluates to Nil."""
        self.result.value = Nil
        return self.result

    def while_true(self, arg: Any) -> Result:
        """Execute the argument block while this block evaluates to true."""
        self.result.value = None
        return self.result

    def is_block(self) -> Result:
        """Return True — this object is a block."""
        self.result.value = True
        return self.result
