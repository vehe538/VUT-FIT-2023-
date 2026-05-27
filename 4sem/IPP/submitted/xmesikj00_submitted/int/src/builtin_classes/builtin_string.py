"""SOL26 string type."""

import copy as cp
from typing import Any, cast

import interpreter.input_stream as input_stream
from builtin_classes.builtin_object import Object, Result
from builtin_classes.singletons.builtin_nil import Nil
from interpreter.error_codes import ErrorCode


class String(Object):
    """SOL26 string object."""

    def __init__(self, str_value: str = ""):
        super().__init__(str_value)
        self.str_value = str_value

    def constructor_new(self) -> Result | Object:
        """Create a new empty string."""
        return String()

    def constructor_from(self, args: list[Object] | None = None) -> Result | Object:
        """Create a new string from the first argument's string value."""
        if args is None or not isinstance(args[0], String):
            self.result.error = ErrorCode.INT_INVALID_ARG
            return self.result

        new = String(args[0].str_value)
        if hasattr(args[0], "methods"):
            new.methods = args[0].methods
        if hasattr(args[0], "attributes"):
            new.attributes = cp.copy(args[0].attributes)

        return new

    def read(self) -> Result:
        """Read a string from standard input and return a new String instance."""
        self.result.value = input_stream.stream.readline().rstrip("\n")
        return self.result

    def method_print(self) -> Object:
        """Print this string to stdout and return self."""
        # print("--OUTPUT--")
        print(self.str_value, end="")
        # print("--END OF OUTPUT--")
        return self

    def equal_to(self, args: list[Object] | None = None) -> Result:
        """Return whether this string equals the first argument."""
        if args is None:
            args = []
        try:
            if self.str_value == cast(Any, args[0]).obj_value:
                self.result.value = True
            else:
                self.result.value = False
        except AttributeError:
            self.result.error = ErrorCode.INT_INVALID_ARG
            self.result.err_msg = "Invalid argument for method 'equalTo'."

        return self.result

    def identical_to(self, args: list[Object]) -> Result:
        """Return whether this string is identical to another."""
        return super().identical_to(args)

    def as_string(self) -> Result | Object:
        """Return this string object."""
        return self

    def as_integer(self) -> Result | Object:
        """Convert this string to an integer, or Nil if not numeric."""
        if len(self.str_value) == 0:
            return Nil
        if self.str_value.isdigit():
            return Object(int(self.str_value))
        if self.str_value[0] == "-" and self.str_value[1:].isdigit():
            return Object(int(self.str_value))
        return Nil

    def concatenate_with(self, args: list[Object] | None = None) -> Result:
        """Return the concatenation of this string with the first argument."""
        if args is None:
            args = []
        try:
            self.result.value = self.str_value + cast(Any, args[0]).str_value
        except AttributeError:
            self.result.value = Nil

        return self.result

    def starts_with_ends_before(self, args: list[Object] | None = None) -> Result:
        """Return a substring from start to end (exclusive bounds), or Nil for invalid indices."""
        if args is None:
            args = []
        try:
            start = cast(Any, args[0]).int_value
            end = cast(Any, args[1]).int_value
            if start <= 0 or end <= 0:
                self.result.value = Nil
            else:
                if end - start < 0:
                    self.result.value = ""
                else:
                    if end > len(self.str_value):
                        self.result.value = self.str_value[start - 1 :]
                    else:
                        self.result.value = self.str_value[start - 1 : end - 1]
        except AttributeError:
            self.result.error = ErrorCode.INT_INVALID_ARG

        return self.result

    def length(self) -> Object:
        """Return the length of this string as an Integer."""
        return Object(len(self.str_value))

    def is_string(self) -> Result:
        """Return 'true' - self is a String"""
        self.result.value = True
        return self.result
