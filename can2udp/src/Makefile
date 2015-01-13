BINS := can2udp can4lnx2udp can2lan virtual_gfp get-cs-config

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g

can2udp: can2udp.o
	$(CC) $(CFLAGS) -o can2udp can2udp.o

can4lnx2udp: can4lnx2udp.o
	$(CC) $(CFLAGS) -o can4lnx2udp can4lnx2udp.o

can2lan: crc-ccitt.o gio.o can2lan.o
	$(CC) $(CFLAGS) -o can2lan crc-ccitt.o gio.o can2lan.o -lz

virtual_gfp: virtual_gfp.o
	$(CC) $(CFLAGS) -o virtual_gfp virtual_gfp.o

get-cs-config: crc-ccitt.o get-cs-config.o
	$(CC) $(CFLAGS) -o get-cs-config crc-ccitt.o get-cs-config.o -lz

clean:
	$(RM) *.o $(BINS)
