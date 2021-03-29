# with open("P21", "rb") as f:
#     x = f.read()

# for idx in range(0,200, 8):
#     for a,b in enumerate(x[idx:idx+8]):
#         print(f"{b:02x}", end=" " if a==3 else "")
#     print()
import warnings
import itertools
import struct
import typing
try:
    from .log_parser import EntryHeader
except ImportError:
    from log_parser import EntryHeader
    
class SectionTerminatedEarly(StopIteration):
    "subclass of StopIteration, used to indicate a partial entry was found and to stop scanning"
def pairwise(iterable):
    # from https://docs.python.org/3/library/itertools.html
    "s -> (s0,s1), (s1,s2), (s2, s3), ..."
    a, b = itertools.tee(iterable)
    next(b, None)
    return zip(a, b)

class log_file_parser():
    """
    this entire class is written to deal with the improbable case where part
    of an entry is written to the log file then crashes, resets and keeps logging valid data
    without corrupting the SD card.  To support this case we need to limit the reading
    so that it will never exceed the next known reset index and any partial entries are discarded
    also ensuring that the entry right at the reset index is actually a reset."""
    header_size = struct.calcsize(EntryHeader._struct_format)
    def __init__(self,log_file, list_of_offsets, current_offset = 0):
        self.log_file = log_file
        self.list_of_reset_offsets = [*list_of_offsets, float("inf")]
        self.current_offset = current_offset
        self.current_end_boundary = float("inf")
    def _read(self, size):
        "reads bytes from file but not past self.current_end_boundary"
        read_size = min([size, self.current_end_boundary - self.current_offset])
        data = self.log_file.read(read_size)
        self.current_offset += len(data)
        if len(data) < size: 
            # either we are trying to read past current_end_boundary
            # or we reached end of file, either way we aren't reading as much data as we need.
            if data: # not direct end of section
                warnings.warn("partial data found")
            raise SectionTerminatedEarly
        return data
    def _next_entry(self, *, ensure_reset=False) -> typing.Tuple[EntryHeader, bytes]:
        """
        returns (header, payload) for the next entry, where header is an EntryHeader and payload is bytes
        if the end of file or end of section is reached it returns (None, None) instead

        if ensure_reset is true and the type of header does not match a reset then this throws a ValueError
        stating the 'section did not start with valid reset command'.
        """
        try:
            header_bytes = self._read(self.header_size)
            header =  EntryHeader.from_bytes(header_bytes)
            # need error here in order to prevent reading potentially massive payload and getting issues from that
            if header.type != 0 and ensure_reset:
                raise ValueError("section did not start with valid reset command")
            payload = self._read(header.length)
            if header.type == 0 and not ensure_reset:
                # weird case where we have a reset that is not pointed to by the offset file
                # this may cause entries between this and the next reset in the file to be skipped
                warnings.warn("reset found not at section boundary, likely skipping entries")
                return (None, None)
            return (header, payload)
        except SectionTerminatedEarly:
            return (None, None)

    def _iterate_reset_section(self, start_offset, end_offset):
        "iterates entries until reset is reached"
        self.current_end_boundary = end_offset
        if self.current_offset > start_offset:
            raise ValueError("cannot backtrack in file, seeking would need to be implemented for that")
        elif self.current_offset < start_offset:
            warnings.warn("discarding excess data")
            # read excess data until starting point, discarding it
            # TODO: probably use seek here instead, have to consider implications for CLI code.
            self._read(start_offset-self.current_offset)
        [first_entry,empty_payload] = self._next_entry(ensure_reset=True)
        assert first_entry.type == 0 and len(empty_payload) == 0, "first entry after reset index is not reset"
        return iter(self._next_entry, (None,None))
    
    def iterate_resets(self):
        "iterates over each reset in the file, producing an iterator for the entries for each reset"
        for start,end in pairwise(self.list_of_reset_offsets):
            section = self._iterate_reset_section(start, end)
            yield section
            # in order to ensure reusing old iterators gives sufficiently good errors
            # we exhaust before continuing to the next section
            # ideally we could cache this or something but there isn't much point
            # this is not necessary under typical circumstances, it would be something like iterating over sections and saving
            # each iterator to go in parallel or something, this won't work with the architecture we have but give non
            # intuitive results because of how we manage state, so we ensure this case fails by exhausting them as we go
            # TODO: could override end_section so it terminates immidiately instead of making unused entries into a list.
            list(section) # use up remaining iterator if there is any left.


