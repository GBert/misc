BINS := pwm-pulse

CFLAGS+=-Wall -Wextra -Wmaybe-uninitialized -pedantic-errors -std=gnu99 -g

BIN = $@

%.o : %.c
	$(CC) $(CFLAGS) -o $@

all:	$(BINS)

clean:
	$(RM) $(BINS)

