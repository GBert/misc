
#CPPFLAGS=-g -fpic -O0 -Wall -std=c++11
CPPFLAGS=-I. -I.. -g -fpic -O2 -Wall -Wextra -Werror -std=c++11
LDFLAGS=-g -shared -Wl,-soname,

LIBS= \
	CS2.so \
	CcSchnitte.so \
	Ecos.so \
	Hsi88.so \
	M6051.so \
	OpenDcc.so \
	RM485.so \
	Virtual.so \
	Z21.so

all: $(LIBS)

CcSchnitte.so: CcSchnitte.cpp *.h ../Manager.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ MaerklinCAN.cpp $<

CS2.so: CS2.cpp *.h ../Manager.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ MaerklinCAN.cpp $<

%.so: %.cpp *.h ../Manager.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ $<

clean:
	rm -f *.so

