
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

all: $(LIBS)

CS2Tcp.so: CS2Tcp.o ProtocolMaerklinCAN.o *.h ../Manager.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ ProtocolMaerklinCAN.o $<

CS2Udp.so: CS2Udp.o ProtocolMaerklinCAN.o *.h ../Manager.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ ProtocolMaerklinCAN.o $<

CcSchnitte.so: CcSchnitte.o ProtocolMaerklinCAN.o *.h ../Manager.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ ProtocolMaerklinCAN.o $<

%.so: %.o *.h ../Manager.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ $<

%.o: %.cpp *.h ../*.h ../Logger/*.h ../Languages.h
	$(CXX) $(CPPFLAGS) -c -o $@ $< 

clean:
	rm -f *.so

