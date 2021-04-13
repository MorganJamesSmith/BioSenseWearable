"""
this contains code to create invalid log files and test that they throw the expected errors

the design goal is to catch what warnings are raised and assert they are the ones we expect
and then also compare the actual output to previous runs of the test cases, to ensure regression
on the recovery techniques
"""
import io
import struct
import traceback
import warnings
try:
    from .temp import log_file_parser
    from .log_parser import entry_type_payloads
except ImportError:
    from temp import log_file_parser
    from log_parser import entry_type_payloads

test_cases = []
def testcase(f):
    "decorator for test cases, adds them to the test_cases set"
    test_cases.append(f)
    return f

def entry(timestamp, type, payload=b'', *, override_length=None):
    "returns bytes packed for entry"
    if override_length:
        length = override_length
    else:
        length = len(payload)
    return struct.pack("IHH", timestamp, type, length) + payload
valid_entry_data_after_timestamp = [4, 0,0,0, 0,0,0, 0]
def valid_entry(timestamp):
    "returns a valid IMU data entry"
    return entry(timestamp, 0x04, struct.pack("7i", *valid_entry_data_after_timestamp[1:]))

RESET_ENTRY = entry(0,0)
RESET_MARKER = {}
START_MARKER = {}
RESET = (RESET_MARKER, RESET_ENTRY)

## Helpers
#####################################################################
## reset cases
@testcase
def test_must_start_with_reset():
    "terminal error if the first entry is not a reset"
    process_log_data([
        valid_entry(100),
        START_MARKER,
        RESET_MARKER,
        valid_entry(200)
    ])
    assert output == []
    assert error == "section did not start with valid reset command"
@testcase
def test_offset_must_be_reset():
    process_log_data([
        *RESET,
        valid_entry(100),
        valid_entry(200),
        RESET_MARKER,
        valid_entry(300)
    ])
    # only output before valid reset marker is accepted
    assert output == [
        (0,100,*valid_entry_data_after_timestamp),
        (0,200,*valid_entry_data_after_timestamp)
    ]
    assert error == "section did not start with valid reset command"
# @testcase
# def test_recover_of_invalid_reset():
#     "not really a goal but checks we can get data after an invalid reset offset"

#     process_log_data([
#         *RESET,
#         valid_entry(100),
#         valid_entry(200),
#         RESET_MARKER,
#         valid_entry(300),
#         valid_entry(350),
#         *RESET,
#         valid_entry(100),
#         valid_entry(200),
#     ])
#     # only output before valid reset marker is accepted
#     assert output == [
#         (0,100,*valid_entry_data_after_timestamp),
#         (0,200,*valid_entry_data_after_timestamp),
#         # entries within invalid reset marker ignored
#         (1,100,*valid_entry_data_after_timestamp),
#         (1,200,*valid_entry_data_after_timestamp)
#     ]
#     assert error == "section did not start with valid reset command"
@testcase
def test_reset_without_marker():
    "tests that a reset without a marker in the OFF file is warned and ignored"
    process_log_data([
        *RESET,
        valid_entry(100),
        entry(150, 0), # reset, timestamp between so we don't expect any out of order timing errors
        valid_entry(200), # this entry is skipped in this case
        *RESET,
        valid_entry(101),
    ])
    assert not error
    assert output == [
        (0,100,*valid_entry_data_after_timestamp),
        (1,101,*valid_entry_data_after_timestamp),
    ]
    assert thrown_warnings == [
        'reset found not at section boundary, likely skipping entries',
        "discarding excess data"]

## reset cases
#####################################################################
## headers / cutoff data
@testcase
def test_header_cutoff():
    "tests when the header of an entry is cutoff at the end f the file"
    process_log_data([
        *RESET,
        valid_entry(100),
        entry(200, 4)[-1:] # cut off last byte
    ])
    assert not error
    assert output == [(0,100,*valid_entry_data_after_timestamp)]
    assert thrown_warnings == ['partial data found']
@testcase
def test_payload_cutoff():
    "tests when the payload of an entry is cutoff at the end of the file"
    process_log_data([
        *RESET,
        valid_entry(100),
        valid_entry(200)[-2:] # cut off last byte
    ])
    assert not error

    assert output == [(0,100,*valid_entry_data_after_timestamp)]
    assert thrown_warnings == ['partial data found']
@testcase
def test_payload_cutoff_midfile():
    "tests when a payload is corrupted at the end of a reset section"
    process_log_data([
        *RESET,
        valid_entry(100),
        valid_entry(200)[-2:], # cut off last byte
        *RESET,
        valid_entry(150)
    ])
    assert not error
    assert output == [
        (0,100,*valid_entry_data_after_timestamp),
        (1,150,*valid_entry_data_after_timestamp)
    ]
    assert thrown_warnings == ['partial data found']

## header tests
#####################################################################
## payload types

@testcase
def test_invalid_type():
    "tests an unimplemented type of entry"
    process_log_data([
        *RESET,
        valid_entry(100),
        entry(200, 10, b'abcd'),
        valid_entry(300)
    ])
    assert not error
    assert output == [
        (0, 100, *valid_entry_data_after_timestamp),
        (0, 300, *valid_entry_data_after_timestamp)
    ]
    assert thrown_warnings == ["unexpected type: 10"]
    
@testcase
def test_too_long_length():
    "tests that length in file that does not match fields expected is skipped"
    process_log_data([
        *RESET,
        valid_entry(100),
        # needs to be longer than 28 bytes for IMU entry
        entry(200, 4, b'super_long_data_abcdefghijklmnopqrstuvwxyz1234567890'),
        valid_entry(300)
    ])
    assert not error
    assert output == [
        (0, 100, *valid_entry_data_after_timestamp),
        (0, 300, *valid_entry_data_after_timestamp)
    ]
    assert thrown_warnings == ["unpack requires a buffer of 28 bytes"]

@testcase
def test_too_short_length():
    "tests that length in file that is smaller than expected is skipped"
    process_log_data([
        *RESET,
        valid_entry(100),
        # needs to be shorter than 28 bytes for IMU entry
        entry(200, 4, b'abc'),
        valid_entry(300)
    ])
    assert not error
    assert output == [
        (0, 100, *valid_entry_data_after_timestamp),
        (0, 300, *valid_entry_data_after_timestamp)
    ]
    assert thrown_warnings == ["unpack requires a buffer of 28 bytes"]

## payload types
#####################################################################
## timing / sanity checking
@testcase
def test_timestamp_out_of_order():
    "tests that warnings are given when timestamps are more than a few ms out of order"
    process_log_data([
        *RESET,
        valid_entry(200),
        valid_entry(100)
    ])
    assert output == [
        (0, 200, *valid_entry_data_after_timestamp),
        (0, 100, *valid_entry_data_after_timestamp)
    ]
    assert not error
    # TODO this should actually throw warnings???
    assert not thrown_warnings
@testcase
def ensure_starting_at_reset_but_not_at_beginning_works_as_expected():
    NotImplemented # TODO

## timing / sanity checking
#####################################################################
## CLI
@testcase
def fail_when_cli_gives_Failed_to_open_file_message():
    NotImplemented
@testcase
def test_detect_weird_symbol_and_retry():
    """sometimes there is a character at the beginning of the serial connection
    it is interpreted as part of the command and fails"""
    NotImplemented # also not sure how to implement?
@testcase
def test_read_back_command_no_error_when_first_letter_is_truncated():
    NotImplemented

## CLI
#####################################################################
## runner

output = []
error = ""
thrown_warnings = []
def wrap_warn(msg, *, OLD_WARN=warnings.warn):
    thrown_warnings.append(str(msg))
    return OLD_WARN(msg)
warnings.warn = wrap_warn
def process_log_data(data):
    "processes log data and sets global variables output and errors"
    global error
    error = ""
    output[:] = []
    thrown_warnings[:] = []
    log_data = b""
    offsets = []
    start_offset = 0
    for entry in data:
        if entry is RESET_MARKER:
            offsets.append(len(log_data))
        elif entry is START_MARKER:
            start_offset = len(log_data)
        else:
            log_data += entry
    
    parser = log_file_parser(io.BytesIO(log_data), offsets, start_offset)
    try:
        for idx,section in enumerate(parser.iterate_resets()):
            print("HERE!")
            for [header,payload_bytes] in section:
                print("HERE!")
                try:
                    payload = entry_type_payloads[header.type].from_bytes(payload_bytes)
                except KeyError as e:
                    warnings.warn(f"unexpected type: {e}")
                except struct.error as e:
                    warnings.warn(e)
                else:
                    output.append((idx, header.timestamp, header.type, *payload))
    except Exception as e:
        error = str(e)
        print("SET ERROR: ", e)


def run_all_test():
    failed_names = []
    for func in test_cases:
        print("running", func.__name__)
        try:
            func()
        except Exception as e:
            traceback.print_exc()
            failed_names.append(func.__name__)
        else:
            print("passed")
    print(f"\n\n TEST CASES PASSED: {len(test_cases)-len(failed_names)}/{len(test_cases)}")
    if failed_names:
        print("\n LIST OF FAILED TESTS:\n")
        print(*failed_names, sep="\n")
    else:
        print(" ALL PASSED")

if __name__ == "__main__":
    run_all_test()
