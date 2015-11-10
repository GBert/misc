; vim:noet:sw=8:ts=8:ai:syn=pic
;
; USB 512-Word CDC Bootloader for PIC16(L)F1454/5/9
; Copyright (c) 2015, Matt Sarnoff (msarnoff.org)
; v1.0, February 12, 2015
; Released under a 3-clause BSD license: see the accompanying LICENSE file.
;
; Constants used by the bootloader communications protocol.

;;; Command is determined by the length of the data packet received.
; Set Address/Checksum/Erase Flag
BCMD_SET_PARAMS_LEN	equ	4
BCMD_SET_PARAMS_ADRL	equ	0	; low byte of address
BCMD_SET_PARAMS_ADRH	equ	1	; high byte of address
BCMD_SET_PARAMS_CKSUM	equ	2	; expected checksum of next 32 words
BCMD_SET_PARAMS_ERASE	equ	3	; if equal to BCMD_ERASE_CHAR, erase this row
BCMD_ERASE_CHAR		equ	'E'

; Write 32 Words
BCMD_WRITE_LEN		equ	64

; Reset Device
BCMD_RESET_LEN		equ	1
BCMD_RESET_CHAR		equ	'R'	; if equal to input byte, reset the device


;;; Status codes returned from device to host
BSTAT_OK		equ	1	; all ok
BSTAT_INVALID_COMMAND	equ	2	; host sent an invalid command
BSTAT_INVALID_CHECKSUM	equ	3	; invalid checksum, no write performed
BSTAT_VERIFY_FAILED	equ	4	; data written to flash doesn't match data sent by host

