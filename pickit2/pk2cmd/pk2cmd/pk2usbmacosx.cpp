/*
 * HID Report for MacOSX
 *
 *
 * USB interface for Mac OSX
 */
/*

     Portions Copyright (c) 2005 Shigenobu Kimura, All rights reserved.

	 Other licenses included with this software do not apply to
	 the software code in this file.  The following license applies
	 to the contents of this file, and the contents of this file
	 only.

     Permission is hereby granted, free of charge, to any person
     obtaining a copy of this software and associated documentation
     files (the "Software"), to deal in the Software without restriction,
     including without limitation the rights to use, copy, modify,
     merge, publish, distribute, sublicense, and/or sell copies of
     the Software, and to permit persons to whom the Software is
     furnished to do so, subject to the following conditions:

     The above copyright notice and this permission notice shall be
     included in all copies or substantial portions of the Software.

     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
     OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
     NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
     BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
     AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
     OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
     IN THE SOFTWARE.

*/

// Modified by Jeff Post to obtain PICkit2 UnitID from USB device descriptor.

#ifndef WIN32

#include	"stdafx.h"
#include	"pk2usb.h"
#include	"pk2usbmacosx.h"

// Definitions for reading PICit2 descriptor serial number (UnitID)

struct recordEntry
{
	IOHIDElementType	type;
	long		usagePage;
	long		usage;
	IOHIDElementCookie	cookie;
	long		min;
	long		max;
	long		scaledMin;
	long		scaledMax;
	long		size;
	Boolean	relative;
	Boolean	wrapping;
	Boolean	nonLinear;
	Boolean	preferredState;
	Boolean	nullState;
	long		units;
	long		unitExp;
	Str255	name;

	long	minReport;
	long	maxReport;
	long	userMin;
	long	userMax;
    
	struct recordEntry	*previous;
	struct recordEntry	*child;
	struct recordEntry	*sibling;
};

struct deviceRecord
{
	IOHIDDeviceInterface	**interface;
	IOHIDQueueInterface	**queue;
	CFRunLoopSourceRef	queueRunLoopSource;
	IOHIDOutputTransactionInterface	**transaction;
	io_object_t	notification;
	Str255		transport;
	long			vendorID;
	long			productID;
	long			version;
	Str255		manufacturer;
	Str255		product;
	Str255		serial;
	long			locID;
	long			usage;
	long			usagePage;
	long			totalRecords;
	long			features;
	long			inputs;
	long			outputs;
	long			collections;
	long			axis;
	long			buttons;
	long			hats;
	long			sliders;
	long			dials;
	long			wheels;
	struct recordEntry	*entryList;
	struct deviceRecord	*next;
};

// Data

pickit_dev	*deviceHandle;
char	PK2UnitIDString[32];

/*
 * 	  Private functions
 */

static void set_number_to_dictionary(CFMutableDictionaryRef dic, const void *key, SInt32 num)
{
	CFNumberRef	n;

	n = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &num);
	CFDictionarySetValue(dic, key, n);
	CFRelease(n);
}

// Create and open an interface to device, required prior to extracting values or building queues
// Note: application now owns the device and must close and release it prior to exiting

unsigned long openDeviceInterface(UInt32 hidDevice, struct deviceRecord *deviceRec)
{
	IOReturn result = 0;
	HRESULT plugInResult = 0;
	SInt32 score = 0;
	IOCFPlugInInterface **plugInInterface = NULL;

	if (!deviceRec->interface)
	{
		result = IOCreatePlugInInterfaceForService(hidDevice, kIOHIDDeviceUserClientTypeID,
						kIOCFPlugInInterfaceID, &plugInInterface, &score);

		if (kIOReturnSuccess == result)
		{
			// Call a method of the intermediate plug-in to create the device interface
			plugInResult =
				(*plugInInterface)->QueryInterface(plugInInterface,
				CFUUIDGetUUIDBytes(kIOHIDDeviceInterfaceID), (void **) &(deviceRec->interface));

			if (verbose && plugInResult)
				printf("Can't get HID device info.\n");

			IODestroyPlugInInterface(plugInInterface);
		}
		else if (verbose)
			printf("Failed to create USB interface.\n");
	}

	if (deviceRec->interface)
	{
		result = (*(IOHIDDeviceInterface**) deviceRec->interface)->open(deviceRec->interface, 0);

		if (verbose && result)
			printf("Failed to open USB device interface.\n");
	}

	return result;
}

// Get device info from CF dictionary records

static void getDeviceInfo(io_object_t hidDevice, CFMutableDictionaryRef hidProperties, struct deviceRecord *deviceRec)
{
	CFMutableDictionaryRef	usbProperties = 0;
	io_registry_entry_t		parent1, parent2;
	CFTypeRef					refCF = 0;

	if ((!IORegistryEntryGetParentEntry(hidDevice, kIOServicePlane, &parent1)) &&
		(!IORegistryEntryGetParentEntry(parent1, kIOServicePlane, &parent2)) &&
		(!IORegistryEntryCreateCFProperties(parent2, &usbProperties, kCFAllocatorDefault, kNilOptions)))
	{
		if (usbProperties)
		{
			refCF = CFDictionaryGetValue(hidProperties, CFSTR(kIOHIDSerialNumberKey));

			if (refCF)
			{
				if (CFStringGetCString((const __CFString *) refCF,
					(char *) deviceRec->serial, 256, CFStringGetSystemEncoding()))
				{
					strcpy(PK2UnitIDString, (const char *) deviceRec->serial);
					PK2UnitIDString[14] = 0; // ensure termination
				}
				else if (verbose)
					printf("Cannot get UnitID from descriptor.\n");
			}
		}
		else if (verbose)
			printf("Cannot get usb properties.\n");

		CFRelease(usbProperties);
		IOObjectRelease(parent2);
		IOObjectRelease(parent1);
	}
}

// Build device record

static struct deviceRecord *buildDeviceRecord(io_object_t hidDevice)
{
	struct deviceRecord		*deviceRec;
	kern_return_t				result;
	CFMutableDictionaryRef	hidProperties = 0;

	deviceRec = (struct deviceRecord *) malloc(sizeof (struct deviceRecord));

	if (deviceRec)			// get dictionary for HID properties
	{
		result = IORegistryEntryCreateCFProperties(hidDevice, &hidProperties, kCFAllocatorDefault, kNilOptions);

		bzero(deviceRec, sizeof(struct deviceRecord));

		if ((!result) && (hidProperties))
		{
						// create device interface
			result = openDeviceInterface(hidDevice, deviceRec);

			if (verbose && result)
				printf("Cannot open HID device interface.\n");

			getDeviceInfo(hidDevice, hidProperties, deviceRec);
			CFRelease(hidProperties);
		}
		else if (verbose)
			printf("Cannot get device properties.\n");
	}
	else if (verbose)
		printf("Cannot get memory for device record.\n");

	return deviceRec;
}

// Get UnitID from USB descriptor.
// If successful, PK2UnitIDString will contain the unit ID,
// otherwise PK2UnitIDString will be an empty string.

static void getPK2UnitID(io_object_t usbDevice)
{
	PK2UnitIDString[0] = 0;
	buildDeviceRecord(usbDevice);
}

static IOHIDDeviceInterface122 **find_device(SInt32 vendor, SInt32 product, int unit)
{
	IOReturn		r;
	SInt32		score;
	IOCFPlugInInterface		**pif;
	IOHIDDeviceInterface122	**devif;
	CFMutableDictionaryRef	dict;
	kern_return_t	service;
	io_iterator_t	usbIter;
	io_object_t		usbDevice = 0;
	int				index = 0;

	dict = IOServiceMatching(kIOHIDDeviceKey);
	set_number_to_dictionary(dict, CFSTR(kIOHIDVendorIDKey),  vendor);
	set_number_to_dictionary(dict, CFSTR(kIOHIDProductIDKey), product);
	service = IOServiceGetMatchingServices(kIOMasterPortDefault, dict, &usbIter);

	if (service != 0)
		return NULL;

	IOIteratorReset(usbIter);

	while ((usbDevice = IOIteratorNext(usbIter)))
	{
		if(index++ == unit)
			break;

		IOObjectRelease(usbDevice);
	}

	if (usbDevice)		// Get UnitID from descriptor
		getPK2UnitID(usbDevice);

	r = IOCreatePlugInInterfaceForService(usbDevice,
		kIOHIDDeviceUserClientTypeID,
		kIOCFPlugInInterfaceID,
		&pif,
		&score);

	if (r != kIOReturnSuccess)
		return NULL;

	IOObjectRelease(usbDevice);

	r = (*pif)->QueryInterface(pif,
		CFUUIDGetUUIDBytes(kIOHIDDeviceInterfaceID122),
		(LPVOID *) &devif);

	(*pif)->Release(pif);

	if (r || !devif)
		return NULL;

	return devif;
}

#ifdef MACOSX105
static void interrupt_report_callback_func(void *target,
	IOReturn	result,
	void		*refcon,
	void		*sender,
	uint32_t	size)
#else
static void interrupt_report_callback_func(void *target,
	IOReturn	result,
	void		*refcon,
	void		*sender,
	UInt32	size)
#endif
{
	hidreport_t rep = (hidreport_t) target;

	if (verbose && (rep->size_received != 0))
		printf("Warning: previous report has not been read.\n");

	rep->size_received = size;
	CFRunLoopStop(CFRunLoopGetCurrent());
}

static void setup_notification(hidreport_t rep)
{
	IOReturn		r;
	mach_port_t	port;
	CFRunLoopSourceRef	es;

	IOHIDDeviceInterface122 **devif;
	devif = rep->intf;
	rep->size_received = 0;
	r = (*devif)->open(devif, 0);
	r = (*devif)->createAsyncPort(devif, &port);
	r = (*devif)->createAsyncEventSource(devif, &es);
	r = (*devif)->setInterruptReportHandlerCallback(
		devif,
		rep->buffer,
		rep->size,
		interrupt_report_callback_func,
		rep,
		0
		);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), es, kCFRunLoopDefaultMode);
}

/*
 * 	 Public functions
 */

hidreport_t hidreport_open(int vendor_id, int product_id, int report_size, int unit)
{
	IOHIDDeviceInterface122 **intf;
	hidreport_t rep;

	intf = find_device(vendor_id, product_id, unit);

	if (intf == NULL)
		return NULL;

	if ((rep = (hidreport_t) malloc(sizeof(struct hidreport))) == NULL)
		return NULL;

	rep->intf = intf;

	if ((rep->buffer = (char *) malloc(report_size)) == NULL)
		return NULL;

	rep->size = report_size;
	rep->size_received = 0;
	rep->timeout = 500;			// timeout in ms
	setup_notification(rep);
	return rep;
}

void hidreport_close(hidreport_t rep)
{
	(*rep->intf)->close(rep->intf);
	free(rep->buffer);
	free(rep);
	rep = NULL;
}

int hidreport_send(hidreport_t rep, void *buf, int size)
{
	IOReturn	r;

	rep->size_received = 0;
	r = (*rep->intf)->setReport(rep->intf, kIOHIDReportTypeOutput, 1, buf, size,
		rep->timeout, NULL, NULL, NULL);
	return r;
}

int hidreport_receive(hidreport_t rep, void *buf, UInt32 *size)
{
	while (rep->size_received == 0)
		CFRunLoopRun();

	memcpy(buf, rep->buffer, rep->size_received);
	*size = rep->size_received;
	rep->size_received = 0;
	return 0;
}

// Send 64 byte block to PICkit2

int sendUSB(pickit_dev *d, byte *src, int len)
{
	int	r, i;
	bool	rescan = false;
	char	buf[REPORT_SIZE + 1];

	if (pickit2mode == NORMAL_MODE)
	{
		if ((src[0] == ENTERBOOTLOADER) && (src[1] == END_OF_BFR))
			rescan = true;
	}
	else
	{
		if (src[0] == RESETFWDEVICE)
			rescan = true;
	}

	bzero(buf, REPORT_SIZE + 1);
	memcpy(buf, src, REPORT_SIZE);

	if (usbdebug & USB_DEBUG_CMD)
		showUSBCommand(src, len);

	if (usbdebug & USB_DEBUG_XMIT)
	{
		if (!usbFile)
			usbFile = stdout;

		fprintf(usbFile, "USB>");

		for (r=0, i=0; r<len; r++)
		{
			fprintf(usbFile, " %02x", src[r] & 0xff);
			i++;

			if ((i > 15) && (i < (len - 1)))
			{
				i = 0;
				fprintf(usbFile, "\n    ");
			}
		}

		fprintf(usbFile, "\n");
		fflush(usbFile);
	}

	hidreport_send((hidreport_t) d, buf, REPORT_SIZE);

	if (rescan)		// Microchip code entered/exited bootloader,
		deviceHandle = NULL;	// so reset the device handle

	return 1;
}

// Receive data packet from PICkit2

int recvUSB(pickit_dev *d, int len, byte *dest)
{
	int	size, i, j;
	byte	buf[REPORT_SIZE + 1];

	i = 0;

	while (i < len)
	{
		bzero(buf, REPORT_SIZE + 1);
		hidreport_receive((hidreport_t) d, buf, (UInt32 *) &size);

		for (j=0; j<size && i<len; j++)
			dest[i++] = buf[j];
	}

	if (usbdebug & USB_DEBUG_RECV)
	{
		if (!usbFile)
			usbFile = stdout;

		fprintf(usbFile, "USB<");

		for (i=0, j=0; i<len; i++)
		{
			fprintf(usbFile, " %02x", buf[i] & 0xff);
			j++;

			if ((j > 15) && (i < (len - 1)))
			{
				j = 0;
				fprintf(usbFile, "\n    ");
			}
		}

		fprintf(usbFile, "\n");
		fflush(usbFile);
	}

	return len;
}

// Scan for next PICkit2 device

pickit_dev *usbPickitOpen(int unit, char *id)
{
	pickit_dev	*pickit2;

	pickit2 = hidreport_open(1240, 51, REPORT_SIZE, unit);		// 0x04d8, 0x0033 PICkit2

	if (pickit2 != NULL)			// found it
	{
		if (PK2UnitIDString[0])				// if we got unit ID from descriptor,
			strcpy(id, PK2UnitIDString);	// pass it back to the caller
		else
			strcpy(id, "-");

		deviceHandle = pickit2;
		pickit_interface = 1;
	}

/* The following should be handled by the higher level code

	if (pickit2mode == NORMAL_MODE)
	{
		if (pickit2firmware < MINFWVERSION)
		{
			pickitOff(pickit2);
			fprintf(stderr, "\npk2cmd requires PICkit2 firmware %d.%d.%d or higher\n\n",
				MINFWVERSION >> 16, (MINFWVERSION >> 8) & 0xff, MINFWVERSION & 0xff);
			fflush(stderr);
		}
	}
*/

	return(pickit2);
}

void usb_release_interface(pickit_dev *deviceHandle, int pickit_interface)
{
	if (deviceHandle != NULL)
		hidreport_close(deviceHandle);

	pickit_interface = 0;
}

#endif // ifndef WIN32

// EOF

