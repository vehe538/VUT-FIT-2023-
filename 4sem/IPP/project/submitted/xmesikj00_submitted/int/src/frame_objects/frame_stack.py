"""Frame stack implementations for the SOL26 interpreter."""

from frame_objects.frames import Frame


class FrameStack:
    """Generic stack of frame objects."""

    def __init__(self) -> None:
        # list of Frame objects acting as stack
        self.stack: list[Frame] = []

    def pop(self) -> None:
        """Remove the top frame from the stack."""
        self.stack.pop(0)

    def push(self, new_frame: Frame) -> None:
        """Push a new frame onto the top of the stack."""
        # print("Pushed new frame to stack.")
        self.stack.insert(0, new_frame)

    def top(self) -> Frame:
        """Return the frame at the top of the stack."""
        return self.stack[0]


class ClassFrameStack(FrameStack):
    """Frame stack for class-level frames."""

    def __init__(self) -> None:
        super().__init__()


class BlockFrameStack(FrameStack):
    """Frame stack for block-level frames."""

    def __init__(self) -> None:
        super().__init__()
