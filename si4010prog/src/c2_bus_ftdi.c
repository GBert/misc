/**
 * c2_bus_ftdi.c - FTDI FT232 based SiLabs C2 programmer control functions
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
//#define C2_FTDI_DEBUG

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <ftdi.h>

#include "c2_bus.h"
#include "c2_bus_module.h"

#define PIN_TX  0x01  /* Orange wire on FTDI cable */
#define PIX_RX  0x02  /* Yellow */
#define PIN_RTS 0x04  /* Green */
#define PIN_CTS 0x08  /* Brown */
#define PIN_DTR 0x10
#define PIN_DSR 0x20
#define PIN_DCD 0x40
#define PIN_RI  0x80

#define PIN_C2CK	PIN_DTR
#define PIN_C2D		PIN_CTS
#define BAUD		450000

// Bit sequences for INS commands
// NOTE: LSB is shifted out first!
#define C2_INS_ADDR_READ  0x02
#define C2_INS_ADDR_WRITE 0x03
#define C2_INS_DATA_READ  0x00
#define C2_INS_DATA_WRITE 0x01

struct c2_bus_ftdi_ctx {
	struct ftdi_context *ftdi;
};

int c2_ftdi_reset(struct c2_bus *bus);
int c2_ftdi_qreset(struct c2_bus *bus);
int c2_ftdi_addr_read(struct c2_bus *bus, unsigned char *addr);
int c2_ftdi_addr_write(struct c2_bus *bus, unsigned char addr);
int c2_ftdi_data_read(struct c2_bus *bus, unsigned char *data, size_t len);
int c2_ftdi_data_write(struct c2_bus *bus,
			const unsigned char *data, size_t len);
void c2_ftdi_destroy(struct c2_bus *bus);

struct c2_bus_ops c2_bus_ftdi_ops = {
	.reset = c2_ftdi_reset,
	.qreset = c2_ftdi_qreset,
	.addr_read = c2_ftdi_addr_read,
	.addr_write = c2_ftdi_addr_write,
	.data_read = c2_ftdi_data_read,
	.data_write = c2_ftdi_data_write,
	.destruct = c2_ftdi_destroy,
};

/**
 * Flush broken first read
 *
 * After opening a FTDI device or purging the buffers the first read/write is
 * somehow broken. To work around this do a one bit read write that doesn't
 * influence the protocol.
 * This is a hack and can be removed if the actual problem is solved.
 */
static int _ftdi_flush(struct c2_bus_ftdi_ctx *hw)
{
	uint8_t data[1] = { PIN_C2CK };
	int ret;

	ret = ftdi_write_data(hw->ftdi, data, 1);
	if (ret != 1) {
		return -1;
	}

	do {
		ret = ftdi_read_data(hw->ftdi, data, 1);
		if (ret < 0) {
			return -1;
		}
	} while (ret == 0);

	return 0;
}

int c2_bus_ftdi_init(struct c2_bus *bus, const char *path)
{
	struct c2_bus_ftdi_ctx *hw;

	hw = (struct c2_bus_ftdi_ctx *) malloc(sizeof(struct c2_bus_ftdi_ctx));
	if (hw == NULL) {
		c2_bus_set_error(bus, "Unable to allocate memory");
		return -1;
	}

	hw->ftdi = ftdi_new();
	if (hw->ftdi == NULL) {
		c2_bus_set_error(bus, "Unable to allocate memory for FTDI context");
		goto bad1;
	}

	if (path[0] != '\0') {
		if (ftdi_usb_open_string(hw->ftdi, path) < 0) {
			//TODO: c2_bus_set_error(bus, "ftdi_usb_open_string failed: %s\n", 
			//		ftdi_get_error_string(&ftdic));
			c2_bus_set_error(bus, "ftdi_usb_open_string failed");
			goto bad2;
		}
	} else {
		if (ftdi_usb_open_desc(hw->ftdi, 0x0403, 0x6014, NULL, NULL) < 0) {
			//TODO: c2_bus_set_error(bus, "ftdi_usb_open_desc failed: %s\n", 
			//		ftdi_get_error_string(&ftdic));
			c2_bus_set_error(bus, "ftdi_usb_open_desc failed");
			goto bad2;
		}
	}

	if (ftdi_set_baudrate(hw->ftdi, BAUD / 16) < 0) {
		//TODO: c2_bus_set_error(bus, "ftdi_set_baudrate failed: %s\n", ftdi_get_error_string(&ftdic));
		c2_bus_set_error(bus, "ftdi_set_baudrate failed");
		goto bad3;
	}

	if (ftdi_set_bitmode(hw->ftdi, PIN_C2CK, BITMODE_SYNCBB) < 0) {
		//TODO: c2_bus_set_error(bus, "ftdi_set_bitmode failed: %s\n", ftdi_get_error_string(&ftdic));
		c2_bus_set_error(bus, "ftdi_set_bitmode failed");
		goto bad3;
	}

	if (_ftdi_flush(hw) != 0) {
		c2_bus_set_error(bus, "Failed to read/write flush byte");
		goto bad3;
	}

	bus->ops = &c2_bus_ftdi_ops;
	bus->ctx = hw;
	return 0;

bad3:
	ftdi_usb_close(hw->ftdi);
bad2:
	ftdi_free(hw->ftdi);
bad1:
	free(hw);
	return -1;
}

static int _sync_ftdi_read_write(struct c2_bus *bus, uint8_t *data, size_t len)
{
	struct c2_bus_ftdi_ctx *hw = (struct c2_bus_ftdi_ctx *) bus->ctx;
	int ret;
	int rlen;

#ifdef C2_FTDI_DEBUG
	int i;
	int x=len;
	fprintf(stderr, "W ");
	for (i = 0; i < x; i++)
		fprintf(stderr, "%x ", data[i]);
	fprintf(stderr, "\n");
#endif

	ret = ftdi_write_data(hw->ftdi, data, len);
	if (ret != len) {
		c2_bus_set_error(bus, "ftdi_write_data failed");
		return -1;
	}

	rlen = 0;
	while (len) {
		ret = ftdi_read_data(hw->ftdi, &data[rlen], len);
		if (ret < 0 || ret > len) {
			c2_bus_set_error(bus, "ftdi_read_data failed");
			ftdi_usb_purge_buffers(hw->ftdi);
//FIXME: purge is broken, next read/write after purge isn't correct.
			return -1;
		}
		rlen += ret;
		len -= ret;
	}

#ifdef C2_FTDI_DEBUG
	fprintf(stderr, "R ");
	for (i = 0; i < x; i++)
		fprintf(stderr, "%x ", data[i] & (PIN_C2D | PIN_C2CK));
	fprintf(stderr, "\n");
#endif

	return 0;
}

static int _strobe_clk(struct c2_bus *bus)
{
	uint8_t buf[2] = { 0x00, PIN_C2CK };

	return _sync_ftdi_read_write(bus, buf, sizeof(buf));
}

static int _enable_c2d_out(struct c2_bus *bus)
{
	struct c2_bus_ftdi_ctx *hw = (struct c2_bus_ftdi_ctx *) bus->ctx;

	if (ftdi_set_bitmode(hw->ftdi, PIN_C2CK | PIN_C2D, BITMODE_SYNCBB) != 0) {
		c2_bus_set_error(bus, "Failed to enable C2D output");
		return -1;
	}

	return 0;
}

static int _disable_c2d_out(struct c2_bus *bus)
{
	struct c2_bus_ftdi_ctx *hw = (struct c2_bus_ftdi_ctx *) bus->ctx;

	if (ftdi_set_bitmode(hw->ftdi, PIN_C2CK, BITMODE_SYNCBB) != 0) {
		c2_bus_set_error(bus, "Failed to disable C2D output");
		return -1;
	}

	return 0;
}

static int _write_bits(struct c2_bus *bus, const uint8_t *buf, size_t bit_cnt)
{
	uint8_t bit_buf[2*8*5];
	uint8_t *bbp = bit_buf;
	const uint8_t *bp = buf;
	uint8_t mask = 1;
	int i;
	
	assert(bit_cnt <= 8*5);

	for (i = 0; i < bit_cnt; i++) {
		if (*bp & mask) {
			bbp[0] = PIN_C2D;
			bbp[1] = PIN_C2D | PIN_C2CK;
		} else {
			bbp[0] = 0;
			bbp[1] = PIN_C2CK;
		}
		bbp += 2;

		mask = (mask << 1) & 0xff;
		if (mask == 0) {
			mask = 1;
			bp++;
		}
	}

	return _sync_ftdi_read_write(bus, bit_buf, bit_cnt * 2);
}

static int _read_bits(struct c2_bus *bus, uint8_t *buf, size_t bit_cnt)
{
	uint8_t bit_buf[2*(8*5)+1];
	uint8_t *bbp = bit_buf;
	uint8_t *bp = buf;
	int i;
	int err;
	
	assert(bit_cnt <= 8*5);

	for (i = 0; i < bit_cnt; i++) {
		bbp[0] = 0;
		bbp[1] = PIN_C2CK;
		bbp += 2;
	}
	// Do one extra read because C2D must be sampled one interval after
	// the rising edge
	bbp[0] = PIN_C2CK;

	err = _sync_ftdi_read_write(bus, bit_buf, bit_cnt * 2 + 1);
	if (err != 0) {
		return err;
	}

	// First valid read bit is at index 3, then at an interval of 2.
	// This way sampling is done just before the falling edge.
	bbp = &bit_buf[2];
	for (i = 0; i < bit_cnt; i++) {
		*bp = (*bp >> 1) & 0x7f;
		if (*bbp & PIN_C2D) {
			*bp |= 0x80;
		}
		
		if (i % 8 == 7) {
			bp++;
		}

		bbp += 2;
	}

	return 0;
}

static int _c2_wait(struct c2_bus *bus)
{
	uint8_t buf[3];
	int err;

	do {
		buf[0] = 0x00;
		buf[1] = PIN_C2CK;
		buf[2] = PIN_C2CK;

		err = _sync_ftdi_read_write(bus, buf, sizeof(buf));
		if (err != 0) {
			return err;
		}
	} while (! (buf[2] & PIN_C2D));
//TODO: add timeout?

	return 0;
}

/**
 * Wait for C2 Slave and directly give a stop bit when slave is done
 *
 * This is equivalent to calling:
 *  _c2_wait(bus);
 *  _c2_strobe(bus);
 * By integrating the two there is a high changes that the strobe doesn't need
 * a seperate USB transfer. This reduces firmware upload time to ~76% of the
 * time it takes without this hack.
 */
static int _c2_wait_and_stop(struct c2_bus *bus)
{
	uint8_t buf[5];
	int err;

	do {
		buf[0] = 0x00;
		buf[1] = PIN_C2CK;
		buf[2] = 0x00;
		buf[3] = PIN_C2CK;
		buf[4] = PIN_C2CK;

		err = _sync_ftdi_read_write(bus, buf, sizeof(buf));
		if (err != 0) {
			return err;
		}
	} while (! ((buf[2] & PIN_C2D) || (buf[4] & PIN_C2D)));
//TODO: add timeout?

	if (!(buf[2] & PIN_C2D)) {
		return _strobe_clk(bus);
	}

	return 0;
}


int c2_ftdi_reset(struct c2_bus *bus)
{
	uint8_t buf[100];
	int i;
	int Trd = 250 / (10000000/BAUD) + 1;
	int Tsd = (Trd/10) + 1;

	assert(Trd + Tsd < sizeof(buf));

	memset(buf, 0, sizeof(buf));
	for (i = Trd; i < Trd + Tsd; i++) {
		buf[i] = PIN_C2CK;
	}

	return _sync_ftdi_read_write(bus, buf, Trd + Tsd);
}

int c2_ftdi_qreset(struct c2_bus *bus)
{
	return _strobe_clk(bus);
}

#define CHECK(X) do { int err; err = X; if (err != 0) return err; } while (0)

int c2_ftdi_addr_read(struct c2_bus *bus, unsigned char *addr)
{
	uint8_t ins[1] = { C2_INS_ADDR_READ };
	uint8_t buf[2];

	// Start
	CHECK(_strobe_clk(bus));

	// Ins
	CHECK(_enable_c2d_out(bus));
	CHECK(_write_bits(bus, ins, 2));
	CHECK(_disable_c2d_out(bus));

	// Addr + Stop
	CHECK(_read_bits(bus, buf, 9));
	*addr = buf[0];

	return 0;
}

int c2_ftdi_addr_write(struct c2_bus *bus, unsigned char addr)
{
	uint8_t ins_addr[2];

	// Start
	CHECK(_strobe_clk(bus));

	// Ins + addr
	CHECK(_enable_c2d_out(bus));
	ins_addr[0] = C2_INS_ADDR_WRITE | (addr << 2);
	ins_addr[1] = (addr >> 6);
	CHECK(_write_bits(bus, ins_addr, 10));
	CHECK(_disable_c2d_out(bus));

	// Stop
	CHECK(_strobe_clk(bus));

	return 0;
}

int c2_ftdi_data_read(struct c2_bus *bus, unsigned char *data, size_t len)
{
	uint8_t ins[1];
	uint8_t buf[5];
	
	assert(len <= 4);

	// Start
	CHECK(_strobe_clk(bus));

	// Ins + Len
	CHECK(_enable_c2d_out(bus));
	ins[0] = C2_INS_DATA_READ | (((len - 1) & 0x3) << 2);
	CHECK(_write_bits(bus, ins, 4));
	CHECK(_disable_c2d_out(bus));

	// Wait
	CHECK(_c2_wait(bus));

	// Data + Stop
	CHECK(_read_bits(bus, buf, (8 * len) + 1));
	memcpy(data, buf, len);

	return 0;
}

int c2_ftdi_data_write(struct c2_bus *bus, const unsigned char *data, size_t len)
{
	uint8_t buf[5];
	int i;

	assert(len <= 4);

	// Start
	CHECK(_strobe_clk(bus));

	// Ins + Len + Data
	CHECK(_enable_c2d_out(bus));
	buf[0] = C2_INS_DATA_WRITE | (((len - 1) & 0x3) << 2);
	for (i=0; i < len; i++) {
		buf[i] = (buf[i] & 0xf) | ((data[i] << 4) & 0xf0);
		buf[i+1] = data[i] >> 4;
	}
	CHECK(_write_bits(bus, buf, 4 + (8 * len)));
	CHECK(_disable_c2d_out(bus));

	// Wait + Stop
	CHECK(_c2_wait_and_stop(bus));

	return 0;
}

void c2_ftdi_destroy(struct c2_bus *bus)
{
	struct c2_bus_ftdi_ctx *hw = (struct c2_bus_ftdi_ctx *) bus->ctx;

	if (hw == NULL) return;

	if (hw->ftdi != NULL) {
		if (ftdi_set_bitmode(hw->ftdi, 0, BITMODE_RESET) < 0) {
			fprintf(stderr, "WARNING: "
				"Unable to reset FTDI bitbang mode: %s\n",
				ftdi_get_error_string(hw->ftdi));
		}

		ftdi_usb_close(hw->ftdi);
		ftdi_free(hw->ftdi);
	}
	free(hw);

	bus->ops = NULL;
	bus->ctx = NULL;
}
