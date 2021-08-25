TARGET=mrcceth
OBJS=main.o ../common/cs2eth.o cc_eth.o
LOCALLIBS=-lmr_ipc -lmr_cs2ms2 -lcs2 -lm -lbytestream -lmrconfig -linipars -lscanner -lmap -lmenge -ldliste -lavl
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -DTRACE=$(TRACE) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

main.o: main.c ../common/cs2eth.h cc_eth.h

cc_eth.o: cc_eth.c cc_eth.h

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
