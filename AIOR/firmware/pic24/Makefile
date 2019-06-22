#
# Copyright (C) 2014-2017 Darron Broad
# All rights reserved.
#
# This file is part of Generator.
#
# Generator is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation.
#
# Generator is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with Generator. If not, see http://www.gnu.org/licenses/
#

PLATFORM := DSPIC

ARCH	:= dsPIC33E
CPU	:= 33EP128GM604

HEAP	:= 64
STACK	:= 64

PIC30	:= /opt/pic30-tools
ifeq (,$(wildcard $(PIC30)))
$(error $(PIC30) unavailable)
endif

COMPILE	:= $(PIC30)/bin/pic30-coff-
GCC	:= gcc-4.5.1

AS	:= $(COMPILE)as
LD	:= $(COMPILE)ld
CC	:= $(COMPILE)$(GCC)
AR	:= $(COMPILE)ar
NM	:= $(COMPILE)nm
HX	:= $(COMPILE)bin2hex
STRIP	:= $(COMPILE)strip
OBJCOPY	:= $(COMPILE)objcopy
OBJDUMP	:= $(COMPILE)objdump

TARGET	:= main.hex

CSOURCE	:= main.c
COBJECT	:= $(CSOURCE:.c=.o)
CHEADER	:= $(CSOURCE:.c=.h)
CTEMPS  := $(CSOURCE:.c=.s)

COF	:= $(TARGET:.hex=.cof)
MAP	:= $(TARGET:.hex=.map)
LSCRIPT := $(TARGET:.hex=.ld)

AFLAGS	:= -p$(CPU)
CFLAGS	:= -mcpu=$(CPU) -Os -std=gnu99 -fverbose-asm -save-temps=obj -fno-short-double -merrata=exch
CLINK	:= -Wl,-T$(LSCRIPT),-Map=$(MAP),--heap=$(HEAP),--stack=$(STACK) -L$(PIC30)/non-free/lib

ifneq (,$(wildcard $(PIC30)))
build:$(TARGET)
else
build:
$(warning $(PIC30) unavailable)
endif

$(TARGET):$(COF)
	@echo -n "[HX] "
	$(HX) $(COF)

$(COF):$(LSCRIPT) $(AOBJECT) $(COBJECT)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(CPPFLAGS) $(AOBJECT) $(COBJECT) -o $(COF) $(CLINK)

$(LSCRIPT):
	@echo -n "[CPP] "
	$(CC) -E -x c $(PIC30)/non-free/support/$(ARCH)/gld/p$(CPU).gld | grep -v '^#' > $(LSCRIPT)

$(COBJECT):$(CHEADER) Makefile

$(AOBJECT):symbol.c Makefile.dspic

install:build

program:build
	@echo -n "[P24] "
	export PICKLE=$(HOME)/.pickle/config && p24 lvp program $(TARGET)

verify:build
	@echo -n "[P24] "
	export PICKLE=$(HOME)/.pickle/config && p24 lvp verify $(TARGET)

clean:
	@echo -n "[RM] "
	$(RM) -f $(CTEMPS) *.o *.i *.cof *.map *.hex *.lst *.ld UartOut.txt *~

%.o:%.S
	@echo -n "[CC] "
	$(CC) $(CFLAGS) $(AFLAGS) $(CPPFLAGS) -c $< -o $@

%.o:%.s
	@echo -n "[AS] "
	$(AS) $(AFLAGS) $< -a=$(OBJ)/$(<:.s=.lst) -o $@

%.o:%.c
	@echo -n "[CC] "
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
