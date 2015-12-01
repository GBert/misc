BINS := ftdi2s88 debounce_test

BIN = $@

all: $(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g

debounce_test: debounce_test.o
	$(CC) $(CFLAGS) -o debounce_test debounce_test.o

ftdi2s88: ftdi2s88.o
	$(CC) $(CFLAGS) -o ftdi2s88 ftdi2s88.o -lftdi

clean:
	$(RM) *.o $(BINS)
