BINS := loconetd lococlient

BIN = $@

all: $(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g

loconetd: loconetd.o
	$(CC) $(CFLAGS) -o loconetd loconetd.o

lococlient: lococlient.o
	$(CC) $(CFLAGS) -o lococlient lococlient.o

clean:
	$(RM) *.o $(BINS)
