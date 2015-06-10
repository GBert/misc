BINS := bitbang_square

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g

bitbang_square: bitbang_square.o
	$(CC) $(CFLAGS) -o bitbang_square bitbang_square.o -lftdi

clean:
	$(RM) *.o $(BINS)
