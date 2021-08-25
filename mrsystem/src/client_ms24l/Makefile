TARGET=mrms24l
OBJS=main.o c4l_client.o ../common/ms2.o
LOCALLIBS=-lmr_ipc -lmr_cs2ms2 -lcs2 -lm -lbytestream -lmrconfig -linipars -lscanner -lmap -lavl -lqueue -ldliste
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -DTRACE=$(TRACE) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

main.o: main.c ms2.h

c4l_client.o: c4l_client.c c4l_client.h

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
