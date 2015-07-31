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

# build targets and backend configuration
include config.mk

#FreeBSD IBMPC: FreeBSD 9.0-RELEASE   amd64
#Debian  IBMPC: Linux   3.2.0-4-amd64 x86_64
#Debian  R-PI:  Linux   3.2.27+       armv6l
SYSTEM:=$(shell uname -s)
RELEASE:=$(shell uname -r)
PLATFORM:=$(shell uname -m)

ifeq "$(SYSTEM)" "FreeBSD"
 ifneq (,$(findstring 10.,$(RELEASE)))
  CC=cc
 else
  CC=gcc
 endif
else
 CC=gcc
endif

CFLAGS=-Os -std=gnu99 -pedantic-errors -I. -Wall -fverbose-asm -save-temps
ifeq "$(GDB)" "1"
CFLAGS+=-ggdb
endif

ifeq "$(VERBOSE)" "1"
CFLAGS+=-DVERBOSE
endif

ifneq (,$(findstring CYGWIN,$(SYSTEM)))
CFLAGS+=-D__USE_LINUX_IOCTL_DEFS
endif

BINDIR=/usr/local/bin

CLIB1=$(CLIB)
TARGET1=pickle
SOURCE1=pickle.c dotconf.c inhx32.c io.c pic.c util.c

CLIB2=$(CLIB)
TARGET2=pio
SOURCE2=pio.c dotconf.c io.c util.c

ifeq "$(DRYRUN)" "1"
CFLAGS+=-DDRYRUN
endif

ifeq "$(P12)" "1"
CFLAGS+=-DP12
SOURCE1+=pic12.c
endif

ifeq "$(P14)" "1"
CFLAGS+=-DP14
SOURCE1+=pic14.c
endif

ifeq "$(P16)" "1"
CFLAGS+=-DP16
SOURCE1+=pic16.c
endif

ifeq "$(P24)" "1"
CFLAGS+=-DP24
SOURCE1+=pic24.c
endif

ifeq "$(P32)" "1"
CFLAGS+=-DP32
SOURCE1+=pic32.c
endif

ifeq "$(PCTRL)" "1"
CFLAGS+=-DPCTRL
endif

ifeq "$(PTEST)" "1"
CFLAGS+=-DPTEST
endif

ifeq "$(PIO)" "1"
CFLAGS+=-DPIO
endif

ifeq "$(TTY)" "1"
CFLAGS+=-DTTY
SOURCE1+=serial_posix.c stk500v2.c stk500v2_load.c stk500v2_listen.c ip.c rw.c
SOURCE2+=serial_posix.c
endif

ifeq "$(RPI)" "1"
 ifeq "$(PLATFORM)" "armv6l"
 CFLAGS+=-DRPI
 SOURCE1+=raspi.c
 SOURCE2+=raspi.c
 endif
endif

ifeq "$(RPI)" "1"
 ifeq "$(PLATFORM)" "armv7l"
 CFLAGS+=-DRPI
 SOURCE1+=raspi.c
 SOURCE2+=raspi.c
 endif
endif

ifeq "$(SYSTEM)" "Linux"
 ifeq "$(MCP23017)" "1"
 CFLAGS+=-DMCP23017
 SOURCE1+=mcp23017.c
 SOURCE2+=mcp23017.c
 endif

 ifeq "$(BITBANG)" "1"
 CFLAGS+=-DBITBANG
 SOURCE1+=gpio-bb.c
 SOURCE2+=gpio-bb.c
 endif

 ifeq "$(FTDI)" "1"
 CFLAGS+=-DFTDI
 SOURCE1+=ftdi-bb.c
 SOURCE2+=ftdi-bb.c
 CLIB+=-lftdi
 endif
endif

ifeq "$(QSORT)" "1"
CFLAGS+=-DQSORT
endif

OBJECT1=$(SOURCE1:.c=.o)
HEADER1=$(SOURCE1:.c=.h)

OBJECT2=$(SOURCE2:.c=.o)
HEADER2=$(SOURCE2:.c=.h)

#
# COMMAND LINE TOOL(S)
#
TARGETS:=$(TARGET1)
ifeq "$(PIO)" "1"
TARGETS:=$(TARGETS) $(TARGET2)
endif
build:$(TARGETS)

$(TARGET1):$(OBJECT1)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT1) -o $(TARGET1) $(CLIB1) -Wl,-map,$(TARGET1).map
ifneq "$(GDB)" "1"
	@echo -n "[STRIP] "
	strip $(TARGET1)
endif

$(OBJECT1):$(HEADER1) Makefile config.mk VERSION

$(TARGET2):$(OBJECT2)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT2) -o $(TARGET2) $(CLIB2) -Wl,-map,$(TARGET2).map
ifneq "$(GDB)" "1"
	@echo -n "[STRIP] "
	strip $(TARGET2)
endif

$(OBJECT2):$(HEADER2) Makefile config.mk VERSION pickle.h

%.o:%.c
	@echo -n "[CC] "
	$(CC) $(CFLAGS) -c $< -o $@

install:build
	mkdir -p $(BINDIR)
	cp $(TARGET1) $(BINDIR)/$(TARGET1)
ifeq "$(P12)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p12
endif
ifeq "$(P14)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p14
endif
ifeq "$(P16)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p16
endif
ifeq "$(P24)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p24
endif
ifeq "$(P32)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p32
endif
ifeq "$(PCTRL)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/pctrl
endif
ifeq "$(TTY)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/pload
endif
ifeq "$(PTEST)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/ptest
endif
ifeq "$(PIO)" "1"
	cp $(TARGET2) $(BINDIR)/$(TARGET2)
endif
ifeq (,$(findstring CYGWIN,$(SYSTEM)))
	chown 0:0 $(BINDIR)/$(TARGET1)
	chmod u+s $(BINDIR)/$(TARGET1)
 ifeq "$(PIO)" "1"
	chown 0:0 $(BINDIR)/$(TARGET2)
	chmod u+s $(BINDIR)/$(TARGET2)
 endif
endif

uninstall:
	rm -f $(BINDIR)/pickle
	rm -f $(BINDIR)/p12
	rm -f $(BINDIR)/p14
	rm -f $(BINDIR)/p16
	rm -f $(BINDIR)/p24
	rm -f $(BINDIR)/p32
	rm -f $(BINDIR)/pctrl
	rm -f $(BINDIR)/ptest
	rm -f $(BINDIR)/pload
	rm -f $(BINDIR)/pio

clean:
	cd dotconf && $(MAKE) clean
	rm -f pickle pio *.o *~ .*~ *.err *.log *.map *.s *.i
