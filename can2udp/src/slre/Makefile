BINS := unit_test

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -pedantic -g

unit_test: slre.o unit_test.o
	$(CC) $(CFLAGS) -o unit_test unit_test.o slre.o

clean:
	$(RM) *.o $(BINS)
