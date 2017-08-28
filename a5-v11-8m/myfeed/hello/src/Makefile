BINS := hello

BIN = $@

all: $(BINS)

CFLAGS+=-Wall -pedantic -std=gnu99 -g

hello: hello.o
	$(CC) $(CFLAGS) -o hello hello.o

clean:
	$(RM) *.o $(BINS)
