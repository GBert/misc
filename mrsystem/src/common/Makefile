OBJS=cs2eth.o logms2.o ms2.o

%.o: %.c
	$(CC) $(CFLAGS) -DTRACE=$(TRACE) -I$(INCLUDE_PATH) -c $<

all: $(OBJS)

.PHONY: cs2eth

cs2eth: cs2eth.o

cs2eth.o: cs2eth.c cs2eth.h

.PHONY: logms2

logms2: logms2.o

logms2.o: logms2.c logms2.h

.PHONY: ms2

ms2: ms2.o

ms2.o: ms2.c  ms2.h

install: $(TARGET)

uninstall:

clean:
	rm -f $(OBJS)

clean_link:

distclean:
	rm -f $(OBJS)
