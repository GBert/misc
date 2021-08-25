BINS := modify-scan-interval

PREFIX=/usr/sbin

BIN = $@

all:	$(BINS)

CFLAGS+=-g -O2 -fstack-protector-strong -Wformat -Werror=format-security -Wall -Wextra -Wmaybe-uninitialized -std=gnu99 -pedantic-errors 

modify-scan-interval: modify-scan-interval.o
	$(CC) $(CFLAGS) -o modify-scan-interval modify-scan-interval.o

clean:
	$(RM) *.o $(BINS)

