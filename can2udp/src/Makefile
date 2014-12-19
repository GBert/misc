BINS := can2udp can2lan virtual_gfp

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99

can2udp: can2udp.o
	$(CC) $(CFLAGS) -o can2udp can2udp.o

can2lan: crc-ccitt.o gio.o can2lan.o
	$(CC) $(CFLAGS) -o can2lan crc-ccitt.o gio.o can2lan.o -lz

virtual_gfp: virtual_gfp.o
	$(CC) $(CFLAGS) -o virtual_gfp virtual_gfp.o

clean:
	$(RM) *.o $(BINS)
