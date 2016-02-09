BINS := can-bounce-test can2eth can2udp can4lnx2udp clone-cs2-config gb2-update ir-tester bpi-rs485 \
        scan2lan can2lan virtual_gfp virtual_links88 virtual_ms2 get-cs-data get-cs-config s88udp-bpi wake-up-links88

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -pedantic -Wextra -g

read_cs2_dump: read_cs2_dump.o
	$(CC) $(CFLAGS) -o read_cs2_dump read_cs2_dump.o -lpcap

bpi-rs485: bpi-rs485.o
	$(CC) $(CFLAGS) -o bpi-rs485 bpi-rs485.o

can-bounce-test: can-bounce-test.o
	$(CC) $(CFLAGS) -o can-bounce-test can-bounce-test.o

can2eth: can2eth.o
	$(CC) $(CFLAGS) -o can2eth can2eth.o

can2udp: can2udp.o
	$(CC) $(CFLAGS) -o can2udp can2udp.o

can4lnx2udp: can4lnx2udp.o
	$(CC) $(CFLAGS) -o can4lnx2udp can4lnx2udp.o

clone-cs2-config: crc-ccitt.o clone-cs2-config.o
	$(CC) $(CFLAGS) -o clone-cs2-config crc-ccitt.o clone-cs2-config.o -lz

gb2-update: crc-ccitt.o gb2-update.o
	$(CC) $(CFLAGS) -o gb2-update crc-ccitt.o gb2-update.o

scan2lan: crc-ccitt.o sgio.o scan2lan.o
	$(CC) $(CFLAGS) -o scan2lan crc-ccitt.o sgio.o scan2lan.o -lz

can2lan: crc-ccitt.o gio.o can2lan.o
	$(CC) $(CFLAGS) -o can2lan crc-ccitt.o gio.o can2lan.o -lz

virtual_gfp: virtual_gfp.o
	$(CC) $(CFLAGS) -o virtual_gfp virtual_gfp.o

virtual_links88: virtual_links88.o
	$(CC) $(CFLAGS) -o virtual_links88 virtual_links88.o

virtual_ms2: virtual_ms2.o
	$(CC) $(CFLAGS) -o virtual_ms2 virtual_ms2.o

get-cs-data: get-cs-data.o
	$(CC) $(CFLAGS) -o get-cs-data get-cs-data.o

ir-tester: ir-tester.o
	$(CC) $(CFLAGS) -o ir-tester ir-tester.o

get-cs-config: crc-ccitt.o get-cs-config.o
	$(CC) $(CFLAGS) -o get-cs-config crc-ccitt.o get-cs-config.o -lz

s88udp-bpi: a20.o s88udp-bpi.o
	$(CC) $(CFLAGS) -o s88udp a20.o s88udp-bpi.o

wake-up-links88: wake-up-links88.o
	$(CC) $(CFLAGS) -o wake-up-links88 wake-up-links88.o

clean:
	$(RM) *.o $(BINS) s88udp
