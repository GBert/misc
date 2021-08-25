BINS := susit

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -pedantic -Wextra -g

read_cs2_dump: susit.o
	$(CC) $(CFLAGS) -o susit susit.o

clean:
	$(RM) *.o $(BINS)
