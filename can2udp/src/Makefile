BINS := can2udp can2lan virtual_gfp

BIN = $@

%.o : %.c
	$(CC) $(CFLAGS) -o $@

all:	$(BINS)

clean:
	$(RM) $(BINS)

