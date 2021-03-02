"""
defines Struct metaclass to define structs as named tuples
annotations of fields of these Struct classes must use the other
types defined here like uint16_t etc.
"""
import collections
import struct
import typing

class int8_t(int):
    STRUCT_FORMAT = "b"
class uint8_t(int):
    STRUCT_FORMAT = "B"
class int16_t(int):
    STRUCT_FORMAT = "h"
class uint16_t(int):
    STRUCT_FORMAT = "H"
class int32_t(int):
    STRUCT_FORMAT = "i"
class uint32_t(int):
    STRUCT_FORMAT = "I"
class int64_t(int):
    STRUCT_FORMAT = "q"
class uint64_t(int):
    STRUCT_FORMAT = "Q"
class float_t(float):
    STRUCT_FORMAT = "f"
class double_t(float):
    STRUCT_FORMAT = "d"


# _CLS is used by methods from_bytes and from_file to denote it returns an instance type of the struct.
_CLS = typing.TypeVar("_CLS")

class Struct(type):
    """turns the class into a namedtuple with static methods for constructing from bytes or from file
    all fields should be annotated with one of the other classes from this file like float_t, int8_t etc.
    note that this sets __slots__ = [] to disallow extra fields by default."""
    _struct_format: str
    def __new__(mcls, name, bases, attrs: dict):
        struct_format_string = ""
        fields = []
        for attr,annotated_type in attrs.get("__annotations__",{}).items():
            struct_format = annotated_type.STRUCT_FORMAT # will fail if not using one of the set classes
            struct_format_string += struct_format
            fields.append(attr)
        attrs["_struct_format"] = struct_format_string
        attrs.setdefault("__slots__", ())
        bases = (*bases, collections.namedtuple(name, fields))
        return super(Struct, mcls).__new__(mcls, name, bases, attrs)

    def from_bytes(cls: typing.Type[_CLS], buffer: typing.SupportsBytes) -> _CLS:
        """constructs the named tuple from the bytes according to struct definition"""
        return cls(*struct.unpack(cls._struct_format, buffer))
    def from_file(cls: typing.Type[_CLS], file) -> _CLS:
        """consumes enough bytes from the file to unpack into the struct"""
        return cls.from_bytes(file.read(struct.calcsize(cls._struct_format)))
