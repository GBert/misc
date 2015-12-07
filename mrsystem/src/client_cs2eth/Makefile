TARGET=mrcs2eth
OBJS=main.o cs2eth.o
LOCALLIBS=-lmr_ipc -lmr_can -lm -lbytestream -lmrconfig -linipars -lscanner -lmap -lmenge -ldliste -lavl
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -DTRACE=1 -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

main.o: main.c cs2eth.h

cs2eth.o: cs2eth.c cs2eth.h

install: $(TARGET)
	install -D $(TARGET) $(DESTDIR)/$(TARGET)

uninstall:
	rm $(DESTDIR)/$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)

distclean:
	rm -f $(TARGET)
	rm -f $(OBJS)
