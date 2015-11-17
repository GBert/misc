AS:=gpasm -a inhx32 -e on

ASOURCE:=$(wildcard *.asm)
AOBJECT:=$(ASOURCE:.asm=.hex)

build:$(AOBJECT)

$(AOBJECT): Makefile

%.hex:%.asm
	@echo -n "[AS] "
	$(AS) $< -o $@

install:build

uninstall:clean

clean:
	rm -f *.lst *.hex *.cod *.log *.o *~
