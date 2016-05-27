;;
; Si4010 MTP memory reader/writer
;
; This program reads/writes the Si4010 MTP memory into/from XRAM. Parameters and
; return data is communicated using direct memory read/writes through the C2
; debug interface.
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

pbMtp_Read = 0x806C      ; uint8_t * pbMtp_Read()
bMtp_Write = 0x8069      ; uint8_t bMtp_Write(uint8_t biProgLimit)

abMTP_RDATA = 0x4040     ; _xdata uint8_t abMTP_RDATA[16]

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
; Write result
wres::
	.blkb	1
; Write offset
woff::
	.blkb	1
; Write Length
wlen::
	.blkb	1
; Write Data
wdata::
	.blkb	16

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Code Area
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.area _CODE

init::
	; setup stack pointer
	mov	SP, #0x80

main:
	lcall	pbMtp_Read
	mov	DPH, R6
	mov	DPL, R7

	; indicate read was successfull
	mov	wres, #2

	; Only read MTP if 'wlen' == 0
	mov	A, wlen
	jz	loop_breakpoint

	; Add offset output buffer pointer
	mov	A, woff
	add	A, DPL
	mov	DPL, A
	clr	A
	addc	A, DPH
	mov	DPH, A

	; Copy new data into output buffer
	mov	R0, #wdata
write_loop:
	mov	A, @R0
	movx	@DPTR, A
	inc	R0
	inc	DPTR
	djnz	wlen, write_loop

	; Write output buffer to MTP
	mov	R7, #40			; biProgLimit
	lcall	bMtp_Write
	mov	wres, R7

	; Wait for programmer, MTP data is now available in abMTP_RDATA[16](0x04040)

loop_breakpoint::
	sjmp	.
