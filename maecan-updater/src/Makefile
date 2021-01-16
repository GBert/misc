BINS := maecan-updater

BIN = $@

all: $(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g 

ms1relay: maecan-updater.0
	$(CC) $(CFLAGS) -o maecan-updater maecan-updater.o

clean:
	$(RM) *.o $(BINS)
