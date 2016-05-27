/*
 * c2-gpio - GPIO Silicon Labs C2 Bus master driver
 *
 * Copyright (c) 2016, David Imhoff <dimhoff.devel@gmail.com>
 * Copyright (c) Wojciech M. Zabolotny <wzab@ise.pw.edu.pl> (Original author)
 *
 * The code is partially based on examples from the book written
 * by Alessandro Rubini & Jonathan Corbet - "Linux Device Drivers"
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/sched.h>

#include <linux/poll.h>
#include <linux/delay.h>
#include <linux/spinlock.h>

#include <linux/gpio.h>

#include <linux/ioctl.h>
#include <asm/uaccess.h>  /* for put_user */

#include "c2_ioctls.h"

static int gpio_c2ck = 17;
module_param(gpio_c2ck, int, 0);
MODULE_PARM_DESC(gpio_c2ck, "C2 Clock GPIO pin number");
static int gpio_c2d = 18;
module_param(gpio_c2d, int, 0);
MODULE_PARM_DESC(gpio_c2d, "C2 Data GPIO pin number");
static int major=0; // requested major number
//MODULE_PARM(major,"i");

#define DEVICE_NAME "c2_gpio"
static int Major = 0; // assigned major number

static int c2_busy=0;

#ifdef WITH_C2CK_IRQ
static int interrupt_detected;
#endif

/************************** Hardware control functions ************************/

static inline void C2CK_ENA(void)
{
  gpio_direction_output(gpio_c2ck, 1);
}

static inline void C2CK_DIS(void)
{
/*
   Do not disable the C2CK driver. According to AN127 this is optional. However
   doing so is not understood by the SI4010. When you want the C2CK interrupts
   to work you probably need some sort pull up construction.
*/
//  gpio_direction_input(gpio_c2ck);
#ifdef WITH_C2CK_IRQ
#warn "FIXME: c2ck is currently always controlled by the bus master"
#endif
}

static inline void C2CK_SET(int val)
{
  gpio_set_value(gpio_c2ck, val);
}

static inline void C2CK_HIGH(void)
{
  C2CK_SET(1);
}

static inline void C2CK_LOW(void)
{
  C2CK_SET(0);
}

static inline int C2D_READ(void)
{
  return gpio_get_value(gpio_c2d);
}

static inline void C2D_DIS(void)
{
  gpio_direction_input(gpio_c2d);
}
//disables the C2D line

static inline void C2D_SET(int val)
{
  gpio_set_value(gpio_c2d, val);
}

static inline void C2D_SETL(void)
{
  C2D_SET(0);
}

static inline void C2D_SETH(void)
{
  C2D_SET(1);
}

static inline void C2D_SETL_EN(void)
{
  gpio_direction_output(gpio_c2d, 0);
} // - sets low and enables

static inline void C2D_SETH_EN(void)
{
  gpio_direction_output(gpio_c2d, 1);
} // - sets high and enables


static inline void C2CK_ENA_IRQ(void)
{
#ifdef WITH_C2CK_IRQ
//TODO:
  result = request_irq(irqNumber,             // The interrupt number requested
                       (irq_handler_t) c2ck_irq_handler, // The pointer to the handler function below
                       IRQF_TRIGGER_RISING,   // Interrupt on rising edge (button press, not release)
                       "c2_gpio",    // Used in /proc/interrupts to identify the owner
                       NULL);
#endif
}

static inline void C2CK_DIS_IRQ(void)
{
#ifdef WITH_C2CK_IRQ
  free_irq(irqNumber, NULL);
#endif
}


/***************************** C2 protocol functions **************************/

static void C2CK_low_pulse(void)
{
  //We disable interrupts for the current CPU - the pulse must not be too long!!!
  //Later we should decide if we need a spinlock there...
  unsigned long flags;
  local_irq_save(flags);
  C2CK_DIS_IRQ();
  C2CK_LOW();
  ndelay(80);
  // Strobe must be atleast 80 nsec width, but may not be longer then 5 usec.
  // Since the gpio control functions add a lot of overhead, we only wait the
  // minimum amount of time. --DI
  C2CK_HIGH();
  C2CK_ENA_IRQ();
  local_irq_restore(flags);
  //printk(KERN_ERR "C2CK pulse\n");
}

/* The C2CK_reset function sends the low pulse on the C2CK line long enough to
   reset our device.
*/
static void C2CK_reset(void)
{
  //This time we do not disable interrupts, as it is not a problem
  //if the pulse lasts longer
  C2CK_ENA();
  C2CK_DIS_IRQ();
  C2CK_LOW();
  udelay(40); // it should be at least 20 us, let's do it twice as long
  C2CK_HIGH();
  C2CK_ENA_IRQ();
  C2CK_DIS();
  udelay(3); //we need to wait at least 2us before we do something else
}

static void AddressWrite(unsigned char a)
{
  int i;

  C2CK_ENA();
  C2CK_low_pulse(); //start
  udelay(1);
  C2D_SETH_EN();
  C2CK_low_pulse();
  udelay(1);
  C2CK_low_pulse();
  udelay(1);
  //Now we are sending the address bits
  for (i=0;i<8;i++) {
    if(a & 1)
      C2D_SETH();
    else
      C2D_SETL();
    C2CK_low_pulse();
    a >>= 1;
    udelay(1);
  }
  C2D_DIS();
  C2CK_low_pulse();
  C2CK_DIS();
  udelay(1);
}

static unsigned char AddressRead(void)
{
  int i;
  unsigned char a=0, mask=1;

  C2CK_ENA();
  C2CK_low_pulse(); //start
  udelay(1);
  C2D_SETL_EN();
  C2CK_low_pulse();
  udelay(1);
  C2D_SETH();
  C2CK_low_pulse();
  udelay(1);
  C2D_DIS();
  //Now we are receiving the address bits
  for (i=0;i<8;i++) {
    C2CK_low_pulse();
    udelay(1);
    if(C2D_READ())
      a |= mask;
    mask <<=1;
  }
  C2CK_low_pulse();
  C2CK_DIS();
  udelay(1);
  return a;
}

/* The data write, and read commands are dangerous, as they may lock up
   the system (if the WAIT will last forever due to a hardware failure)
   that's why we check for the signal pending, and call the schedule function.
*/

static int DataWrite(unsigned char l, unsigned long a)
{
  int i, count=0;

  C2CK_ENA();
  C2CK_low_pulse(); //start
  udelay(1);
  C2D_SETH_EN();
  C2CK_low_pulse();
  udelay(1);
  C2D_SETL();
  C2CK_low_pulse();
  udelay(1);
  //Now we are sending the length bits
  if (l & 1)
    C2D_SETH();
  else
    C2D_SETL();
  C2CK_low_pulse();
  udelay(1);
  if (l & 2)
    C2D_SETH();
  else
    C2D_SETL();
  C2CK_low_pulse();
  udelay(1);
  //Finally we send the data bits
  for (i=0;i<8*(l+1);i++) {
    if(a & 1)
      C2D_SETH();
    else
      C2D_SETL();
    C2CK_low_pulse();
    a >>= 1;
    udelay(1);
  }
  C2D_DIS();
  //And now we enter the WAIT state
  //To avoid deadlock, we need to count the WAIT cycles, check for signals,
  //and call schedule, wherever waiting lasts too long
  while(1) {
    C2CK_low_pulse();
    udelay(1);
    if(C2D_READ())
      break; // We've got `1`!!!
    if(++count == 200) { //OK 200 is just an arbitrary value
      if (signal_pending(current)) {
	return -ERESTARTSYS;
      }
      count=0;
      schedule();
    }
  }
  //We've left the loop, so now we need only to generate the stop pulse
  C2CK_low_pulse();
  C2CK_DIS();
  udelay(1);
  return 0;
}

static int DataRead(unsigned char l, unsigned long *a)
{
  int i, count=0;
  unsigned mask=1;
  *a=0;

  C2CK_ENA();
  C2CK_low_pulse(); //start
  udelay(1);
  C2D_SETL_EN();
  C2CK_low_pulse();
  udelay(1);
  C2CK_low_pulse();
  udelay(1);
  //Now we are sending the length bits
  if (l & 1)
    C2D_SETH();
  else
    C2D_SETL();
  C2CK_low_pulse();
  udelay(1);
  if (l & 2)
    C2D_SETH();
  else
    C2D_SETL();
  C2CK_low_pulse();
  udelay(1);
  //And now we enter the WAIT state
  //To avoid deadlock, we need to count the WAIT cycles, check for signals,
  //and call schedule, wherever waiting lasts too long
  C2D_DIS();
  while(1) {
    C2CK_low_pulse();
    udelay(1);
    if(C2D_READ())
      break; // We've got `1`!!!
    if(++count == 200) { //OK 200 is just an arbitrary value
      if (signal_pending(current)) {
	return -ERESTARTSYS;
      }
      count=0;
      schedule();
    }
  }
  //We've left the loop, so now we can read the data bits
  for (i=0;i<8*(l+1);i++) {
    //for (i=0;i<8;i++) {
    C2CK_low_pulse();
    udelay(1);
    if(C2D_READ())
      (*a) |= mask;
    mask <<= 1;
  }
  C2CK_low_pulse();
  C2CK_DIS();
  udelay(1);
  return 0;
}

#ifdef WITH_C2CK_IRQ
static irqreturn_t c2_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
  // keep interrupt_detected positive
  if (interrupt_detected != INT_MAX) {
    interrupt_detected++;
  }
  return IRQ_HANDLED;
}

static int CheckForInterrupts(void)
{
  unsigned long flags;
  int rtn_val;

  local_irq_save(flags);
  rtn_val = interrupt_detected;
  interrupt_detected = 0;
  local_irq_restore(flags);
  return rtn_val;
}
#endif

/* Here we have the standard functions for the driver */

/* Procedure servicing the ioctl */
long c2_ioctl (struct file *filp,
	      unsigned int cmd, unsigned long arg)
{
//TODO: Locking is probably required since we don't run under BKL anymore.
//      Although it isn't possible to open the c2 device twice, a single
//      process with multiple threads might be abble to issue simultaneous
//      ioctls() --DI
  long buf;
  unsigned char cbuf;
  int err=0,ret;
  if (_IOC_TYPE(cmd) != C2_IOC_MAGIC) return -ENOTTY;
  if (_IOC_NR(cmd) > C2_IOC_MAXNR) return -ENOTTY;
  if (_IOC_DIR(cmd) & _IOC_READ)
    err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
  else if (_IOC_DIR(cmd) & _IOC_WRITE)
    err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
  if (err) return -EFAULT;
  switch(_IOC_NR(cmd)) {
  case _IOC_NR(C2_IOCRESET):
    //printk(KERN_ERR "C2_IOCRESET");
    C2CK_reset();
    return 0;
  case _IOC_NR(C2_IOCAWRITE):
    ret = __get_user(cbuf, (unsigned char *)arg);
    //printk(KERN_ERR "C2_IOCAWRITE: address=%u\n", cbuf);
    AddressWrite(cbuf);
    return ret;
  case _IOC_NR(C2_IOCDWRITE):
    ret = __get_user(buf, (long *)arg);
    //printk(KERN_ERR "C2_IOCDWRITE: len=%u, data=%u\n", (_IOC_SIZE(cmd)-1) & 3, buf);
    return DataWrite((_IOC_SIZE(cmd)-1) & 3, buf);
  case _IOC_NR(C2_IOCAREAD):
    cbuf=AddressRead();
    //printk(KERN_ERR "C2_IOCAREAD: address=%u\n", cbuf);
    ret = __put_user(cbuf, (unsigned char *)arg);
    return ret;
  case _IOC_NR(C2_IOCDREAD):
    ret = DataRead((_IOC_SIZE(cmd)-1) & 3, &buf);
    if(ret) return ret;
    //printk(KERN_ERR "C2_IOCDREAD: len=%u, data=%u\n", (_IOC_SIZE(cmd)-1) & 3, buf);
    ret = __put_user(buf, (long *)arg);
    return ret;
  case _IOC_NR(C2_IOCQRESET):
    //printk(KERN_ERR "C2_IOCQRESET");
    C2CK_ENA();
    C2CK_low_pulse();  /* a "quick reset" is just a low pulse, less than 20us */
    C2CK_DIS();
    return 0;
  case _IOC_NR(C2_IOCIRQCHK):
    //printk(KERN_ERR "C2_IOCIRQCHK");
#ifdef WITH_C2CK_IRQ
    return CheckForInterrupts();
#else
    printk(KERN_ERR "C2_IOCIRQCHK unsupported");
    return -EOPNOTSUPP;
#endif
  default:
    return -ENOTTY;
  }
}

static DEFINE_SPINLOCK(c2_lock);

static int c2_open(struct inode *inode,
		      struct file *file)
{
  unsigned long flags;
  spin_lock_irqsave(&c2_lock, flags);
  if(c2_busy) {
    spin_unlock_irqrestore(&c2_lock, flags);
    return -EBUSY;
  }
  c2_busy++;
  spin_unlock_irqrestore(&c2_lock, flags);

#ifdef WITH_C2CK_IRQ
  interrupt_detected = 0;
  C2CK_ENA_IRQ();
#endif

  C2CK_HIGH();
  C2CK_DIS();    // leave C2CK disabled so we can look for MCU-generated pulses
  return 0;
}

static int c2_release(struct inode *inode,
			 struct file *file)
{
  unsigned long flags;
  spin_lock_irqsave(&c2_lock, flags);
  c2_busy--;
  spin_unlock_irqrestore(&c2_lock, flags);

  C2CK_DIS_IRQ();

  //Now we need to set reasonable state of interface lines
  C2D_DIS();
  C2CK_DIS();
  return 0;
}


/* File operations structure - only ioctl, open and release implemented*/
struct file_operations Fops = {
  .owner=THIS_MODULE,
  .unlocked_ioctl=c2_ioctl, /* ioctl */
  .open=c2_open,
  .release=c2_release,
};


/* Initialization */
int c2_init(void)
{
  /* Register the character device (at least try) */
  Major = register_chrdev(major,
                          DEVICE_NAME,
                          &Fops);
  /* Negative values signify an error */
  if (Major < 0) {
    printk (KERN_ERR
       "c2_gpio: Failed to register character device with major %d\n",
        Major);
    return Major;
  }

  if(major) Major=major; /* The register_chrdev returns "0" if
			    major number was given, and registration
			    is successfull */
  printk(KERN_INFO
     "c2_gpio: Successfully registered major device number %d\n",
      Major);
  return 0;
}

/* Cleanup when removing the module */
void c2_exit(void)
{
#ifdef WITH_C2CK_IRQ
  if (irq) {
    free_irq(irq, NULL);
  }
#endif
  unregister_chrdev(Major, DEVICE_NAME);
}

module_init(c2_init);
module_exit(c2_exit);
MODULE_LICENSE("GPL");
