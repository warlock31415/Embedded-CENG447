import asyncio
from bleak import BleakClient
from readchar import readkey

## the service and characteristic UUIDs for our bluetooth module's UART
GATT_SERVICE =        "0000ffe0-0000-1000-8000-00805f9b34fb"
GATT_CHARACTERISTIC = "0000ffe1-0000-1000-8000-00805f9b34fb"

## connects to and writes to the bluetooth device listed
async def main(mac_addr: str, loop: asyncio.AbstractEventLoop):
	async with BleakClient(mac_addr, loop=loop) as client:
		await client.connect()
		await client.get_services()
		print('Connected. Type to send keystrokes. Press ESC to quit.')
		k = await getkey()
		while ord(k) != 27:
			await client.write_gatt_char(GATT_CHARACTERISTIC, bytearray(k,'ascii'))
			k = await getkey()
			print(k)

async def getkey():
	return readkey()

mac_addr = "FC:69:47:73:45:08" # CHANGE THIS to your BT's MAC
loop = asyncio.get_event_loop()
loop.run_until_complete(main(mac_addr, loop))
