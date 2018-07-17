TARGET=mrsrcp
OBJS=main.o ../common/cs2eth.o srcp.o states.o srcp_parse.o
LOCALLIBS=-lrt -lmr_ipc -lmr_can -lm -lbytestream -lmrconfig -linipars -lscanner -lfsm -lmap -lmenge -lavl -lqueue -ldliste
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -DTRACE=$(TRACE) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)


main.o: main.c ../common/cs2eth.h srcp.h

srcp.o: srcp.c srcp.h srcp_parse.h

states.o: states.c srcp.h srcp_parse.h

srcp_parse.o: srcp_parse.c srcp_parse.h

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
