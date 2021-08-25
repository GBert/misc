#------------------------------------------------------------------------------
#
# Can-Can Makefile
#
# Copyright (c) 2014 Darron M Broad
#
#------------------------------------------------------------------------------
#
# This file is part of the Can-Can CAN bus interface project.
#
# Can-Can is licensed under the CC BY-NC-SA 4.0.
#
# See file /LICENSE for details.
# 
#------------------------------------------------------------------------------

GPASM:=$(shell gpasm -l 2>/dev/null)
ifeq ($(strip $(GPASM)),)
$(error gputils unavailable)
endif

AS:=gpasm -f -a inhx32 -e on -I ../include/
LN:=gplink --map -c -s $(SCRIPT) -o $(OUTPUT) $(OBJECTS)

ASOURCE:=$(filter-out template.asm, $(wildcard *.asm))
AHEADER:=$(wildcard ../include/*.inc)
AOBJECT:=$(ASOURCE:.asm=.hex)

MCU=18F45K80
TARGET=canbuster.hex
build:$(AOBJECT)

$(AOBJECT):$(AHEADER) Makefile

%.hex:%.asm
	@echo -n "[AS] "
	$(AS) $< -o $@
#	$(LN) $< -o $@

install:build

uninstall:clean

upload:build
	pk2cmd -B/usr/share/pk2/ -PPIC$(MCU) -F$(TARGET) -M -R


clean:
	rm -f *.lst *.hex *.cod *.log *.o *~
