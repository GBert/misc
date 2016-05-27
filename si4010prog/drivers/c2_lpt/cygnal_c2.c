/* 
   This file implements a kernel space part of the Cygnal 
   (now Silicon Laboratories) C2 interface driver
   It has been written by Wojciech M. Zabolotny (wzab@ise.pw.edu.pl)
   and is published under the GPL (Gnu Public License) version 2.0 or later

   The code is partially based on examples from the book written
   by Alessandro Rubini & Jonathan Corbet - "Linux Device Drivers"

   Most the C2 related information was taken from the Cygnal's
   "C2 Interface Specification"

   Additional C2 features and tweaks for the 2.6 kernel by Matt Otto.
   PNP code is based on the parport_pc driver.

   For hardware level portability, (eg. porting to ETRAX platform?)
   we assume, that we have some macros for setting/reading I/O lines
   used to comunicate with the C2 interface.
 
   Anyway from the software point of wiev we should have independent macros:

   C2CK_ENA; - enables the clock/reset line
   C2CK_DIS; - disables the clock/reset line
   C2CK_SET(int); (0 or 1)
   C2CK_HIGH();
   C2CK_LOW();
   C2CK_ENA_IRQ(); - enables interrupts on C2CK pulses
   C2CK_DIS_IRQ(); - disables interrupts on C2CK pulses

   C2D_READ(); - reads the C2D line
   C2D_ENA(); - enables the C2D line
   C2D_DIS(); - disables the C2D line
   C2D_SET(int); (0 or 1)
   C2D_SETL(); - sets low
   C2D_SETH(); - sets high
   C2D_SETL_EN(); - sets high and enables
   C2D_SETH_EN(); - sets low and enables

   Hardware dependent macros are defined in the c2_lpt.h file
*/
   
#include <linux/config.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pnp.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/sched.h>

//#include <linux/fs.h>
//#include <linux/mm.h>

#include <linux/ioport.h>
#include <linux/poll.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <asm/io.h>
                                                                                
#include <linux/ioctl.h>
#include <asm/uaccess.h>  /* for put_user */

#define USE_PNP     // Matt: my LPT hardware needs to be enabled via PNP.

static int io_base=0x378;
module_param(io_base, int, 0);
MODULE_PARM_DESC(io_base, "LPT port I/O base address");
static int irq = 7;
module_param(irq, int, 0);
MODULE_PARM_DESC(irq, "LPT port IRQ number");
static int major=0; // requested major number
module_param(major, int, 0);
MODULE_PARM_DESC(major, "C2 device file major number to request");

#define DEVICE_NAME "cygnal"
static int Major = 0; // assigned major number

static int c2_busy=0;

//Include the hardware dependent macros
#include "c2_lpt.h"
#include "c2_ioctls.h"

static int interrupt_detected;

static void C2CK_low_pulse(void)
{
  //We disable interrupts for the current CPU - the pulse must not be too long!!!
  //Later we should decide if we need a spinlock there...
  unsigned long flags;
  local_irq_save(flags);
  C2CK_DIS_IRQ();
  C2CK_LOW();
  udelay(1); // 1 us should be enough, and safe
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
    return CheckForInterrupts();
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

  interrupt_detected = 0;
  C2CK_ENA_IRQ();

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


#ifdef USE_PNP
static const struct pnp_device_id c2_pnp_tbl[] = {
  /* Standard LPT Printer Port */
  {.id = "PNP0400", .driver_data = 0},
  /* ECP Printer Port */
  {.id = "PNP0401", .driver_data = 0},
  { }
};

MODULE_DEVICE_TABLE(pnp,c2_pnp_tbl);

/* For some reason, printk doesn't seem to work in this function.  I think */
/* it works in parport_pc's probe function, so why not here?  */
static int c2_pnp_probe(struct pnp_dev *dev, const struct pnp_device_id *id)
{
  if (pnp_port_valid(dev,0) &&
      !(pnp_port_flags(dev,0) & IORESOURCE_DISABLED)) {
    io_base = pnp_port_start(dev,0);
  }
  else {
    return -EINVAL;
  }
  if (pnp_irq_valid(dev,0) &&
      !(pnp_irq_flags(dev,0) & IORESOURCE_DISABLED)) {
    irq = pnp_irq(dev,0);
    if (request_irq(irq, c2_interrupt, SA_INTERRUPT, DEVICE_NAME, NULL)) {
      return -EINVAL;
    }
  }
  else {
    return -EINVAL;
  }

  return 0;
}

static void c2_pnp_remove(struct pnp_dev *dev)
{
}

/* we only need the pnp layer to activate the device, at least for now */
static struct pnp_driver c2_pnp_driver = {
  .name     = DEVICE_NAME,
  .id_table = c2_pnp_tbl,
  .probe    = c2_pnp_probe,
  .remove   = c2_pnp_remove,
};
#endif


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
       "cygnal_c2: Failed to register character device with major %d\n",
        Major);
    return Major;
  }

#ifdef USE_PNP
  if (pnp_register_driver(&c2_pnp_driver) < 0) {
    unregister_chrdev(Major, DEVICE_NAME);
    return -ENODEV;
  }
#else
  request_region(io_base,0x10,DEVICE_NAME);
  if (request_irq(irq, c2_interrupt, SA_INTERRUPT, DEVICE_NAME, NULL)) {
    printf("<1>Requesting IRQ %d failed\n", irq);
    unregister_chrdev(Major, DEVICE_NAME);
    return -ENODEV;
  }
#endif
  if(major) Major=major; /* The register_chrdev returns "0" if
			    major number was given, and registration
			    is successfull */
  printk(KERN_INFO
     "cygnal_c2: Successfully registered major device number %d\n",
      Major);
  return 0;
}

/* Cleanup when removing the module */
void c2_exit(void)
{
  if (irq) {
    free_irq(irq, NULL);
  }
#ifdef USE_PNP
  pnp_unregister_driver(&c2_pnp_driver);
#else
  release_region(io_base,0x10);
#endif
  unregister_chrdev(Major, DEVICE_NAME);
}
                                                                                
module_init(c2_init);
module_exit(c2_exit);
MODULE_LICENSE("GPL");
