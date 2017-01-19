BINDIR=/usr/local/bin
BINFILE=mosquitto_poll

CC=gcc
CFLAGS=-Os -std=gnu99 -pedantic-errors -Wall
CLIB=-lmosquitto

CSOURCE=mosquitto_poll.c
COBJECT=mosquitto_poll.o
CTARGET=mosquitto_poll

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
