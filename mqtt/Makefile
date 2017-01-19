BINDIR=/usr/local/bin
BINFILE=mqtt_s_serial

CC=gcc
CFLAGS=-Os -std=gnu99 -pedantic-errors -Wall
CLIB=-lmosquitto

CSOURCE=mqtt_s_serial.c
COBJECT=mqtt_s_serial.o
CTARGET=mqtt_s_serial

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
