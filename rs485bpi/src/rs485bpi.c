/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Gerhard Bertelsmann
 * --------------------------------------------------------------------------------
 */

#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define DRV_NAME "RS485_BPI"
#define UART_BASE_ADDRESS	0x01C28000
#define UART_MEM_SIZE		0x0400
/* using UART2 */
#define RS485_UART		2

/* PH2 -> 224 */
static int gpio = 226;
/* ttyS2 -> IRQ 51 */ 
static int irq = 51;
/* XpressNet -> 62500 */
static int baud = 62500;
module_param(gpio, int, 0);
MODULE_PARM_DESC(gpio, "used GPIO for DE/RE");
module_param(irq, int, 0);
MODULE_PARM_DESC(irq, "used IRQ for UART");
module_param(baud, int, 0);
MODULE_PARM_DESC(baud, "UART baud");

void __iomem *uart_address = NULL;
uint32_t uart_start_address;

void init_uart(int uart) {
    uart_start_address = UART_BASE_ADDRESS + uart * UART_MEM_SIZE; 
    uart_address = ioremap(uart_start_address, UART_MEM_SIZE);
    return;
}

void release_uart(int uart) {
    iounmap(uart_address);
}

static irqreturn_t uart_interrupt(int irq, void *dev_id) {
    printk (KERN_INFO "got IRQ 51");
    return (IRQ_HANDLED);
}

static int __init rs485_init(void) {
    int err;


    if (gpio_request(gpio, "RS485 DE")) {
	printk (KERN_ERR "can't get GPIO %d as DE/RE\n", gpio);
	return -1;
    }
    if (gpio_direction_output(gpio,0)) {
	printk (KERN_ERR "can't set GPIO %d as output DE/RE\n", gpio);
	goto FREE_GPIO;
    }
    err = request_irq(irq, uart_interrupt, IRQ_TYPE_LEVEL_HIGH, "uart_interrupt", NULL);

    if (err) {
	printk(KERN_ERR "UART IRQ: trouble requesting IRQ %d error %d\n", irq, err);
	goto IRQ_REQUEST_ERR;
    } else {
	printk(KERN_INFO "UART IRQ: requested IRQ %d and GPIO %d-> fine\n", irq, gpio);
    }

    init_uart(RS485_UART);

    return(0);

FREE_GPIO:
    return -1;
IRQ_REQUEST_ERR:
    gpio_free(gpio);
    return -1;
}

static void __exit rs485_exit(void) {
   gpio_free(gpio);
   /* free_irq(irq, uart_interrupt); */
   release_uart(RS485_UART);
   printk(KERN_INFO "RS485 module removed - GPIO %d freed\n", irq, gpio);
   return;
}

module_init(rs485_init);
module_exit(rs485_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gerhard Bertelsmann");
MODULE_DESCRIPTION("Multidrop RS485 on BPi");
MODULE_LICENSE("GPL");
