"""SOL26 integer type."""

import copy as cp
from typing import Any, cast

from builtin_classes.builtin_nil import Nil
from builtin_classes.builtin_object import Object, Result
from interpreter.error_codes import ErrorCode


class Integer(Object):
    """SOL26 integer object."""

    def __init__(self, int_value: int = 0):
        super().__init__(int_value)
        self.int_value = int_value

    def constructor_new(self, args: list[Object] | None = None) -> Result | Object:
        """Create a new integer with this object's value."""
        return Integer()

    def constructor_from(self, args: list[Object] | None = None) -> Result | Object:
        """Create a new integer from the first argument's integer value."""
        if args is None or not isinstance(args[0], Integer):
            self.result.error = ErrorCode.INT_INVALID_ARG
            return self.result

        new = Integer(args[0].int_value)
        if hasattr(args[0], "methods"):
            new.methods = args[0].methods
        if hasattr(self, "methods"):
            new.methods = self.methods
        if hasattr(args[0], "attributes"):
            new.attributes = cp.copy(args[0].attributes)
        return new

    def identical_to(self, args: list[Object]) -> Result:
        """Return whether this integer is identical to another."""
        return super().identical_to(args)

    def equal_to(self, args: list[Object] | None = None) -> Result:
        """Return whether this integer equals the first argument."""

        if args is None:
            args = []
        try:
            # print(f"Comparing '{self.int_value}' with '{args[0].int_value}'")
            self.result.value = self.int_value == cast(Any, args[0]).int_value
        except AttributeError:
            # the argument doesnt have int_value as attribute - it is of conflicting type
            self.result.error = ErrorCode.INT_INVALID_ARG
            self.result.err_msg = "Invalid argument for 'equalTo' method.'"

        return self.result

    def greater_than(self, args: list[Object] | None = None) -> Result:
        """Return whether this integer is greater than the first argument."""
        if args is None:
            args = []
        try:
            if self.int_value > cast(Any, args[0]).int_value:
                self.result.value = True
            else:
                self.result.value = False
        except AttributeError:
            self.result.error = ErrorCode.INT_INVALID_ARG
            self.result.err_msg = "Invalid argument for 'greaterThan method.'"

        return self.result

    def plus(self, args: list[Object] | None = None) -> Result:
        """Return the sum of this integer and the first argument."""
        if args is None:
            args = []
        try:
            self.result.value = self.int_value + cast(Any, args[0]).int_value
        except AttributeError:
            self.result.error = ErrorCode.INT_INVALID_ARG
            self.result.err_msg = "Invalid argument for 'plus' method."
        return self.result

    def minus(self, args: list[Object] | None = None) -> Result:
        """Return the difference of this integer minus the first argument."""
        if args is None:
            args = []
        try:
            self.result.value = self.int_value - cast(Any, args[0]).int_value

        except AttributeError:
            self.result.error = ErrorCode.INT_INVALID_ARG
            self.result.err_msg = "Invalid argument for 'minus' method"

        return self.result

    def multiply_by(self, args: list[Object] | None = None) -> Result:
        """Return the product of this integer and the first argument."""
        if args is None:
            args = []
        try:
            self.result.value = self.int_value * cast(Any, args[0]).int_value
        except AttributeError:
            self.result.error = ErrorCode.INT_INVALID_ARG
            self.result.err_msg = "Invalid argument for 'multiplyBy' method."

        return self.result

    def div_by(self, args: list[Object] | None = None) -> Result:
        """Return the integer quotient of this integer divided by the first argument."""
        if args is None:
            args = []
        try:
            if not hasattr(args[0], "int_value"):
                raise AttributeError

            self.result.value = self.int_value // args[0].int_value

        except AttributeError:
            self.result.err_msg = "Invalid argument for 'divBy' method."
            self.result.error = ErrorCode.INT_INVALID_ARG

        except ZeroDivisionError:
            self.result.err_msg = "Division by zero."
            self.result.error = ErrorCode.INT_INVALID_ARG

        return self.result

    def as_string(self) -> Result | Object:
        """Return the string representation of this integer."""
        return Object(str(self.int_value))

    def as_integer(self) -> Result | Object:
        """Return this integer object."""
        return self

    def times_repeat(self, args: list[Object] | None = None) -> Result | Object:
        """Execute the given block a number of times equal to this integer."""
        if args is None:
            args = []
        try:
            # args[0] must be of type Block
            if self.int_value > 0:
                for i in range(1, self.int_value + 1):
                    last = cast(Any, args[0]).value([Integer(i)])

                return Object(last.value)
            return Nil

        except AttributeError:
            # receiver is not Block object - wrong argument
            self.result.error = ErrorCode.INT_INVALID_ARG

        return self.result

    def is_number(self) -> Result:
        """Return True, since this object is a number."""
        self.result.value = True
        return self.result
