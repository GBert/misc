STM32 SLCAN
===========

using libopencm3 to build cheapest CAN Adapter ever :

![STM32F103C8T6 microcontroller development board]
(https://github.com/GBert/misc/raw/master/stm32-slcan/pictures/stm32f103c8t6_dev_board.jpg)

Why Serial (SLCAN) and not USB ?
--------------------------------
it's not possible to use CAN and USB at the same time on STM32F103

CAN-Interface
-------------
```
/* CAN1 / CAN GPIO */
#define GPIO_CAN_PB_RX                  GPIO8           /* PB8 */
#define GPIO_CAN_PB_TX                  GPIO9           /* PB9 */
```

Flashing Serial
---------------
using stm32-flash (does USART2 also work for STM32F103 ?)

```
/* USART1 GPIO */
#define GPIO_USART1_TX                  GPIO9           /* PA9 */
#define GPIO_USART1_RX                  GPIO10          /* PA10 */

/* USART2 GPIO */
#define GPIO_USART2_CTS                 GPIO0           /* PA0 */
#define GPIO_USART2_RTS                 GPIO1           /* PA1 */
#define GPIO_USART2_TX                  GPIO2           /* PA2 */
#define GPIO_USART2_RX                  GPIO3           /* PA3 */
#define GPIO_USART2_CK                  GPIO4           /* PA4 */
```

