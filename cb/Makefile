BINS := compare ixia-trailer read_reuters_dump

BIN = $@

all: $(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g

compare: compare.o
	$(CC) $(CFLAGS) -o compare compare.o -lpcap

ixia-trailer: ixia-trailer.o
	$(CC) $(CFLAGS) -o ixia-trailer ixia-trailer.o -lpcap

read_reuters_dump: read_reuters_dump.o
	$(CC) $(CFLAGS) -o read_reuters_dump read_reuters_dump.o -lpcap

clean:
	$(RM) *.o $(BINS)
