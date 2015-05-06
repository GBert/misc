/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Gerhard Bertelsmann
 * --------------------------------------------------------------------------------
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/kobject.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define DRV_NAME "S88_GPIO"

#define GPIO_LOAD	0
#define GPIO_RESET	1
#define GPIO_CLOCK	2
#define GPIO_DATA_IN	3
#define GPIO_DATA_OUT	4

/* normally we not use data out */
static int gpios [] = {20, 19, 18, 7, 255};
static int gpio_count;
module_param_array(gpios, int, &gpio_count, 0);
MODULE_PARM_DESC(gpios, "used GPIOs for Load, Reset, Clock, Data-In, Data-Out");

static struct kobject *s88_kobj;
static struct task_struct *task;

static int s88_task(void *arg) {
    while(!kthread_should_stop()) {
	set_current_state(TASK_RUNNING);
	set_current_state(TASK_INTERRUPTIBLE);
	msleep(2);
    }
   return 0;
}

static int __init mymodule_init(void) {
    if (gpio_request(gpios[GPIO_LOAD], "S88 Load")) {
        printk (KERN_ERR "can't get GPIO %d as S88 Load\n", gpios[GPIO_LOAD]);
        return -1;
    }
    if (gpio_direction_output(gpios[GPIO_LOAD],0)) {
        printk (KERN_ERR "can't set GPIO %d as output (S88 Load)\n", gpios[GPIO_LOAD]);
	goto FREE_LOAD;
    }

    if (gpio_request(gpios[GPIO_RESET], "S88 Reset")) {
        printk (KERN_ERR "can't get GPIO %d as S88 Reset\n", gpios[GPIO_RESET]);
	goto FREE_LOAD;
    }
    if (gpio_direction_output(gpios[GPIO_RESET],0)) {
        printk (KERN_ERR "can't set GPIO %d as output (S88 Reset)\n", gpios[GPIO_RESET]);
	goto FREE_RESET;
    }

    if (gpio_request(gpios[GPIO_CLOCK], "S88 Clock")) {
        printk (KERN_ERR "can't get GPIO %d as S88 Clock\n", gpios[GPIO_CLOCK]);
	goto FREE_RESET;
    }
    if (gpio_direction_output(gpios[GPIO_CLOCK],0)) {
        printk (KERN_ERR "can't set GPIO %d as output (S88 Clock)\n", gpios[GPIO_CLOCK]);
	goto FREE_CLOCK;
    }

    if (gpio_request(gpios[GPIO_DATA_IN], "S88 Data In")) {
        printk (KERN_ERR "can't get GPIO %d as S88 Data In\n", gpios[GPIO_DATA_IN]);
	goto FREE_CLOCK;
    }
    if (gpio_direction_input(gpios[GPIO_DATA_IN])) {
        printk (KERN_ERR "can't set GPIO %d as input (S88 Data In)\n", gpios[GPIO_DATA_IN]);
	goto FREE_DATA_IN;
    }

    /* normally S88 is data in only */
    if (gpios[GPIO_DATA_OUT] != 255) {
	if (gpio_request(gpios[GPIO_DATA_OUT], "S88 Data Out")) {
	    printk (KERN_ERR "can't get GPIO %d as S88 Data Out\n", gpios[GPIO_DATA_OUT]);
	    goto FREE_DATA_IN;
	}
	if (gpio_direction_output(gpios[GPIO_DATA_OUT],0)) {
	    printk (KERN_ERR "can't set GPIO %d as output (S88 Data Out)\n", gpios[GPIO_DATA_OUT]);
	    goto FREE_DATA_OUT;
	}
    }

    s88_kobj = kobject_create_and_add("s88", kernel_kobj->parent); // kernel_kobj points to /sys/kernel
    if(!s88_kobj){
	printk(KERN_ALERT "S88: failed to create kobject mapping\n");
	return -ENOMEM;
    }

    task = kthread_run(s88_task, NULL, "s88_thread");
    if(IS_ERR(task)) {
	printk(KERN_ALERT "S88: failed to create the reading task\n");
	goto FREE_DATA_OUT;
    }

    return(0);

FREE_DATA_OUT:
    gpio_free(gpios[GPIO_DATA_OUT]);
FREE_DATA_IN:
    gpio_free(gpios[GPIO_DATA_IN]);
FREE_CLOCK:
    gpio_free(gpios[GPIO_CLOCK]);
FREE_RESET:
    gpio_free(gpios[GPIO_RESET]);
FREE_LOAD:
    gpio_free(gpios[GPIO_LOAD]);
    return -1;
}

static void __exit mymodule_exit(void) {
   kthread_stop(task);
   kobject_put(s88_kobj);
   printk (KERN_INFO "freeing GPIOs \n");
   gpio_free(gpios[GPIO_LOAD]);
   gpio_free(gpios[GPIO_RESET]);
   gpio_free(gpios[GPIO_CLOCK]);
   gpio_free(gpios[GPIO_DATA_IN]);
   if (gpios[GPIO_DATA_OUT] != 255)
	gpio_free(gpios[GPIO_DATA_OUT]);
   return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gerhard Bertelsmann");
MODULE_DESCRIPTION("S88 via GPIO");
MODULE_LICENSE("GPL");
