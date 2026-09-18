"""This module contains class definition of Result wrapper."""

from typing import Any


class Result:
    """Wrapper object for results of SOL methods' executions."""

    def __init__(self, value: Any = None, error: int | None = None, err_msg: str = "") -> None:
        self.value = value
        self.error = error
        self.err_msg = err_msg

    def ok(self) -> bool:
        """Return True if there was no semantic error while executing a method."""
        return self.error is None
