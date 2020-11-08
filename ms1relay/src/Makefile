BINS := ms1relay

BIN = $@

all: $(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g 

ms1relay: ms1relay.o
	$(CC) $(CFLAGS) -o ms1relay ms1relay.o

clean:
	$(RM) *.o $(BINS)
