;------------------------------------------------------------------------------
; 
; CANBuster CAN inspect
;
; Copyright (c) 2016 Gerhard Bertelsmann
;
;------------------------------------------------------------------------------
;
; Setting Defaults for S88
;   using IDLOCS with gpasm and PIC18F2.K80 isn't possible as of today
;
;------------------------------------------------------------------------------

; Location ID
; 0x200000      UID High
; 0x200001      UID Low
; 0x200002      Start counting at X-High for event
; 0x200003      Start counting at X-Low for event
; 0x200004      Start counting at X-High for event - S88
; 0x200005      Start counting at X-Low for event - S88
; 0x200006      Scan Interval (in 512us steps)
; 0x200007      S88 Bus length


		ERRORLEVEL  -220
		ORG	_IDLOC0
		DB	0x55, 0x53, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01
		ERRORLEVEL  +220


