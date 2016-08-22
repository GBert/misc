#
# Simple Makefile for programming Atmel AVR MCUs using avra and avrdude
#
# Assemble with 'make', flash hexfile to microcontroller with 'make flash'.
#
# Configuration:
#
# MCU     -> name of microcontroller to program (see 'avrdude -p ?' for a list)
# TARGET  -> target board/programmer to use (see 'avrdude -c ?' for a list)
# DEVICE  -> linux device file refering to the interface your programmer is plugged in to
# INCPATH -> path to the AVR include files
# SRCFILE -> single assembler file that contains the source
#
 
MCU = m16
TARGET = stk500
DEVICE = /dev/tty.usbserial
INCPATH = /usr/share/avra/includes
INCPATH = /usr/local/Cellar/avra/1.3.0/share/avra
SRCFILE = I2CSNIFF.ASM
 
$(SRCFILE).hex: $(SRCFILE)
	avra -l $(SRCFILE).lst -I $(INCPATH) $(SRCFILE)
 
flash:
	avrdude -c $(TARGET) -p $(MCU) -P $(DEVICE) -U flash:w:$(SRCFILE).hex:i
 
showfuses:
	avrdude -c $(TARGET) -p $(MCU) -P $(DEVICE) -v 2>&1 |  grep "fuse reads" | tail -n2
 
clean:
	rm -f $(SRCFILE).hex $(SRCFILE).obj $(SRCFILE).cof
