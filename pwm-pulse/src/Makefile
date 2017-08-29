BINS := pwm-pulse

BIN = $@

all:	$(BINS)

CFLAGS+=-Wall -Wextra -Wuninitialized -pedantic-errors -std=gnu99 -g

pwm-pulse: allwinner.o pwm-pulse.o
	$(CC) $(CFLAGS) -o pwm-pulse allwinner.o pwm-pulse.o

clean:
	$(RM) *.o $(BINS)
