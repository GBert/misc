STM32 SLCAN
===========

using libopencm3 to build cheapest CAN Adapter ever :

image::pictures/stm32f103c8t6_dev_board.jpg["STM32F103C8T6 microcontroller development board"]

Flashing Serial
---------------

```
/* CAN1 / CAN GPIO */
#define GPIO_CAN_PB_RX                  GPIO8           /* PB8 */
#define GPIO_CAN_PB_TX                  GPIO9           /* PB9 */

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

