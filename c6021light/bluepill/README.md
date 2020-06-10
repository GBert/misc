STM32 C6021Light
================

Universal Maerklin CAN to Maerklin I2C / RS485 / Loconet Board

![3D Board](https://github.com/GBert/railroad/raw/master/c6021light/bluepill/hardware/c6021light.png)

Based on Blue Pill


![STM32F103C8T6 microcontroller development board](https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/stm32f103c8t6_dev_pinout.gif)

using libopencm3 (real OpenSource - no questionable STMicroelectronics License)


CAN-Interface
-------------
```
/* CAN1 / CAN GPIO */
#define GPIO_CAN_PB_RX                  GPIO8           /* PB8 */
#define GPIO_CAN_PB_TX                  GPIO9           /* PB9 */
/* I2C2 / I2C GPIO */
#define GPIO_I2C2_SCL1                  GPIO6           /* PB6 */
#define GPIO_I2C2_SDA1                  GPIO7           /* PB7 */
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

more information here: https://wiki.kewl.org/dokuwiki/boards:vcc-gnd

