BINS := z21emu

BIN = $@

all: $(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g 

z21emu: read-cs2-config.o utils.o z21emu.o
	$(CC) $(CFLAGS) -o z21emu utils.o read-cs2-config.o z21emu.o -lpthread

clean:
	$(RM) *.o $(BINS)
