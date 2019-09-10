DOKUDIR=mr_cs2ms2
DOKUPATH="../../doku/quellcode/$(DOKUDIR)"
DOKUWEBPATH="/var/www/Doku/mrsystem/quellcode/$(DOKUDIR)"
TARGET=libmr_cs2ms2.a
OBJS=enceth.o deceth.o sendeth.o sendtoeth.o recveth.o starteth.o starteth_bc.o mk_bcaddr.o starteth_tcp.o accept_tcp.o cs2_encode.o cs2_decode.o \
	conn_eth.o conn_tcp.o trace.o dump_msg.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp mr_cs2ms2.h $(INCLUDE_PATH)

enceth.o: enceth.c mr_cs2ms2.h

deceth.o: deceth.c mr_cs2ms2.h

sendtoeth.o: sendtoeth.c mr_cs2ms2.h

sendeth.o: sendeth.c mr_cs2ms2.h

recveth.o: recveth.c mr_cs2ms2.h

accept_tcp.o: accept_tcp.c mr_cs2ms2.h

starteth.o: starteth.c mr_cs2ms2.h

starteth_bc.o: starteth_bc.c mr_cs2ms2.h

mk_bcaddr.o: mk_bcaddr.c mr_cs2ms2.h

starteth_tcp.o: starteth_tcp.c mr_cs2ms2.h

conn_eth.o: conn_eth.c mr_cs2ms2.h

conn_tcp.o: conn_tcp.c mr_cs2ms2.h

cs2_encode.o: cs2_encode.c mr_cs2ms2.h

cs2_decode.o: cs2_decode.c mr_cs2ms2.h

trace.o: trace.c mr_cs2ms2.h

dump_frame.o: dump_frame.c mr_cs2ms2.h

dump_msg.o: dump_msg.c mr_cs2ms2.h

doku:
	if [ ! -d $(DOKUPATH) ] ; then mkdir $(DOKUPATH) ; fi
	doxygen Doxyfile

install_web:
	if [ ! -d $(DOKUWEBPATH) ] ; then mkdir $(DOKUWEBPATH) ; fi
	if [ ! -d $(DOKUWEBPATH)/search ] ; then mkdir $(DOKUWEBPATH)/search ; fi
	find $(DOKUPATH) -type f -maxdepth 0 -exec install -m 644 -D "{}" $(DOKUWEBPATH) ";"
	find $(DOKUPATH)/search -type f -maxdepth 0 -exec install -m 644 -D "{}" $(DOKUWEBPATH)/search ";"

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f $(INCLUDE_PATH)/mr_cs2ms2.h
	rm -f $(LIB_PATH)/$(TARGET)

clean_doku:
	if [ -d $(DOKUPATH) ] ; then rm -r $(DOKUPATH) ; fi

test:
	make -C test INCLUDE_PATH=$(INCLUDE_PATH) LIB_PATH=$(LIB_PATH)
