<<<<<<< HEAD
BINS := ftdi2s88 debounce_test
=======
BINS := ftdi2s88
>>>>>>> 3767e96a75a4733ad47409fcb315803d012e7f18

BIN = $@

all: $(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g

<<<<<<< HEAD
debounce_test: debounce_test.o
	$(CC) $(CFLAGS) -o debounce_test debounce_test.o

ftdi2s88: ftdi2s88.o
	$(CC) $(CFLAGS) -o ftdi2s88 ftdi2s88.o -lftdi
=======
ftdi2s88: ftdi2s88.o
	$(CC) $(CFLAGS) -o ftdi2s88 ftdi2s88.o -lftdi1
>>>>>>> 3767e96a75a4733ad47409fcb315803d012e7f18

clean:
	$(RM) *.o $(BINS)
