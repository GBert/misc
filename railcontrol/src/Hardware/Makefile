
#CPPFLAGS=-g -fpic -O0 -Wall -std=c++11
CPPFLAGS+=-I. -I.. -g -fpic -O2 -Wall -Wextra -Werror -std=c++11
LDFLAGS=-g -shared -Wl,-soname,

LIBS= \
	CS2Tcp.so \
	CS2Udp.so \
	CcSchnitte.so \
	Ecos.so \
	Hsi88.so \
	M6051.so \
	OpenDcc.so \
	RM485.so \
	Virtual.so \
	Z21.so

OBJZLIB=zlib/adler32.o \
	zlib/compress.o \
	zlib/crc32.o \
	zlib/deflate.o \
	zlib/gzclose.o \
	zlib/gzlib.o \
	zlib/gzread.o \
	zlib/gzwrite.o \
	zlib/infback.o \
	zlib/inffast.o \
	zlib/inflate.o \
	zlib/inftrees.o \
	zlib/trees.o \
	zlib/uncompr.o \
	zlib/zutil.o \
	ZLib.o

all: $(LIBS)

zlib: $(OBJZLIB)

zlib/%.o: zlib/%.c zlib/*.h
	$(CC) $(CPPFLAGS) -c -o $@ $<

CS2Tcp.so: CS2Tcp.o ProtocolMaerklinCAN.o $(OBJZLIB) *.h ../Manager.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ ProtocolMaerklinCAN.o $<

CS2Udp.so: CS2Udp.o ProtocolMaerklinCAN.o ZLib.o *.h ../Manager.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ ProtocolMaerklinCAN.o $<

CcSchnitte.so: CcSchnitte.o ProtocolMaerklinCAN.o ZLib.o *.h ../Manager.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ ProtocolMaerklinCAN.o $<

%.so: %.o *.h ../Manager.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ $<

%.o: %.cpp *.h ../*.h ../Logger/*.h ../Languages.h
	$(CXX) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f *.so *.o zlib/*.o

