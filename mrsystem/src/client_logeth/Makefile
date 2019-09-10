TARGET=mrlogeth
OBJS=main.o ../client_cs2sl/can_sleth.o ../common/logms2.o
LOCALLIBS=-lmr_ipc -lmr_cs2ms2 -lcs2 -lm -lbytestream -lmrconfig -linipars -lscanner -lmap -lavl
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -DTRACE=$(TRACE) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

main.o: main.c ../client_cs2sl/can_sleth.h  ../common/logms2.h

../client_cs2sl/can_sleth.o: ../client_cs2sl/can_sleth.c ../client_cs2sl/can_sleth.h
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -o $@ -c $<

../common/logms2.o:
	make -C ../common CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

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
