#-------------------------------------------------------------------------------
#
#   The Cowboy Boot Loader for Enhanced PIC12/PIC16
#
#   Copyright (c) 2017 Darron M Broad
#
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
#   This file is part of The Cowboy Boot Loader.
#
#   The Cowboy Boot Loader is free software: you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as published
#   by the Free Software Foundation.
#
#   The Cowboy Boot Loader is distributed in the hope that it will be
#   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with The Cowboy Boot Loader. If not, see http://www.gnu.org/licenses/
#-------------------------------------------------------------------------------

AS:=gpasm -a inhx32 -e on

ASOURCE:=$(filter-out template.asm, $(wildcard *.asm))
AHEADER:=$(wildcard ../loader/*.inc)
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
