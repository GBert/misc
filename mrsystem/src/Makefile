#PROJECTS=client_cs2sl client_cs2eth client_inject client_log client_logms2 client_ms1 client_ms2 client_cc client_slcan client_srcp client_tty client_gpios88 client_zentrale drehscheibe
PROJECTS=client_cs2sl client_cs2eth client_cceth client_inject client_log client_logms2 client_logeth client_ms2 client_cc client_slcan client_srcp client_tty client_zentrale drehscheibe
CFLAGS=
INCLUDE_PATH=include
LIB_PATH=lib
LDFLAGS=

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: $(PROJECTS) libs

all: $(PROJECTS) libs

common: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_cs2sl: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_cs2eth: libs common
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_cceth: libs common
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_inject: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_log: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_logeth: libs client_cs2sl
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_logms2: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_ms1: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_ms2: libs common
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_cc: libs common
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_slcan: libs common
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_srcp: libs common
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_tty: libs common
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_gpios88: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

client_zentrale: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

drehscheibe: libs
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH) TRACE=$(TRACE)

libs:
	if [ ! -d include ] ; then mkdir include ; fi
	if [ ! -d lib ] ; then mkdir lib ; fi
	make -C $@ CFLAGS="$(CFLAGS)" INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

install:
	make -C client_cs2eth install
	make -C client_cs2sl install
	# make -C client_gpios88 install
	make -C client_inject install
	make -C client_log install
	make -C client_logms2 install
	make -C client_ms1 install
	make -C client_ms2 install
	# make -C client_slcan install
	make -C client_zentrale install
	make -C drehscheibe install
	install -D etc/mrsystem /etc/mrsystem
	install -D etc/init.d/mrsystem /etc/init.d/mrsystem

clean_apps:
	make -C client_cc clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_cs2sl clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_cs2eth clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_slcan clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_gpios88 clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_inject clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_log clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_ms1 clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_ms2 clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_logms2 clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_zentrale clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C drehscheibe clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

clean_link:
	make -C client_cs2eth clean_link INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_gpios88 clean_link INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_inject clean_link INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_log clean_link INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_ms1 clean_link INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_ms2 clean_link INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_logms2 clean_link INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C client_zentrale clean_link INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C drehscheibe clean_link INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

clean: clean_apps
	make -C libs clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	rm -fr lib
	rm -fr include

uninstall:
	make -C client_cs2sl uninstall
	make -C client_cs2eth uninstall
	make -C client_inject uninstall
	make -C client_log uninstall
	make -C client_logms2 uninstall
	make -C client_ms1 uninstall
	make -C client_ms2 uninstall
	make -C client_zentrale uninstall
	make -C drehscheibe uninstall
	rm $DESTDIR/mrupdate
	rm /etc/init.d/mrsystem
