TARGET=mrzentrale
RELEASE_DIR=release
DEBUG_DIR=debug
RELEASE_TARGET=$(RELEASE_DIR)/$(TARGET)
DEBUG_TARGET=$(DEBUG_DIR)/$(TARGET)
RELEASE_OBJS=$(RELEASE_DIR)/main.o \
	$(RELEASE_DIR)/zentrale.o \
	$(RELEASE_DIR)/states.o \
	$(RELEASE_DIR)/cron.o \
	$(RELEASE_DIR)/lok.o \
	$(RELEASE_DIR)/gleisbild.o \
	$(RELEASE_DIR)/gleisbildpage.o \
	$(RELEASE_DIR)/magnetartikel.o \
	$(RELEASE_DIR)/magstat.o \
	$(RELEASE_DIR)/fahrstrasse.o \
	$(RELEASE_DIR)/lokstatus.o \
	$(RELEASE_DIR)/gbsstat.o \
	$(RELEASE_DIR)/fsstat.o \
	$(RELEASE_DIR)/config.o \
	$(RELEASE_DIR)/zfile.o \
	$(RELEASE_DIR)/canmember.o \
	$(RELEASE_DIR)/cs2cfg.o
DEBUG_OBJS=$(DEBUG_DIR)/main.o \
	$(DEBUG_DIR)/zentrale.o \
	$(DEBUG_DIR)/states.o \
	$(DEBUG_DIR)/cron.o \
	$(DEBUG_DIR)/lok.o \
	$(DEBUG_DIR)/gleisbild.o \
	$(DEBUG_DIR)/gleisbildpage.o \
	$(DEBUG_DIR)/magnetartikel.o \
	$(DEBUG_DIR)/magstat.o \
	$(DEBUG_DIR)/fahrstrasse.o \
	$(DEBUG_DIR)/lokstatus.o \
	$(DEBUG_DIR)/gbsstat.o \
	$(DEBUG_DIR)/fsstat.o \
	$(DEBUG_DIR)/config.o \
	$(DEBUG_DIR)/zfile.o \
	$(DEBUG_DIR)/canmember.o \
	$(DEBUG_DIR)/cs2cfg.o
LOCALLIBS=-lmr_ipchl -lmr_ipc -lmr_cs2ms2 -lcs2 -lcs2_parse -lfsm -lm -lbytestream -lmrconfig -linipars -luci -lscanner -lmap -lmenge -lavl -ldliste -lz
DEBUG_LOCALLIBS=-lmr_ipchl -lmr_ipc -lcs2 -lcs2_parse -lfsm -lm -lbytestream -lmrconfig -linipars -luci -lscanner -lmap -lmenge -lavl -ldliste -lz -llxdbg -lefence
DESTDIR=/usr/local/bin

$(RELEASE_DIR)/%.o: %.c
	if [ ! -d $(RELEASE_DIR) ] ; then mkdir $(RELEASE_DIR) ; fi
	$(CC) $(CFLAGS) -DTRACE=$(TRACE) -I$(INCLUDE_PATH) -o $@ -c $<

$(DEBUG_DIR)/%.o: %.c
	if [ ! -d $(DEBUG_DIR) ] ; then mkdir $(DEBUG_DIR) ; fi
	$(CC) $(CFLAGS) -DTRACE=$(TRACE) -I$(INCLUDE_PATH) -o $@ -c $<

all: $(RELEASE_TARGET)

debug: $(DEBUG_TARGET)

$(RELEASE_TARGET): $(RELEASE_OBJS)
	if [ ! -d $(RELEASE_DIR) ] ; then mkdir $(RELEASE_DIR) ; fi
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(RELEASE_OBJS) $(LDLIBS) $(LOCALLIBS)

$(DEBUG_TARGET): $(DEBUG_OBJS)
	if [ ! -d $(DEBUG_DIR) ] ; then mkdir $(DEBUG_DIR) ; fi
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(DEBUG_OBJS) $(LDLIBS) $(DEBUG_LOCALLIBS)

$(RELEASE_DIR)/main.o: main.c zentrale.h

$(DEBUG_DIR)/main.o: main.c zentrale.h

$(RELEASE_DIR)/zentrale.o: zentrale.c zentrale.h canmember.h cron.h lok.h lokstatus.h gleisbild.h gleisbildpage.h gbsstat.h magnetartikel.h magstat.h fahrstrasse.h fsstat.h zfile.h cs2cfg.h

$(DEBUG_DIR)/zentrale.o: zentrale.c zentrale.h canmember.h cron.h lok.h lokstatus.h gleisbild.h gleisbildpage.h gbsstat.h magnetartikel.h magstat.h fahrstrasse.h fsstat.h zfile.h cs2cfg.h config.h

$(RELEASE_DIR)/states.o: states.c zentrale.h cron.h lokstatus.h magstat.h gbsstat.h fsstat.h cs2cfg.h fsmfkt_ms2master.h fsmfkt_proxy.h fsmtab_ms2master.h fsmtab_proxy.h

$(DEBUG_DIR)/states.o: states.c zentrale.h cron.h lokstatus.h magstat.h gbsstat.h fsstat.h config.h cs2cfg.h fsmfkt_ms2master.h fsmfkt_proxy.h fsmtab_ms2master.h fsmtab_proxy.h

$(RELEASE_DIR)/cron.o: cron.c cron.h

$(DEBUG_DIR)/cron.o: cron.c cron.h

$(RELEASE_DIR)/lok.o: lok.c lok.h

$(DEBUG_DIR)/lok.o: lok.c lok.h

$(RELEASE_DIR)/lokstatus.o: lokstatus.c lokstatus.h lok.h

$(DEBUG_DIR)/lokstatus.o: lokstatus.c lokstatus.h lok.h

$(RELEASE_DIR)/gleisbild.o: gleisbild.c gleisbild.h

$(DEBUG_DIR)/gleisbild.o: gleisbild.c gleisbild.h

$(RELEASE_DIR)/gbsstat.o: gbsstat.c gbsstat.h gleisbild.h

$(DEBUG_DIR)/gbsstat.o: gbsstat.c gbsstat.h gleisbild.h

$(RELEASE_DIR)/gleisbildpage.o: gleisbildpage.c gleisbildpage.h

$(DEBUG_DIR)/gleisbildpage.o: gleisbildpage.c gleisbildpage.h

$(RELEASE_DIR)/magnetartikel.o: magnetartikel.c magnetartikel.h

$(DEBUG_DIR)/magnetartikel.o: magnetartikel.c magnetartikel.h

$(RELEASE_DIR)/magstat.o: magstat.c magstat.h magnetartikel.h

$(DEBUG_DIR)/magstat.o: magstat.c magstat.h magnetartikel.h

$(RELEASE_DIR)/fahrstrasse.o: fahrstrasse.c fahrstrasse.h

$(DEBUG_DIR)/fahrstrasse.o: fahrstrasse.c fahrstrasse.h

$(RELEASE_DIR)/fsstat.o: fsstat.c fsstat.h fahrstrasse.h

$(DEBUG_DIR)/fsstat.o: fsstat.c fsstat.h fahrstrasse.h

$(RELEASE_DIR)/config.o: config.c config.h

$(DEBUG_DIR)/config.o: config.c config.h

$(RELEASE_DIR)/canmember.o: canmember.c canmember.h

$(DEBUG_DIR)/canmember.o: canmember.c canmember.h

$(RELEASE_DIR)/zfile.o: zfile.c zfile.h

$(DEBUG_DIR)/zfile.o: zfile.c zfile.h

$(RELEASE_DIR)/cs2cfg.o: cs2cfg.h

$(DEBUG_DIR)/cs2cfg.o: cs2cfg.h

install: $(RELEASE_TARGET)
	install -D $(RELEASE_TARGET) $(DESTDIR)/$(TARGET)

uninstall:
	rm $(DESTDIR)/$(TARGET)

clean:
	rm -rf release
	rm -rf debug

clean_link:
	rm -f $(RELEASE_TARGET)
	rm -f $(DEBUG_TARGET)

distclean:
	rm -rf release
	rm -rf debug

.PHONY: test

test:
	make -C test CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
