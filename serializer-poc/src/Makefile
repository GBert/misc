#
# Copyright (C) 2012 Žilvinas Valinskas
# See LICENSE for more information.
#

CFLAGS	+= -Wall -Os

all:	serializer

libmmio.o: libmmio.c
	$(CC) $(CFLAGS) -c libmmio.c -o libmmio.o

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c -o utils.o

strbuf.o : strbuf.c
	$(CC) $(CFLAGS) -c strbuf.c -o strbuf.o

mmio: mmio.o utils.o strbuf.o libmmio.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

serializer: serializer.o utils.o strbuf.o libmmio.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f *.o serializer
	rm -f tags cscope.*
