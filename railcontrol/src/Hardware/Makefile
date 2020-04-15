
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

CS2Tcp.so: CS2Tcp.cpp *.h ../Manager.h ../Languages.h MaerklinCAN.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ MaerklinCAN.cpp $<

CS2Udp.so: CS2Udp.cpp *.h ../Manager.h ../Languages.h MaerklinCAN.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ MaerklinCAN.cpp $<

CcSchnitte.so: CcSchnitte.cpp *.h ../Manager.h ../Languages.h MaerklinCAN.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ MaerklinCAN.cpp $<

%.so: %.cpp *.h ../Manager.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ $<

clean:
	rm -f *.so

