"""
contains relevent library for parsing out log file on wearable device

when called from terminal takes a local file containing a log file and it writes (potentially overriding)
csv files for each type of data collection.
"""

import struct
import typing

try:
    from . import struct_types as types
except ImportError:
    import struct_types as types
    
###########################################################################
### Raw Data Entries

class EntryHeader(metaclass=types.Struct):
    """header for each entry of data as logged in the filesystem
    Attributes:
        timestamp   timestamp since last reset in milliseconds
        type        type as found in _entry_type_names
        legnth      length of payload in bytes, payload directly follows header
    """
    timestamp:  types.uint32_t
    type:       types.uint16_t
    length:     types.uint16_t

def parse_raw_entries(fileio: typing.BinaryIO) -> typing.Iterator[typing.Tuple[EntryHeader, bytes]]:
    "yields tuple of (header, payload) for each entry of a log file. payload is in bytes"
    header_size = struct.calcsize(EntryHeader._struct_format)
    def next_header(): return fileio.read(header_size)
    # iterate over headers until empty string is returned meaning end of file is reached.
    for header_bytes in iter(next_header, b''):
        assert len(header_bytes) == header_size, f"got unexpected trailing data: {header_bytes!r}"
        header = EntryHeader.from_bytes(header_bytes)
        payload_bytes = fileio.read(header.length)
        assert len(payload_bytes) == header.length, "file ended mid payload"
        yield (header, payload_bytes)

### Raw Data Entries
###########################################################################
### Payloads

class ResetDataPayload(metaclass=types.Struct):
    "reset command, has no data in payload"
    pass

class TimeDataPayload(metaclass=types.Struct):
    NotImplemented
class VoltageDataPayload(metaclass=types.Struct):
    NotImplemented
class TemperatureDataPayload(metaclass=types.Struct):
    NotImplemented

class ImuDataPayload(metaclass=types.Struct):
    "contains IMU data for acceleration and gyroscope"
    accel_x:    types.int32_t # 0.1 mg per LSB
    accel_y:    types.int32_t
    accel_z:    types.int32_t

    gyro_x:     types.int32_t # 1 m°/s per LSB
    gyro_y:     types.int32_t
    gyro_z:     types.int32_t

    temperature:types.int32_t # 1 m°C per LSB

class LogMessagePayload(metaclass=types.Struct):
    "Text Log Message"
    message: types.string
    @classmethod
    def from_bytes(cls, buffer: bytes):
        return cls(buffer.decode())

AnyEntry = typing.Union[
    ResetDataPayload,   TimeDataPayload,
    VoltageDataPayload, TemperatureDataPayload,
    ImuDataPayload,     LogMessagePayload]

entry_type_payloads: typing.Dict[int, AnyEntry] = {
    0x00: ResetDataPayload,
    0x01: TimeDataPayload,
    0x02: VoltageDataPayload,
    0x03: TemperatureDataPayload,
    0x04: ImuDataPayload,
    0x05: LogMessagePayload,
}
### Payloads
###########################################################################
### main parser and command line utility.

FIELDS_BEFORE_PAYLOAD = ["reset_idx", "timestamp"]
def parse_data(entries_iterator: typing.Iterator[typing.Tuple[EntryHeader, bytes]]):
    """main parser for data, takes iterator in form that parse_raw_entries produces.
    yields tuples (type, *F, *P) where:
    - type is the type index, can be used as key in the entry_type_payloads dict.
    - P is the payload fields, as described for each payload type.
    - F is the extra fields in FIELDS_BEFORE_PAYLOAD
        reset_idx: starts at 0 and increments each reset action
        timestamp: local timestamp for the given reset session
    """
    entries_iterator = iter(entries_iterator) # looping structure really relies on it being an iterator not iterable
    reset_idx = -1
    while True: # broken by end of entries_iterator

        reset_idx += 1

        for header, payload_bytes in entries_iterator:
            if entry_type_payloads[header.type] is ResetDataPayload:
                break # break out of inner loop and go back to while True loop
            # else: handle data entry
            payload: AnyEntry = entry_type_payloads[header.type].from_bytes(payload_bytes)

            yield (header.type, reset_idx, header.timestamp, *payload)
        else: # if the entries_iterator ended (loop ended on it's own)
            return # reached end of file so we are done the function.

def write_multifile(file: typing.BinaryIO):
    """writes a csv file for each type of collected data"""
    open_files = {}
    for type_index, payload_cls in entry_type_payloads.items():
        if payload_cls is ResetDataPayload:
            continue # don't make a file for resets, we don't produce those events anyway
        filename = f"collected_{payload_cls.__name__.rpartition('Payload')[0]}.csv"
        f = open_files[type_index] = open(filename, "w")
        print(*FIELDS_BEFORE_PAYLOAD, *payload_cls._fields, sep=",", file=f)
    try:
        for [type, *fields] in parse_data(parse_raw_entries(file)):
            print(*fields, sep=",", file=open_files[type])
    finally:
        for f in open_files.values(): f.close()


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input_file", type=argparse.FileType('rb'))
    ns = parser.parse_args()
    with ns.input_file:
        write_multifile(ns.input_file)