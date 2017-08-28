BINS := pwm-pulse

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -Wextra -Wmaybe-uninitialized -pedantic-errors -std=gnu99 -g

pwm-pulse: allwinner.o pwm-pulse.o
	$(CC) $(CFLAGS) -o pwm-pulse allwinner.o pwm-pulse.o

clean:
	$(RM) *.o $(BINS) s88udp
