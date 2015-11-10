# Mac OS X Makefile for PIC programming using GPASM
# Copyright (c) 2015, Matt Sarnoff (msarnoff.org)
# v1.0, February 12, 2015
# Released under a 3-clause BSD license: see the accompanying LICENSE file.
#
# Run `make` to build the project as a .hex file.
# Run `make flash` to program the device.
#
# MPLAB X is required if using a PICkit 3 to program the device.
# This Makefile assumes it's installed in /Applications/microchip.

########## Project-specific definitions ##########

# Project name
OUT = bootloader

# Source files to assemble
ASM = bootloader.asm

# USB serial number for the device
SERIAL_NUMBER=1

# (use `make list-devices` if not known)
AS_DEVICE = p16f1454

# The MDB-specific part number of the chip, used for programming with MDB
# (should be the actual PIC part number, e.g. PIC16LF1454)
MDB_DEVICE = PIC16F1454



########## Build settings ##########

AS = gpasm
DASM = gpdasm

########## Make rules ##########

HEX = $(OUT).hex

# Link
$(HEX): $(ASM)
	$(AS) -p $(AS_DEVICE) -DSERIAL_NUMBER=$(SERIAL_NUMBER) -o $(HEX) $(ASM)

# Disassemble
dis: $(HEX)
	$(DASM) -p p$(AS_DEVICE) $(HEX)

# List supported device types
list-devices:
	@$(AS) -l

# Clean
clean:
	rm -f $(ASM:.asm=.lst) $(HEX) $(OUT).cod $(OUT).lst

.PHONY: all flash clean list-devices
