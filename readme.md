# P21

## Building the Project

The project can be built either with the Makefile in the gcc folder in order to
use the arm-none-eabi-gcc toolchain or with the Segger Embedded Studio project
in the ses folder.

The SES project and Makefile are both configured to build the project for the
nRF52840 Dongle (PCA10059). The Makefile can also be used to build the project 
for the nRF5284-DK (PCA10056) by setting an additional variable when running
make (`make BOARD=BOARD_PCA10056`).

## Programming the Board

If using the Makefile, the board can be programmed through gdb with
`make debug`, which will start and OpenOCD instance and launch a GDB session
conneted to it. To debug the board with a Black Magic Probe, use the `GDB_FILE`
variable to specify the probe's serial port. For example,
`make debug GDB_FILE=/dev/ttyACM0`.

