"""Frame types for storing variables, methods, and class records."""

from typing import Any, cast

from builtin_classes.builtin_object import Object
from helper_objects.interfaces.block_interface import Block


class Frame:
    """Abstract base frame mapping names to values."""

    def __init__(self) -> None:
        # dict with keys as variable/method names and values as expressions/blocks
        self.obj_dict: dict[str, Any] = {}

    def write(self, *_args: Any, **_kwargs: Any) -> None:
        """Add a new variable to this frame."""

    def read(self, _name: str) -> Any:
        """Return the value of the variable with the given name."""

    def print(self) -> None:
        """Print the contents of this frame."""


# frame for variables in block
# obj_dict will have keys as variable names and values Objects
class BlockFrame(Frame):
    """Frame holding local variables for a block execution."""

    def __init__(self, parent: Any = None):
        super().__init__()
        # print("Created Block Frame")
        self.parent = parent

    def write(self, name: str, expr: Object) -> None:
        """Store an object under the given variable name."""
        # if name exists in current frame, edit it
        self.obj_dict[name] = expr

    def read(self, name: str) -> Object:
        """Return the object stored under the given variable name."""
        if name in self.obj_dict:
            return cast(Object, self.obj_dict[name])
        raise KeyError(name)

    def print(self) -> None:
        """Print all variables in this block frame."""
        print("---BLOCKFRAME START---")
        for item in self.obj_dict:
            attr_values = {
                a: self.obj_dict[item].attributes[a].obj_value
                for a in self.obj_dict[item].attributes
            }
            if self.obj_dict[item].obj_value is None:
                print(f"'{item}': {self.obj_dict[item]} ... attributes: '{attr_values}'")
            else:
                if self.obj_dict[item].obj_value == "":
                    print(f"'{item}': <empty_string> ... attributes: {attr_values}")
                else:
                    val = self.obj_dict[item].obj_value
                    print(f"'{item}': {val} {self.obj_dict[item]}... attributes: {attr_values}")
        print("---BLOCKFRAME END---")


# Frame for methods in class + object referenced by 'self'
# obj_dict will have keys as method selectors and values will be blocks
class ClassFrame(Frame):
    """Frame holding method blocks for a SOL26 class."""

    def __init__(self, current_class: str, parent_class: str):
        super().__init__()
        self.current_class = current_class
        self.parent_class = parent_class

    def write(self, selector: str, block: Block) -> None:
        """Store a block under the given message selector."""
        self.obj_dict[selector] = block

    def read(self, selector: str) -> Block | Object:
        """Return the block stored under the given selector."""
        return cast(Block | Object, self.obj_dict[selector])

    def print(self) -> None:
        """Print all selectors in this class frame."""
        print("---CLASSFRAME START---")
        for item in self.obj_dict:
            attrs = self.obj_dict[item].attributes
            print(f"'{item}': {(self.obj_dict[item])} ... attributes '{attrs}'")
        print("---CLASSFFRAME END---")


# frame for classes and their parents
# obj_dict has keys as child classes and values as list where first is base class
#  that child inherits from and second are methods that the object uderstands
# {"Main": ClasssRecord[<base_obj>, <methods_dict>]}
class ProgramFrame(Frame):
    """Frame holding class records for the entire SOL26 program."""

    def __init__(self) -> None:
        super().__init__()

    def write(self, child: str, parent: Any) -> None:
        """Store a class record under the given class name."""
        self.obj_dict[child] = parent

    def read(self, child: str) -> Any:
        """Return the class record stored under the given class name."""
        return self.obj_dict[child]

    def print(self) -> None:
        """Print all class records in this program frame."""
        print("---PROGRAMFRAME START---")
        for item in self.obj_dict:
            print(f"'{item}': {(self.obj_dict[item])}")
        print("---PROGRAMFRAME END---")

    def find_object(self, name: str) -> Any | None:
        """Find and return the class record for the given name, or None if not found."""
        for c in self.obj_dict:
            if c == name and name != "Main":
                # print(f"Program frame found {name}, creating {self.obj_dict[c]}")
                return self.obj_dict[c]
            if self.obj_dict[c] == name:
                # print(f"Program frame found {name}, creating {name}")
                return name
        return None
