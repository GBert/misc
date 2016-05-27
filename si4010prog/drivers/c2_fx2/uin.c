/*
 * uin.c - FX2 USB input stream.
 *
 * Copyright (c) 2008, David Imhoff <dimhoff.devel@gmail.com>
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

unsigned short buf_idx = 0;
unsigned short cur_len = 0;


void uin_init() {
	REVCTL=0x03;     SYNCDELAY;  // See TRM... required for OUTPKTEND to work

	EP2CFG=0xA2;  // 1010 0010 (bulk OUT, 512 bytes, double-buffered)
	SYNCDELAY;

//TODO: why do we always get one byte extra the first time we read?? --old
// 20120628: maybee the next fixes this...
	// Some voodoo that hopefully fixes problems of ghost pages being received
	// from: http://www.triplespark.net/elec/periph/USB-FX2/software/local_examples.html
	FIFORESET=0x80;  SYNCDELAY;  // NAK all requests from host.
	FIFORESET=0x82;  SYNCDELAY;  // Reset individual EP (2,4,6,8)
	FIFORESET=0x84;  SYNCDELAY;
	FIFORESET=0x86;  SYNCDELAY;
	FIFORESET=0x88;  SYNCDELAY;
	FIFORESET=0x00;  SYNCDELAY;  // Resume normal operation.
	EP2FIFOCFG=0x00; SYNCDELAY;  // Make sure AUTOOUT=0.

	// Clear the 2 buffers (double-buffered)
	// writing EP2BCL with 0x80 doesn't work somehow
	OUTPKTEND = 0x82;  SYNCDELAY;
	OUTPKTEND = 0x82;  SYNCDELAY;

	buf_idx = 0;
	cur_len = 0;
}

unsigned char uin_read_byte_nb(unsigned char *c) {
	__xdata unsigned char *src = EP2FIFOBUF;

	// If empty, return...
	if ((cur_len == 0) && (EP2468STAT & 0x01)) {
		return 0;
	}
	cur_len = (EP2BCH<<8) | EP2BCL;
	
	*c = EP2FIFOBUF[buf_idx];
	buf_idx +=1;

	// all bytes processed, reload endpoint
	if (buf_idx == cur_len) {
		OUTPKTEND = 0x82;  SYNCDELAY;
		buf_idx = 0;
		cur_len = 0;
	}

#ifdef UIN_DEBUG_LEDS
	IOE ^= 0x02;
#endif
	return 1;
}
