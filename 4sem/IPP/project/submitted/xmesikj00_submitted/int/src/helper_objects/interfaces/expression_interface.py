"""Base expression class for the SOL26 interpreter."""

from abc import abstractmethod
from typing import Any

from builtin_classes.builtin_object import Object


class Expression:
    """Abstract base for all expression types evaluated during interpretation."""

    def __init__(
        self,
        root: Any,
        frame_stack: Any,
        class_frame: Any,
        program_frame: Any,
        is_receiver: bool = False,
    ):
        self.frame_stack = frame_stack
        self.class_frame = class_frame
        self.program_frame = program_frame
        self.root = root
        self.is_receiver = is_receiver

    @abstractmethod
    def get_value(self) -> Object:
        """Evaluate the expression and return the resulting object."""
        ...
