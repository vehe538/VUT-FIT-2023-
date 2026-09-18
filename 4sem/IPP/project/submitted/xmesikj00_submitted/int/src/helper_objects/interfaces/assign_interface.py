"""Abstract base class for assignment execution in the SOL26 interpreter."""

from abc import abstractmethod
from typing import Any

from builtin_classes.builtin_object import Object


class Assign:
    """Abstract base defining the interface for an assignment statement."""

    def __init__(self, root: Any, frame_stack: Any, class_frame: Any, program_frame: Any):
        self.name = root[0].get("name")
        self.root = root
        self.frame_stack = frame_stack
        self.class_frame = class_frame
        self.program_frame = program_frame

    @abstractmethod
    def exec(self) -> Object:
        """Execute the assignment and return the resulting object."""
