BINS := maecanserver

BIN = $@

all: $(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g

maecanserver: maecan_strings.o maecanserver.o
	$(CC) $(CFLAGS) -o maecanserver maecan_strings.o maecanserver.o -lm -lpthread -lwebsockets

clean:
	$(RM) *.o $(BINS)
