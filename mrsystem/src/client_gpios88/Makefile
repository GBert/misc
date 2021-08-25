TARGET=mrgpios88
OBJS=s88udp-bpi.o allwinner.o
LOCALLIBS=-lmr_ipc -lm -lmr_cs2ms2 -lcs2 -lbytestream
DESTDIR=/usr/local/bin
LOCAL_CFLAGS=-DBPI_PRO

%.o: %.c
	$(CC) $(CFLAGS) -DTRACE=$(TRACE) -std=gnu99 -DUSE_MRSYSTEM -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

allwinner.c: ../../railroad/can2udp/src/allwinner.c
	cp $< $@

allwinner.h: ../../railroad/can2udp/src/allwinner.h
	cp $< $@

allwinner.o: allwinner.c allwinner.h

s88udp-bpi.c: ../../railroad/can2udp/src/s88udp-bpi.c
	cp $< $@
	patch < s88.patch

s88udp-bpi.h: ../../railroad/can2udp/src/s88udp-bpi.h
	cp $< $@

s88udp-bpi.o: s88udp-bpi.c s88udp-bpi.h allwinner.h

install: $(TARGET)
	install -D $(TARGET) $(DESTDIR)/$(TARGET)
	install -D startmrgpios88 $(DESTDIR)

uninstall:
	rm $(DESTDIR)/$(TARGET)
	rm $(DESTDIR)/startmrgpios88

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)

clean_link:
	rm -f $(TARGET)

distclean:
	rm -f $(TARGET)
	rm -f $(OBJS)
