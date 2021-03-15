"""
implements a wrapper for loading live data from the serial connection and passing it to plotting

"""
import serial

try:
    from . import log_parser
except ImportError:
    import log_parser

class LiveLogFile():
    def __init__(self, serial_device_name: str="/dev/ttyACM0"):
        self.serial_device_name = serial_device_name
        self.internal_buffer: bytes = b""
        self.log_file_offset: int = 0
    def read(self, nbytes=1):
        if len(self.internal_buffer) < nbytes:
            new_data = self.read_from_device()
            self.internal_buffer  = self.internal_buffer + new_data
            if len(self.internal_buffer) < nbytes:
                import warnings
                warnings.warn("reading data from device didn't produce enough content to keep going.")
        togive = self.internal_buffer[:nbytes]
        self.internal_buffer = self.internal_buffer[nbytes:]
        return togive  

    def read_from_device(self):
        print("READING FROM DEVICE")
        with serial.Serial(self.serial_device_name) as conn:
            command_to_send = f"hcat P21 {self.log_file_offset}\n\r".encode()
            conn.write(command_to_send)
            data_we_just_sent_and_want_to_ignore = conn.read_until("\n\r")
            if command_to_send != data_we_just_sent_and_want_to_ignore:
                import warnings; warnings.warn("sent: {command_to_send!r} but saw back {data_we_just_sent_and_want_to_ignore!r}")
            hex_data = conn.read_until(b"\n\r> ")
            return bytes.fromhex(hex_data.decode().replace("\n\r",""))

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input_file", default="/dev/ttyACM0")
    ns = parser.parse_args()
    for [type, *fields] in log_parser.parse_data(log_parser.parse_raw_entries(LiveLogFile(ns.input_file))):
        if type != 4:
            continue # ignore all but IMU data
        print(*map("{:>8}".format, fields), sep=",")