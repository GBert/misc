STM32 USB2RS485 (Xpressnet)
===============

using BluePill as USB2RS485 Converter for Xpressnet (fixed to 62500 Baud)
based on https://github.com/karlp/libopencm3-tests/tree/master/tests/usb-serial-rs485

![STM32F103C8T6 microcontroller development board](https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/stm32f103c8t6_dev_pinout.gif)

Pinout
------
```
RJ12
====
1 Signal, DCC (optional) 
2 GND 
3 B RS485 - 
4 A RS485 + 
5 +12V 
6 Signal, DCC (optional)
```

![USB Xpressnet Adapter](https://github.com/GBert/misc/raw/master/RS485/stm32/pictures/usb2xn_sch.jpg)

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
sending 16bit:
```
0x01 0xee -> 0x1ee
```

Price (28/09/17 Aliexpress)
---------------------------
```
Blue-Pill STM32	1,45 Euro
ST-LinkV2 Clone	1,55 Euro
SN75176 (10pcs)	1,20 Euro
```

Links
-----
Schematic https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/arduino_stm32f103c8t6_schematics.png
https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/STM32F103C8T6-DEV-BOARD-SCH.pdf

more information here: https://wiki.kewl.org/dokuwiki/boards:vcc-gnd

