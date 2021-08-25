DOKUDIR=cs2
DOKUPATH="../../doku/quellcode/$(DOKUDIR)"
DOKUWEBPATH="/var/www/Doku/mrsystem/quellcode/$(DOKUDIR)"
TARGET=libcs2.a
OBJS=calc_hash.o calc_hash_fnr.o crc.o iscs2msg.o encode_id.o decode_id.o \
	cand0000.o cand0001.o cand0002.o cand0003.o cand0004.o cand0005.o \
	cand0006.o cand0007.o cand0008.o cand0009.o cand000a.o cand000b.o \
	cand000c.o cand0080.o cane0000.o cane0001.o cane0002.o cane0003.o \
	cane0004.o cane0005.o cane0006.o cane0007.o cane0008.o cane0009.o \
	cane000a.o cane000b.o cane000c.o cane0080.o \
	cand01.o cand02.o cand03.o cand04.o cand05.o cand06.o cand07.o \
	cand08.o cand0b.o cand10.o cand11.o cand18.o cand1b.o cand1d.o \
	cand20.o cand21.o cand30.o cand42.o \
	cane01.o cane02.o cane03.o cane04.o cane05.o cane06.o cane07.o \
	cane08.o cane0b.o cane10.o cane11.o cane18.o cane1b.o cane1d.o \
	cane20.o cane21.o cane30.o cane42.o \
	statinfenc.o statinfdec.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp cs2.h $(INCLUDE_PATH)

crc.o: crc.c cs2.h

calc_hash.o: calc_hash.c cs2.h

calc_hash_fnr.o: calc_hash_fnr.c cs2.h

iscs2msg.o: iscs2msg.c cs2.h

encode_id.o: encode_id.c cs2.h

decode_id.o: decode_id.c cs2.h

cand0000.o: cand0000.c cs2.h

cand0001.o: cand0001.c cs2.h

cand0002.o: cand0002.c cs2.h

cand0003.o: cand0003.c cs2.h

cand0004.o: cand0004.c cs2.h

cand0005.o: cand0005.c cs2.h

cand0006.o: cand0006.c cs2.h

cand0007.o: cand0007.c cs2.h

cand0008.o: cand0008.c cs2.h

cand0009.o: cand0009.c cs2.h

cand000a.o: cand000a.c cs2.h

cand000b.o: cand000b.c cs2.h

cand000c.o: cand000c.c cs2.h

cand0080.o: cand0080.c cs2.h

cane0000.o: cane0000.c cs2.h

cane0001.o: cane0001.c cs2.h

cane0002.o: cane0002.c cs2.h

cane0003.o: cane0003.c cs2.h

cane0004.o: cane0004.c cs2.h

cane0005.o: cane0005.c cs2.h

cane0006.o: cane0006.c cs2.h

cane0007.o: cane0007.c cs2.h

cane0008.o: cane0008.c cs2.h

cane0009.o: cane0009.c cs2.h

cane000a.o: cane000a.c cs2.h

cane000b.o: cane000b.c cs2.h

cane000c.o: cane000c.c cs2.h

cane0080.o: cane0080.c cs2.h

cand01.o: cand01.c cs2.h

cand02.o: cand02.c cs2.h

cand03.o: cand03.c cs2.h

cand04.o: cand04.c cs2.h

cand05.o: cand05.c cs2.h

cand06.o: cand06.c cs2.h

cand07.o: cand07.c cs2.h

cand08.o: cand08.c cs2.h

cand0b.o: cand0b.c cs2.h

cand10.o: cand10.c cs2.h

cand11.o: cand11.c cs2.h

cand18.o: cand18.c cs2.h

cand1b.o: cand1b.c cs2.h

cand1d.o: cand1d.c cs2.h

cand20.o: cand20.c cs2.h

cand21.o: cand21.c cs2.h

cand30.o: cand30.c cs2.h

cand42.o: cand42.c cs2.h

cane01.o: cane01.c cs2.h

cane02.o: cane02.c cs2.h

cane03.o: cane03.c cs2.h

cane04.o: cane04.c cs2.h

cane05.o: cane05.c cs2.h

cane06.o: cane06.c cs2.h

cane07.o: cane07.c cs2.h

cane08.o: cane08.c cs2.h

cane0b.o: cane0b.c cs2.h

cane10.o: cane10.c cs2.h

cane11.o: cane11.c cs2.h

cane18.o: cane18.c cs2.h

cane1b.o: cane1b.c cs2.h

cane1d.o: cane1d.c cs2.h

cane20.o: cane20.c cs2.h

cane21.o: cane21.c cs2.h

cane30.o: cane30.c cs2.h

cane42.o: cane42.c cs2.h

statinfenc.o: statinfenc.c cs2.h

statinfdec.o: statinfdec.c cs2.h

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
	rm -f $(INCLUDE_PATH)/mr_can.h
	rm -f $(INCLUDE_PATH)/m_cs2ms2.h
	rm -f $(LIB_PATH)/$(TARGET)

clean_doku:
	if [ -d $(DOKUPATH) ] ; then rm -r $(DOKUPATH) ; fi

.PHONY: test

test:
	make -C test INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
