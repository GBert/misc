; vim:noet:sw=8:ts=8:ai:syn=pic
;
; USB 512-Word CDC Bootloader for PIC16(L)F1454/5/9
; Copyright (c) 2015, Matt Sarnoff (msarnoff.org)
; v1.0, February 12, 2015
; Released under a 3-clause BSD license: see the accompanying LICENSE file.
;
; USB-related constants.

; USB PID values
PID_OUT		equ	(0x1<<PID0)
PID_ACK		equ	(0x2<<PID0)
PID_DATA0	equ	(0x3<<PID0)
PID_PING	equ	(0x4<<PID0)
PID_SOF		equ	(0x5<<PID0)
PID_NYET	equ	(0x6<<PID0)
PID_DATA2	equ	(0x7<<PID0)
PID_SPLIT	equ	(0x8<<PID0)
PID_IN		equ	(0x9<<PID0)
PID_NAK		equ	(0xA<<PID0)
PID_DATA1	equ	(0xB<<PID0)
PID_PRE		equ	(0xC<<PID0)
PID_ERR		equ	(0xC<<PID0) ; same as PRE
PID_SETUP	equ	(0xD<<PID0)
PID_STALL	equ	(0xE<<PID0)
PID_MDATA	equ	(0xF<<PID0)

; SETUP data packet fields
bmRequestType	equ	0
bRequest	equ	1
wValueL		equ	2
wValueH		equ	3
wIndexL		equ	4
wIndexH		equ	5
wLengthL	equ	6
wLengthH	equ	7

; bmRequestType bit masks
_DIRECTION	equ	b'10000000'
_REQ_TYPE	equ	b'01100000'
_RECIPIENT	equ	b'00011111'

; Standard device requests
GET_STATUS	equ	0x00
CLEAR_FEATURE	equ	0x01
SET_FEATURE	equ	0x03
SET_ADDRESS	equ	0x05
GET_DESCRIPTOR	equ	0x06
SET_DESCRIPTOR	equ	0x07
GET_CONFIG	equ	0x08
SET_CONFIG	equ	0x09
GET_INTERFACE	equ	0x0A
SET_INTERFACE	equ	0x0B
SYNCH_FRAME	equ	0x0C
SET_SEL		equ	0x30
SET_ISOCH_DELAY	equ	0x31

; Descriptor types
DESC_DEVICE	equ	0x01
DESC_CONFIG	equ	0x02
DESC_STRING	equ	0x03
DESC_INTERFACE	equ	0x04
DESC_ENDPOINT	equ	0x05
DESC_DEV_QUAL	equ	0x06
DESC_OTHER_SPD	equ	0x07
DESC_INTF_PWR	equ	0x08
DESC_OTG	equ	0x09

