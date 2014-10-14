#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/mm.h>

#define DRV_NAME "S88 GPIO"

static unsigned int gpio = 0;
module_param(gpio, uint, 0);
MODULE_PARM_DESC(gpio, "GPIO");

static int __init mymodule_init(void) {
    if (gpio_request(gpio, "S88 Clock")) {
        printk (KERN_ERR "can't get GPIO %d\n", gpio);
        return -1;
    } else {
        printk (KERN_INFO "requested GPIO %d\n", gpio);
    }

    if (gpio_direction_output(gpio,0)) {
        printk (KERN_ERR "can't set GPIO %d as output\n", gpio);
        printk (KERN_INFO "freeing GPIO %d\n", gpio);
        gpio_free(gpio);
        return -1;
    }
    return(0);
}

static void __exit mymodule_exit(void) {
   printk (KERN_INFO "freeing GPIO %d\n", gpio);
   gpio_free(gpio);
   return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gerhard Bertelsmann");
