; vim:noet:sw=8:ts=8:ai:syn=pic
;
; USB 512-Word CDC Bootloader for PIC16(L)F1454/5/9
; Copyright (c) 2015, Matt Sarnoff (msarnoff.org)
; v1.0, February 12, 2015
; Released under a 3-clause BSD license: see the accompanying LICENSE file.
;
; PIC assembly language macros.

;;; Loads the address of the given symbol into FSR0.
ldfsr0	macro 	x
	movlw	low x
	movwf	FSR0L
	movlw	high x
	movwf	FSR0H
	endm

;;; Loads the given address in data space into FSR0.
;;; (This ensures that the high bit is not set, which the high directive may
;;; do implicitly)
ldfsr0d	macro	x
	movlw	low x
	movwf	FSR0L
	movlw	(high x) & 0x7F
	movwf	FSR0H
	endm

;;; Loads the address of the given symbol into FSR1.
ldfsr1	macro 	x
	movlw	low x
	movwf	FSR1L
	movlw	high x
	movwf	FSR1H
	endm

;;; Loads the given address in data space into FSR1.
;;; (This ensures that the high bit is not set, which the high directive may
;;; do implicitly)
ldfsr1d	macro	x
	movlw	low x
	movwf	FSR1L
	movlw	(high x) & 0x7F
	movwf	FSR1H
	endm

;;; Loads the address of the given symbol into PMADRH:PMADRL.
ldpmadr	macro	x
	banksel	PMADRL
	movlw	low x
	movwf	PMADRL
	movlw	high x
	movwf	PMADRH
	endm

;;; Waits until the bit in the specified register is set.
waitfs	macro 	reg,bit
	btfss	reg,bit
	goto	$-1
	endm

;;; Waits until the bit in the specified register is cleared.
waitfc	macro 	reg,bit
	btfsc	reg,bit
	goto	$-1
	endm

;;; Returns if the Z flag is set.
retz	macro
	skpnz
	return
	endm

;;; Returns if the Z flag is not set.
retnz	macro
	skpz
	return
	endm

;;; Return if bit in file is set.
retbfs	macro	f,b
	btfsc	f,b
	return
	endm

;;; Return if bit in file is clear.
retbfc	macro	f,b
	btfss	f,b
	return
	endm

;;; Branch to label if bit in file is set.
bbfs	macro	f,b,label
	btfsc	f,b
	goto	label
	endm

;;; Branch to label if bit in file is clear.
bbfc	macro	f,b,label
	btfss	f,b
	goto	label
	endm

;;; Subtracts the literal from W. (opposite of 'sublw')
subwl	macro	x
	addlw	256-x
	endm

;;; Increments W.
incw	macro
	addlw	1
	endm

;;; Decrements W.
decw	macro
	addlw	255
	endm

