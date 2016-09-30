TARGET=mrzentrale
OBJS=main.o zentrale.o states.o cron.o lok.o gleisbild.o gleisbildpage.o \
	magnetartikel.o magstat.o fahrstrasse.o lokstatus.o gbsstat.o \
	fsstat.o zfile.o canmember.o
LOCALLIBS=-lmr_ipc -lmr_can -lcs2_parse -lfsm -lm -lbytestream -lmrconfig -linipars -lscanner -lmap -lmenge -lavl -ldliste -lz
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -Wall -pedantic -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

main.o: main.c zentrale.h

zentrale.o: zentrale.c zentrale.h canmember.h cron.h lok.h lokstatus.h gleisbild.h gleisbildpage.h gbsstat.h magnetartikel.h magstat.h fahrstrasse.h fsstat.h zfile.h

states.o: states.c zentrale.h cron.h lokstatus.h magstat.h gbsstat.h fsstat.h

cron.o: cron.c cron.h

lok.o: lok.c lok.h

lokstatus.o: lokstatus.c lokstatus.h lok.h

gleisbild.o: gleisbild.c gleisbild.h

gbsstat.o: gbsstat.c gbsstat.h gleisbild.h

gleisbildpage.o: gleisbildpage.c gleisbildpage.h

magnetartikel.o: magnetartikel.c magnetartikel.h

magstat.o: magstat.c magstat.h magnetartikel.h

fahrstrasse.o: fahrstrasse.c fahrstrasse.h

fsstat.o: fsstat.c fsstat.h fahrstrasse.h

canmember.o: canmember.c canmember.h

zfile.o: zfile.c zfile.h

lx_debug.o: lx_debug.c lx_debug.h

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
