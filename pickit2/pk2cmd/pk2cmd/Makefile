#
# Makefile for pk2cmd for Linux/OSX
#
# To build for Linux, uncomment one of the CFLAGS options, then type 'make'.
# To build on OS/X, select the appropriate CFLAGS and type 'make osx' or 'make osxhid'.
#
# You MUST select a CFLAGS below in order to compile.
#
# Alternatively you can select a CFLAGS by specifying the target architecture
# on the command line, such as:
#    make linux
# or
#    make mac
#

APP=pk2cmd
CC=g++
# Uncomment this for debugging with gdb
#DBG=-ggdb -O0
# Uncomment this for no debugging (default)
DBG=-O2
OPTS=-Wall -D_GNU_SOURCE $(DBG)
OBJS=cmd_app.o DeviceData.o DeviceFile.o ImportExportHex.o PICkitFunctions.o PIC32PE.o Pk2BootLoader.o pk2cmd.o stdafx.o pk2usbcommon.o pk2usb.o P24F_PE.o dsP33_PE.o strnatcmp.o

# Grab the version of libusb in /usr/local if possible:
LIBUSB=/usr/local
LDFLAGS=-L$(LIBUSB)/lib -lusb

# CHOOSE A TARGET BEFORE COMPILING
#
# linux = Linux kernel 2.6 or greater
# linux-old = Linux kernel 2.4
# mac104 = Mac OSX version 10.4
# mac105 = Mac OSX version 10.5
# freebsd = FreeBSD (see Note in ReadmeMakefile.txt)
#
#TARGET=linux
#TARGET=linux-old
#TARGET=freebsd
#TARGET=mac104
#TARGET=mac105

# ----- This may be necessary for OSX fink
#LIBUSB=/sw

ifeq ($(TARGET),linux)
CFLAGS=$(OPTS) -I$(LIBUSB)/include -DLINUX -DUSE_DETACH -DCLAIM_USB
endif
ifeq ($(TARGET),linux-old)
CFLAGS=$(OPTS) -I$(LIBUSB)/include -DLINUX
endif
ifeq ($(TARGET),freebsd)
CFLAGS=$(OPTS) -I$(LIBUSB)/include -DCLAIM_USB
endif
ifeq ($(TARGET),mac104)
CFLAGS=$(OPTS) -I$(LIBUSB)/include -DDARWIN -DMACOSX_HID
OBJS=cmd_app.o DeviceData.o DeviceFile.o ImportExportHex.o PICkitFunctions.o PIC32PE.o Pk2BootLoader.o pk2cmd.o stdafx.o pk2usbcommon.o pk2usbmacosx.o P24F_PE.o dsP33_PE.o strnatcmp.o
LDFLAGS=-framework IOKit -framework CoreFoundation
endif
ifeq ($(TARGET),mac105)
CFLAGS=$(OPTS) -I$(LIBUSB)/include -DDARWIN -DMACOSX_HID -DMACOSX105
OBJS=cmd_app.o DeviceData.o DeviceFile.o ImportExportHex.o PICkitFunctions.o PIC32PE.o Pk2BootLoader.o pk2cmd.o stdafx.o pk2usbcommon.o pk2usbmacosx.o P24F_PE.o dsP33_PE.o strnatcmp.o
LDFLAGS=-framework IOKit -framework CoreFoundation
endif

.PHONY: target

all: target $(APP)
#	@echo $(HOME)

$(APP): target $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) 

cmd_app.o: cmd_app.cpp cmd_app.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

DeviceData.o: DeviceData.cpp DeviceData.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

DeviceFile.o: DeviceFile.cpp DeviceFile.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

ImportExportHex.o: ImportExportHex.cpp ImportExportHex.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

PICkitFunctions.o: PICkitFunctions.cpp PICkitFunctions.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

PIC32PE.o: PIC32PE.cpp PIC32PE.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

Pk2BootLoader.o: Pk2BootLoader.cpp Pk2BootLoader.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

pk2cmd.o: pk2cmd.cpp cmd_app.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

stdafx.o: stdafx.cpp stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

pk2usbcommon.o: pk2usbcommon.cpp pk2usb.h
	$(CC) $(CFLAGS) -o $@  -c $<

pk2usb.o: pk2usb.cpp pk2usb.h
	$(CC) $(CFLAGS) -o $@  -c $<

pk2usbmacosx.o: pk2usbmacosx.cpp pk2usbmacosx.h pk2usb.h
	$(CC) $(CFLAGS) -o $@  -c $<

P24F_PE.o: P24F_PE.cpp P24F_PE.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

dsP33_PE.o: dsP33_PE.cpp dsP33_PE.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

strnatcmp.o: strnatcmp.c strnatcmp.h stdafx.h
	$(CC) $(CFLAGS) -x c -o $@  -c $<

install: 
	mkdir -p /usr/share/pk2
	cp $(APP) /usr/local/bin
	chmod u+s /usr/local/bin/$(APP)
	cp PK2DeviceFile.dat /usr/share/pk2/PK2DeviceFile.dat

clean:
	rm -f *.o
	rm -f $(APP)

# Specific platforms:

linux:		# build for Linux kernel 2.6
	$(MAKE) TARGET=linux

linux-old:	# build for Linux kernel 2.4
	$(MAKE) TARGET=linux-old

freebsd:	# build for freeBSD
	$(MAKE) TARGET=freebsd

mac104:		# build for Mac OSX 10.4
	$(MAKE) TARGET=mac104

mac105:		# build for Mac OSX 10.5
	$(MAKE) TARGET=mac105

# This must be last in the Makefile, or ugly things might happen
target:
ifeq ($(TARGET),)
	$(error You must select a TARGET in Makefile or specify the target on the command line, for example: make linux)
endif

