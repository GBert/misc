TARGET=mrsrcp
OBJS=main.o ../common/cs2eth.o srcp.o states.o
LOCALLIBS=-lrt -lmr_ipc -lmr_cs2ms2 -lcs2 -lsrcp -lm -lbytestream -lmrconfig -linipars -lscanner -lfsm -lmap -lmenge -lavl -lqueue -ldliste
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -DTRACE=$(TRACE) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)


main.o: main.c ../common/cs2eth.h srcp.h

srcp.o: srcp.c srcp.h

states.o: states.c srcp.h


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
