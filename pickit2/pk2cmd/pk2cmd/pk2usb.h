//
// A USB interface to the Microchip(tm) PICkit(tm) 2 FLASH Starter Kit
// device programmer and breadboard.
// PIC, PICkit2 are registered trademarks of Microchip, Inc.
//
// By Jeff Post, j_post <AT> pacbell <DOT> net
//
//-----------------------------------------------------------------------------
//
//	This program is free software; you can redistribute it and/or
//	modify it under the terms of the GNU General Public License
//	as published by the Free Software Foundation; either version 3
//	of the License, or (at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//-----------------------------------------------------------------------------
//
#ifndef	WIN32

#ifndef _PK2USB_H
#define _PK2USB_H

// To enable USB debugging info, set USB_DEBUG_FLAGS to a combination of any of
// the following:

#define	USB_DEBUG_XMIT		1		// display transmitted data
#define	USB_DEBUG_RECV		2		// display received data
#define	USB_DEBUG_CMD		(4 | USB_DEBUG_XMIT)		// interpret commands in display
#define	USB_DEBUG_SCRIPT	(8 | USB_DEBUG_XMIT)		// interpret scripts in display
#define	USB_DEBUG_DRIVER	0x10	// enable libusb debugging info (Linux only)
#define	USB_DEBUG_FULL	(USB_DEBUG_RECV | USB_DEBUG_CMD | USB_DEBUG_SCRIPT)
#define	USB_DEBUG_MAX	(USB_DEBUG_RECV | USB_DEBUG_CMD | USB_DEBUG_SCRIPT | USB_DEBUG_DRIVER)

// Examples:
//#define	USB_DEBUG_FLAGS	(USB_DEBUG_XMIT | USB_DEBUG_RECV)
//#define	USB_DEBUG_FLAGS	USB_DEBUG_FULL

#define	USB_DEBUG_FLAGS	0		// No USB debugging by default

#ifndef __APPLE__
#include	<usb.h>	// Linux
typedef struct usb_dev_handle pickit_dev;
extern usb_dev_handle	*deviceHandle;
#else		// Mac OSX

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/IOMessage.h>
#include <IOKit/hid/IOHIDLib.h>
#include <AvailabilityMacros.h>

struct hidreport {
 	IOHIDDeviceInterface122 **intf;
	char	*buffer;
 	int	size;
 	int	size_received;
 	int	timeout;
};

typedef struct hidreport pickit_dev;
typedef struct hidreport *hidreport_t;

extern pickit_dev	*deviceHandle;
#endif  // Mac OSX

#define	byte		unsigned char
#define	uint		unsigned int
#define	ushort	unsigned short int

#define	reqLen	64			// PICkit 2 always uses 64-byte transfers

#define	CONFIG_HID		1	// Use HID for pickit configuration
#define	CONFIG_VENDOR	2	// Vendor specific configuration

#define	NORMAL_MODE		1
#define	BOOTLOAD_MODE	2

#define	AUTOPOWER		0	// auto detect target power
#define	PK2POWER			1	// power target from PICkit2
#define	SELFPOWER		2	// target is self-powered

#define	MINFWVERSION	0x020a00

// Bootloader commands

#define	READFWFLASH		1
#define	WRITEFWFLASH	2
#define	ERASEFWFLASH	3
#define	READFWEEDATA	4
#define	WRITEFWEEDATA	5
#define	NOOPERATION		'Z'
#define	RESETFWDEVICE	0xff

// PICkit2 commands (version 2 firmware)

#define	ENTERBOOTLOADER	'B'	// 0x42 - version 1 and version 2
#define	POWERCTRL			'V'	// 0x56
#define	GETVERSION			'v'	// 0x76 - version 1 and version 2
#define	SETVDD				0xa0
#define	SETVPP				0xa1
#define	READ_STATUS			0xa2
#define	READ_VOLTAGES		0xa3
#define	DOWNLOAD_SCRIPT	0xa4
#define	RUN_SCRIPT			0xa5	// run script from buffer
#define	EXECUTE_SCRIPT		0xa6	// run included script
#define	CLR_DOWNLOAD_BFR	0xa7
#define	DOWNLOAD_DATA		0xa8
#define	CLR_UPLOAD_BFR		0xa9
#define	UPLOAD_DATA			0xaa
#define	CLR_SCRIPT_BFR		0xab
#define	UPLOAD_DATA_NOLEN	0xac
#define	END_OF_BFR			0xad
#define	RESET					0xae
#define	SCRIPT_BFR_CHKSM	0xaf
#define	WR_INTERNAL_EE		0xb1
#define	RD_INTERNAL_EE		0xb2

// Script commands
#define	BUSY_LED_OFF		0xf4
#define	BUSY_LED_ON			0xf5
#define	MCLR_GND_OFF		0xf6
#define	MCLR_GND_ON			0xf7
#define	VPP_PWM_OFF			0xf8
#define	VPP_PWM_ON			0xf9
#define	VPP_OFF				0xfa
#define	VPP_ON				0xfb
#define	VDD_GND_OFF			0xfc
#define	VDD_GND_ON			0xfd
#define	VDD_OFF				0xfe
#define	VDD_ON				0xff

extern bool	verbose;
extern int	pickit_interface;
extern int	pickit2mode;
extern int	usbdebug;
extern int	pickit2firmware;
extern int	targetPower;
extern FILE	*usbFile;
extern byte	cmd[reqLen + 1];

class CUsbhidioc //: public CDialog
{
public:
	CUsbhidioc(void);
	char *GetPK2UnitID(void);
    bool FindTheHID(int unitIndex);
    bool ReadReport (char *);
    bool WriteReport(char *, unsigned int);
    void CloseReport ();

protected:
//    void GetDeviceCapabilities();
//    void PrepareForOverlappedTransfer();

		char m_UnitID[32];
};

struct scriptInterpreter {
	byte	scriptcode;	// opcode in script buffer
	byte	args;			// number of arguments (255 = variable, first arg is count, 254 = script #, then count)
	char	*name;		// mnemonic for script
};

extern struct scriptInterpreter scriptInterpreter[];
extern char	*scriptNames[];

extern int	indexScriptInterpreter(byte code);
extern void	showUSBCommand(byte *src, int len);
extern void	sendPickitCmd(pickit_dev *d, byte *src, int len);
extern void	sendScriptCmd(pickit_dev *d, byte *src, int len);
extern void	enableTargetPower(pickit_dev *d);
extern void	disableTargetPower(pickit_dev *d);
extern void	pickitOn(pickit_dev *d);
extern void	pickitOff(pickit_dev *d);

extern bool	FindTheHID(int unitIndex);
extern bool	ReadReport(char InBuffer[]);
extern bool	WriteReport(char OutputBuffer[], unsigned int nBytes);
extern void	CloseReport(void);

// Send data over usb
extern int sendUSB(pickit_dev *d, byte *src, int len);

// Read data from usb
extern int recvUSB(pickit_dev *d, int len, byte *dest);

// Open the pickit as a usb device.  Aborts on errors.
extern pickit_dev *usbPickitOpen(int unitIndex, char *id);

#ifdef __APPLE__
extern void usb_release_interface(pickit_dev *deviceHandle, int pickit_interface);
#endif

#endif  // _PK2USB_H

#endif  // WIN32
