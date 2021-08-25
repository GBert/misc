/*
 * Copyright (C) 2021 Gerhard Bertelsmann
 * All rights reserved.
 * 
 * XpressNet device driver is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation. 
 * 
 * XpressNet device driver is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along
 * with XpressNet device driver. If not, see http://www.gnu.org/licenses/
 */

#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/mutex.h>

static DEFINE_MUTEX(xpnchar_mutex);

static int majorNumber;
#define  DEVICE_NAME "xpnchar"
#define  CLASS_NAME  "xpn"

static struct class *xpncharClass = NULL;
static struct device *xpncharDevice = NULL;

struct xpn_device_data {
    struct cdev cdev;
    /* my data starts here */
    size_t size;
    uint8_t buffer[256];
};

struct xpn_device_data xpn_data;

// static DEFINE_SPINLOCK(xpn_lock);
/* BPi CAN PH2 -> GPIO 226 */
// static uint16_t direction_pin = 226;

static long xpndev_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    return 0;
}

static int xpn_open(struct inode *inode, struct file *file) {
    struct xpn_device_data *xpn_data;
    xpn_data = container_of(inode->i_cdev, struct xpn_device_data, cdev);


    printk(KERN_INFO "open\n");

    /* validate access to device */
    file->private_data = xpn_data;

    /* initialize device */

    return 0;
}

static int xpn_release(struct inode *inode, struct file *file) {
    mutex_unlock(&xpnchar_mutex);
    printk(KERN_INFO "release\n");

    return 0;
}

static int xpn_read(struct file *file, char __user * user_buffer, size_t size, loff_t * offset) {
    struct xpn_device_data *xpn_data = (struct xpn_device_data *)file->private_data;
    ssize_t len = min(xpn_data->size - *offset, size);

    printk(KERN_INFO "read\n");
    if (len <= 0)
	return 0;

    /* read data from my_data->buffer to user buffer */
    if (copy_to_user(user_buffer, xpn_data->buffer + *offset, len))
	return -EFAULT;

    *offset += len;
    return len;
}

static int xpn_write(struct file *file, const char __user * user_buffer, size_t size, loff_t * offset) {
    struct xpn_device_data *xpn_data = (struct xpn_device_data *)file->private_data;
    ssize_t len = min(xpn_data->size - *offset, size);

    printk(KERN_INFO "write len=%d\n", len);
    if (len <= 0)
	return 0;

    /* read data from user buffer to my_data->buffer */
    if (copy_from_user(xpn_data->buffer + *offset, user_buffer, len))
	return -EFAULT;

    *offset += len;
    return len;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = xpn_open,
    .read = xpn_read,
    .write = xpn_write,
    .release = xpn_release,
    .unlocked_ioctl = xpndev_ioctl,
};

static int __init xpn_device_init(void) {
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
	printk(KERN_ALERT "XPNChar failed to register a major number\n");
	return majorNumber;
    }
    printk(KERN_INFO "XPNChar: registered with major number %d\n", majorNumber);

    xpncharClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(xpncharClass)) {
	unregister_chrdev(majorNumber, DEVICE_NAME);
	printk(KERN_ALERT "Failed to register device class\n");
	return PTR_ERR(xpncharClass);
    }
    printk(KERN_INFO "XPNChar: device class registered\n");

    xpncharDevice = device_create(xpncharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(xpncharDevice)) {
	class_destroy(xpncharClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);
	printk(KERN_ALERT "Failed to create the device\n");
	return PTR_ERR(xpncharDevice);
    }
    printk(KERN_INFO "XPNChar: device class created\n");

    return 0;
}

static void xpn_device_exit(void) {
    // gpio_free(direction_pin);
    device_destroy(xpncharClass, MKDEV(majorNumber, 0));	// remove the device
    class_destroy(xpncharClass);	// remove the device class
    unregister_chrdev(majorNumber, DEVICE_NAME);	// unregister the major number
}

module_init(xpn_device_init);
module_exit(xpn_device_exit);
MODULE_DESCRIPTION("XpressNet Device Driver for Allwinner SoC");
MODULE_AUTHOR("Gerhard Bertelsmann <info@gerhard-bertelsmann.de>");
MODULE_LICENSE("GPL");
