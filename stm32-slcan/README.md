STM32 SLCAN
===========

using libopencm3 (real OpenSource - no questionable STMicroelectronics License) to build cheapest CAN Adapter ever:

![STM32F103C8T6 microcontroller development board](https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/stm32f103c8t6_dev_pinout.gif)

Why Serial (SLCAN) and not USB ?
--------------------------------
it's not possible to use CAN and USB at the same time on STM32F103

Schematic
---------
![Cheapest CAN Interface ever](https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/stm32-slcan.png)

either use MCP2562 or ISO1050 transceiver - not both !

CAN-Interface
-------------
```
/* CAN1 / CAN GPIO */
#define GPIO_CAN_PB_RX                  GPIO8           /* PB8 */
#define GPIO_CAN_PB_TX                  GPIO9           /* PB9 */
```

Flashing Serial
---------------
using SWD interface with st-flash or serial loader stm32-flash (works only with USART1)

```
/* USART1 GPIO */
#define GPIO_USART1_TX                  GPIO9           /* PA9 */
#define GPIO_USART1_RX                  GPIO10          /* PA10 */
```

Usage
-----
Hardware handshaking and LEDs usage is still missing as of today - is made sometime.

connect your favorite/cheap 3V3 USB2Serial board to USART2:
```
/* USART2 GPIO */
#define GPIO_USART2_CTS                 GPIO0           /* PA0 */
#define GPIO_USART2_RTS                 GPIO1           /* PA1 */
#define GPIO_USART2_TX                  GPIO2           /* PA2 */
#define GPIO_USART2_RX                  GPIO3           /* PA3 */
```
and do:
```
sudo modprobe can
sudo modprobe can-raw
sudo modprobe slcan
sudo slcand -s5 -S500000 /dev/ttyUSB0 can0 # CAN Speed 5 ->250 kBaud - 500,000 Baud serial (fix)
ifconfig can0 up
```
Now you can use it as SocketCAN interface - enjoy :-)

Compile under Ubuntu/Raspbian
-----------------------------
```
sudo apt-get install git gcc-arm-none-eabi stm32flash

sudo mkdir -p /embedded/arm-cortex/src
sudo chown -R "$(whoami).$(whoami)" /embedded

cd /embedded/arm-cortex/src
git clone https://github.com/libopencm3/libopencm3.git
cd libopencm3
make PREFIX=arm-none-eabi-

cd ~
git clone https://github.com/GBert/misc gbert-misc
cd gbert-misc/stm32-slcan
make # make stm32flash
```

Links
-----
Schematic https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/arduino_stm32f103c8t6_schematics.png
https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/STM32F103C8T6-DEV-BOARD-SCH.pdf

more information here: https://wiki.kewl.org/dokuwiki/boards:vcc-gnd

