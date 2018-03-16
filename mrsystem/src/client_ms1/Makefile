TARGET=mrms1
OBJS=main.o can_client.o ms1.o
LOCALLIBS=-lmr_ipc -lmr_can -lm -lmrconfig -linipars -lscanner -lmap -lavl
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

main.o: main.c ms1.h

can_client.o: can_client.c can_client.h

ms1.o: ms1.c ms1.h

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
