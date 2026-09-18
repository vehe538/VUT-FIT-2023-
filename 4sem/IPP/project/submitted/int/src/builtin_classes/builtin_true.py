"""SOL26 True singleton."""

from typing import Any, cast

from builtin_classes.builtin_object import Object, Result
from helper_objects.interfaces.boolean_interface import BoolInterface
from interpreter.error_codes import ErrorCode


class TrueC(BoolInterface):
    """Singleton representing the boolean true value in SOL26."""

    def __init__(self, true_val: bool = True):
        super().__init__(true_val)
        self.true_val = true_val

    def as_string(self) -> Result | Object:
        """Return the string representation 'true'."""
        return Object("true")

    def not_method(self) -> Result:
        """Return the logical negation (False)."""
        self.result.value = False
        return self.result

    def and_method(self, args: list[Any] | None = None) -> Result:
        """Return the value of the argument block."""
        if args is None:
            args = []
        try:
            if hasattr(args[0], "value"):
                return cast(Result, args[0].value())

            self.result.error = ErrorCode.INT_DNU
            self.result.err_msg = "Argument of 'and' did not understand the message 'value'."
        except AttributeError:
            self.result.error = ErrorCode.INT_INVALID_ARG
            self.result.err_msg = "Method 'and' received invalid arguments."

        return self.result

    def or_method(self, args: list[Any] | None = None) -> Result:
        """Return True (short-circuit: true or anything is true)."""
        self.result.value = True
        return self.result

    def if_true_if_false(self, args: list[Any] | None = None) -> Result | Object:
        """Execute the true branch (first argument block)."""
        if args is None:
            args = []

        try:
            if hasattr(args[0], "value") and hasattr(args[1], "value"):
                self.result.value = args[0].value().value
                return self.result

            self.result.error = ErrorCode.INT_DNU
            self.result.err_msg = (
                "Arguments of 'ifTrueifFalse' did not understand the message 'value'."
            )

        except AttributeError:
            self.result.error = ErrorCode.INT_INVALID_ARG
            self.result.err_msg = "Method 'ifTrueifFalse' received invalid arguments."

        return self.result

    def is_nil(self) -> Result:
        """Return 'False', since this object is not nil."""
        self.result.value = False
        return self.result

true = TrueC()




