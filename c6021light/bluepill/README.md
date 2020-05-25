STM32 C6021Light
================

using libopencm3 (real OpenSource - no questionable STMicroelectronics License) to build cheapest CAN Adapter ever:

![STM32F103C8T6 microcontroller development board](https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/stm32f103c8t6_dev_pinout.gif)

CAN-Interface
-------------
```
/* CAN1 / CAN GPIO */
#define GPIO_CAN_PB_RX                  GPIO8           /* PB8 */
#define GPIO_CAN_PB_TX                  GPIO9           /* PB9 */
/* I2C2 / I2C GPIO */
#define GPIO_I2C2_SCL2                  GPIO10          /* PB10 */
#define GPIO_I2C2_SDA2                  GPIO11          /* PB11 */
```

Flashing Serial
---------------
using SWD interface with st-flash or serial loader stm32-flash (works only with USART1)

```
/* USART1 GPIO */
#define GPIO_USART1_TX                  GPIO9           /* PA9 */
#define GPIO_USART1_RX                  GPIO10          /* PA10 */
```

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
git clone https://github.com/GBert/railroad
cd railroad/c6021light/bluepill
make # make stm32flash
```

Links
-----
Schematic https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/arduino_stm32f103c8t6_schematics.png
https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/STM32F103C8T6-DEV-BOARD-SCH.pdf

more information here: https://wiki.kewl.org/dokuwiki/boards:vcc-gnd

