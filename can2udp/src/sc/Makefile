BINS := read_loccard 

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -pedantic -Wextra -g
# CFLAGS+=-g

read_loccard: read_loccard.o
	$(CC) $(CFLAGS) -o read_loccard read_loccard.o

clean:
	$(RM) *.o $(BINS)
