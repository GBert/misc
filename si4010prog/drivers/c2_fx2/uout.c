/*
 * uout.c - FX2 USB output stream.
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

unsigned char buf_len = 0;

void uout_init() {
	EP6CFG=0xe2;  // 1110 0010 (bulk IN, 512 bytes, double-buffered)
	SYNCDELAY;

	buf_len = 0;
}

void uout_flush() {
	if (buf_len != 0) {
		// Arm the endpoint. Be sure to set BCH before BCL because BCL access 
		// actually arms the endpoint. 
		EP6BCH=0; SYNCDELAY;
		EP6BCL=buf_len; SYNCDELAY;

		buf_len = 0;
	}
}

void uout_write_byte(unsigned char b) {
	__xdata unsigned char *dest = EP6FIFOBUF;

	// If full, wait...
	while ((EP2468STAT & 0x20)) { 
#ifdef UOUT_DEBUG_LEDS
		IOE &= ~0x01;
	}
	IOE |= 0x01;
#else
	}
#endif
	
	dest[buf_len] = b;

	if (++buf_len == 63) {
		// Arm the endpoint. Be sure to set BCH before BCL because BCL access 
		// actually arms the endpoint. 
		EP6BCH=0; SYNCDELAY;
		EP6BCL=buf_len; SYNCDELAY;

		buf_len = 0;
	} else {
//		buf_len++;
	}
#ifdef UOUT_DEBUG_LEDS
	IOE ^= 0x02;
#endif
}


