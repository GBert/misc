TARGET=mrms1
OBJS=main.o ms1.o ../client_ms2/can_client.o
LOCALLIBS=-lmr_ipc -lmr_can -lm -lmrconfig -linipars -lscanner -lmap -lavl -lbytestream
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -DTRACE=$(TRACE) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

main.o: main.c ms1.h

ms1.o: ms1.c ms1.h

../client_ms2/can_client.o: ../client_ms2/can_client.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -o $@ -c $<

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
