"""SOL26 Nil singleton."""

from typing import Any

from builtin_classes.builtin_object import Object, Result


class NilC(Object):
    """Singleton representing the nil value in SOL26."""

    def __init__(self) -> None:
        super().__init__()

    def constructor_new(self, args: list[Any] | None = None) -> Result | Object:
        """Return the Nil singleton."""
        self.result.value = Nil
        return self.result

    def constructor_from(self, args: list[Any] | None = None) -> Result | Object:
        """Return the Nil singleton — Nil cannot be copied from another object."""
        self.result.value = Nil
        return self.result

    def as_string(self) -> Result | Object:
        """Return the string representation 'nil'."""
        return Object("nil")

    def is_nil(self) -> Result:
        """Return True, since this object is nil."""
        self.result.value = True
        return self.result


Nil = NilC()


class NilSubclass(NilC):
    """Singleton representing the nil value in SOL26."""

    def __init__(self) -> None:
        super().__init__()

    def constructor_new(self, args: list[Any] | None = None) -> Result | Object:
        """Return self — NilSubclass cannot be re-instantiated."""
        return self

    def constructor_from(self, args: list[Any] | None = None) -> Result | Object:
        """Return a default Object — NilSubclass cannot be copied from another object."""
        self.result.value = Object()
        return self.result

    def as_string(self) -> Result | Object:
        """Return the string representation 'nil'."""
        return Object("nil")

    def is_nil(self) -> Result:
        """Return True, since this object is nil."""
        self.result.value = True
        return self.result
