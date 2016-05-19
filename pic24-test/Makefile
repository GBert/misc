#
# Copyright (C) 2005-2015 Darron Broad
# All rights reserved.
# 
# This file is part of Pickle Microchip PIC ICSP.
#
# Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation.
#
# Pickle Microchip PIC ICSP is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
#

ARCH    = dsPIC33F
CPU     = 33EP32GP502

HEAP    = 64
STACK   = 64

PIC30   = /opt/pic30-tools

COMPILE = $(PIC30)/bin/pic30-coff-
GCC     = gcc-4.5.1

AS      = $(COMPILE)as
LD      = $(COMPILE)ld
CC      = $(COMPILE)$(GCC)
AR      = $(COMPILE)ar
NM      = $(COMPILE)nm
HX      = $(COMPILE)bin2hex
STRIP   = $(COMPILE)strip
OBJCOPY = $(COMPILE)objcopy
OBJDUMP = $(COMPILE)objdump

RM      = /bin/rm

TARGET  = main.hex

CSOURCE = main.c usart.c can.c
COBJECT = $(CSOURCE:.c=.o)
CHEADER = $(CSOURCE:.c=.h)
CTEMPS  = $(CSOURCE:.c=.s)

# ASOURCE = blink.s
# AOBJECT = $(ASOURCE:.s=.o)

COF     = $(TARGET:.hex=.cof)
MAP     = $(TARGET:.hex=.map)

SIM30   = /opt/microchip/xc16/v1.21/bin/sim30
SIM     = $(TARGET:.hex=.sim)

AFLAGS  = -p$(CPU) -g
CFLAGS  = -mcpu=$(CPU) -Os -std=gnu99 -pedantic-errors -Wall -g -fno-short-double -fverbose-asm -save-temps -merrata=exch
CLINK   = -Wl,-Tp${CPU}.gld,-Map=$(MAP),--heap=$(HEAP),--stack=$(STACK) -L$(PIC30)/non-free/lib

PIHOST  = pi3

ifneq (,$(wildcard $(PIC30)))
build:$(TARGET)
else
build:
$(warning $(PIC30) unavailable)
endif

offsetof.h:offsetof.s
	@echo -n "[AWK] "
	awk '($$1 == "OFFSETOF") { print "#define " toupper($$2) " (" $$3 ")" }' offsetof.s > offsetof.h

offsetof.inc:offsetof.s
	@echo -n "[AWK] "
	awk '($$1 == "OFFSETOF") { print ".equiv      " toupper($$2) ", " $$3 }' offsetof.s > offsetof.inc

offsetof.s:offsetof.c Makefile
	@echo -n "[CC] "
	$(CC) $(CFLAGS) $(CPPFLAGS) -S offsetof.c -o offsetof.s

$(TARGET):$(COF)
	@echo -n "[HX] "
	$(HX) $(COF)

$(COF):$(AOBJECT) $(COBJECT)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(CPPFLAGS) $(AOBJECT) $(COBJECT) -o $(COF) $(CLINK)

$(COBJECT):$(CHEADER) Makefile

$(AOBJECT):Makefile

program:build
	p24 select 33EP128GP502 lvp  programm$(TARGET)

verify:build
	p24 select 33EP128GP502 lvp verify $(TARGET)

program-pi:build
	ssh ${PIHOST} p24 lvp program < $(TARGET)

verify-pi:build
	ssh ${PIHOST} p24 lvp verify < $(TARGET)

sim:build
	$(SIM30) $(SIM)

%.o:%.c
	@echo -n "[CC] "
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.o:%.S
	@echo -n "[CC] "
	$(CC) $(CFLAGS) $(AFLAGS) $(CPPFLAGS) -c $< -o $@

%.o:%.s
	@echo -n "[AS] "
	$(AS) $(AFLAGS) $< -a=$(<:.s=.lst) -o $@

install:build

uninstall:clean

clean:
	@echo -n "[RM] "
	$(RM) -f $(CTEMPS) offsetof.s offsetof.h offsetof.inc *.o *.i *.cof *.map *.hex *.lst UartOut.txt *~
