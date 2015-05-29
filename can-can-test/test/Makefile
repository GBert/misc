#------------------------------------------------------------------------------
#
# Can-Can Makefile
#
# Copyright (c) 2014 Darron M Broad
#
#------------------------------------------------------------------------------
#
# This file is part of the Can-Can CAN bus interface project.
#
# Can-Can is licensed under the CC BY-NC-SA 4.0.
#
# See file /LICENSE for details.
# 
#------------------------------------------------------------------------------

BINDIR=/usr/local/bin
BINFILE=can-can

CC=gcc
CFLAGS=-Os -std=gnu99 -pedantic-errors -Wall -ggdb
CLIB=-lm

CSOURCE=can-can.c
COBJECT=can-can.o
CTARGET=can-can

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
