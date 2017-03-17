BINS := z21emu

BIN = $@

all: $(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g

z21emu: utils.o z21emu.o
	$(CC) $(CFLAGS) -o z21emu utils.o z21emu.o -lpthread

clean:
	$(RM) *.o $(BINS)
