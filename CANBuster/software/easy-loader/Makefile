#------------------------------------------------------------------------------
#
#   Take it Easy Loader for The Wellington Boot Loader for PIC18
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

BINDIR=/usr/local/bin
BINFILE=easy-loader

CC=gcc
CFLAGS=-Os -std=gnu99 -pedantic-errors -Wall
CLIB=

CSOURCE=easy-loader.c
COBJECT=easy-loader.o
CTARGET=easy-loader

build:$(COBJECT) $(CTARGET)

$(CTARGET):$(COBJECT)
	$(CC) $(COBJECT) -o $(CTARGET) $(CLIB)

$(COBJECT):$(CHEADER) Makefile

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

install:build
	mkdir -p $(BINDIR)
	cp $(CTARGET) $(BINDIR)/$(BINFILE)

uninstall:clean

clean:
	rm -f *.o *~ $(CTARGET)
