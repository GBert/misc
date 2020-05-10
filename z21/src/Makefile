BINS := z21emu

BIN = $@

all: $(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g 

z21emu: crc-ccitt.o cs2-data-functions.o read-cs2-config.o subscribe.o utils.o z21emu.o
	$(CC) $(CFLAGS) -o z21emu crc-ccitt.o cs2-data-functions.o read-cs2-config.o subscribe.o utils.o z21emu.o -lpthread -lz

clean:
	$(RM) *.o $(BINS)
