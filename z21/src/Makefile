BINS := z21emu

BIN = $@

all: $(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g

ftdi2s88: z21emu.o
	$(CC) $(CFLAGS) -o z21emu z21emu.o

clean:
	$(RM) *.o $(BINS)
