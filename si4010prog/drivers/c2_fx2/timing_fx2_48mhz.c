/*
 * timing_fx2_48mhz.c - Timing functions for Cypress EZ-USB FX2 @ 48 MHz
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
#include "timing_fx2_48mhz.h"

void wait_1us() __naked
{
	__asm
	; lcall =			  4c
	orl	A, #0x00		; 2c
	orl	A, #0x00		; 2c
	ret				; 4c
	__endasm;
}

void wait_5us() __naked
{
	__asm
	; lcall =			  4c
	push	ACC			; 2c

	nop				; 1c
	nop				; 1c

	mov	A, #11			; 2c
00001$:
	djnz	ACC, 00001$		; 4c

	pop	ACC			; 2c
	ret				; 4c
	__endasm;
}

// 10 us @ 48 Mhz = 10 / (1/ (48 /4)) = 120 cycles
void wait_10us(unsigned char times) __naked
{
	times;
	__asm
	; mov dpl + lcall =		  7c

	; First run compensate for call overhead
	push	ACC			; 2c
	nop				; 1c
	mov	A, #24			; 2c
00002$:
	djnz	ACC, 00002$		; 4c

	mov	A, DPL			; 2c
	dec	A			; 1c
	jz	00005$			; 3c

	; subsequential runs
00003$:
	mov	DPL, A			; 2c
	nop				; 1c
	nop				; 1c
	mov	A, #27			; 2c
00004$:
	djnz	ACC, 00004$		; 4c

	mov	A, DPL			; 2c
	djnz	ACC, 00003$		; 4c

00005$:
	pop	ACC			; 2c
	ret				; 4c
	__endasm;
}
