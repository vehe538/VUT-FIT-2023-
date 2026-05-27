"""emtpy block for the SOL26 interpreter."""

from typing import Any

from builtin_classes.builtin_nil import Nil
from builtin_classes.builtin_object import Object, Result
from frame_objects.frames import BlockFrame
from helper_objects.interfaces.block_interface import Block


class EmptyBlock(Block):
    """Emtpy block when receiving message 'new'"""

    def __init__(self, root: Any, frame_stack: Any, class_frame: Any, program_frame: Any) -> None:
        super().__init__(root, frame_stack, class_frame, program_frame)

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
