BINS := unit_test read-cs2-config read-cs2-config_s

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -pedantic -Wextra -g

unit_test: slre.o unit_test.o
	$(CC) $(CFLAGS) -o unit_test unit_test.o slre.o

read-cs2-config: slre.o read-cs2-config.o
	$(CC) $(CFLAGS) -o read-cs2-config read-cs2-config.o slre.o

read-cs2-config_s: read-cs2-config_s.o
	$(CC) $(CFLAGS) -o read-cs2-config_s read-cs2-config_s.o

clean:
	$(RM) *.o $(BINS)
