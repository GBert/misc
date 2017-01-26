BINS := mqtt_s_serial mqtt_master mqtt_gpio

BINDIR=/usr/local/bin

CC=gcc
CFLAGS=-Wall -pedantic-errors -std=gnu99 -Os
CLIB=-lmosquitto

BIN = $@

all:	$(BINS)

mqtt_s_serial: mqtt_s_serial.o
	$(CC) $(CFLAGS) -o mqtt_s_serial mqtt_s_serial.o $(CLIB)

mqtt_master: mqtt_master.o
	$(CC) $(CFLAGS) -o mqtt_master mqtt_master.o $(CLIB)

mqtt_gpio: gpio.o mqtt_gpio.o
	$(CC) $(CFLAGS) -o mqtt_gpio gpio.o mqtt_gpio.o $(CLIB)


clean:
	rm -f *.o *~ $(BINS)
