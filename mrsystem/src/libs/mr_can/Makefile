TARGET=libmr_can.a
OBJS=enceth.o deceth.o sendeth.o sendtoeth.o recveth.o starteth.o starteth_bc.o mk_bcaddr.o starteth_tcp.o accept_tcp.o cs2_encode.o cs2_decode.o \
	calc_hash.o calc_hash_fnr.o crc.o iscs2msg.o encode_id.o decode_id.o conn_eth.o conn_tcp.o \
	cand0000.o cand0001.o cand0002.o cand0003.o cand0004.o cand0005.o cand0006.o cand0007.o cand0008.o cand0009.o cand000a.o cand000b.o cand000c.o cand0080.o \
	cane0000.o cane0001.o cane0002.o cane0003.o cane0004.o cane0005.o cane0006.o cane0007.o cane0008.o cane0009.o cane000a.o cane000b.o cane000c.o cane0080.o \
	cand01.o cand02.o cand03.o cand04.o cand05.o cand06.o cand07.o cand08.o \
	cand0b.o cand10.o cand11.o cand18.o cand1b.o cand1d.o cand20.o cand21.o cand30.o \
	cane01.o cane02.o cane03.o cane04.o cane05.o cane06.o cane07.o cane08.o \
	cane0b.o cane10.o cane11.o cane18.o cane1b.o cane1d.o cane20.o cane21.o cane30.o \
	statinfenc.o statinfdec.o trace.o dump_msg.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp mr_can.h $(INCLUDE_PATH)
	cp m_cs2ms2.h $(INCLUDE_PATH)

crc.o: crc.c mr_can.h m_cs2ms2.h

enceth.o: enceth.c mr_can.h m_cs2ms2.h

deceth.o: deceth.c mr_can.h m_cs2ms2.h

sendtoeth.o: sendtoeth.c mr_can.h m_cs2ms2.h

sendeth.o: sendeth.c mr_can.h m_cs2ms2.h

recveth.o: recveth.c mr_can.h m_cs2ms2.h

accept_tcp.o: accept_tcp.c mr_can.h m_cs2ms2.h

starteth.o: starteth.c mr_can.h m_cs2ms2.h

starteth_bc.o: starteth_bc.c mr_can.h m_cs2ms2.h

mk_bcaddr.o: mk_bcaddr.c mr_can.h m_cs2ms2.h

starteth_tcp.o: starteth_tcp.c mr_can.h m_cs2ms2.h

conn_eth.o: conn_eth.c mr_can.h m_cs2ms2.h

conn_tcp.o: conn_tcp.c mr_can.h m_cs2ms2.h

cs2_encode.o: cs2_encode.c mr_can.h m_cs2ms2.h

cs2_decode.o: cs2_decode.c mr_can.h m_cs2ms2.h

calc_hash.o: calc_hash.c mr_can.h m_cs2ms2.h

trace.o: trace.c mr_can.h m_cs2ms2.h

dump_frame.o: dump_frame.c mr_can.h m_cs2ms2.h

dump_msg.o: dump_msg.c mr_can.h m_cs2ms2.h

cand0000.o: cand0000.c mr_can.h m_cs2ms2.h

cand0001.o: cand0001.c mr_can.h m_cs2ms2.h

cand0002.o: cand0002.c mr_can.h m_cs2ms2.h

cand0003.o: cand0003.c mr_can.h m_cs2ms2.h

cand0004.o: cand0004.c mr_can.h m_cs2ms2.h

cand0005.o: cand0005.c mr_can.h m_cs2ms2.h

cand0006.o: cand0006.c mr_can.h m_cs2ms2.h

cand0007.o: cand0007.c mr_can.h m_cs2ms2.h

cand0008.o: cand0008.c mr_can.h m_cs2ms2.h

cand0009.o: cand0009.c mr_can.h m_cs2ms2.h

cand000a.o: cand000a.c mr_can.h m_cs2ms2.h

cand000b.o: cand000b.c mr_can.h m_cs2ms2.h

cand000c.o: cand000c.c mr_can.h m_cs2ms2.h

cand0080.o: cand0080.c mr_can.h m_cs2ms2.h

cane0000.o: cane0000.c mr_can.h m_cs2ms2.h

cane0001.o: cane0001.c mr_can.h m_cs2ms2.h

cane0002.o: cane0002.c mr_can.h m_cs2ms2.h

cane0003.o: cane0003.c mr_can.h m_cs2ms2.h

cane0004.o: cane0004.c mr_can.h m_cs2ms2.h

cane0005.o: cane0005.c mr_can.h m_cs2ms2.h

cane0006.o: cane0006.c mr_can.h m_cs2ms2.h

cane0007.o: cane0007.c mr_can.h m_cs2ms2.h

cane0008.o: cane0008.c mr_can.h m_cs2ms2.h

cane0009.o: cane0009.c mr_can.h m_cs2ms2.h

cane000a.o: cane000a.c mr_can.h m_cs2ms2.h

cane000b.o: cane000b.c mr_can.h m_cs2ms2.h

cane000c.o: cane000c.c mr_can.h m_cs2ms2.h

cane0080.o: cane0080.c mr_can.h m_cs2ms2.h

cand01.o: cand01.c mr_can.h m_cs2ms2.h

cand02.o: cand02.c mr_can.h m_cs2ms2.h

cand03.o: cand03.c mr_can.h m_cs2ms2.h

cand04.o: cand04.c mr_can.h m_cs2ms2.h

cand05.o: cand05.c mr_can.h m_cs2ms2.h

cand06.o: cand06.c mr_can.h m_cs2ms2.h

cand07.o: cand07.c mr_can.h m_cs2ms2.h

cand08.o: cand08.c mr_can.h m_cs2ms2.h

cand0b.o: cand0b.c mr_can.h m_cs2ms2.h

cand10.o: cand10.c mr_can.h m_cs2ms2.h

cand11.o: cand11.c mr_can.h m_cs2ms2.h

cand18.o: cand18.c mr_can.h m_cs2ms2.h

cand1b.o: cand1b.c mr_can.h m_cs2ms2.h

cand1d.o: cand1d.c mr_can.h m_cs2ms2.h

cand20.o: cand20.c mr_can.h m_cs2ms2.h

cand21.o: cand21.c mr_can.h m_cs2ms2.h

cand30.o: cand30.c mr_can.h m_cs2ms2.h

cane01.o: cane01.c mr_can.h m_cs2ms2.h

cane02.o: cane02.c mr_can.h m_cs2ms2.h

cane03.o: cane03.c mr_can.h m_cs2ms2.h

cane04.o: cane04.c mr_can.h m_cs2ms2.h

cane05.o: cane05.c mr_can.h m_cs2ms2.h

cane06.o: cane06.c mr_can.h m_cs2ms2.h

cane07.o: cane07.c mr_can.h m_cs2ms2.h

cane08.o: cane08.c mr_can.h m_cs2ms2.h

cane0b.o: cane0b.c mr_can.h m_cs2ms2.h

cane10.o: cane10.c mr_can.h m_cs2ms2.h

cane11.o: cane11.c mr_can.h m_cs2ms2.h

cane18.o: cane18.c mr_can.h m_cs2ms2.h

cane1b.o: cane1b.c mr_can.h m_cs2ms2.h

cane1d.o: cane1d.c mr_can.h m_cs2ms2.h

cane20.o: cane20.c mr_can.h m_cs2ms2.h

cane21.o: cane21.c mr_can.h m_cs2ms2.h

cane30.o: cane30.c mr_can.h m_cs2ms2.h

statinfenc.o: statinfenc.c m_cs2ms2.h

statinfdec.o: statinfdec.c m_cs2ms2.h

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f $(INCLUDE_PATH)/mr_can.h
	rm -f $(INCLUDE_PATH)/m_cs2ms2.h
	rm -f $(LIB_PATH)/$(TARGET)

test:
	make -C test INCLUDE_PATH=$(INCLUDE_PATH) LIB_PATH=$(LIB_PATH)
