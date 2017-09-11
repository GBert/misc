##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##

PREFIX = /embedded/arm-cortex/gcc-arm-none-eabi-5_2-2015q4/bin/arm-none-eabi
OPENCM3_DIR = /embedded/arm-cortex/src/libopencm3

TTYISP = /dev/ttyUSB1

OBJS = usb_cdcacm.o usb_cdcacm-arch.o ringb.o utils.o
BINARY = main-stm32f103-generic

LDSCRIPT = STM32F103C8.ld

CFLAGS += -DSPEED62500

include mk/Makefile.include


st-flash:$(BINARY).stlink-flash

stm32flash:$(BINARY).bin
	@printf "  FLASH  $<\n"
	stm32flash -w $< -b 57600 -v -g 0 $(TTYISP)

backup:
	stm32flash -r $(BINARY)_s.bin -b 57600 $(TTYISP)

distclean:clean
	find . -name "*~" -delete
