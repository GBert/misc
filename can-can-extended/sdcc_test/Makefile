SYSTEM:=$(shell uname -s)

MCU=18f25k80
CC=sdcc
CFLAGS=--use-non-free -mpic16 -p$(MCU) #-DDEBUG=1
CLIB=-llibc18f.lib

MAIN=main
TARGET=main.hex
SOURCE=main.c
OBJECT=$(SOURCE:.c=.o)
HEADER=$(SOURCE:.c=.h) prototypes.h

BOOTLOADER	= pirate-loader
TERMINAL	= com

build:$(TARGET)

$(TARGET):$(OBJECT)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT) -o $(TARGET) $(CLIB) -m$(MCU)

$(OBJECT):$(HEADER) Makefile

install:build

uninstall:clean

clean:
ifneq (,$(findstring Linux,$(SYSTEM)))
	find . -regextype posix-extended -regex ".*\.(o|asm|cod|err|hex|log|lst)" -delete
else
	find -E . -regex ".*\.(o|asm|cod|err|hex|log|lst)" -delete
endif

%.o:%.c
	@echo -n "[CC]   "
	$(CC) $(CFLAGS) -c $< -o $@

flash:
	$(BOOTLOADER) --reset_rts --dev=/dev/ttyUSB0 --hex=$(MAIN).hex

com:
	$(TERMINAL) /dev/ttyUSB0 500000

terminal:
	$(TERMINAL) /dev/ttyUSB0 500000
