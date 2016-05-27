/*
 * c2prog.c - FX2 C2 Bus Master Interface
 *
 * Copyright (c) 2012, David Imhoff <dimhoff.devel@gmail.com>
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
#define ALLOCATE_EXTERN
#include <fx2regs.h>
#include <fx2_util.h>

#include "timing_fx2_48mhz.h"
#include "uout.h"
#include "uin.h"

#define PIN_NONE  0x00
#define PIN_C2D   0x04
#define PIN_C2CLK 0x02

static void Initialize(void)
{
	CPUCS=0x10;   // 48 MHz, CLKOUT output disabled. 
	
	IFCONFIG=0xc0;  // Internal IFCLK, 48MHz; A,B as normal ports. 
	SYNCDELAY;

	uout_init();
	uin_init();

	// port A (A.2 = c2d, A.1 = c2clk)
	IOA = PIN_C2CLK;
	OEA = PIN_C2CLK;
}

#define CLOCK_DELAY 
#define ENABLE_C2D_OUT  OEA = PIN_C2CLK | PIN_C2D
#define DISABLE_C2D_OUT OEA = PIN_C2CLK
	
#define SEND_BIT_0 IOA = PIN_NONE; CLOCK_DELAY; IOA = PIN_C2CLK;           CLOCK_DELAY;
#define SEND_BIT_1 IOA = PIN_C2D;  CLOCK_DELAY; IOA = PIN_C2CLK | PIN_C2D; CLOCK_DELAY;

#define STROBE_CLOCK SEND_BIT_0

unsigned char c2_data[5];

static unsigned char read_bit()
{
	IOA = PIN_NONE;
	CLOCK_DELAY;
	IOA = PIN_C2CLK;
	CLOCK_DELAY;
	return (IOA & PIN_C2D) != 0;
}

void c2_reset()
{
	// RESET
	IOA = PIN_NONE;
	wait_10us(2);
	wait_1us();
	wait_1us();
	IOA = PIN_C2CLK;
	wait_1us();
	wait_1us();
}

void c2_qreset()
{
	// RESET
	STROBE_CLOCK;
}

void c2_read_addr()
{
	unsigned char i;
	unsigned char j;

	// START
	STROBE_CLOCK;

	//INS
	ENABLE_C2D_OUT;
	SEND_BIT_0;
	SEND_BIT_1;
	DISABLE_C2D_OUT;

	//DATA
	j = 0; // Address register can be bigger than 1 byte?
	i = 8;
	while ( i != 0) {
		c2_data[j] = (c2_data[j] >> 1) | (read_bit() << 7); 
		i--;
	}

	// STOP
	STROBE_CLOCK;
}

void c2_write_addr()
{
	unsigned char i;
	unsigned char j;

	// START
	STROBE_CLOCK;

	//INS
	ENABLE_C2D_OUT;
	SEND_BIT_1;
	SEND_BIT_1;

	//DATA
	j = 0; // Address register can be bigger than 1 byte?
	i = 8;
	while ( i != 0) {
		if (c2_data[j] & 0x01) {
			SEND_BIT_1;
		} else {
			SEND_BIT_0;
		}
		c2_data[j] = (c2_data[j] >> 1);
		i--;
	}
	DISABLE_C2D_OUT;

	// STOP
	STROBE_CLOCK;
}

void c2_data_read(unsigned char len)
{
	unsigned char i;
	unsigned char j;

	len -= 1;
	len = len & 0x3;

	// START
	STROBE_CLOCK;

	//INS
	ENABLE_C2D_OUT;
	SEND_BIT_0;
	SEND_BIT_0;

	// LEN
	if (len & 1) {
		SEND_BIT_1;
	} else {
		SEND_BIT_0;
	}
	if (len & 2) {
		SEND_BIT_1;
	} else {
		SEND_BIT_0;
	}
	DISABLE_C2D_OUT;

	// WAIT
	while (read_bit() == 0) ;

	// DATA
	for (j=0; j < len + 1; j++) {
		i = 8;
		while ( i != 0) {
			c2_data[j] = (c2_data[j] >> 1) | (read_bit() << 7); 
			i--;
		}
	}

	// STOP
	STROBE_CLOCK;
}

void c2_data_write(unsigned char len)
{
	unsigned char i;
	unsigned char j;
	unsigned char b;

	len -= 1;
	len = len & 0x3;

	// START
	STROBE_CLOCK;

	//INS
	ENABLE_C2D_OUT;
	SEND_BIT_1;
	SEND_BIT_0;

	// LEN
	if (len & 1) {
		SEND_BIT_1;
	} else {
		SEND_BIT_0;
	}
	if (len & 2) {
		SEND_BIT_1;
	} else {
		SEND_BIT_0;
	}

	// DATA
	j=0;
	for (j=0; j < len + 1; j++) {
		b = c2_data[j];
		i = 8;
		while ( i != 0) {
			if (b & 0x01) {
				SEND_BIT_1;
			} else {
				SEND_BIT_0;
			}
			b = (b >> 1);
			i--;
		}
	}
	DISABLE_C2D_OUT;

	//WAIT
	while (read_bit() == 0) ;

	// STOP
	STROBE_CLOCK;
}

enum CmdOpcodes {
	CmdReset,
	CmdQReset,
	CmdReadAddr,
	CmdWriteAddr,
	CmdReadData,
	CmdWriteData
};

enum InputParseState {
	InputIdle,
	InputLen,
	InputData
} input_state;

unsigned char input_buf[6];
unsigned char input_cnt;
unsigned char input_len;

void input_cmd_cb()
{
	unsigned char dlen;
	unsigned char i;

	uout_write_byte('>');
	switch (input_buf[0]) {
	case CmdReset:
		if (input_len != 1) {
			uout_write_byte(0xff);
			break;
		}

		c2_reset();

		uout_write_byte(0);
		break;
	case CmdQReset:
		if (input_len != 1) {
			uout_write_byte(0xff);
			break;
		}

		c2_qreset();

		uout_write_byte(0);
		break;
	case CmdReadAddr:
		//TODO: support for other address lengths
		if (input_len != 1) {
			uout_write_byte(0xff);
			break;
		}

		c2_read_addr();

		uout_write_byte(1);
		uout_write_byte(c2_data[0]);
		break;
	case CmdWriteAddr:
		//TODO: support for other address lengths
		if (input_len != 2) {
			uout_write_byte(0xff);
			break;
		}

		c2_data[0] = input_buf[1];
		c2_write_addr();

		uout_write_byte(0);
		break;
	case CmdReadData:
		if (input_len != 2) {
			uout_write_byte(0xff);
			break;
		}
		dlen = input_buf[1];
		if (dlen > 4 || dlen < 1) {
			uout_write_byte(0xff);
			break;
		}

		c2_data_read(dlen);

		uout_write_byte(dlen);
		for (i=0; i < dlen; i++) {
			uout_write_byte(c2_data[i]);
		}
		break;
	case CmdWriteData:
		if (input_len == 1) {
			uout_write_byte(0xff);
			break;
		}
		dlen = input_len - 1;
		for (i=0; i < dlen; i++) {
			c2_data[i] = input_buf[i+1];
		}

		c2_data_write(dlen);

		uout_write_byte(0);
		break;
	default:
		uout_write_byte(0xff);
		break;
	}

	uout_flush();
}

void usb_recv_cb(char b)
{
	switch (input_state) {
	case InputIdle:
		if (b == '<') {
			input_state = InputLen;
		}
		break;
	case InputLen:
		if (b > 6 || b < 1) {
			input_state = InputIdle;
		} else {
			input_len = b;
			input_cnt = 0;
			input_state = InputData;
		}
		break;
	case InputData:
		input_buf[input_cnt] = b;
		input_cnt++;
		if (input_cnt == input_len) {
			input_cmd_cb();
			input_state = InputIdle;
		}
		break;
	default:
		input_state = InputIdle;
		break;
	}
}

void main(void)
{
	unsigned char i=0;
	unsigned char j=0;
	unsigned char res=0;
	unsigned char b;

	Initialize();

	for(;;) {
		// Wait for user input
		if (uin_read_byte_nb(&b))
			usb_recv_cb(b);
	}
}
