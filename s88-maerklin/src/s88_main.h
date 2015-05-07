// -----------------------------------------------------------------------------
// $RCSfile: s88_main.h,v $
// Project: Maerklin CST2
// Subproject: Treiber für die S88 Rückmeldemodule
// Date of Creation: 13.09.2007
// Main Author: C.Biermann
// Copyright: (c) 2007 ies GmbH, 58566 Kierspe, Germany
//            All Rights Reserved.
//
// $Revision: 1.2 $
// -----------------------------------------------------------------------------
// Description:
//! \file  s88_main.h
//! \brief This file contains globally used declarations, that can be hidden
//!        to applications
//!
// -----------------------------------------------------------------------------
// Optional Information, e.g. Module List
//
//
// -----------------------------------------------------------------------------
// $Log: s88_main.h,v $
// Revision 1.2  2008/02/12 08:57:43  bie
// Error correction: Inputs 9-16 of a module are now read correctly;
// now using dynamic major device numbers during device registration;
//
// Revision 1.1  2007/12/28 14:29:42  bie
// erste komplett lauffaehige Version
//
// -----------------------------------------------------------------------------

#ifndef S88_MAIN_H
#define S88_MAIN_H

#include <linux/poll.h>
#include <linux/cdev.h>


#include "types.h"

//#define S88_DEBUG		1			// enable verbose error messages
//#define S88_TEST		1			// enable "test only" code
#define S88_USE_OST		1			// shift clock with pxa operating system timer


#define INFO_MSG(format, args...)   printk(KERN_INFO "s88 (info): " format, ##args)

#define WARNING_MSG(format, args...) printk(KERN_WARNING "s88 (warning): " format, ##args)

#define ERROR_MSG(format, args...)  printk(KERN_ERR "s88 (error): " format, ##args)

#ifdef S88_DEBUG
#define DEBUG_MSG(format, args...)  printk(KERN_INFO "s88 (debug): " format, ##args)
#else
#define DEBUG_MSG(format, args...)
#endif


/**
 * The interface to the s88 bus is made of four pxa gpios
 */

#define GPIO_S88_RESET  110
#define GPIO_S88_LOAD   114
#define GPIO_S88_CLOCK  115
#define GPIO_S88_DATA   112


typedef struct
{
	WORD w_bus_length;
	WORD w_new_bus_length;

	DWORD d_cycle_time;
	DWORD d_new_cycle_time;
	
	DWORD d_shift_clock;
	DWORD d_new_shift_clock;

	WORD w_operating_mode;

	wait_queue_head_t x_wait_q;
	struct semaphore x_ioctl_sem;
	struct semaphore x_read_sem;
	struct cdev x_cdev;
	dev_t x_devno;

	DWORD d_size;
	
	long l_interrupts;
	
}
S88_DEV_TYP;

extern S88_DEV_TYP gx_s88_dev;


int s88_read_procmem(char *pc_buf, char **ppc_start,
                    off_t l_offset, int i_count, int *pi_eof, void *pv_data);

int s88_open(struct inode *px_inode, struct file *px_file);
int s88_release(struct inode *px_inode, struct file *px_file);

ssize_t s88_read(struct file *px_file,
                 char *pc_buf, size_t d_count, loff_t *pd_pos);
ssize_t s88_write(struct file *px_file,
                  const char *pc_buf, size_t d_count, loff_t *pd_pos);

unsigned int s88_poll(struct file *px_file, struct poll_table_struct *px_poll_table);
int s88_fasync(int i_fd, struct file *px_file, int i_mode);
int s88_ioctl(struct inode *px_inode, struct file *px_file,
              unsigned int w_cmd, DWORD d_arg);

#endif
