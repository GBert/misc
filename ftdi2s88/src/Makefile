BINS := ftdi2s88

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g

ftdi2s88: ftdi2s88.o
	$(CC) $(CFLAGS) -o ftdi2s88 ftdi2s88.o -lftdi

clean:
	$(RM) *.o $(BINS)
