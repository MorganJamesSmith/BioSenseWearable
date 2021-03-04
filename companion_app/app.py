#Simple CLI for the Project21 Bluetooth connection

#This program uses msvcrt so it will only work on windows
#Python does not seem to have a cross platform way to get key presses
#The call to msvcrt.getch can be replaced by the equivalent for other OSes
#Make sure the device is advertising before running this program
#At the time of writing, advertising occurs for a one minute period after a reset

import asyncio
import sys
import msvcrt
from bleak import BleakScanner, BleakClient

UART_RX_UUID = '6e400002-b5a3-f393-e0a9-e50e24dcca9e'
UART_TX_UUID = '6e400003-b5a3-f393-e0a9-e50e24dcca9e'
DEVICE_NAME = 'Project21'

def TX_handler(sender, data):
    sys.stdout.write(data.decode())
    sys.stdout.flush()
    
async def ainput() -> str:
    return await asyncio.get_event_loop().run_in_executor(None, msvcrt.getch)

async def scan(name):
    "scans for a advertising bluetooth device by name, returns device info"
    for device in await BleakScanner.discover():
        if(str(device).split()[1] == name):
            return device
    return None

async def main():
    print(f'Searching for device {DEVICE_NAME}...')
    device = await scan(DEVICE_NAME)
    if(device is None):
        raise RuntimeError(f'{DEVICE_NAME} not found. Make sure it is advertising.')

    print(f'Found {DEVICE_NAME} at address {device.address}')
    print('Opening connection')
    
    async with BleakClient(device) as client:
        await client.is_connected()
        await client.start_notify(UART_TX_UUID, TX_handler)
        print('Connected')
        
        run = True
        while(run):
            char = await ainput()
            if(char.decode() == '\x0d'):
                char = '\x0a'.encode()
            
            if(char.decode() == '\x1b'):
                run = False
            else:
                await client.write_gatt_char(UART_RX_UUID, char)
        
        print('\nClosing connection')
        await client.stop_notify(UART_TX_UUID)

if __name__ == "__main__":
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())