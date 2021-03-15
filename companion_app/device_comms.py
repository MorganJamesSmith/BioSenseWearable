import asyncio
import io
import serial
import bleak
try:
    from . import log_parser
except ImportError:
    import log_parser

async def scan_for_device(device_name, *, log=print):
    device_was_found_future = asyncio.Future()
    def any_device_discovered_callback(device, advertisement_data):
        if device.name == device_name:
            device_was_found_future.set_result(device)
            log(f"FOUND device: {device.name}, rssi is {device.rssi} address is {device.address}")
        elif device.name != "Unknown": log(f"seeing device: {device.name}")
            
    async with bleak.BleakScanner() as scanner:
        scanner.register_detection_callback(any_device_discovered_callback)
        log(f"looking for {device_name}")
        return await device_was_found_future

async def open_bluetooth_cli(device_name='Project21', 
                             rx_uuid='6e400002-b5a3-f393-e0a9-e50e24dcca9e', 
                             tx_uuid='6e400003-b5a3-f393-e0a9-e50e24dcca9e', 
                             *, log=print):
    """opens cli with bluetooth device as async coroutine
    first iteration will connect to the device and yield initial prompt from device
    subsequently should call .asend(message) to send the message to the device
    this will read the output and produce it as a response to the asend call.
    """
    device = await scan_for_device(device_name, log=log)
    is_connected = True
    def device_has_disconnected(c=None):
        nonlocal is_connected
        log("BLUETOOTH DEVICE DISCONNECTED")
        is_connected = False
    log("BEFORE WITH")
    async with bleak.BleakClient(device.address) as client:
        log("INSIDE WITH")
        client.set_disconnected_callback(device_has_disconnected)
        # TODO: check this makes sense once I can actually test it.
        while is_connected:
            output_from_device = await client.read_gatt_char(rx_uuid)
            msg_send_to_device = yield output_from_device
            await client.write_gatt_char(tx_uuid, msg_send_to_device.encode())
    log("closed connection with bluetooth device")

async def open_usb_cli(device="/dev/ttyACM0", prompt="\n> "):
    "opens serial connection to device, mimics behaviour to bluetooth cli"
    with serial.Serial(device) as conn:
        while True:
            msg_to_send = yield conn.read_until(prompt)
            conn.write(msg_to_send)

def hex_output_to_binary_file(hexbuffer: str):
    """converts hex output from hcat command to file buffer for parser."""
    return io.BytesIO(bytes.fromhex(hexbuffer.replace("\n","")))

async def get_log_file():
    cli = open_cli()
    await cli.asend(None)
    log_text_bytearray = await cli.asend("hcat P21")
    print("!!!!!!GOT DATA!!!!!!!!")
    print(log_text_bytearray)
    return hex_output_to_binary_file(log_text_bytearray.decode())


async def main():
    f = await get_log_file()
    log_parser.write_multifile(f)

open_cli = open_usb_cli
if __name__ == "__main__":
    asyncio.run(main())