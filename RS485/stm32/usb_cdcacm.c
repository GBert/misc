/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 * Copyright (C) 2013 Alexandru Gagniuc <mr.nuke.me@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * This package is _meant_ to be platform independent, just a full
 * cdc-acm impl, with callbacks
 */

#include "usb_cdcacm.h"

#include <stdio.h>
#include <stdlib.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include <libopencm3/cm3/scb.h>
// NOTHING ELSE! this file _does_ not know about part specifics!

#include "syscfg.h"
#include "ringb.h"

#define ER_DEBUG
#ifdef ER_DEBUG
#define ER_DPRINTF(fmt, ...) \
    do { printf(fmt, ## __VA_ARGS__); } while (0)
#else
#define ER_DPRINTF(fmt, ...) \
    do { } while (0)
#endif

static uint8_t usbd_control_buffer[128];
static usbd_device *acm_dev;

static const struct usb_device_descriptor dev = {
    .bLength = USB_DT_DEVICE_SIZE,
    .bDescriptorType = USB_DT_DEVICE,
    .bcdUSB = 0x2000,
    .bDeviceClass = USB_CLASS_CDC,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = 64,
    .idVendor = 0xc03e,
    .idProduct = 0xb007,
    .bcdDevice = 0x2000,
    .iManufacturer = 1,
    .iProduct = 2,
    .iSerialNumber = 3,
    .bNumConfigurations = 1,
};

/*
 * This notification endpoint isn't implemented. According to CDC spec it's
 * optional, but its absence causes a NULL pointer dereference in the
 * Linux cdc_acm driver.
 */
static const struct usb_endpoint_descriptor comm_endp[] = { {
							     .bLength = USB_DT_ENDPOINT_SIZE,
							     .bDescriptorType = USB_DT_ENDPOINT,
							     .bEndpointAddress = 0x83,
							     .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
							     .wMaxPacketSize = 16,
							     .bInterval = 1,
							     }
};

static const struct usb_endpoint_descriptor data_endp[] = { {
							     .bLength = USB_DT_ENDPOINT_SIZE,
							     .bDescriptorType = USB_DT_ENDPOINT,
							     .bEndpointAddress = 0x01,
							     .bmAttributes = USB_ENDPOINT_ATTR_BULK,
							     .wMaxPacketSize = 64,
							     .bInterval = 1,
							     }, {
								 .bLength = USB_DT_ENDPOINT_SIZE,
								 .bDescriptorType = USB_DT_ENDPOINT,
								 .bEndpointAddress = 0x82,
								 .bmAttributes = USB_ENDPOINT_ATTR_BULK,
								 .wMaxPacketSize = 64,
								 .bInterval = 1,
								 }
};

static const struct {
    struct usb_cdc_header_descriptor header;
    struct usb_cdc_call_management_descriptor call_mgmt;
    struct usb_cdc_acm_descriptor acm;
    struct usb_cdc_union_descriptor cdc_union;
} __attribute__ ((packed)) cdcacm_functional_descriptors = {
    .header = {
    .bFunctionLength = sizeof(struct usb_cdc_header_descriptor),.bDescriptorType =
	    CS_INTERFACE,.bDescriptorSubtype = USB_CDC_TYPE_HEADER,.bcdCDC = 0x0110,},.call_mgmt = {
    .bFunctionLength = sizeof(struct usb_cdc_call_management_descriptor),.bDescriptorType =
	    CS_INTERFACE,.bDescriptorSubtype = USB_CDC_TYPE_CALL_MANAGEMENT,.bmCapabilities = 0,.bDataInterface =
	    1,},.acm = {
    .bFunctionLength = sizeof(struct usb_cdc_acm_descriptor),.bDescriptorType = CS_INTERFACE,.bDescriptorSubtype =
	    USB_CDC_TYPE_ACM,.bmCapabilities = (1 << 1),},.cdc_union = {
    .bFunctionLength = sizeof(struct usb_cdc_union_descriptor),.bDescriptorType = CS_INTERFACE,.bDescriptorSubtype =
	    USB_CDC_TYPE_UNION,.bControlInterface = 0,.bSubordinateInterface0 = 1,}
};

static const struct usb_interface_descriptor comm_iface[] = { {
							       .bLength = USB_DT_INTERFACE_SIZE,
							       .bDescriptorType = USB_DT_INTERFACE,
							       .bInterfaceNumber = 0,
							       .bAlternateSetting = 0,
							       .bNumEndpoints = 1,
							       .bInterfaceClass = USB_CLASS_CDC,
							       .bInterfaceSubClass = USB_CDC_SUBCLASS_ACM,
							       .bInterfaceProtocol = USB_CDC_PROTOCOL_AT,
							       .iInterface = 0,

							       .endpoint = comm_endp,

							       .extra = &cdcacm_functional_descriptors,
							       .extralen = sizeof(cdcacm_functional_descriptors)
							       }
};

static const struct usb_interface_descriptor data_iface[] = { {
							       .bLength = USB_DT_INTERFACE_SIZE,
							       .bDescriptorType = USB_DT_INTERFACE,
							       .bInterfaceNumber = 1,
							       .bAlternateSetting = 0,
							       .bNumEndpoints = 2,
							       .bInterfaceClass = USB_CLASS_DATA,
							       .bInterfaceSubClass = 0,
							       .bInterfaceProtocol = 0,
							       .iInterface = 0,

							       .endpoint = data_endp,
							       }
};

static const struct usb_interface ifaces[] = { {
						.num_altsetting = 1,
						.altsetting = comm_iface,
						}, {
						    .num_altsetting = 1,
						    .altsetting = data_iface,
						    }
};

static const struct usb_config_descriptor config = {
    .bLength = USB_DT_CONFIGURATION_SIZE,
    .bDescriptorType = USB_DT_CONFIGURATION,
    .wTotalLength = 0,
    .bNumInterfaces = 2,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = 0x80,
    .bMaxPower = 0x32,

    .interface = ifaces,
};

static char serial[] = "none";
static const char *usb_strings[] = {
    "libopencm3",
    "usb_to_serial_cdcacm",
    serial,
    "DEMO",
};

struct ringb rx_ring;
static uint8_t rx_ring_data[64];
struct ringb tx_ring;
static uint8_t tx_ring_data[128];
bool nakked = false;

static int cdcacm_control_request(usbd_device * usbd_dev,
				  struct usb_setup_data *req, uint8_t ** buf,
				  uint16_t * len,
				  void (**complete) (usbd_device * usbd_dev, struct usb_setup_data * req)) {
    uint8_t dtr, rts;

    (void)complete;
    (void)buf;
    (void)usbd_dev;

    switch (req->bRequest) {
    case USB_CDC_REQ_SET_CONTROL_LINE_STATE:
	{
	    /*
	     * This Linux cdc_acm driver requires this to be implemented
	     * even though it's optional in the CDC spec, and we don't
	     * advertise it in the ACM functional descriptor.
	     */

	    dtr = (req->wValue & (1 << 0)) ? 1 : 0;
	    rts = (req->wValue & (1 << 1)) ? 1 : 0;
	    ER_DPRINTF("CTRLRQ:%d Set Line state: dtr:%d rts: %d\n", req->wIndex, dtr, rts);

	    // FIXME - need to get port based on wIndex I believe?
	    cdcacm_arch_set_line_state(0, dtr, rts);

	    return 1;
	}
    case USB_CDC_REQ_SET_LINE_CODING:
	{
	    struct usb_cdc_line_coding *coding;

	    if (*len < sizeof(struct usb_cdc_line_coding))
		return 0;

	    coding = (struct usb_cdc_line_coding *)*buf;
	    ER_DPRINTF("CTRLRQ: line coding: %lu(%u:%u:%u)\n", coding->dwDTERate,
		       coding->bDataBits, coding->bParityType, coding->bCharFormat);
	    return glue_set_line_coding_cb(coding->dwDTERate,
					   coding->bDataBits, coding->bParityType, coding->bCharFormat);
	}
    }
    return 0;
}

static void cdcacm_data_rx_cb(usbd_device * usbd_dev, uint8_t ep) {
    uint8_t buf[64];
    /* nak right now, we're not sure whether we'll be able to even process this! */
    usbd_ep_nak_set(usbd_dev, ep, 1);
    int len = usbd_ep_read_packet(usbd_dev, ep, buf, 64);
    ER_DPRINTF("Hrx%db\n", len);
    cdcacm_arch_pin(0, CDCACM_PIN_LED_TX, 1);
    cdcacm_arch_pin(0, CDCACM_PIN_RS485DE, 1);
    for (int x = 0; x < len; x++) {
	if (!ringb_put(&tx_ring, buf[x])) {
	    // failed to process usb traffic properly.
	    // should _never_ happen, means we failed to nak in time.
	    // this is _never_recoverable beyond watchdog reset.
	    while (1) ;
	}
    }
    // look up port to suart mapping which side?
    cdcacm_arch_txirq(0, 1);

    if (ringb_depth(&tx_ring) < 64) {
	ER_DPRINTF("ACK\n");
	usbd_ep_nak_set(usbd_dev, ep, 0);
    } else {
	nakked = true;
    }
}

static void cdcacm_set_config(usbd_device * usbd_dev, uint16_t wValue) {
    (void)wValue;

    usbd_ep_setup(usbd_dev, 0x01, USB_ENDPOINT_ATTR_BULK, 64, cdcacm_data_rx_cb);
    usbd_ep_setup(usbd_dev, 0x82, USB_ENDPOINT_ATTR_BULK, 64, NULL);
    usbd_ep_setup(usbd_dev, 0x83, USB_ENDPOINT_ATTR_INTERRUPT, 16, NULL);

    usbd_register_control_callback(usbd_dev,
				   USB_REQ_TYPE_CLASS |
				   USB_REQ_TYPE_INTERFACE,
				   USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT, cdcacm_control_request);
}

/* FIXME - need to report this! */
void cdcacm_line_state_changed_cb(uint8_t linemask) {
    const int size = sizeof(struct usb_cdc_notification) + 2;
    uint8_t buf[size];

    struct usb_cdc_notification *notify = (void *)buf;
    notify->bmRequestType = 0xa1;
    notify->bNotification = USB_CDC_NOTIFY_SERIAL_STATE;
    notify->wValue = 0;
    notify->wIndex = 1;
    notify->wLength = 2;
    uint16_t *data = (void *)&buf[sizeof(struct usb_cdc_notification)];
    *data = linemask;

    while (usbd_ep_write_packet(acm_dev, 0x83, buf, size) == size) ;
}

/* Y0, moron, nothing's stopping rx irqs from happening, have fun when you overflow temp buffer! */
static void task_drain_rx(struct ringb *r) {
    uint8_t zero_copy_is_for_losers[sizeof(rx_ring_data)];
    int zci = 0;
    int c = ringb_get(r);
    while (c >= 0) {
	//trace_send8(STIMULUS_RING_DRAIN, c);
	zero_copy_is_for_losers[zci++] = c;
	c = ringb_get(r);
    }
    if (zci) {
	//trace_send16(STIMULUS_RING_DRAIN, zci);
	ER_DPRINTF("Drx %db\n", zci);
	usbd_ep_write_packet(acm_dev, 0x82, zero_copy_is_for_losers, zci);
    }
}

usbd_device *usb_cdcacm_init(const usbd_driver * driver, const char *userserial) {
    ringb_init(&rx_ring, rx_ring_data, sizeof(rx_ring_data));
    ringb_init(&tx_ring, tx_ring_data, sizeof(tx_ring_data));
    if (userserial) {
	usb_strings[2] = userserial;
    }

    acm_dev = usbd_init(driver, &dev, &config, usb_strings, 4, usbd_control_buffer, sizeof(usbd_control_buffer));
    usbd_register_set_config_callback(acm_dev, cdcacm_set_config);
    return acm_dev;
}

void usb_cdcacm_poll(usbd_device * usbd_dev)	// FIXME -drop to acm_dev internal
{
    // Originally, calling this every 50 times caused some rx character droppage,
    // and every 500 times caused _none_.  _probably_ needs to be tied to
    // a timer and something like the current baud rate and the inter character time
    static int i = 0;
    if (i++ > 500) {
	// hacktastic
	if (ringb_depth(&tx_ring) < 64 && nakked) {
	    usbd_ep_nak_set(usbd_dev, 1, 0);
	    nakked = false;
	}

	task_drain_rx(&rx_ring);
	i = 0;
    }

}
