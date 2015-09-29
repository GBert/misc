#------------------------------------------------------------------------------
#
#   The Wellington Boot Loader for PIC18
#
#   Copyright (c) 2014 - 2015 Darron M Broad
#
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
#   This file is part of The Wellington Boot Loader.
#
#   The Wellington Boot Loader is free software: you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as published
#   by the Free Software Foundation.
#
#   The Wellington Boot Loader is distributed in the hope that it will be
#   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with The Wellington Boot Loader. If not, see http://www.gnu.org/licenses/
#------------------------------------------------------------------------------

GPASM:=$(shell gpasm -l 2>/dev/null)
ifeq ($(strip $(GPASM)),)
$(error gputils unavailable)
endif

AS:=gpasm -a inhx32 -e on -I ../include/

ASOURCE:=pic18f26k80-LED.asm proton-mkII-LED.asm proton-mkII-CAN.asm
AHEADER:=
AOBJECT:=$(ASOURCE:.asm=.hex)

build:$(AOBJECT)

$(AOBJECT):$(AHEADER) Makefile

%.hex:%.asm
	@echo -n "[AS] "
	$(AS) $< -o $@

install:build

uninstall:clean

clean:
	rm -f *.lst *.hex *.cod *.log *.o *~
