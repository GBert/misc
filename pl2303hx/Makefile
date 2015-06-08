BINS := pl2303hx pl2303hx_org

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g

pl2303hx: pl2303hx.o
	$(CC) $(CFLAGS) -o pl2303hx pl2303hx.o -lm

pl2303hx_org: pl2303hx_org.o
	$(CC) $(CFLAGS) -o pl2303hx_org pl2303hx_org.o -lm

clean:
	$(RM) *.o $(BINS)
