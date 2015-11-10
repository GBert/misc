; vim:noet:sw=8:ts=8:ai:syn=pic
;
; this code is basically functional; it does need a USB VID:PID
;
; USB 512-Word DFU Bootloader for PIC16(L)F1454/5/9
; Copyright (c) 2015, Peter Lawrence
; derived from
; USB 512-Word CDC Bootloader for PIC16(L)F1454/5/9
; Copyright (c) 2015, Matt Sarnoff (msarnoff.org)

; Released under a 3-clause BSD license: see the accompanying LICENSE file.
;
; the download/upload file is a 16384 byte LSB-first binary (the entire program memory of PIC16F145x)
; for writes, it doesn't matter what the first 1024 bytes are; _WRT_BOOT protects these
; dfu-util syntax is:
;  dfu-util -U read.bin -t 64
;  dfu-util -D write.bin -t 64
; the download file must incorporate a valid CRC-14 for the bootloader to consider it valid
;
; Bootloader is entered if:
; - the MCLR/RA3 pin is grounded at power-up or reset,
; (The internal pull-up is used; no external resistor is necessary.)
; - there is no valid application programmed,
; - the watchdog timed out
;
; A pre-computed CRC-14 at 0x1F7F confirms a valid application.
;
; At application start, the device is configured with a 48MHz CPU clock,
; using the internal oscillator and 3x PLL. If a different oscillator
; configuration is required, it must be set by the application.
;
; A serial number between 0 and 65535 should be specified during the build
; by using the gpasm -D argument to set the SERIAL_NUMBER symbol, e.g.
;   gpasm -D SERIAL_NUMBER=12345
; If not specified, it will default to zero.
; A host may not behave correctly if multiple PICs with the same serial number
; are connected simultaneously.
;
; Code notes:
; - Labels that do not begin with an underscore can be called as functions.
;   Labels that begin with an underscore are not safe to call, they should only
;   be reached via goto.
;
; - As much stuff as possible is packed into bank 0 of USB RAM. This includes the
;   buffer descriptors, bootloader state, and endpoint 0 OUT and IN buffers
;
; - Using DFU has the substantive advantage of needing only EP0.  A backwards-
;   compatible extension to the protocol is to use wBlockNum in DFU_DNLOAD and 
;   DFU_UPLOAD as the PIC flash row index (and optional PMCON1 CFGS select)

	radix dec
	list n=0,st=off
	include "p16f1454.inc"
	nolist
	include "macros.inc"
	include "bdt.inc"
	include "usb.inc"
	include "protocol_constants.inc"
	list
	errorlevel -302

;;; Compile options
; there is a genuine upside to a globally unique serial number (in a known memory location) programmed at the factory
; however, for hobbyists compiling this code, it is highly problematic to ensure uniqueness
; USB does not require serial numbers; their operational advantage is when resolving multiple devices plugged into the same computer
; if multiple devices with the same serial number are inserted at the same time to a computer, problems may result
; so, the operationally safe solution for this bootloader is to enable "HIDE_SERIAL_NUMBER" to prevent possible conflicts
HIDE_SERIAL_NUMBER	equ	1

;;; Configuration
	__config _CONFIG1, _FOSC_INTOSC & _WDTE_SWDTEN & _PWRTE_ON & _MCLRE_OFF & _CP_ON & _BOREN_ON & _IESO_OFF & _FCMEN_OFF
	__config _CONFIG2, _WRT_BOOT & _CPUDIV_NOCLKDIV & _USBLSCLK_48MHz & _PLLMULT_3x & _PLLEN_ENABLED & _STVREN_ON & _BORV_LO & _LPBOR_OFF & _LVP_ON

;;; Constants and variable addresses
	ifndef SERIAL_NUMBER
	variable SERIAL_NUMBER=0	; Why doesnt 'equ' work here? Go figure
	endif

; these values are temporary and for development testing only
; If your organization has its own vendor ID/product ID, substitute it here.
; the VID:PID for the DFU bootloader must be distinct from the product itself, as Windows insists on it
USB_VENDOR_ID		equ	0x1234
USB_PRODUCT_ID		equ	0x0001

SERIAL_NUMBER_DIGIT_CNT	equ	8	; length (in unicode characters) of string in SN descriptor
DEVICE_DESC_LEN		equ	18	; device descriptor length
CONFIG_DESC_TOTAL_LEN	equ	27	; total length of configuration descriptor and sub-descriptors
EXTRAS_LEN		equ	11	; total length of extras
SERIAL_NUM_DESC_LEN	equ	2+(SERIAL_NUMBER_DIGIT_CNT*2)
ALL_DESCS_TOTAL_LEN	equ	DEVICE_DESC_LEN+CONFIG_DESC_TOTAL_LEN+EXTRAS_LEN+SERIAL_NUM_DESC_LEN

EP0_BUF_SIZE 		equ	64	; endpoint 0 buffer size

; We're only using the USB minimum of 2 endpoints (EP0OUT and EP0IN); use the remaining BDT area for buffers.

; Use the 4 bytes normally occupied by the EP1 OUT (immediately after EP0IN) buffer descriptor for variables.
USB_STATE		equ	BANKED_EP1OUT+0
EP0_DATA_IN_PTR		equ	BANKED_EP1OUT+1	; pointer to descriptor to be sent (low byte only)
EP0_DATA_IN_COUNT	equ	BANKED_EP1OUT+2	; remaining bytes to be sent
;			equ	BANKED_EP1OUT+3	; spare

; USB data buffers go immediately after memory re-purposed for variables
EP0OUT_BUF		equ	EP1IN
BANKED_EP0OUT_BUF	equ	BANKED_EP1IN
EP0IN_BUF		equ	EP0OUT_BUF+EP0_BUF_SIZE
BANKED_EP0IN_BUF	equ	BANKED_EP0OUT_BUF+EP0_BUF_SIZE
EP_DATA_BUF_END		equ	EP0IN_BUF+EP0_BUF_SIZE

; High byte of all endpoint buffers.
EPBUF_ADRH		equ	(EP0OUT_BUF>>8)
	if ((EP0IN_BUF>>8) != (EP0OUT_BUF>>8))
	error "Endpoint buffers must be in the same 256-word region"
	endif

; Total length of all RAM (variables, buffers, BDT entries) used by the bootloader,
USED_RAM_LEN		equ	EP_DATA_BUF_END-BDT_START

BOOTLOADER_SIZE		equ	0x200

; Application code locations
APP_ENTRY_POINT		equ	0x200
APP_INTERRUPT		equ	(APP_ENTRY_POINT+4)

; USB_STATE bit flags
IS_CONTROL_WRITE	equ	0	; current endpoint 0 transaction is a control write
ADDRESS_PENDING		equ	1	; need to set address in next IN transaction
DEVICE_CONFIGURED	equ	2	; the device is configured
IS_DFU_UPLOAD		equ	3	; when active, ep0_read_in diverts to an alternate routine
IS_DFU_DNLOAD		equ	4	; when active, _its_an_out diverts to an alternate routine
DFU_DNLOAD_ACTIVE	equ	5	; when inactive: state=dfuIDLE, when active: state=dfuDNLOAD-IDLE

; scratchpad variables for CRC calculation (which overlap with bootloader variables, but are not used concurrently)
SCRATCHPAD		equ	0x70
COUNTDOWN		equ	0x71
CRCL			equ	0x72
CRCH			equ	0x73
ROW_COUNT		equ	0x74

;;; Vectors
	org	0x0000
RESET_VECT
; Enable weak pull-ups
	banksel	OPTION_REG
	bcf	OPTION_REG,NOT_WPUEN
	banksel	OSCCON
	goto	bootloader_start	; to be continued further down in the file

	org	0x0004
INTERRUPT_VECT
	movlp	high APP_INTERRUPT	; XC8 *expects* this
	goto	APP_INTERRUPT

; perform flash unlock sequence
; BSR=PMADRL
flash_unlock_sequence
	movlw	0x55
	movwf	PMCON2
	movlw	0xAA
	movwf	PMCON2
	bsf	PMCON1,WR
	nop				; mandatory nops
	nop
	return

_its_an_out
	btfss	USB_STATE,IS_DFU_DNLOAD
	goto	arm_ep0_out		; it must be a status (or other message whose contents we are not concerned about)

	; I'm genuinely unhappy with the present implementation below
	; dfu-util (or the USB library it uses) gets impatient and won't wait for multiple milliseconds for the flash operation
	; this necessitates sending a STATUS, making a quick copy of the data, re-arming OUT, and then doing the flash operation
	; I believe/hope that there is a better solution, but optimization will take more time

	bcf	BANKED_EP0IN_STAT,UOWN	; ensure we have ownership of the buffer
	clrf	BANKED_EP0IN_CNT	; we'll be sending a zero-length packet
	movlw	_DAT1|_DTSEN		; arm IN buffer
	movwf	BANKED_EP0IN_STAT
	bsf	BANKED_EP0IN_STAT,UOWN

	ldfsr0d	EP0OUT_BUF		; set up source pointer
	ldfsr1d	EP_DATA_BUF_END		; set up destination pointer
out_copy_loop
	tstf	BANKED_EP0OUT_CNT
	bz	out_finish
	moviw	FSR0++
	movwi	FSR1++
	decf	BANKED_EP0OUT_CNT,f
	goto out_copy_loop
out_finish
	movlw	_DAT0|_DTSEN|_BSTALL	; make OUT buffer ready for next SETUP packet
	call	arm_ep0_out_with_flags
; row of flash data to write is in BANKED_EP0OUT_BUF; PMADRL:PMADRH are already written
	ldfsr0d	EP_DATA_BUF_END		; set up source pointer
	banksel	PMADRL
; erase row
	bsf	PMCON1,FREE
	bsf	PMCON1,WREN
	call	flash_unlock_sequence
; write row
	bcf	PMCON1,FREE
	bsf	PMCON1,LWLO
	bsf	PMCON1,WREN
_flash_write_loop
	moviw	FSR0++
	movwf	PMDATL
	moviw	FSR0++
	movwf	PMDATH
	movfw	PMADRL
	andlw	b'00011111'	; mask address to yield row element number
	sublw	31
	btfsc	STATUS,Z
	bcf	PMCON1,LWLO	; we've now written to all the latches; this unlock is going to be special
	call	flash_unlock_sequence
	incf	PMADRL,f
	movfw	PMADRL
	andlw	b'00011111'	; mask address to yield row element number
	bnz	_flash_write_loop
	clrf	PMCON1
	banksel	BANKED_EP0IN_STAT
	return


;;; Handles a control transfer on endpoint 0.
;;; arguments:	expects USTAT value in FSR1H
;;;		BSR=0
;;; returns:	none
;;; clobbers:	W, FSR1H
usb_service_ep0
	btfsc	FSR1H,DIR	; is it an IN transfer or an OUT/SETUP?
	goto	_usb_ctrl_in
; it's an OUT or SETUP transfer
	movfw	BANKED_EP0OUT_STAT
	andlw	b'00111100'	; isolate PID bits
	sublw	PID_SETUP	; is it a SETUP packet?
	bnz	_its_an_out	; if not, it's a regular OUT
	; it's a SETUP packet--fall through

; Handles a SETUP control transfer on endpoint 0.
; BSR=0
_usb_ctrl_setup
	bcf	USB_STATE,IS_CONTROL_WRITE
	bcf	USB_STATE,IS_DFU_UPLOAD
	bcf	USB_STATE,IS_DFU_DNLOAD
; set IS_CONTROL_WRITE bit in USB_STATE according to MSB in bmRequestType
	btfss	BANKED_EP0OUT_BUF+bmRequestType,7	; is this host->device?
	bsf	USB_STATE,IS_CONTROL_WRITE		; if so, this is a control write
; check if bmRequestType is DFU
	movlw	0x21
	subwf	BANKED_EP0OUT_BUF+bmRequestType,w
	andlw	b'01111111'	; mask out MSB
	bz	_its_a_dfu_message
; check request number: is it Get Descriptor?
	movlw	GET_DESCRIPTOR
	subwf	BANKED_EP0OUT_BUF+bRequest,w
	bz	_usb_get_descriptor
; is it Set Address?
	movlw	SET_ADDRESS
	subwf	BANKED_EP0OUT_BUF+bRequest,w
	bz	_usb_set_address
; is it Set_Configuration?
	movlw	SET_CONFIG
	subwf	BANKED_EP0OUT_BUF+bRequest,w
	bz	_usb_set_configuration
; is it Get Configuration?
	movlw	GET_CONFIG
	subwf	BANKED_EP0OUT_BUF+bRequest,w
	bz	_usb_get_configuration
; unhandled request? fall through to _usb_ctrl_invalid

; Finishes a rejected SETUP transaction: the endpoints are stalled
_usb_ctrl_invalid
	banksel	UCON
	bcf	UCON,PKTDIS	; reenable packet processing
	banksel	BANKED_EP0IN_STAT
	movlw	_DAT0|_DTSEN|_BSTALL
	call	arm_ep0_in_with_flags
arm_ep0_out
	movlw	_DAT0|_DTSEN|_BSTALL
arm_ep0_out_with_flags			; W specifies STAT flags
	movwf	BANKED_EP0OUT_STAT
	movlw	EP0_BUF_SIZE		; reset the buffer count
	movwf	BANKED_EP0OUT_CNT
	bsf	BANKED_EP0OUT_STAT,UOWN	; arm the OUT endpoint
	return

_its_a_dfu_message
	movfw	BANKED_EP0OUT_BUF+bRequest
	bz	_dfu_detach	; enum=0
	decw
	bz	_dfu_dnload	; enum=1
	decw
	bz	_dfu_upload	; enum=2
	decw
	bz	_dfu_getstatus	; enum=3
	decw
	bz	_dfu_clrstatus	; enum=4
	decw
	bz	_dfu_getstate	; enum=5
	decw
	bz	_dfu_abort	; enum=6
	goto	_usb_ctrl_invalid

_dfu_dnload
	banksel	UCON
	bcf	UCON,PKTDIS		; reenable packet processing
	banksel	BANKED_EP0OUT_STAT
	movlw	_DAT1|_DTSEN
	movwf	BANKED_EP0OUT_STAT
	movlw	EP0_BUF_SIZE		; reset the buffer count
	movwf	BANKED_EP0OUT_CNT
	bsf	BANKED_EP0OUT_STAT,UOWN	; arm the OUT endpoint
	tstf	BANKED_EP0OUT_BUF+wLengthL
	btfsc	STATUS,Z
	goto	_dfu_dnload_exit	; wLength is zero, indicating end of download
	call	set_pm_address
	bsf	USB_STATE,DFU_DNLOAD_ACTIVE
	bsf	USB_STATE,IS_DFU_DNLOAD
	return
_dfu_dnload_exit
	bcf	USB_STATE,DFU_DNLOAD_ACTIVE
	goto	_cwrite			; wLength is zero: there will be no data stage, so treat like control write
_dfu_getstatus
	movlw	low DFU_STATUS_RESPONSE1
	btfsc	USB_STATE,DFU_DNLOAD_ACTIVE
	movlw	low DFU_STATUS_RESPONSE2
	movwf	EP0_DATA_IN_PTR
	movlw	6
	goto	_set_data_in_count_from_w
_dfu_getstate
	movlw	low DFU_STATE_RESPONSE
	movwf	EP0_DATA_IN_PTR
	movlw	1
	goto	_set_data_in_count_from_w
_dfu_upload
	tstf	BANKED_EP0OUT_BUF+wValueH
	bnz	_dfu_zero			; if wBlockNum is over 255, this is beyond the memory range of the device
	bsf	USB_STATE,IS_DFU_UPLOAD		; set flag to divert the transfer
_dfu_upload_already_happening
	movlw	EP0_BUF_SIZE
	goto	_set_data_in_count_from_w
_dfu_detach
_dfu_clrstatus
_dfu_abort
_dfu_zero
	bcf	USB_STATE,IS_DFU_UPLOAD
	bcf	USB_STATE,IS_DFU_DNLOAD
	movlw	0
	goto	_set_data_in_count_from_w

; Finishes a successful SETUP transaction.
_usb_ctrl_complete
	banksel	UCON
	bcf	UCON,PKTDIS		; reenable packet processing
	banksel	USB_STATE
	btfsc	USB_STATE,IS_CONTROL_WRITE
	goto	_cwrite
; this is a control read; prepare the IN endpoint for the data stage
; and the OUT endpoint for the status stage
_cread	call	ep0_read_in		; read data into IN buffer
	movlw	_DAT1|_DTSEN		; OUT buffer will be ready for status stage
; value in W is used to specify the EP0 OUT flags
_armbfs	call	arm_ep0_out_with_flags
	movlw	_DAT1|_DTSEN		; arm IN buffer
arm_ep0_in_with_flags			; W specifies STAT flags
	movwf	BANKED_EP0IN_STAT
	bsf	BANKED_EP0IN_STAT,UOWN
	return
; this is a control write: prepare the IN endpoint for the status stage
; and the OUT endpoint for the next SETUP transaction
_cwrite	bcf	BANKED_EP0IN_STAT,UOWN	; ensure we have ownership of the buffer
	clrf	BANKED_EP0IN_CNT	; we'll be sending a zero-length packet
	movlw	_DAT0|_DTSEN|_BSTALL	; make OUT buffer ready for next SETUP packet
	goto	_armbfs			; arm OUT and IN buffers



; Handles a Get Descriptor request.
; BSR=0
_usb_get_descriptor
; check descriptor type
	decf	BANKED_EP0OUT_BUF+wValueH,w
	bz	_device_descriptor	; 1=DESC_DEVICE
	decw
	bz	_config_descriptor	; 2=DESC_CONFIG
	decw
	bnz	_usb_ctrl_invalid
_string_descriptor			; 3=DESC_STRING
; only one string descriptor (serial number) is supported,
; so don't bother checking wValueL
	if HIDE_SERIAL_NUMBER
		clrw
	else
		movlw	low SERIAL_NUMBER_STRING_DESCRIPTOR
		movwf	EP0_DATA_IN_PTR
		movlw	SERIAL_NUM_DESC_LEN
	endif
_set_data_in_count_from_w
	movwf	EP0_DATA_IN_COUNT
; the count needs to be set to the minimum of the descriptor's length (in W)
; and the requested length
	subwf	BANKED_EP0OUT_BUF+wLengthL,w	; just ignore high byte...
	bc	_usb_ctrl_complete		; if W <= f, no need to adjust
	movfw	BANKED_EP0OUT_BUF+wLengthL
	movwf	EP0_DATA_IN_COUNT
	goto	_usb_ctrl_complete
_device_descriptor
	movlw	low DEVICE_DESCRIPTOR
	movwf	EP0_DATA_IN_PTR
	movlw	DEVICE_DESC_LEN
	goto	_set_data_in_count_from_w
_config_descriptor
	movlw	low CONFIGURATION_DESCRIPTOR
	movwf	EP0_DATA_IN_PTR
	movlw	CONFIG_DESC_TOTAL_LEN	; length includes all subordinate descriptors
	goto	_set_data_in_count_from_w

; Handles a Set Address request.
; The address is actually set in the IN status stage.
_usb_set_address
	bsf	USB_STATE,ADDRESS_PENDING	; address will be assigned in the status stage
	goto	_usb_ctrl_complete

; Handles a Set Configuration request.
; For now just accept any nonzero configuration.
; BSR=0
_usb_set_configuration
	bcf	USB_STATE,DEVICE_CONFIGURED	; temporarily clear flag
	tstf	BANKED_EP0OUT_BUF+wValueL	; anything other than 0 is valid
	skpz
	bsf	USB_STATE,DEVICE_CONFIGURED
	goto	_usb_ctrl_complete

; Handles a Get Configuration request.
; BSR=0
_usb_get_configuration
; load a pointer to either a 0 or a 1 in ROM
; the 0 and 1 have been chosen so that they are adjacent
	movlw	low OPPORTUNISTIC_0_CONSTANT
	btfsc	USB_STATE,DEVICE_CONFIGURED
	incw
	movwf	EP0_DATA_IN_PTR
	movlw	1
	movwf	EP0_DATA_IN_COUNT
	goto	_usb_ctrl_complete

; Handles an IN control transfer on endpoint 0.
; BSR=0
_usb_ctrl_in
	btfsc	USB_STATE,IS_CONTROL_WRITE	; is this a control read or write?
	goto	_check_for_pending_address
; fetch more data and re-arm the IN endpoint
	call	ep0_read_in
	movlw	_DTSEN
	btfss	BANKED_EP0IN_STAT,DTS	; toggle DTS
	bsf	WREG,DTS
	goto	arm_ep0_in_with_flags	; arm the IN buffer
	
; if this is the status stage of a Set Address request, assign the address here.
; The OUT buffer has already been armed for the next SETUP.
_check_for_pending_address
	btfss	USB_STATE,ADDRESS_PENDING
	return
; read the address out of the setup packed in the OUT buffer
	bcf	USB_STATE,ADDRESS_PENDING
	movfw	BANKED_EP0OUT_BUF+wValueL
	banksel	UADDR
	movwf	UADDR
	return

	if ( (DEVICE_DESC_LEN > EP0_BUF_SIZE) || (CONFIG_DESC_TOTAL_LEN > EP0_BUF_SIZE) || (SERIAL_NUM_DESC_LEN > EP0_BUF_SIZE) )
	error "descriptors must be no more than EP0_BUF_SIZE"
	endif

;;; Reads descriptor data from EP0_DATA_IN_PTR, copies it to the EP0 IN buffer,
;;; and decrements EP0_DATA_IN_COUNT.
;;; function simplied since we are assured EP0_DATA_IN_COUNT <= EP0_BUF_SIZE
;;; arguments:	BSR=0
;;; returns:	
;;; clobbers:	W, FSR0, FSR1
ep0_read_in
	bcf	BANKED_EP0IN_STAT,UOWN	; make sure we have ownership of the buffer
	clrf	BANKED_EP0IN_CNT	; initialize transmit size to 0
	btfsc	USB_STATE,IS_DFU_UPLOAD
	goto	ep0_read_dfu_in
	movfw	EP0_DATA_IN_PTR		; set up source pointer
	movwf	FSR0L
	movlw	DESCRIPTOR_ADRH|0x80
	movwf	FSR0H
	ldfsr1d	EP0IN_BUF		; set up destination pointer
; byte copy loop
_bcopy	tstf	EP0_DATA_IN_COUNT	; do nothing if there are 0 bytes to send
	retz
	moviw	FSR0++
	movwi	FSR1++
	incf	BANKED_EP0IN_CNT,f	; increase number of bytes copied
	decf	EP0_DATA_IN_COUNT,f	; decrement number of bytes remaining
	goto	_bcopy

;;; Reads wValue from SETUP in EP0 OUT buffer, converts to Physical Memory address,
;;; and writes it to PMADRL:PMADRH
;;; returns:	PMADRL, PMADRH, PMCON1
;;; clobbers:	W, BSL
set_pm_address
	; PMADRH:PMADRL = wValueL << 5
	movfw	BANKED_EP0OUT_BUF+wValueL
	banksel	PMADRL
	clrf	PMCON1
	clrf	PMADRL
	lsrf	WREG,f
	rrf	PMADRL,f
	lsrf	WREG,f
	rrf	PMADRL,f
	lsrf	WREG,f
	rrf	PMADRL,f
	movwf	PMADRH
	banksel	BANKED_EP0OUT_STAT
	return

; copy flash contents (PMDATH/PMDATL) to EP0IN_BUF (FSR1)
ep0_read_dfu_in
; BANKED_EP0IN_CNT was already cleared in ep0_read_in
	ldfsr1d	EP0IN_BUF		; set up destination pointer
	call	set_pm_address
read_flash
	clrw
_pmcopy
	sublw	EP0_BUF_SIZE		; have we filled the buffer?
	bz	_pmbail
	call	_core_flash_read
	movwi	FSR1++
	movfw	PMDATH
	movwi	FSR1++
	incf	PMADRL,f		; increment LSB of Program Memory address
	banksel	BANKED_EP0OUT_STAT
	incf	BANKED_EP0IN_CNT,f	; increase number of bytes copied by two
	incf	BANKED_EP0IN_CNT,f
	movfw	BANKED_EP0IN_CNT	; save to test on the next iteration
	goto	_pmcopy
_pmbail
ret	return

_core_flash_read
	banksel	PMADRL
	bsf	PMCON1,RD		; read word from flash
	nop				; 2 required nops
	nop
	movfw	PMDATL
	return

_crc_calc
	call	_core_flash_read
	call	_core_crc
	movfw	PMDATH
	call	_core_crc
	incf	PMADRL,f		; increment LSB of Program Memory address
	btfsc   STATUS,Z
	incf	PMADRH,f		; increment MSB of Program Memory address
	movfw	PMADRL
	andlw	b'00011111'
	bnz	_crc_calc
	return

_core_crc
	movwf	SCRATCHPAD
	movlw	8
	movwf	COUNTDOWN
_crc_loop
	lsrf	CRCH,f
	rrf	CRCL,f
	rlf	CRCL,w			; burp C into LSB of WREG
	xorwf	SCRATCHPAD,w		; XOR WREG with SCRATCHPAD (we only care about bit 0 result)
	btfss	WREG,0
	goto	_crc_no_xor
	movlw	0x23
	xorwf	CRCH,f
	movlw	0xB1
	xorwf	CRCL,f
_crc_no_xor
	lsrf	SCRATCHPAD,f
	decf	COUNTDOWN,f
	bnz	_crc_loop
	return	


;;; Main function
;;; BSR=1 (OSCCON bank)
bootloader_start
; Configure the oscillator (48MHz from INTOSC using 3x PLL)
	movlw	(1<<SPLLEN)|(1<<SPLLMULT)|(1<<IRCF3)|(1<<IRCF2)|(1<<IRCF1)|(1<<IRCF0)
	movwf	OSCCON

; Wait for the oscillator and PLL to stabilize
; NOTE: remove in a pinch? the time taken below for the CRC calculation *should* be more than the worse case here
_wosc	movlw	(1<<PLLRDY)|(1<<HFIOFR)|(1<<HFIOFS)
	andwf	OSCSTAT,w
	sublw	(1<<PLLRDY)|(1<<HFIOFR)|(1<<HFIOFS)
	bnz	_wosc

; Enable active clock tuning
	movlw	(1<<ACTEN)|(1<<ACTSRC)
	movwf	ACTCON

; calc CRC of application (and provide enough delay for the pull-up on RA3/MCLR to work)
	banksel	PMADRL
	movlw	low APP_ENTRY_POINT	; set start address of read to beginning of app
	movwf	PMADRL
	movlw	high APP_ENTRY_POINT
	movwf	PMADRH
	movlw	236			; total rows excluding bootloader and high-endurance flash
	movwf	ROW_COUNT
	clrf	CRCL			; initialize CRC value
	clrf	CRCH
app_check_loop
	call	_crc_calc
	decf	ROW_COUNT,f
	bnz	app_check_loop

; do not run application if the CRC check fails
	tstf	CRCL
	bnz	_bootloader_main
	tstf	CRCH
	bnz	_bootloader_main

; do not run application if the watchdog timed out (providing a mechanism for the app to trigger a firmware update)
	btfss	STATUS,NOT_TO
	goto	_bootloader_main

; We have a valid application? Check if the entry pin is grounded
	banksel	PORTA
	btfss	PORTA,RA3
	goto	_bootloader_main	; enter bootloader mode if input is low

; We have a valid application and the entry pin is high. Start the application.
	banksel	OPTION_REG
	bsf	OPTION_REG,NOT_WPUEN	; but first, disable weak pullups
	movlp	high APP_ENTRY_POINT	; attempt to appease certain user apps
	goto	APP_ENTRY_POINT

; Not entering application code: initialize the USB interface and wait for commands.
_bootloader_main

; Initialize USB
	call	usb_init


; Attach to the bus (could be a subroutine, but inlining it saves 2 instructions)
_usb_attach
	banksel	UCON		; reset UCON
	clrf	UCON
	banksel	UCON
_usben	bsf	UCON,USBEN	; enable USB module and wait until ready
	btfss	UCON,USBEN
	goto	_usben

bootloader_main_loop
	banksel	UIR
; reset?
	btfss	UIR,URSTIF
	goto	_utrans		; not a reset? just start servicing transactions
	call	usb_init	; if so, reset the USB interface (clears interrupts)
	banksel	UIR
	bcf	UIR,URSTIF	; clear the flag
; service transactions
_utrans	banksel	UIR
	btfss	UIR,TRNIF
	goto	_usdone
	movfw	USTAT		; stash the status in a temp register
	movwf	FSR1H
	bcf	UIR,TRNIF	; clear flag and advance USTAT fifo
	banksel	BANKED_EP0OUT_STAT
	andlw	b'01111000'	; check endpoint number
	bnz	_usdone		; bail if not endpoint 0
	call	usb_service_ep0	; handle the control message
	goto	_utrans
; clear USB interrupt
_usdone	banksel	PIR2
	bcf	PIR2,USBIF
	goto	bootloader_main_loop


;;; Initializes the USB system and resets all associated registers.
;;; arguments:	none
;;; returns:	none
;;; clobbers:	W, BSR, FSR0, FSR1H
usb_init
; clear USB registers
	banksel	UEIR
	clrf	UEIR
	clrf	UIR
	movlw	(1<<UPUEN)|(1<<FSEN)
	movwf	UCFG		; enable pullups, full speed, no ping-pong buffering
; clear all BDT entries, variables, and buffers
	clrf	FSR0L
	movlw	high BDT_START	; BDT starts at 0x2000
	movwf	FSR0H
	movlw	USED_RAM_LEN
	movwf	FSR1H		; loop count
	movlw	0
_ramclr	movwi	FSR0++
	decfsz	FSR1H,f
	goto	_ramclr
; reset ping-pong buffers and address
	banksel	UCON
	bsf	UCON,PPBRST
	clrf	UADDR
	bcf	UCON,PKTDIS	; enable packet processing
	bcf	UCON,PPBRST	; clear ping-pong buffer reset flag
; flush pending transactions
_tflush	btfss	UIR,TRNIF
	goto	_initep
	bcf	UIR,TRNIF
	call	ret		; need at least 6 cycles before checking TRNIF again
	goto	_tflush
; initialize endpoints:
; EP0 (in and out) for control
; my intuition was that I should wait until a SET_CONFIGURATION is received
; before setting up endpoints 1 and 2... but there seemed to be a timing issue
; when doing so, so I moved them here
_initep	movlw	(1<<EPHSHK)|(1<<EPOUTEN)|(1<<EPINEN)
	movwf	UEP0
; initialize endpoint buffers and counts
	banksel	BANKED_EP0OUT_ADRL
	movlw	low EP0OUT_BUF	; set endpoint 0 OUT address low
	movwf	BANKED_EP0OUT_ADRL
	movlw	low EP0IN_BUF	; set endpoint 0 IN address low
	movwf	BANKED_EP0IN_ADRL
	movlw	EPBUF_ADRH	; set all ADRH values
	movwf	BANKED_EP0OUT_ADRH
	movwf	BANKED_EP0IN_ADRH
	goto	arm_ep0_out



;;; Descriptors 

; Place all the descriptors at the end of the bootloader region.
; This serves 2 purposes: 1) as long as the total length of all descriptors is
; less than 256, we can address them with an 8-bit pointer,
; and 2) the assembler will raise an error if space is exhausted.
	org	BOOTLOADER_SIZE-ALL_DESCS_TOTAL_LEN
DESCRIPTOR_ADRH	equ	high $
DEVICE_DESCRIPTOR
	dt	DEVICE_DESC_LEN	; bLength
	dt	0x01		; bDescriptorType
	; bcdUSB (USB 1.0)
OPPORTUNISTIC_0_CONSTANT
	dt	0x00	; bcdUSB LSB
OPPORTUNISTIC_1_CONSTANT
	dt	0x01	; bcdUSB MSB
	dt	0xFE		; bDeviceClass
	dt	0x01		; bDeviceSubclass
	dt	0x00		; bDeviceProtocol
	dt	EP0_BUF_SIZE	; bMaxPacketSize0
	dt	low USB_VENDOR_ID, high USB_VENDOR_ID	; idVendor
	dt	low USB_PRODUCT_ID, high USB_PRODUCT_ID	; idProduct
	dt	0x01, 0x00	; bcdDevice (1)
	dt	0x00		; iManufacturer
	dt	0x00		; iProduct
	if HIDE_SERIAL_NUMBER
		dt	0x00		; iSerialNumber
	else
		dt	0x01		; iSerialNumber
	endif
	dt	0x01		; bNumConfigurations

CONFIGURATION_DESCRIPTOR
	dt	0x09		; bLength
	dt	0x02		; bDescriptorType
	dt	low CONFIG_DESC_TOTAL_LEN, high CONFIG_DESC_TOTAL_LEN	; wTotalLength
	dt	0x01		; bNumInterfaces
	dt	0x01		; bConfigurationValue
	dt	0x00		; iConfiguration
	dt	0x80		; bmAttributes
	dt	0x32		; bMaxPower

INTERFACE_DESCRIPTOR
	dt	0x09		; bLength
	dt	0x04		; bDescriptorType (INTERFACE)
	dt	0x00		; bInterfaceNumber
	dt	0x00		; bAlternateSetting
	dt	0x00		; bNumEndpoints
	dt	0xFE		; bInterfaceClass
	dt	0x01		; bInterfaceSubclass
	dt	0x00		; bInterfaceProtocol
	dt	0x00		; iInterface

; omit in a pinch?  at nine words (plus change to CONFIG_DESC_TOTAL_LEN), this is a rather 
; expensive way to allow the user to omit the "-t 64" argument (and warning message) for dfu_utils
FUNCTIONAL_DESCRIPTOR
	dt	0x09		; bLength
	dt	0x21		; bDescriptorType (DFU)
	dt	0x03		; bmAttributes
	dt	0x00, 0x00	; wDetachTimeout
	dt	low EP0_BUF_SIZE, high EP0_BUF_SIZE	; wTransferSize
	dt	0x00, 0x01	; bcdDFUversion

	if (OPPORTUNISTIC_0_CONSTANT>>8) != (OPPORTUNISTIC_1_CONSTANT>>8)
	error "CONSTANT_0 and CONSTANT_1 must be in the same 256-word region"
	endif

DFU_STATUS_RESPONSE1
	dt	0x00			; bStatus = OK
	dt	0x00, 0x00, 0x00	; bwPollTimeout
DFU_STATE_RESPONSE
	dt	0x02			; bState = dfuIDLE
DFU_STATUS_RESPONSE2
	dt	0x00			; iString / bStatus = OK
	dt	0x00, 0x00, 0x00	; bwPollTimeout
	dt	0x05			; bState = dfuDNLOAD-IDLE
	dt	0x00			; iString

; extract nibbles from serial number
SN1	equ	(SERIAL_NUMBER>>12) & 0xF
SN2	equ	(SERIAL_NUMBER>>8) & 0xF
SN3	equ	(SERIAL_NUMBER>>4) & 0xF
SN4	equ	SERIAL_NUMBER & 0xF

; the objective here *SHOULD* be to SQTP program globally unique values in production
; this data then doubles as a unique serial number that can be used by the user app
SERIAL_NUMBER_STRING_DESCRIPTOR
	dt	SERIAL_NUM_DESC_LEN	; bLength
	dt	0x03		; bDescriptorType (STRING)
	dt	'0'                , 0x00
	dt	'0'                , 0x00
	dt	'0'                , 0x00
	dt	'0'                , 0x00
	dt	'0'+SN1+((SN1>9)*7), 0x00	; convert hex digits to ASCII
	dt	'0'+SN2+((SN2>9)*7), 0x00
	dt	'0'+SN3+((SN3>9)*7), 0x00
	dt	'0'+SN4+((SN4>9)*7), 0x00
	
; Raise an error if the descriptors aren't properly aligned. (This means you
; changed the descriptors withouth updating the definition of ALL_DESCS_TOTAL_LEN.)
	if $!=BOOTLOADER_SIZE
	error "Descriptors must be aligned with the end of the bootloader region"
	endif

	end
