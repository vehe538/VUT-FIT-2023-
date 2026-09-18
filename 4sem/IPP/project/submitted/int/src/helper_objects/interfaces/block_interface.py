"""Abstract block interface for the SOL26 interpreter."""

from abc import abstractmethod
from typing import Any

from builtin_classes.builtin_object import Object, Result


# interface for Block
class Block(Object):
    """Abstract base for all SOL26 block types."""

    def __init__(self, root: Any, frame_stack: Any, class_frame: Any, program_frame: Any):
        super().__init__()
        # print(f"Creating new block object for {root.get('selector')} method.", self)
        self.current_root = root
        self.block_value: Any = None
        self.attributes: dict[str, Any] = {}
        # array of all assigns inside this block
        self.assign_arr: list[Any] = []
        # array of arguements of this block
        self.args: list[Any] = []

        # create new frame and push it onto a stack
        self.frame_stack = frame_stack
        self.class_frame = class_frame
        self.program_frame = program_frame
        self.current_frame: Any = None

    # this function fills 'assign_arr' attribute with Assign objects
    @abstractmethod
    def _handle_assigns(self) -> None:
        pass

    @abstractmethod
    def _handle_args(self) -> None:
        pass

    @abstractmethod
    def value(self, args: list[Any] | None = None, frame: Any = None) -> Result | None:
        """Execute this block with the given arguments."""

    @abstractmethod
    def while_true(self, arg: Any) -> Result | Object:
        """Execute the argument block while this block evaluates to true."""

    def is_block(self) -> Result:
        """Return True — this object is a block."""
        self.result.value = True
        return self.result
