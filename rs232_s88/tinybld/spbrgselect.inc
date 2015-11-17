;------------------------------------------------------------------------------
;  UART Baud Rate Generator (FOSC / BAUDRATE / 4 - 1)
;------------------------------------------------------------------------------

#DEFINE CLOCK	xtal
#DEFINE BAUDRATE baud

#DEFINE UBAUD   ((((CLOCK / BAUDRATE) / 2) - 1) / 2)
#IF UBAUD > 65535
	ERROR       "UART BRG overflow"
#ENDIF
#IF UBAUD == 0
	ERROR       "UART BRG zero"
#ENDIF

#DEFINE spbrg_value	UBAUD



