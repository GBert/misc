// -----------------------------------------------------------------------------
// $RCSfile: s88_main.c,v $
// Project: Maerklin CST2
// Subproject: device driver for S88 decoders
// Date of Creation: 12.09.2007
// Main Author: C.Biermann
// Copyright: (c) 2007 ies GmbH, 58566 Kierspe, Germany
//            All Rights Reserved.
//
// $Revision: 1.5 $
// -----------------------------------------------------------------------------
// Description:
//! \file  s88_main.c
//! \brief This file contains the main module of the s88 driver.
//
// -----------------------------------------------------------------------------
// Optional Information, e.g. Module List
//
//
// -----------------------------------------------------------------------------
// $Log: s88_main.c,v $
// Revision 1.5  2008/07/14 13:14:06  bie
// extended maximal length of s88 bus from 496 to 1024 inputs;
//
// Revision 1.4  2008/06/25 14:28:31  bie
// use periodic mode of operating system timer instead of one-shot mode again;
//
// Revision 1.3  2008/06/17 13:16:31  bie
// moved falling edge of s88 shift clock towards end of shift cycle;
//
// Revision 1.2  2008/02/12 08:57:43  bie
// Error correction: Inputs 9-16 of a module are now read correctly;
// now using dynamic major device numbers during device registration;
//
// Revision 1.1  2007/12/28 14:28:35  bie
// erste komplett lauff�hige Version
//
//
// -----------------------------------------------------------------------------


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <asm-arm/delay.h>
#include <asm-arm/types.h>
#include <asm-arm/arch-pxa/gpio.h>
#include <asm-arm/arch-pxa/irqs.h>

#include "s88.h"
#include "s88_main.h"


S88_DEV_TYP gx_s88_dev =
{
	.w_bus_length = S88_DFL_BUS_LENGTH,
	.w_new_bus_length = S88_DFL_BUS_LENGTH,
	.d_cycle_time = S88_DFL_CYCLE_TIME,
	.d_new_cycle_time = S88_DFL_CYCLE_TIME,
	.d_shift_clock = S88_DFL_SHIFT_CLOCK,
	.d_new_shift_clock = S88_DFL_SHIFT_CLOCK,
	.w_operating_mode = S88_MODE_ABSOLUTE,
	.d_size = (S88_DFL_BUS_LENGTH + 7) / 8,
	.l_interrupts = 0,
};


/**
 * data buffer for user access
 */

static BYTE gay_new_input_data[(S88_MAX_BUS_LENGTH + 7) / 8];
static BOOL gb_new_input_available = false;

static BYTE gay_last_read_input_data[(S88_MAX_BUS_LENGTH + 7) / 8] =
{
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
	0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
};


/***********************************************************************************************************************************
                    D R I V E R   A P I
 **********************************************************************************************************************************/

struct file_operations gx_s88_fops =
{
open:       s88_open,                    // open method
release:    s88_release,                 // close method
read:       s88_read,                    // read method
poll:       s88_poll,                    // poll method
	//    fasync:     s88_fasync,                  // asynchronous notification method
ioctl:      s88_ioctl,                   // I/O control method
};


// -----------------------------------------------------------------------------
// Function: s88_open
// Type: int
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief The drivers open method
//
//! (insert detailed function description here)
//!
//! \param px_inode pointer to inode structure (struct inode *)
//! \param px_file pointer to file structure (struct file *)
//! \todo function implementation
// -----------------------------------------------------------------------------

int s88_open(struct inode *px_inode, struct file *px_file)
{
	px_file->private_data = &gx_s88_dev;

	DEBUG_MSG("s88_open() called, dev=%p\n", &gx_s88_dev);

	return 0;
}


// -----------------------------------------------------------------------------
// Function: s88_release
// Type: int
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief The drivers Release method
//
//! (insert detailed function description here)
//!
//! \param px_inode pointer to inode structure (struct inode *)
//! \param px_file pointer to file structure (struct file *)
//! \todo function implementation
// -----------------------------------------------------------------------------


int s88_release(struct inode *px_inode, struct file *px_file)
{
	//S88_DEV_TYP *dev = px_file->private_data;

	DEBUG_MSG("s88_release() called, dev=%px\n", dev);

	return 0;

}


// -----------------------------------------------------------------------------
// Function: s88_ioctl
// Type: int
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief The drivers iotcl method
//
//! (insert detailed function description here)
//!
//! \param px_inode pointer to inode structure
//! \param px_file pointer to file structure
//! \param ui_cmd command code
//! \param d_arg command argument
//! \todo function implementation
// -----------------------------------------------------------------------------

int s88_ioctl(struct inode *px_inode, struct file *px_file,
              unsigned int ui_cmd, DWORD d_arg)
{
	S88_DEV_TYP *dev = px_file->private_data;
	int i_result = 0;

	DEBUG_MSG("s88_ioctl() called, dev=%p\n", dev);


	/**
	 * check command type and number
	 */

	if (_IOC_TYPE(ui_cmd) != S88_IOC_MAGIC)
	{
		WARNING_MSG("s88_ioctl(): wrong IOC_MAGIC (%c)\n", _IOC_TYPE(ui_cmd));
		return -EINVAL;
	}

	if (_IOC_NR(ui_cmd) > S88_IOC_MAX_NUMBER)
	{
		WARNING_MSG("s88_ioctl(): unknown command (%d)\n", ui_cmd);
		return -EINVAL;
	}


	/**
	 * obtain I/O control semaphore
	 */

	if (0 != down_interruptible(&dev->x_ioctl_sem))
	{
		INFO_MSG("s88_ioctl(): cannot obtain semaphore\n");
		return -EFAULT;
	}


	/**
	 * check and execute command
	 */

	switch (ui_cmd)
	{
		case S88_IOC_SET_CYCLE:
			if ((S88_MIN_CYCLE_TIME > d_arg) || (S88_MAX_CYCLE_TIME < d_arg))
			{
				DEBUG_MSG("s88_ioctl(,,S88_IOC_SET_CYCLE,%ld) rejected\n", d_arg);
				i_result = -EINVAL;
			}
			else
			{
				DEBUG_MSG("s88_ioctl(,,S88_IOC_SET_CYCLE,%ld) accepted\n", d_arg);
				dev->d_new_cycle_time = d_arg;
			}
			break;

		case S88_IOC_SET_CLOCK:
			if ((S88_MIN_SHIFT_CLOCK > d_arg) || (S88_MAX_SHIFT_CLOCK < d_arg))
			{
				DEBUG_MSG("s88_ioctl(,,S88_IOC_SET_CLOCK,%ld) rejected\n", d_arg);
				i_result = -EINVAL;
			}
			else
			{
				DEBUG_MSG("s88_ioctl(,,S88_IOC_SET_CLOCK,%ld) accepted\n", d_arg);
				dev->d_new_shift_clock = d_arg;
			}
			break;

		case S88_IOC_SET_BUS_LENGTH:
			if ((S88_MIN_BUS_LENGTH > d_arg) || (S88_MAX_BUS_LENGTH < d_arg))
			{
				DEBUG_MSG("s88_ioctl(,,S88_IOC_SET_BUS_LENGTH,%ld) rejected\n", d_arg);
				i_result = -EINVAL;
			}
			else
			{
				DEBUG_MSG("s88_ioctl(,,S88_IOC_SET_BUS_LENGTH,%ld) accepted\n", d_arg);
				dev->w_new_bus_length = d_arg;
			}
			break;

		case S88_IOC_SET_MODE:
			if ((0 > d_arg) || (S88_LAST_MODE < d_arg))
			{
				DEBUG_MSG("s88_ioctl(,,S88_IOC_SET_MODE,%ld) rejected\n", d_arg);
				i_result = -EINVAL;
			}
			else
			{
				DEBUG_MSG("s88_ioctl(,,S88_IOC_SET_MODE,%ld) accepted\n", d_arg);
				dev->w_operating_mode = (WORD)d_arg;
			}
			break;

		default:
			ERROR_MSG("s88_ioctl(): command %d not implemented\n", ui_cmd);
			i_result = -EINVAL;
			break;
	}


	/**
	 * release I/O control semaphore
	 */

	up(&dev->x_ioctl_sem);


	return i_result;
}


// -----------------------------------------------------------------------------
// Function: s88_read
// Type: ssize_t
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief the drivers read method
//
//! (insert detailed function description here)
//!
//! \param (describe a function parameter)
//! \return number of returned bytes or error
//! \todo function implementation
// -----------------------------------------------------------------------------

ssize_t s88_read(struct file *px_file, char __user *pc_buf, size_t d_count,
                 loff_t *pd_pos)
{
	S88_DEV_TYP *dev = px_file->private_data;

	ssize_t retval = 0;


	if (0 != down_interruptible(&dev->x_read_sem))
	{
		WARNING_MSG("s88_read(): cannot obtain semaphore\n");
		return -ERESTARTSYS;
	}

	if (*pd_pos >= dev->d_size)
	{
		WARNING_MSG("s88_read(): reading beyond end of file (%lld >= %lu)\n",
		            *pd_pos, dev->d_size);
		goto out;
	}


	while (!gb_new_input_available)
	{
		/*
		 * check file operation mode
		 */
		if (px_file->f_flags & O_NONBLOCK)
		{
			goto out;
		}

		wait_event_interruptible_timeout(dev->x_wait_q, gb_new_input_available, HZ);
	}


	gb_new_input_available = false;

	if (S88_MODE_ABSOLUTE == dev->w_operating_mode)
	{
		/**
		 * In mode S88_MODE_ABSOLUTE all requested inputs will be returned
		 */

		DEBUG_MSG("s88_read(): mode=S88_MODE_ABSOLUTE, pos=%lld, count=%d\n",
		          *pd_pos, d_count);

		if (*pd_pos + d_count > dev->d_size)
		{
			d_count = dev->d_size - *pd_pos;
		}

		if (copy_to_user(pc_buf, &gay_new_input_data[*pd_pos], d_count))
		{
			retval = -EFAULT;
			goto out;
		}
		*pd_pos += d_count;
		retval = d_count;
	}
	else
	{
		/**
		 * In mode S88_MODE_CHANGES only newly activated inputs of the
		 * requested range will be returned.
		 */

		BYTE ay_return_input_data[(S88_MAX_BUS_LENGTH + 7) / 8];
		BYTE ay_tmp_input_data[(S88_MAX_BUS_LENGTH + 7) / 8];
		int i;
		int act_count = 0;

		DEBUG_MSG("s88_read(): mode=S88_MODE_CHANGES, pos=%lld, count=%d\n",
		          *pd_pos, d_count);

		if (*pd_pos + d_count > dev->d_size)
		{
			d_count = dev->d_size - *pd_pos;
		}

		memcpy(&ay_tmp_input_data, &gay_new_input_data,
		       sizeof(ay_tmp_input_data));

		for (i = *pd_pos; i < (*pd_pos + d_count); i++)
		{
			if (0 != (ay_return_input_data[i] =
			              (ay_tmp_input_data[i] | gay_last_read_input_data[i]) -
			              gay_last_read_input_data[i]))
			{
				DEBUG_MSG("s88_read(): changed input, index=%d, data=%02x\n",
				          i, ay_return_input_data[i]);
				act_count = (i - *pd_pos) + 1;
			}
			gay_last_read_input_data[i] = ay_tmp_input_data[i];
		}

		if (copy_to_user(pc_buf, &ay_return_input_data[*pd_pos], act_count))
		{
			retval = -EFAULT;
			goto out;
		}
		*pd_pos += act_count;
		retval = act_count;
	}


out:
	up(&dev->x_read_sem);
	return retval;
}


// -----------------------------------------------------------------------------
// Function: s88_poll
// Type: unsigned int
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief the drivers poll method
//
//! (insert detailed function description here)
//!
//! \param px_file pointer to the file structure
//! \param px_poll_table contains wait queues that could wake up the process
//! \return bitmask describing the operations that can be performed without blocking
// -----------------------------------------------------------------------------

unsigned int s88_poll(struct file *px_file, struct poll_table_struct *px_poll_table)
{
	S88_DEV_TYP *px_dev = px_file->private_data;

	WORD w_mask = 0;

	DEBUG_MSG("s88_poll() called, dev=%p new_input_available=%d\n",
	          px_dev, gb_new_input_available);

	/**
	 * add wait queue to poll table
	 */

	poll_wait(px_file, &px_dev->x_wait_q, px_poll_table);


	/**
	 * prepare bit mask
	 */
	
	if (gb_new_input_available)
	{
		/**
		 * signal readability
		 */
		w_mask |= (POLLIN | POLLRDNORM);
	}

	return w_mask;
}


/*****************************************************************************************
                S H I F T - T I M E R   A D M I N I S T R A T I O N
 ****************************************************************************************/

struct timer_list gx_shift_timer;

static void s88_shift_timer_callback(void *data);


// -----------------------------------------------------------------------------
// Function: s88_shift_timer_init
// Type: void
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief Initialize shift clock generator
//
//! (insert detailed function description here)
//!
//! \param dev pointer to the driver data
//! \param period requested timer period in microseconds
// -----------------------------------------------------------------------------

static void s88_shift_timer_init(S88_DEV_TYP *dev, DWORD period)
{
#ifdef S88_USE_OST

	/**
	 * initialize ost4:
	 * resolution = 1 us, reset counter on match,
	 * counter is not externally synchronized,
	 * counter is periodic, write to OSCR starts counter
	 */

	OMCR4 = OMCR_C | OMCR_P | OMCR_R | OMCR_CRES_1US;


	/**
	 * write specified us value into match register
	 */

	OSMR4 = period;


	/**
	 * (re)enable interrupts by writing the corresponding status register bit
	 * and by writing the corresponding interrupt enable register
	 */

	OSSR = OSSR_M4;
	OIER |= OIER_E4;

#else

	/**
	* initialize software timer
	*/

	init_timer(&gx_shift_timer);
	gx_shift_timer.function = s88_shift_timer_callback;
	gx_shift_timer.data = dev;
#endif
}


// -----------------------------------------------------------------------------
// Function: s88_shift_timer_delete
// Type: void
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief Delete shift clock generator
//
//! (insert detailed function description here)
//!
//! \param dev pointer to the driver data
// -----------------------------------------------------------------------------

static void s88_shift_timer_delete(S88_DEV_TYP *dev)
{
#ifdef S88_USE_OST

	/**
	 * disable ost4 counter
	 */
	
	OMCR4 = 0;

	
	/**
	 * disable interrupts of ost4
	 */

	OIER &= ~OIER_E4;
#else

	/**
	* delete software timer
	*/

	del_timer_sync(&gx_shift_timer);
#endif
}


// -----------------------------------------------------------------------------
// Function: s88_shift_timer_restart
// Type: void
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief Restart shift clock generator
//
//! (insert detailed function description here)
//!
//! \param dev pointer to the driver data
//! \param period requested timer period
// -----------------------------------------------------------------------------

static void s88_shift_timer_restart(S88_DEV_TYP *dev, DWORD period)
{
#ifdef S88_USE_OST

	
	/**
	 * write specified us value into match register
	 * and start counter
	 */
	
	OSMR4 = period;
	OSCR4 = 0;

	
	/**
	 * (re)enable interrupts by writing the corresponding status register bit
	 * and by writing the corresponding interrupt enable register
	 */

	OSSR = OSSR_M4;
	OIER |= OIER_E4;
	
#else

	/**
	* restart software timer
	*/

	mod_timer(&gx_shift_timer, jiffies + usecs_to_jiffies(period));

#endif
}


// -----------------------------------------------------------------------------
// Function: s88_shift_timer_stop
// Type: void
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief stop shift clock generator
//
//! (insert detailed function description here)
//!
//! \param dev pointer to the driver data
// -----------------------------------------------------------------------------

static void s88_shift_timer_stop(S88_DEV_TYP *dev)
{
#ifdef S88_USE_OST

	/**
	 * clear interrupt status of pxa timer 4
	 */

	OSSR = OSSR_M4;


	/**
	 * disable interrupts of ost4
	 */

	OIER &= ~OIER_E4;
#else
#endif
}


/*****************************************************************************************
                      B A C K G R O U N D   P R O C E S S I N G
 ****************************************************************************************/

struct timer_list gx_scan_timer;


static DWORD gd_scan_counter = 0;
static DWORD gd_shift_counter = 0;

static WORD gw_act_bit = 0;
static bool gb_first_half = true;
static bool gb_in_cycle = false;
static DWORD gd_lost_scan_cycles = 0;
static DWORD gd_spurious_shift_cycles = 0;

static WORD gw_data[(S88_MAX_BUS_LENGTH + 15) / 16];
static BOOL gb_new_active_input = false;


// -----------------------------------------------------------------------------
// Function: s88_shift_timer_callback
// Type: void
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief perform s88 bit shifting
//
//! (insert detailed function description here)
//!
//! \param data unused
// -----------------------------------------------------------------------------

static void s88_shift_timer_callback(void *data)
{
	S88_DEV_TYP *dev = (S88_DEV_TYP *)data;
	int input;

	gd_shift_counter++;

	if (gb_in_cycle)
	{
		if (gb_first_half)
		{
			/**
			 * generate end of latch pulse during 1st bit
			 */

			pxa_gpio_set_value(GPIO_S88_LOAD, (0 == gw_act_bit));


			/**
			 * generate reset pulse during second bit
			 */
			pxa_gpio_set_value(GPIO_S88_RESET, (1 == gw_act_bit));


			/**
			 * activate clock line, then wait
			 */
			pxa_gpio_set_value(GPIO_S88_CLOCK, 1);
			s88_shift_timer_restart(dev,
			                        (dev->d_shift_clock - 10
			                         - (S88_SHIFT_CLOCK_PAUSE / 3)));
		}
		else
		{
			/**
			 * input and store one bit
			 */

			WORD *pw_data = &gw_data[(gw_act_bit >> 4) & 31];
			WORD w_mask = (1 << (gw_act_bit & 0x0f));

			input = pxa_gpio_get_value(GPIO_S88_DATA);
			if (input)
			{
				if (0 == (*pw_data & w_mask))
				{
					gb_new_active_input = true;
				}
				*pw_data |= w_mask;
			}
			else
			{
				*pw_data &= ~w_mask;
			}


			/**
			 * deactivate clock line
			 */

			pxa_gpio_set_value(GPIO_S88_CLOCK, 0);


			/**
			 * increment bit counter, then restart wait timer
			 * for next bit or terminate cycle
			 */

			gw_act_bit++;
			if (dev->w_bus_length > gw_act_bit)
			{
				s88_shift_timer_restart(dev, (S88_SHIFT_CLOCK_PAUSE / 3));
			}
			else
			{
				s88_shift_timer_stop(dev);
				gb_in_cycle = false;

				memcpy(&gay_new_input_data, &gw_data, sizeof(gay_new_input_data));
				if ((S88_MODE_ABSOLUTE == dev->w_operating_mode) ||
				    (gb_new_active_input))
				{
					gb_new_input_available = true;
					wake_up_interruptible(&dev->x_wait_q);
				}
			}
		}
		gb_first_half = !gb_first_half;
	}
	else
	{
		/**
		 * shift clock is running, but not in scan cycle
		 */

		gd_spurious_shift_cycles++;
		s88_shift_timer_stop(dev);
	}
}


// -----------------------------------------------------------------------------
// Function: s88_scan_timer_callback
// Type: void
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief start new scan cycle
//
//! (insert detailed function description here)
//!
//! \param data pointer to device structure
// -----------------------------------------------------------------------------

static void s88_scan_timer_callback(unsigned long data)
{
	S88_DEV_TYP *dev = (S88_DEV_TYP *)data;
	DWORD d_min_cycle = S88_DFL_CYCLE_TIME;


	/**
	 * restart scan timer
	 */

	mod_timer(&gx_scan_timer, jiffies + msecs_to_jiffies(dev->d_cycle_time));


	/**
	 * activate changed parameters
	 */

	if (0 == down_trylock(&dev->x_ioctl_sem))
	{
		if (dev->w_new_bus_length != dev->w_bus_length)
		{
			dev->w_bus_length = dev->w_new_bus_length;
			dev->d_size = (dev->w_bus_length + 7) / 8;
			DEBUG_MSG("s88_scan_timer_callback(): new bus length is %u\n",
			          dev->w_bus_length);
		}

		if (dev->d_new_shift_clock != dev->d_shift_clock)
		{
			dev->d_shift_clock = dev->d_new_shift_clock;
			s88_shift_timer_delete(dev);
			s88_shift_timer_init(dev,
			                     (dev->d_shift_clock - 10 - (S88_SHIFT_CLOCK_PAUSE / 3)));
			DEBUG_MSG("s88_scan_timer_callback(): new shift clock is %ld\n",
			          dev->d_shift_clock);
		}

		/**
		 * minimum cycle time depends on bus length and
		 * shift clock frequency
		 */

		d_min_cycle = (dev->d_shift_clock * (DWORD)dev->w_bus_length) / 1000 + 1;
		if (d_min_cycle > dev->d_new_cycle_time)
		{
			dev->d_new_cycle_time = d_min_cycle;
		}
		
		if (dev->d_new_cycle_time != dev->d_cycle_time)
		{
			dev->d_cycle_time = dev->d_new_cycle_time;
			DEBUG_MSG("s88_scan_timer_callback(): new scan cycle is %ld\n",
			          dev->d_cycle_time);
		}
		up(&dev->x_ioctl_sem);
	}
	else
	{
		WARNING_MSG("s88_scan_timer_callback(): could not lock semaphore!\n");
	}


	if (0 < dev->w_bus_length)
	{
		/**
		 * start new scan cycle if no scan cycle running
		 */

		if (!gb_in_cycle)
		{
			gd_scan_counter++;


			/**
			 * latch s88 data
			 */
			pxa_gpio_set_value(GPIO_S88_LOAD, 1);


			/**
			 * start first shift cycle
			 */

			gb_in_cycle = true;
			gw_act_bit = 0;
			gb_first_half = true;
			gb_new_active_input = false;
			s88_shift_timer_restart(dev,
			                        (dev->d_shift_clock - 10 - (S88_SHIFT_CLOCK_PAUSE / 3)));
		}
		else
		{
			gd_lost_scan_cycles++;
		}
	}
}


// -----------------------------------------------------------------------------
// Function: s88_ost_handler
// Type: irqreturn_t
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief Handler for PXA operating system timer interrupt
//
//! (insert detailed function description here)
//!
//! \param i_irq interrupt number
//! \param pv_dev_id pointer to driver data
//! \return IRQ_HANDLED (handler could handle interrupt)
// -----------------------------------------------------------------------------

irqreturn_t s88_ost_handler(int i_irq, void *pv_dev_id)
{
	S88_DEV_TYP *px_dev = (S88_DEV_TYP *)pv_dev_id;


	/**
	 * perform s88 bit shifting
	 */

	s88_shift_timer_callback(px_dev);


	/**
	 * increment interrupt counter
	 */

	if (NULL != px_dev)
	{
		px_dev->l_interrupts++;

	}

	return IRQ_HANDLED;
}



/*****************************************************************************************
                   / P R O C  -  I N T E R F A C E
 ****************************************************************************************/

// -----------------------------------------------------------------------------
// Function: s88_read_procmem
// Type: int
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief show driver status in /proc directory
//
//! (insert detailed function description here)
//!
//! \param pc_buf pointer to result buffer
//! \param ppc_start pointer to start pointer
//! \param l_offset offset
//! \param i_count count
//! \param pi_eof EOF pointer
//! \param pv_data data pointer
//! \return number of bytes copied to result buffer
// -----------------------------------------------------------------------------

int s88_read_procmem(char *pc_buf, char **ppc_start,
                     off_t l_offset, int i_count, int *pi_eof, void *pv_data)
{
	S88_DEV_TYP *px_dev = pv_data;
	int i_len = 0;
	int i;


	/**
	 * print driver version
	 */

	i_len += sprintf(pc_buf + i_len, "CST2 S88 driver "S88_VERSION"\n");


	/**
	 *  print driver settings
	 */

	i_len += sprintf(pc_buf + i_len, "\n--- driver settings ---\n");
	i_len += sprintf(pc_buf + i_len, "bus length :             %7d bits\n",
	                 px_dev->w_bus_length);
	i_len += sprintf(pc_buf + i_len, "aquisition cycle time :  %7ld ms\n",
	                 px_dev->d_cycle_time);
	i_len += sprintf(pc_buf + i_len, "shift clock cycle time : %7ld us\n",
	                 px_dev->d_shift_clock);
	i_len += sprintf(pc_buf + i_len, "operating mode :         %7d\n",
	                 px_dev->w_operating_mode);


	/**
	 *  print driver status
	 */

	i_len += sprintf(pc_buf + i_len, "\n--- driver status ---\n");
	i_len += sprintf(pc_buf + i_len, "scan counter :           %7ld\n",
	                 gd_scan_counter);
	i_len += sprintf(pc_buf + i_len, "shift counter :          %7ld\n",
	                 gd_shift_counter);
	i_len += sprintf(pc_buf + i_len, "bit number :             %7d\n",
	                 gw_act_bit);
	i_len += sprintf(pc_buf + i_len, "cycle running :          %7s\n",
	                 gb_in_cycle ? "   true" : "  false");
	i_len += sprintf(pc_buf + i_len, "first half of shift :    %7s\n",
	                 gb_first_half ? "   true" : "  false");
	i_len += sprintf(pc_buf + i_len, "lost scan cycles :       %7ld\n",
	                 gd_lost_scan_cycles);
	i_len += sprintf(pc_buf + i_len, "spurious shift cycles :  %7ld\n",
	                 gd_spurious_shift_cycles);


	/**
	 *  print input status
	 */

	i_len += sprintf(pc_buf + i_len, "\n--- input status ---\n");
	i_len += sprintf(pc_buf + i_len, "new data available:      %7s\n",
	                 gb_new_input_available ? "   true" : "  false");
	
	i_len += sprintf(pc_buf + i_len, "data : ");
	for (i = 0; i < ((px_dev->w_bus_length + 15) / 16); i++)
	{
		i_len += sprintf(pc_buf + i_len, "%04x ", gw_data[i]);
	}
	i_len += sprintf(pc_buf + i_len, "\n");


	/**
	 * set EOF flag and return
	 */

	*pi_eof = 1;

	return i_len;

}


/*****************************************************************************************
                  I N I T I A L I Z A T I O N   /   C L E A N U P
 ****************************************************************************************/

static bool b_proc_entry_created = false;
static bool b_cdev_added = false;
static bool b_devno_allocated = false;
static bool gb_ost_interrupt_installed = false;


static void s88_exit(void);


// -----------------------------------------------------------------------------
// Function: s88_init
// Type: int
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief driver initialization
//
//! (insert detailed function description here)
//!
//! \return 0 on success
// -----------------------------------------------------------------------------

static int __init s88_init(void)
{
	int retval = 0;

	DEBUG_MSG("module initialization\n");


	/**
	 * create /proc entry
	 */

	if (NULL != create_proc_read_entry("cst2_s88", 0, NULL, s88_read_procmem, &gx_s88_dev))
	{
		b_proc_entry_created = true;
	}
	else
	{
		ERROR_MSG("s88_init(): could not create /proc entry\n");
		retval = -1;
		goto exit;
	}


	/**
	 * set interrupt handler
	 */

	retval = request_irq(IRQ_OST_4_11, s88_ost_handler, IRQF_SHARED, "s88 ost4",
	                       &gx_s88_dev);
	if (0 == retval)
	{
		gb_ost_interrupt_installed = true;
	}
	else
	{
		ERROR_MSG("s88_init(): could not install handler for interrupt %d\n",
		          IRQ_OST_4_11);
		goto exit;
	}


	/**
	 * initialize S88 gpios
	 */

	pxa_gpio_mode(GPIO_S88_RESET | GPIO_OUT);
	pxa_gpio_set_value(GPIO_S88_RESET, 0);
	pxa_gpio_mode(GPIO_S88_LOAD | GPIO_OUT);
	pxa_gpio_set_value(GPIO_S88_LOAD, 0);
	pxa_gpio_mode(GPIO_S88_CLOCK | GPIO_OUT);
	pxa_gpio_set_value(GPIO_S88_CLOCK, 0);
	pxa_gpio_mode(GPIO_S88_DATA | GPIO_IN);


	/**
	 * initialize semaphore
	 */

	sema_init(&gx_s88_dev.x_ioctl_sem, 1);
	sema_init(&gx_s88_dev.x_read_sem, 1);
	init_waitqueue_head(&gx_s88_dev.x_wait_q);


	/**
	 * initialize shift timer, enable timer interrupts
	 */

	s88_shift_timer_init(&gx_s88_dev,
	                     (gx_s88_dev.d_shift_clock - 10 - (S88_SHIFT_CLOCK_PAUSE / 3)));


	/**
	 * initialize scan timer
	 */

	init_timer(&gx_scan_timer);
	gx_scan_timer.function = s88_scan_timer_callback;
	gx_scan_timer.data = (unsigned long) & gx_s88_dev;


	/**
	 * initialize status variables
	 */

	gd_scan_counter = 0;
	gd_shift_counter = 0;
	gd_lost_scan_cycles = 0;
	gd_spurious_shift_cycles = 0;


	/**
	 * start scan timer
	 */

	mod_timer(&gx_scan_timer, jiffies + msecs_to_jiffies(gx_s88_dev.d_cycle_time));


	retval = alloc_chrdev_region(&gx_s88_dev.x_devno, 0, 1, "cst2_s88");
	if (0 == retval)
	{
		b_devno_allocated = true;
	}
	else
	{
		ERROR_MSG("s88_init(): error %d allocating device number\n", retval);
		goto exit;
	}
	
	
	cdev_init(&gx_s88_dev.x_cdev, &gx_s88_fops);
	gx_s88_dev.x_cdev.owner = THIS_MODULE;
	retval = cdev_add(&gx_s88_dev.x_cdev, gx_s88_dev.x_devno, 1);
	if (0 == retval)
	{
		b_cdev_added = true;
	}
	else
	{
		ERROR_MSG("s88_init(): error %d adding character device\n", retval);
		goto exit;
	}

	
exit:
	if (0 == retval)
	{
		INFO_MSG(S88_VERSION" registered successfully: device major=%d\n",
		         MAJOR(gx_s88_dev.x_devno));
	}
	else
	{
		s88_exit();
	}
	return retval;
}


// -----------------------------------------------------------------------------
// Function: s88_exit
// Type: void
// Initial Author: Christian Biermann
// -----------------------------------------------------------------------------
// Description:
//! \brief driver cleanup
//
//! (insert detailed function description here)
//!
// -----------------------------------------------------------------------------

static void __exit s88_exit(void)
{
	DEBUG_MSG("module termination\n");

	
	if (b_cdev_added)
	{
		b_cdev_added = false;
		cdev_del(&gx_s88_dev.x_cdev);
	}

	
	if (b_devno_allocated)
	{
		b_devno_allocated = false;
		unregister_chrdev_region(gx_s88_dev.x_devno, 1);
	}

	
	/**
	 * delete scan timer
	 */

	del_timer_sync(&gx_scan_timer);


	/**
	 * delete shift timer
	 */

	s88_shift_timer_delete(&gx_s88_dev);



	/**
	 * terminate interrupt handler
	 */

	if (gb_ost_interrupt_installed)
	{
		free_irq(IRQ_OST_4_11, &gx_s88_dev);
		gb_ost_interrupt_installed = false;
	}


	/**
	 * remove /proc entry
	 */

	if (b_proc_entry_created)
	{
		b_proc_entry_created = false;
		remove_proc_entry("cst2_s88", NULL);
	}

	
	INFO_MSG(S88_VERSION" unregistered successfully\n");
}


MODULE_DESCRIPTION("Maerklin Central Station II - s88 driver");
MODULE_AUTHOR("Christian Biermann (bie@ies-gmbh.de)");
MODULE_LICENSE("Dual BSD/GPL");

module_init(s88_init);
module_exit(s88_exit);


