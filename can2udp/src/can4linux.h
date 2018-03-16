/*
 * can4linux.h - can4linux CAN driver module
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 2001-2011 oe @ port GmbH Halle/Saale
 * Copyright (c) 2013 Heinz-Jürgen Oertel <hj.oertel@t-online.de>
 *------------------------------------------------------------------
 *
 */


/**
* \file can.h
* \author Heinz-Jürgen Oertel
*
* can4linux interface definitions
*
*
*
*/


#ifndef __CAN_H
#define __CAN_H


#define CAN4LINUXVERSION 0x0403 /* Version 4.3 */

#ifndef __KERNEL__
#include <sys/time.h>
#endif
#include <linux/types.h>

 /*---------- the can message structure */

#if defined CANFD
# define CAN_MSG_LENGTH 64		/**< maximum length of a CAN frame */
#else
# define CAN_MSG_LENGTH 8		/**< maximum length of a CAN frame */
#endif


/* values of the canmsg.flags field */
#define MSG_ACTIVE	(0)		/**< Controller Error Active */
#define MSG_BASE	(0)		/**< Base Frame Format */
#define MSG_RTR		(1<<0)		/**< RTR Message */
#define MSG_OVR		(1<<1)		/**< CAN controller Msg overflow error */
#define MSG_EXT		(1<<2)		/**< extended message format */
#define MSG_SELF	(1<<3)		/**< message received from own tx */
#define MSG_PASSIVE	(1<<4)		/**< controller in error passive */
#define MSG_BUSOFF	(1<<5)		/**< controller Bus Off  */
#define MSG_WARNING	(1<<6)		/**< CAN Warning Level reached */
#define MSG_BOVR	(1<<7)		/**< receive/transmit buffer overflow */

#define MSG_CANFD	(1<<8)		/**< Frame is a CAN FD frame */
#define MSG_RBRS	(1<<9)		/**< Bit rate switch, frame with fast data phase */
#define MSG_RESI	(1<<10)		/**< received Error Status Indicator */
/**
* mask used for detecting CAN errors in the canmsg_t flags field
*/
#define MSG_ERR_MASK	(MSG_OVR+MSG_PASSIVE+MSG_BUSOFF+MSG_BOVR+MSG_WARNING)


/* valid bits in CAN ID for frame formats */
#define CAN_SFF_MASK 0x000007FFU	/* base frame format (SFF) */
#define CAN_EFF_MASK 0x1FFFFFFFU	/* extended frame format (EFF) */
#define CANDRIVERERROR  0xFFFFFFFFul	/* invalid CAN ID == Error */

/**
* The CAN message structure.
* Used for all data transfers between the application and the driver
* using read() or write().
*
* For reporting errors the fields \e flags and \e id are used.
*/
#if 1
typedef struct {
	/** flags, indicating or controlling special message properties */
	int             flags;
	int             cob;	 /**< CAN object number, used in Full CAN  */
	unsigned   long id;	 /**< CAN message ID, 4 bytes
	    if in receive mode an id 0xFFFF.FFFF is received,
	    the driver reports an internal or CAN controller error condition.
	    In this case the \e flags field has to be evaluated. */
	struct timeval  timestamp;	/**< time stamp for received messages */
	short      int  length;	 /**< number of bytes in the CAN message */
	unsigned   char data[CAN_MSG_LENGTH];
	/**< data, 0...8 bytes for classic CAN
	 and 0...64 bytes for CAN FD extended data frames */
} canmsg_t;
#else
typedef struct {
	/** flags, indicating or controlling special message properties */
	__u32	flags;
	__u32	cob;	 /**< CAN object number, used in Full CAN  */
	__u32	id;	 /**< CAN message ID, 4 bytes
	    if in receive mode an id 0xFFFF.FFFF is received,
	    the driver reports an internal or CAN controller error condition.
	    In this case the \e flags field has to be evaluated. */
	struct timeval  timestamp;	/**< time stamp for received messages */
	__u32	length;	 /**< number of bytes in the CAN message */
	__u8 x;
	__u8	data[CAN_MSG_LENGTH] __attribute__((aligned(8)));
	/**< data, 0...8 bytes for classic CAN
	 and 0...64 bytes for CAN FD extended data frames */
} canmsg_t;
#endif



/**
---------- IOCTL requests */
/* Use 'c' as magic number, follow chapter 6 of LDD3 */
#define CAN4L_IOC_MAGIC 'c'

#define CAN_IOCTL_COMMAND	0	/**< IOCTL command request */
#define CAN_IOCTL_CONFIG	1	/**< IOCTL configuration request */
#define CAN_IOCTL_SEND		2	/**< IOCTL request */
#define CAN_IOCTL_CONFIGURERTR	4	/**< IOCTL request */
#define CAN_IOCTL_STATUS	5	/**< IOCTL status request */

/*---------- CAN ioctl parameter types */
/**
 IOCTL Command request parameter structure */
struct command_par {
	int cmd;			/**< special driver command */
	int target;			/**< special configuration target */
	unsigned long val1;		/**< 1. parameter for the target */
	unsigned long val2;		/**< 2. parameter for the target */
	int error;			/**< return value */
	unsigned long retval;		/**< return value */
};


/**
 IOCTL Command request parameter structure */
typedef struct command_par command_par_t; /**< Command parameter struct */
/**
 IOCTL CConfiguration request parameter structure */
typedef struct command_par config_par_t; /**< Configuration parameter struct */


/**
 IOCTL generic CAN controller status request parameter structure */
typedef struct can_statuspar {
	unsigned int baud;		/**< actual bit rate */
	unsigned int status;		/**< CAN controller status register */
	unsigned int error_warning_limit;	/**< the error warning limit */
	unsigned int rx_errors;		/**< content of RX error counter */
	unsigned int tx_errors;		/**< content of TX error counter */
	unsigned int error_code;	/**< content of error code register */
	unsigned int rx_buffer_size;	/**< size of rx buffer  */
	unsigned int rx_buffer_used;	/**< number of messages */
	unsigned int tx_buffer_size;	/**< size of tx buffer  */
	unsigned int tx_buffer_used;	/**< number of messages */
	unsigned long retval;		/**< return value */
	unsigned int type;		/**< CAN controller / driver type */
} can_statuspar_t;

/**
 IOCTL  CanStatusPar.type CAN controller hardware chips */
#define CAN_TYPE_UNSPEC		0
#define CAN_TYPE_SJA1000	1
#define CAN_TYPE_FlexCAN	2
#define CAN_TYPE_TouCAN		3
#define CAN_TYPE_82527		4
#define CAN_TYPE_TwinCAN	5
#define CAN_TYPE_BlackFinCAN	6
#define CAN_TYPE_AT91SAM9263	7
#define CAN_TYPE_MCP2515	8
#define CAN_TYPE_XCANPS		9
#define CAN_TYPE_DCAN		10
#define CAN_TYPE_IFI_CAN_FD	11
#define CAN_TYPE_ALLWINNER_CAN	12
#define CAN_TYPE_VIRTUAL	100


/**
 IOCTL Send request parameter structure */
typedef struct send_par {
	canmsg_t *tx;			/**< CAN message struct  */
	int error;			/**< return value for errno */
	unsigned long retval;		/**< return value */
} send_par_t;

/**
 IOCTL Receive request parameter structure */
typedef struct receive_par {
	canmsg_t *rx;			/**< CAN message struct  */
	int error;			/**< return value for errno */
	unsigned long retval;		/**< return value */
} receive_par_t;

/**
IOCTL ConfigureRTR request parameter structure */
typedef struct configure_rtr_par {
	unsigned message;		/**< CAN message ID */
	canmsg_t *tx;			/**< CAN message struct  */
	int error;			/**< return value for errno */
	unsigned long retval;		/**< return value */
} configure_rtr_par_t;

/**
---------- IOCTL Command subcommands and there targets */

# define CMD_START		1
# define CMD_STOP		2
# define CMD_RESET		3
# define CMD_CLEARBUFFERS	4
# define CMD_CTRL_LED		5 /* on board LEDS */
# define CMD_CTRL_TERM		6 /* termination resistor */


/* LEDs on Interface Boards (can4linux-examples/canled.c) */
enum can_led_color {
		green  = 1,
		yellow = 2,
		red    = 4
	    };
enum can_led_state {
		off = 0,
		on
	    };

enum can_termination_state {
		term_off = 0,
		term_on
	    };


/**
---------- IOCTL Configure targets */

# define CONF_ACC		0	/* mask and code */
# define CONF_ACCM		1	/* mask only */
# define CONF_ACCC		2	/* code only */
# define CONF_TIMING		3	/* bit timing */
# define CONF_OMODE		4	/* output control register */
# define CONF_FILTER		5
# define CONF_FENABLE		6
# define CONF_FDISABLE		7
# define CONF_LISTEN_ONLY_MODE	8	/* for SJA1000 PeliCAN */
# define CONF_SELF_RECEPTION	9	/* */
# define CONF_BTR		10      /* set direct bit timing registers
					   (SJA1000) */
# define CONF_TIMESTAMP		11      /* use TS in received messages */
# define CONF_WAKEUP		12      /* wake up processes */
# define CONF_SPEEDFACTOR       13      /* speedfactor for CAN-FD second bitrate*/

# define  CONF_ACC1	100		/* for addidional code/mask settings */
# define  CONF_ACC2	101		/* for addidional code/mask settings */
# define  CONF_ACC3	102		/* for addidional code/mask settings */
# define  CONF_ACC4	103		/* for addidional code/mask settings */
# define  CONF_ACC5	104		/* for addidional code/mask settings */
# define  CONF_ACC6	105		/* for addidional code/mask settings */
# define  CONF_ACC7	106		/* for addidional code/mask settings */

#endif	/* __CAN_H */
