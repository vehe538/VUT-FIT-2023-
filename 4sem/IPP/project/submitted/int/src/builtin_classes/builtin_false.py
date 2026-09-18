"""SOL26 False singleton."""

from typing import Any, cast

from builtin_classes.builtin_object import Object, Result
from helper_objects.interfaces.boolean_interface import BoolInterface
from interpreter.error_codes import ErrorCode


class FalseC(BoolInterface):
    """Singleton representing the boolean false value in SOL26."""

    def __init__(self, false_val: bool = False):
        super().__init__(false_val)
        self.false_val = false_val

    def as_string(self) -> Result | Object:
        """Return the string representation 'false'."""
        return Object("false")

    def not_method(self) -> Result:
        """Return the logical negation (True)."""
        self.result.value = True
        return self.result

    def and_method(self, args: list[Any] | None = None) -> Result:
        """Return False (short-circuit: false and anything is false)."""
        self.result.value = False
        return self.result

    def or_method(self, args: list[Any] | None = None) -> Result:
        """Return the value of the argument block."""
        if args is None:
            args = []
        try:
            if hasattr(args[0], "value"):
                return cast(Result, args[0].value())

            self.result.error = ErrorCode.INT_DNU
            self.result.err_msg = "Argument of 'or' did not understand the message 'value'."

        except AttributeError:
            self.result.error = ErrorCode.INT_INVALID_ARG
            self.result.err_msg = "Method 'or' received invalid arguments."

        return self.result

    def if_true_if_false(self, args: list[Any] | None = None) -> Result | Object:
        """Execute the false branch (second argument block)."""
        if args is None:
            args = []
        try:
            if hasattr(args[0], "value") and hasattr(args[1], "value"):
                self.result.value = args[1].value().value
                return self.result

            self.result.error = ErrorCode.INT_DNU
            self.result.err_msg = (
                "Arguments of 'ifTrueifFalse' did not understand the message 'value'."
            )

        except AttributeError:
            self.result.error = ErrorCode.INT_INVALID_ARG
            self.result.err_msg = "Method 'ifTrueIfFalse' received invalid arguments"

        return self.result

    def is_nil(self) -> Result:
        """Return 'False', since this object is not nil."""
        self.result.value = False
        return self.result

false = FalseC()

