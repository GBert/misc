/*
    example minimal CDC serial port adapter using PIC16F1454 microcontroller

    based on M-Stack by Alan Ott, Signal 11 Software

    culled from USB CDC-ACM Demo (by Alan Ott, Signal 11 Software)
    and ANSI C12.18 optical interface (by Peter Lawrence)

    Copyright (C) 2014,2015 Peter Lawrence

    Permission is hereby granted, free of charge, to any person obtaining a 
    copy of this software and associated documentation files (the "Software"), 
    to deal in the Software without restriction, including without limitation 
    the rights to use, copy, modify, merge, publish, distribute, sublicense, 
    and/or sell copies of the Software, and to permit persons to whom the 
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in 
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
    DEALINGS IN THE SOFTWARE.
*/

#include "usb_config.h"
#include "usb.h"
#include "usb_ch9.h"
#include "usb_cdc.h"

#ifdef __C18
#define ROMPTR rom
#else
#define ROMPTR
#endif

struct configuration_1_packet
{
	struct configuration_descriptor  config;

	/* CDC Class Interface */
	struct interface_descriptor      cdc_class_interface;
	struct cdc_functional_descriptor_header cdc_func_header;
	struct cdc_acm_functional_descriptor cdc_acm;
	struct cdc_union_functional_descriptor cdc_union;
	struct cdc_union_functional_descriptor cdc_mgt;
	struct endpoint_descriptor       cdc_ep;

	/* CDC Data Interface */
	struct interface_descriptor      cdc_data_interface;
	struct endpoint_descriptor       data_ep_in;
	struct endpoint_descriptor       data_ep_out;

};


const ROMPTR struct device_descriptor this_device_descriptor =
{
	sizeof(struct device_descriptor), // bLength
	DESC_DEVICE, // bDescriptorType
	0x0200, // 0x0200 = USB 2.0, 0x0110 = USB 1.1
	CDC_DEVICE_CLASS, /* Device class */
	0x00, /* Device Subclass. */
	0x00, /* Protocol. */
	EP_0_LEN, // bMaxPacketSize0
	0x04D8, // Vendor
	0x000A, // Product
	0x0100, // device release (1.0)
	0, // Manufacturer
	0, // Product
	0, // Serial
	NUMBER_OF_CONFIGURATIONS // NumConfigurations
};

static const ROMPTR struct configuration_1_packet configuration_1 =
{
	{
	// Members from struct configuration_descriptor
	sizeof(struct configuration_descriptor),
	DESC_CONFIGURATION,
	sizeof(configuration_1), // wTotalLength (length of the whole packet)
	2, // bNumInterfaces
	1, // bConfigurationValue
	0, // iConfiguration (index of string descriptor)
	0b10000000,
	100/2,   // 100/2 indicates 100mA
	},

	/* CDC Class Interface */
	{
	// Members from struct interface_descriptor
	sizeof(struct interface_descriptor), // bLength;
	DESC_INTERFACE,
	0x0, // InterfaceNumber
	0x0, // AlternateSetting
	0x1, // bNumEndpoints
	CDC_COMMUNICATION_INTERFACE_CLASS, // bInterfaceClass
	CDC_COMMUNICATION_INTERFACE_CLASS_ACM_SUBCLASS, // bInterfaceSubclass
	0x00, // bInterfaceProtocol
	0, // iInterface (index of string describing interface)
	},

	/* CDC Functional Descriptor Header */
	{
	sizeof(struct cdc_functional_descriptor_header),
	DESC_CS_INTERFACE,
	CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_HEADER,
	0x0110, /* bcdCDC (version in BCD) */
	},

	/* CDC ACM Functional Descriptor */
	{
	sizeof(struct cdc_acm_functional_descriptor),
	DESC_CS_INTERFACE,
	CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_ACM,
	/*
	bmCapabilities:
	  Linux honors 0 (no capabilities) when said, but expects too much if any are advertised (so a value of zero is best)
	  Windows ignores what is said here and expects CDC_SET_LINE_CODING_CALLBACK (and CDC_GET_LINE_CODING_CALLBACK) anyway
	*/
	0,
	},

	/* CDC Union Functional Descriptor */
	{
	sizeof (struct cdc_union_functional_descriptor),
	DESC_CS_INTERFACE,
	CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_UNION,
	0, /* bMasterInterface: comm */
	1, /* bSlaveInterface0: data */
	},

	/* CDC Management Functional Descriptor */
	{
	sizeof (struct cdc_union_functional_descriptor),
	DESC_CS_INTERFACE,
	CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_MGT,
	0, /* bMasterInterface: comm */
	1, /* bSlaveInterface0: data */
	},

	/* CDC ACM Notification Endpoint (Endpoint 1 IN) */
	{
	sizeof(struct endpoint_descriptor),
	DESC_ENDPOINT,
	0x01 | 0x80, // endpoint #1 0x80=IN
	EP_INTERRUPT, // bmAttributes
	EP_1_IN_LEN, // wMaxPacketSize
	1, // bInterval in ms.
	},

	/* CDC Data Interface */
	{
	// Members from struct interface_descriptor
	sizeof(struct interface_descriptor), // bLength;
	DESC_INTERFACE,
	0x1, // InterfaceNumber
	0x0, // AlternateSetting
	0x2, // bNumEndpoints
	CDC_DATA_INTERFACE_CLASS, // bInterfaceClass
	0, // bInterfaceSubclass (no subclass)
	CDC_DATA_INTERFACE_CLASS_PROTOCOL_NONE, // bInterfaceProtocol
	0, // iInterface (index of string describing interface)
	},

	/* CDC Data OUT Endpoint */
	{
	sizeof(struct endpoint_descriptor),
	DESC_ENDPOINT,
	0x02, // endpoint #2 0x00=OUT
	EP_BULK, // bmAttributes
	EP_2_OUT_LEN, // wMaxPacketSize
	0, // bInterval in ms.
	},

	/* CDC Data IN Endpoint */
	{
	sizeof(struct endpoint_descriptor),
	DESC_ENDPOINT,
	0x02 | 0x80, // endpoint #2 0x80=IN
	EP_BULK, // bmAttributes
	EP_2_IN_LEN, // wMaxPacketSize
	0, // bInterval in ms.
	},
};

static const ROMPTR struct { uint8_t bLength; uint8_t bDescriptorType; uint16_t lang; } str00 =
{
	sizeof(str00),
	DESC_STRING,
	0x0409 // US English
};

int16_t usb_application_get_string(uint8_t string_number, const void **ptr)
{
	if (0 == string_number)
	{
		*ptr = &str00;
		return sizeof(str00);
	}

	return -1;
}

const struct configuration_descriptor *usb_application_config_descs[] =
{
	(struct configuration_descriptor*) &configuration_1,
};

STATIC_SIZE_CHECK_EQUAL(USB_ARRAYLEN(USB_CONFIG_DESCRIPTOR_MAP), NUMBER_OF_CONFIGURATIONS);
STATIC_SIZE_CHECK_EQUAL(sizeof(USB_DEVICE_DESCRIPTOR), 18);
