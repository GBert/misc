TARGET=mrzentrale
OBJS=main.o zentrale.o states.o lok.o gleisbild.o gleisbildpage.o \
	magnetartikel.o fahrstrasse.o zfile.o
LOCALLIBS=-lmr_ipc -lmr_can -lcs2_parse -lfsm -lm -lbytestream -lmrconfig -linipars -lscanner -lmap -lavl -lz
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -Wall -pedantic -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

main.o: main.c zentrale.h

zentrale.o: zentrale.c zentrale.h lok.h gleisbild.h gleisbildpage.h magnetartikel.h fahrstrasse.h zfile.h

states.o: states.c zentrale.h

lok.o: lok.c lok.h

gleisbild.o: gleisbild.c gleisbild.h

gleisbildpage.o: gleisbildpage.c gleisbildpage.h

magnetartikel.o: magnetartikel.c magnetartikel.h

fahrstrasse.o: fahrstrasse.c fahrstrasse.h

zfile.o: zfile.c zfile.h

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
