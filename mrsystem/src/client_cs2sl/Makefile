TARGET=mrcs2sl
OBJS=main.o cs2sl.o can_sleth.o
LOCALLIBS=-lmr_ipc -lmr_cs2ms2 -lcs2 -lm -lbytestream -lmrconfig -linipars -lscanner -lmap -lmenge -ldliste -lavl
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -DTRACE=$(TRACE) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

main.o: main.c cs2sl.h can_sleth.h

cs2sl.o: cs2sl.c cs2sl.h

can_sleth.o: can_sleth.c can_sleth.h

install: $(TARGET)
	install -D $(TARGET) $(DESTDIR)/$(TARGET)

uninstall:
	rm $(DESTDIR)/$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)

clean_link:
	rm -f $(TARGET)

distclean:
	rm -f $(TARGET)
	rm -f $(OBJS)
