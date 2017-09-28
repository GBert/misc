STM32 USB2RS485 (Xpressnet)
===============

using BluePill as USB2RS485 Converter for Xpressnet (fixed to 62500 Baud)
based on https://github.com/karlp/libopencm3-tests/tree/master/tests/usb-serial-rs485

![STM32F103C8T6 microcontroller development board](https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/stm32f103c8t6_dev_pinout.gif)

Pinout
------
```
                 Blue Pill

        B12        MICRO USB         GND
        B13                          GND
        B14                          3V3
        B15   RESET    GND    GND  RESET
        A8    BUTTON  *BOOT1 *BOOT0  B11
(TX0)   A9             3V3    3V3    B10
(RX0)   A10                           B1
        A11                           B0
        A12                           A7
        A15                           A6
        B3       STM32F103C8T6        A5
        B4                            A4
        B5                            A3 RX2 RS485
        B6                            A2 TX2 RS485
        B7              8M            A1 DE
(CANRX) B8           32768            A0
(CANTX) B9                           C15
        5V       PC13     POWER      C14
        GND      LED      LED        C13
        3V3           SWD           VBAT
          3V3 DIO  DCLK GND
              PA13 PA14
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
sending 16bit


Links
-----
Schematic https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/arduino_stm32f103c8t6_schematics.png
https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/STM32F103C8T6-DEV-BOARD-SCH.pdf

more information here: https://wiki.kewl.org/dokuwiki/boards:vcc-gnd

