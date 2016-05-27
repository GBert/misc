/**
 * dehexify.c - Convert hexadecimal encoded ASCII string to binary
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
#include <string.h>
#include "dehexify.h"

static int dehex_nibble(unsigned char n)
{
	if (n >= '0' && n <= '9')
		return n - '0';
	if (n >= 'A' && n <= 'F')
		return 10 + n - 'A';
	if (n >= 'a' && n <= 'f')
		return 10 + n - 'a';
	return -1;
}

int dehexify(const char *in, size_t bytes, unsigned char *out)
{
	int i;
	int x;

	if (strlen(in) < bytes*2) {
		return -1;
	}

	memset(out, 0, bytes);
	for (i=0; i<bytes; i++) {
		x = dehex_nibble(in[(i*2)]);
		if (x < 0 || x > 15) {
			return -1;
		}
		out[i] = (x << 4);

		x = dehex_nibble(in[(i*2)+1]);
		if (x < 0 || x > 15) {
			return -1;
		}
		out[i] |= x;
	}

	return 0;
}


