TARGET=mrlogms2
OBJS=main.o can_client.o logms2.o
LOCALLIBS=-lmr_ipc -lmr_can -lm -lbytestream -lmrconfig -linipars -lscanner -lmap -lavl
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

main.o: main.c logms2.h

can_client.o: can_client.c can_client.h

logms2.o: logms2.c logms2.h

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
