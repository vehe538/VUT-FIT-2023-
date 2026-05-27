"""Abstract boolean interface for the SOL26 interpreter."""

from abc import abstractmethod
from typing import Any

from builtin_classes.builtin_object import Object, Result


class BoolInterface(Object):
    """Abstract base for SOL26 boolean types (True and False)."""

    def __init__(self, value: Any = None):
        super().__init__(value)

    @abstractmethod
    def as_string(self) -> Result | Object:
        """Return the string representation of this boolean."""

    @abstractmethod
    def not_method(self) -> Result:
        """Return the logical negation of this boolean."""

    @abstractmethod
    def and_method(self, args: list[Any] | None = None) -> Result:
        """Return the logical AND of this boolean with the argument."""

    @abstractmethod
    def or_method(self, args: list[Any] | None = None) -> Result:
        """Return the logical OR of this boolean with the argument."""

    @abstractmethod
    def if_true_if_false(self, args: list[Any] | None = None) -> Result | Object:
        """Execute the true or false branch depending on this boolean value."""

    def is_boolean(self) -> Result:
        """Return True, since this object is a boolean."""
        self.result.value = True
        return self.result
