/*
 *  Linux support for SUSI
 *
 *  Copyright (c) 2016 Gerhard Bertelsmann <info@gerhard-bertelsmann.de>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation
 */

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

#include "susi-gpio.h"

static struct cdev cdev;
static int susiclk = 267;
static int susidat = 269;
static int delay = 10;

module_param(susiclk, int, S_IRUSR);
MODULE_PARM_DESC(susiclk, "SUSICLK pin");
module_param(susidat, int, S_IRUSR);
MODULE_PARM_DESC(susidat, "SUSIDAT pin");
module_param(delay, int, S_IRUSR);
MODULE_PARM_DESC(delay, "delay [us]");

void susiport_byte_out(u8 data) {
	int i;

	for (i = 0; i < 8; i++) {
		/* shift bit out */
		gpio_set_value(susidat, data & 0x01);
		data >>= 1;
		gpio_set_value(susiclk, 1);
		udelay(delay);
		gpio_set_value(susiclk, 0);
		udelay(delay);
	}
}

static int susiport_command(u8 * data, u8 length)
{
	int i;

	if (length > MAX_DATA)
		return(-EFAULT);

	local_irq_disable();

	for (i = 0; i < length; i++) {
		/* insert extra delay for analyzing on scope */
		udelay(delay);
		susiport_byte_out(*(data++));
	}

	local_irq_enable();
	return 0;
}

static int susiport_command_ack(u8 * data, u8 length, u8 * ack)
{
	int old_ack, i, ret;

	ret = susiport_command(data, length);
	if (ret)
		return(ret);

	gpio_direction_input(susidat);
	old_ack = 0;
	*ack = 0;

	/* scanning for ACK - use 500us */
	for (i = 0; i < 40; i++) {
		if (gpio_get_value(susidat)) {
			*ack = 1;
			if (old_ack) {
				goto ACK_FINISHED;
			}
		} else {
			*ack = 0;
		}
		old_ack = *ack;
		udelay(500);
	}
	*ack = 0;
ACK_FINISHED:
	gpio_direction_output(susidat, 0);
	return 0;
}

static long susiport_gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int err;
	struct susi_command susidata;

	if (_IOC_TYPE(cmd) != SUSI_GPIO_MAJOR) {
		return -EINVAL;
	}
	if (_IOC_DIR(cmd) & _IOC_READ) {
		if (!access_ok(VERIFY_WRITE, arg, _IOC_SIZE(cmd))) {
			return -EFAULT;
		}
	}
	if (_IOC_DIR(cmd) & _IOC_WRITE) {
		if (!access_ok(VERIFY_READ, arg, _IOC_SIZE(cmd))) {
			return -EFAULT;
		}
	}
	switch (cmd) {

	case SUSI_COMMAND:
		if (copy_from_user(&susidata, (struct susi_command *)arg, sizeof(susidata)) != 0)
			return -EFAULT;
		err = susiport_command(susidata.data, susidata.length);
		/* printk(KERN_INFO "%s : SUSI_COMMAND ...\n", __func__); */
		if (err)
			return err;
		if (copy_to_user((struct susiport_command *)arg, &susidata, sizeof(susidata)) != 0)
			return -EFAULT;
		/* printk(KERN_INFO "%s : SUSI_COMMAND done data 0x%02X\n", __func__, (unsigned int)susidata.dr); */
		break;

	case SUSI_COMMAND_ACK:
		if (copy_from_user(&susidata, (struct susi_command *)arg, sizeof(susidata)) != 0)
			return -EFAULT;
		err = susiport_command_ack(susidata.data, susidata.length, &susidata.ack);
		/* printk(KERN_INFO "%s : SUSI_COMMAND_ACK ...\n", __func__); */
		if (err)
			return err;
		if (copy_to_user((struct susiport_command *)arg, &susidata, sizeof(susidata)) != 0)
			return -EFAULT;
		/* printk(KERN_INFO "%s : SUSI_COMMAND_ACK done data 0x%02X\n", __func__, (unsigned int)susidata.dr); */
		break;
	}
	return 0;
}

struct file_operations susiport_gpio_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = susiport_gpio_ioctl,
};

static int __init susiport_init(void)
{
	int ret;

	ret = gpio_request(susiclk, "SUSI clock");
	if (ret)
		goto exit;
	gpio_direction_output(susiclk, 0);

	ret = gpio_request(susidat, "SUSI data");
	if (ret)
		goto free_gpio;
	gpio_direction_output(susidat, 0);
	printk(KERN_INFO "susitool-gpio using GPIO%02d (SUSI clock) and GPIO%02d (SUSI data) delay %d us\n", susiclk, susidat, delay);

	cdev_init(&cdev, &susiport_gpio_fops);
	cdev.owner = THIS_MODULE;
	return cdev_add(&cdev, MKDEV(SUSI_GPIO_MAJOR, 0), 1);

free_gpio:
	gpio_free(susiclk);
exit:
	return ret;
}

static void __exit susiport_exit(void)
{
	/* Setup the GPIOs as input by default (access = 0) */
	gpio_free(susiclk);
	gpio_free(susidat);

	cdev_del(&cdev);
}

module_init(susiport_init);
module_exit(susiport_exit);

MODULE_AUTHOR("Gerhard Bertelsmann <info@gerhard-bertelsmann.de>");
MODULE_DESCRIPTION("Linux support for SUSI");
MODULE_LICENSE("GPL");
