/**
 * c2_bus_fx2.c - EZ-USB FX2 based SiLabs C2 programmer control functions
 *
 * Copyright (c) 2014, David Imhoff <dimhoff.devel@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors may
 *       be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <usb.h>

#include "c2_bus.h"
#include "c2_bus_module.h"

#define VID 0x04b4
#define PID 0x8613

#define CMD_OUT	 0x02
#define CMD_IN	 0x86
#define IFNUM 0
#define ALTIFNUM 1

#define USB_TIMEOUT 5000 // 5 sec?

enum CmdOpcodes { 
        CmdReset, 
        CmdQReset, 
        CmdReadAddr, 
        CmdWriteAddr, 
        CmdReadData, 
        CmdWriteData 
};

struct c2_bus_fx2_ctx {
	usb_dev_handle *dev;
};

int c2_fx2_reset(struct c2_bus *bus);
int c2_fx2_qreset(struct c2_bus *bus);
int c2_fx2_addr_read(struct c2_bus *bus, unsigned char *addr);
int c2_fx2_addr_write(struct c2_bus *bus, unsigned char addr);
int c2_fx2_data_read(struct c2_bus *bus, unsigned char *data, size_t len);
int c2_fx2_data_write(struct c2_bus *bus,
			const unsigned char *data, size_t len);
void c2_fx2_destroy(struct c2_bus *bus);

struct c2_bus_ops c2_bus_fx2_ops = {
	.reset = c2_fx2_reset,
	.qreset = c2_fx2_qreset,
	.addr_read = c2_fx2_addr_read,
	.addr_write = c2_fx2_addr_write,
	.data_read = c2_fx2_data_read,
	.data_write = c2_fx2_data_write,
	.destruct = c2_fx2_destroy,
};

static struct usb_device * locate_fx2(const char *path)
{
	struct usb_bus *bus;
	struct usb_device *dev;

	if (path != NULL && !(strlen(path) == 7 || path[0] == '\0')) {
		return NULL;
	}

	// rescan busses and devices
	usb_find_busses();
	usb_find_devices();

	for (bus = usb_get_busses(); bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			if (path != NULL && path[0] != '\0') {
				if ((strncmp(path, bus->dirname, 3) == 0) &&
				    (strncmp(path+4, dev->filename, 3) == 0))
				{
					return dev;
				}
			} else if (dev->descriptor.idVendor == VID &&
				   (dev->descriptor.idProduct == PID 
				   ))
			{
				return dev;
			}
		}
	}

	return NULL;
}

int c2_bus_fx2_init(struct c2_bus *bus, const char *path)
{
	struct usb_device *device;
	struct c2_bus_fx2_ctx *hw;

	hw = (struct c2_bus_fx2_ctx *) malloc(sizeof(struct c2_bus_fx2_ctx));
	if (hw == NULL) {
		c2_bus_set_error(bus, "Unable to allocate memory");
		return -1;
	}

	usb_init();
//	usb_set_debug(255);

	// find device, reset device and open device handle
	if ((device = locate_fx2(path)) == NULL) {
		c2_bus_set_perror(bus, "Could not locate the fx2 device");
		goto bad1;
	}
	hw->dev = usb_open(device);

	if ((usb_claim_interface(hw->dev, IFNUM)) != 0) {
		c2_bus_set_perror(bus, "claim_interface");
		goto bad2;
	}

	if ((usb_set_altinterface(hw->dev, ALTIFNUM)) != 0) {
		c2_bus_set_perror(bus, "set_altinterface");
		goto bad2;
	}

	bus->ops = &c2_bus_fx2_ops;
	bus->ctx = hw;
	return 0;

bad2:
	usb_close(hw->dev);
bad1:
	free(hw);
	return -1;
}

int c2_fx2_reset(struct c2_bus *bus)
{
	struct c2_bus_fx2_ctx *hw = (struct c2_bus_fx2_ctx *) bus->ctx;
	int read_len;
	unsigned char buf[3] = { '<', 1, CmdReset };

	if (usb_bulk_write(hw->dev, CMD_OUT, (char *) buf, sizeof(buf), USB_TIMEOUT) != sizeof(buf)) {
		c2_bus_set_error(bus, "Unable to write to USB device");
		return -1;
	}

	read_len = usb_bulk_read(hw->dev, CMD_IN, (char *) buf, sizeof(buf), USB_TIMEOUT);
	if (read_len < 0) {
		c2_bus_set_error(bus, "Unable to read from USB device");
		return -1;
	}

	if (read_len != 2 || buf[0] != '>' || buf[1] != 0) {
		c2_bus_set_error(bus, "Unexpected response from USB device");
		return -1;
	}

	return 0;
}

int c2_fx2_qreset(struct c2_bus *bus)
{
	struct c2_bus_fx2_ctx *hw = (struct c2_bus_fx2_ctx *) bus->ctx;
	int read_len;
	unsigned char buf[3] = { '<', 1, CmdQReset };

	if (usb_bulk_write(hw->dev, CMD_OUT, (char *) buf, sizeof(buf), USB_TIMEOUT) != sizeof(buf)) {
		c2_bus_set_error(bus, "Unable to write to USB device");
		return -1;
	}

	read_len = usb_bulk_read(hw->dev, CMD_IN, (char *) buf, sizeof(buf), USB_TIMEOUT);
	if (read_len < 0) {
		c2_bus_set_error(bus, "Unable to read from USB device");
		return -1;
	}

	if (read_len != 2 || buf[0] != '>' || buf[1] != 0) {
		c2_bus_set_error(bus, "Unexpected response from USB device");
		return -1;
	}

	return 0;
}

int c2_fx2_addr_read(struct c2_bus *bus, unsigned char *addr)
{
	struct c2_bus_fx2_ctx *hw = (struct c2_bus_fx2_ctx *) bus->ctx;
	int read_len;
	unsigned char buf[3] = { '<', 1, CmdReadAddr };

	if (usb_bulk_write(hw->dev, CMD_OUT, (char *) buf, sizeof(buf), USB_TIMEOUT) != sizeof(buf)) {
		c2_bus_set_error(bus, "Unable to write to USB device");
		return -1;
	}

	read_len = usb_bulk_read(hw->dev, CMD_IN, (char *) buf, sizeof(buf), USB_TIMEOUT);
	if (read_len < 0) {
		c2_bus_set_error(bus, "Unable to read from USB device");
		return -1;
	}

	if (read_len != 3 || buf[0] != '>' || buf[1] != 1) {
		c2_bus_set_error(bus, "Unexpected response from USB device");
		return -1;
	}

	*addr = buf[2];

	return 0;
}

int c2_fx2_addr_write(struct c2_bus *bus, unsigned char addr)
{
	struct c2_bus_fx2_ctx *hw = (struct c2_bus_fx2_ctx *) bus->ctx;
	int read_len;
	unsigned char buf[4] = { '<', 2, CmdWriteAddr, 0 };

	buf[3] = addr;

	if (usb_bulk_write(hw->dev, CMD_OUT, (char *) buf, sizeof(buf), USB_TIMEOUT) != sizeof(buf)) {
		c2_bus_set_error(bus, "Unable to write to USB device");
		return -1;
	}

	read_len = usb_bulk_read(hw->dev, CMD_IN, (char *) buf, sizeof(buf), USB_TIMEOUT);
	if (read_len < 0) {
		c2_bus_set_error(bus, "Unable to read from USB device");
		return -1;
	}

	if (read_len != 2 || buf[0] != '>' || buf[1] != 0) {
		c2_bus_set_error(bus, "Unexpected response from USB device");
		return -1;
	}

	return 0;
}

int c2_fx2_data_read(struct c2_bus *bus, unsigned char *data, size_t len)
{
	struct c2_bus_fx2_ctx *hw = (struct c2_bus_fx2_ctx *) bus->ctx;
	int read_len;
	unsigned char buf[6] = { '<', 2, CmdReadData, 0 };

	if (len < 1 || len > 4) {
		c2_bus_set_error(bus, "C2 Data Write length out of range");
		return -1;
	}

	buf[3] = len;

	if (usb_bulk_write(hw->dev, CMD_OUT, (char *) buf, 4, USB_TIMEOUT) != 4) {
		c2_bus_set_error(bus, "Unable to write to USB device");
		return -1;
	}

	read_len = usb_bulk_read(hw->dev, CMD_IN, (char *) buf, sizeof(buf), USB_TIMEOUT);
	if (read_len < 0) {
		c2_bus_set_error(bus, "Unable to read from USB device");
		return -1;
	}

	if (read_len != 3 || buf[0] != '>' || buf[1] != len) {
		c2_bus_set_error(bus, "Unexpected response from USB device");
		return -1;
	}

	memcpy(data, &buf[2], len);

	return 0;
}

int c2_fx2_data_write(struct c2_bus *bus, const unsigned char *data, size_t len)
{
	struct c2_bus_fx2_ctx *hw = (struct c2_bus_fx2_ctx *) bus->ctx;
	int read_len;
	unsigned char buf[7] = { '<', 0, CmdWriteData, 0 };

	if (len < 1 || len > 4) {
		c2_bus_set_error(bus, "C2 Data Write length out of range");
		return -1;
	}

	buf[1] = 1 + len;
	memcpy(&buf[3], data, len);

	if (usb_bulk_write(hw->dev, CMD_OUT, (char *) buf, 3 + len, USB_TIMEOUT) != 3 + len) {
		c2_bus_set_error(bus, "Unable to write to USB device");
		return -1;
	}

	read_len = usb_bulk_read(hw->dev, CMD_IN, (char *) buf, sizeof(buf), USB_TIMEOUT);
	if (read_len < 0) {
		c2_bus_set_error(bus, "Unable to read from USB device");
		return -1;
	}

	if (read_len != 2 || buf[0] != '>' || buf[1] != 0) {
		c2_bus_set_error(bus, "Unexpected response from USB device");
		return -1;
	}

	return 0;
}

void c2_fx2_destroy(struct c2_bus *bus)
{
	struct c2_bus_fx2_ctx *hw = (struct c2_bus_fx2_ctx *) bus->ctx;

	if (hw == NULL) return;

	if (hw->dev != NULL) {
		usb_close(hw->dev);
	}
	free(hw);

	bus->ops = NULL;
	bus->ctx = NULL;
}
