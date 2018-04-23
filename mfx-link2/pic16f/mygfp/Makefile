#------------------------------------------------------------------------------
#
# My GFP Makefile
#
# Copyright (c) 2018 Grhard Brtelsmann
#
#------------------------------------------------------------------------------

PROCESSOR	= 16F1709

GPASM:=$(shell gpasm -l 2>/dev/null)
ifeq ($(strip $(GPASM)),)
$(error gputils unavailable)
endif

AS:=gpasm -a inhx32 -e on

ASOURCE:=$(filter-out template.asm, $(wildcard *.asm))
AHEADER:=$(wildcard ../include/*.inc)
AOBJECT:=$(ASOURCE:.asm=.hex)

build:$(AOBJECT)

$(AOBJECT):$(AHEADER) Makefile

all: mygfp.hex

%.hex:%.asm
	@echo -n "[AS] "
	$(AS) $< -o $@

flash: all
	pk2cmd -B/usr/share/pickit2 -PPIC$(PROCESSOR) -Fmygfp.hex -M -R -T

upload:build
	easy-loader -v /dev/ttyUSBx $(AOBJECT)

install:build

uninstall:clean

clean:
	rm -f *.lst *.hex *.cod *.log *.o *~
