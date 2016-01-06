BINS := can-bounce-test can2eth can2udp can4lnx2udp gb2-update scan2lan can2lan virtual_gfp virtual_ms2 get-cs-data get-cs-config wake-up-links88

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -pedantic -Wextra -std=gnu99 -g

read_cs2_dump: read_cs2_dump.o
	$(CC) $(CFLAGS) -o read_cs2_dump read_cs2_dump.o -lpcap

can-bounce-test: can-bounce-test.o
	$(CC) $(CFLAGS) -o can-bounce-test can-bounce-test.o

can2eth: can2eth.o
	$(CC) $(CFLAGS) -o can2eth can2eth.o

can2udp: can2udp.o
	$(CC) $(CFLAGS) -o can2udp can2udp.o

can4lnx2udp: can4lnx2udp.o
	$(CC) $(CFLAGS) -o can4lnx2udp can4lnx2udp.o

gb2-update: crc-ccitt.o gb2-update.o
	$(CC) $(CFLAGS) -o gb2-update crc-ccitt.o gb2-update.o

scan2lan: crc-ccitt.o sgio.o scan2lan.o
	$(CC) $(CFLAGS) -o scan2lan crc-ccitt.o sgio.o scan2lan.o -lz

can2lan: crc-ccitt.o gio.o can2lan.o
	$(CC) $(CFLAGS) -o can2lan crc-ccitt.o gio.o can2lan.o -lz

virtual_gfp: virtual_gfp.o
	$(CC) $(CFLAGS) -o virtual_gfp virtual_gfp.o

virtual_ms2: virtual_ms2.o
	$(CC) $(CFLAGS) -o virtual_ms2 virtual_ms2.o

get-cs-data: get-cs-data.o
	$(CC) $(CFLAGS) -o get-cs-data get-cs-data.o

get-cs-config: crc-ccitt.o get-cs-config.o
	$(CC) $(CFLAGS) -o get-cs-config crc-ccitt.o get-cs-config.o -lz

wake-up-links88: wake-up-links88.o
	$(CC) $(CFLAGS) -o wake-up-links88 wake-up-links88.o

clean:
	$(RM) *.o $(BINS)
