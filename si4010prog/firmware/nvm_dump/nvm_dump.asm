;;
; NVM memory reader
;
; This program reads the Si4010 NVM memory in blocks into XRAM. From where it
; can be retrieved using the C2 debug interface.
;
; Target: Si4010
; Toolchain: SDCC
;
; Copyright (c) 2014, David Imhoff <dimhoff.devel@gmail.com>
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are met:
;     * Redistributions of source code must retain the above copyright
;       notice, this list of conditions and the following disclaimer.
;     * Redistributions in binary form must reproduce the above copyright
;       notice, this list of conditions and the following disclaimer in the
;       documentation and/or other materials provided with the distribution.
;     * Neither the name of the author nor the names of its contributors may
;       be used to endorse or promote products derived from this software
;       without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
; WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
; MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
; EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
; SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
; WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
; OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
; ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;

NVM_ADDRL = 0xd1
NVM_ADDRH = 0xd2
NVM_CTRL  = 0xd3
NVM_CFG   = 0xd4
NVM_DATA  = 0xd6

vNvm_McDisableRead = 0x8081
vNvm_McEnableRead = 0x807E

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Jump Table
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.area JUMP_TABLE (ABS)
	.org	0
	ljmp	init

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Parameters writen from debugger
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.area	PARAM (ABS)
	.org	0x40
; Start Address
addr:
	.blkb	2
; Block Length in bytes
blocklen:
	.blkb	2

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Code Area
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.area _CODE

init::
	; setup stack pointer
	mov	SP, #0x80

main:
; if (blocklen > 0x1000) blocklen = 0x1000
	mov	A, #0xf0
	add	A, blocklen
	jnc	blocklen_check_ok
	mov	blocklen, #0x10
	mov	(blocklen + 1), #0x0
blocklen_check_ok:

; NVM_CFG[2:0] = 010b
	anl	NVM_CFG, #0xF8
	orl	NVM_CFG, #0x02

; NVM_ADDR = start
	mov	NVM_ADDRH, addr
	mov	NVM_ADDRL, (addr + 1)

; vNvm_McEnableRead();
	lcall	vNvm_McEnableRead

; NVM_CTRL = 2; // Trigger first read
	mov	NVM_CTRL, #0x02

; do {
block_loop:
	mov	DPTR, #ret_addr

; ret.addr = addr;
	mov	A, addr
	movx	@DPTR, A
	inc	DPTR
	mov	A, (addr + 1)
	movx	@DPTR, A
	inc	DPTR

; ret.len = 0;
	clr	A
	movx	@DPTR, A
	inc	DPTR
	movx	@DPTR, A
	inc	DPTR

;;;;
; bytes_left = 0x10000 - addr
	clr	C
	subb	A, (addr + 1)
	mov	R7, A
	clr	A
	subb	A, addr
	mov	R6, A

; if (bytes_left < blocklen)
;   read_len = bytes_left
; else
;   read_len = len
	clr	C
	mov	A, (blocklen + 1)
	subb	A, R7
	mov	A, blocklen
	subb	A, R6
	jnc	bytes_left_skip
	mov	R6, blocklen
	mov	R7, (blocklen + 1)
bytes_left_skip:

; i = read_len;
	mov	R4, 6
	mov	R5, 7
; while (i != 0) {
read_loop:
	mov	A, R5
	orl	A, R4
	jz	read_loop_break

;   while (NVM_CTRL != 0);
wait_nvm1:
	mov	A, NVM_CTRL
	jnz	wait_nvm1

;   *val = NVM_DATA; val++;
	mov	A, NVM_DATA
	movx	@DPTR, A
	inc	DPTR

;   i--;
	dec     R5
	cjne    R5, #0xff, read_loop
	dec	R4
	sjmp	read_loop
read_loop_break:
;}

; ret.len = bytes_read;
	mov	R0, #ret_len
	mov	A, R6
	movx	@R0, A
	inc	R0
	mov	A, R7
	movx	@R0, A

; addr += bytes_read;
	mov	A, (addr + 1)
	add	A, R7
	mov	(addr + 1), A
	mov	A, addr
	addc	A, R6
	mov	addr, A

loop_breakpoint::
; } while (1);
	sjmp	block_loop

; Disable since I assume that a MCU reset will have the same effect
; vNvm_McDisableRead();
;	lcall	vNvm_McDisableRead
; while (1);
;	sjmp	.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Data return read from debugger
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	.org	0x76	; Stay under 0x80-(2*2), cause factory data starts at 0x1080
ret::

ret_addr = ret
ret_len = ret + 2
ret_data = ret + 4
