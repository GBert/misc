BINS := rs485-test
       
BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -Wextra -Wmaybe-uninitialized -pedantic-errors -std=gnu99 -g

rs485-test: rs485-test.o
	$(CC) $(CFLAGS) -o rs485-test rs485-test.o -lftdi1

clean:
	$(RM) *.o $(BINS)
