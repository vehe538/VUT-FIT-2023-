"""Base SOL26 object type."""

import copy as cp
from typing import Any

from helper_objects.wrappers.result_wrapper import Result
from interpreter.error_codes import ErrorCode

__all__ = ["Object", "Result"]


class Object:
    """Base object from which all SOL26 types inherit."""

    def __init__(self, value: Any = None, is_receiver: bool = False):
        self.obj_value = value
        self.result = Result()
        self.attributes: dict[str, Any] = {}
        self.methods: dict[str, Any] = {}
        self.is_receiver = is_receiver

    # def __str__(self):
    #    return f"Object with value: {self.obj_value} and attributes: {self.attributes}"

    def _set_attribute(self, name: str, value: Any) -> None:
        self.attributes[name] = value

    def constructor_new(self) -> Result | Object:
        """Create a new instance using this object's value."""
        self.result.value = self.obj_value
        return self.result

    def constructor_from(self, args: list[Any] | None = None) -> Result | Object:
        """Create a new instance from the first argument's value."""
        if args is None:
            self.result.error = ErrorCode.INT_INVALID_ARG
            return self.result

        new = Object(args[0].obj_value)
        if hasattr(args[0], "methods"):
            new.methods = args[0].methods
        if hasattr(args[0], "attributes"):
            new.attributes = cp.copy(args[0].attributes)

        return new

    def identical_to(self, args: list[Any]) -> Result:
        """Return whether this object is identical to another."""
        self.result.value = args[0] is self
        return self.result

    def equal_to(self, args: list[Any] | None = None) -> Result:
        """Return whether this object equals the first argument - invocation of identical_to"""
        self.result = self.identical_to(args if args is not None else [])
        return self.result

    def as_string(self) -> Result | Object:
        """Return string representation of this object."""
        if self.obj_value is None:
            self.result.value = ""
        else:
            self.result.value = str(self.obj_value)
        return self.result

    def is_number(self) -> Result:
        """Return whether this object is a number."""
        self.result.value = False
        return self.result

    def is_string(self) -> Result:
        """Return whether this object is a string."""
        self.result.value = False
        return self.result

    def is_block(self) -> Result:
        """Return whether this object is a block."""
        self.result.value = False
        return self.result

    def is_nil(self) -> Result:
        """Return whether this object is nil."""
        self.result.value = False
        return self.result

    def is_boolean(self) -> Result:
        """Return whether this object is a boolean."""
        self.result.value = False
        return self.result
