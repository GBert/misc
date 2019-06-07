GPASM:=$(shell gpasm -l 2>/dev/null)
ifeq ($(strip $(GPASM)),)
$(error gputils unavailable)
endif

AS:=gpasm -a inhx32 -e on 

ASOURCE:=$(wildcard *.asm)
AOBJECT:=$(ASOURCE:.asm=.hex)

MCU=10F322
TARGET=rs485f.hex
build:$(AOBJECT)

$(AOBJECT):$(AHEADER) Makefile

%.hex:%.asm
	@echo -n "[AS] "
	$(AS) $< -o $@

install:build

uninstall:clean

upload:build
	pk2cmd -B/usr/share/pk2/ -PPIC$(MCU) -F$(TARGET) -M -R


clean:
	rm -f *.lst *.hex *.cod *.log *.o *~
