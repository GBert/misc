
#CC=gcc
#CPP=g++

CPPFLAGS+=-I. -g -O2 -Wall -Wextra -Werror -std=c++11
LDFLAGS+=-g -Wl,--export-dynamic
LIBS=-lpthread -ldl

TMPDIR=RailControl


CXXOBJ= $(patsubst %.cpp,%.o,$(wildcard *.cpp WebServer/*.cpp DataModel/*.cpp Hardware/*.cpp Logger/*.cpp Network/*.cpp Storage/*.cpp Utils/*.cpp))
COBJ= $(patsubst %.c,%.o,$(wildcard Hardware/zlib/*.c))
OBJ=Timestamp.o Storage/sqlite/sqlite3.o $(CXXOBJ) $(COBJ)

all: $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o railcontrol $(LIBS)
	rm Timestamp.cpp
	rm Timestamp.o

dist: all
	strip railcontrol
	tar cvJf railcontrol.tar.xz Hardware/*.so Storage/*.so railcontrol railcontrol.conf.dist html/*
	mkdir $(TMPDIR)
	cp -r \
		html \
		railcontrol.conf.dist \
		railcontrol \
		$(TMPDIR)
	tar cvJf railcontrol.`date +"%Y%m%d"`.tar.xz $(TMPDIR)/* $(TMPDIR)/html/*
	rm -r $(TMPDIR)

sqlite-shell:
	make -C Storage/sqlite

Timestamp.o: Timestamp.cpp Timestamp.h
	$(CXX) $(CPPFLAGS) -c -o $@ $<

%.o: %.cpp *.h DataModel/*.h Hardware/HardwareHandler.h Logger/*.h Network/*.h Storage/StorageHandler.h Utils/*.h WebServer/*.h
	$(CXX) $(CPPFLAGS) -c -o $@ $<

clean:
	make -C Hardware clean
	make -C Storage clean
	rm -f *.o DataModel/*.o Hardware/*.o Hardware/zlib/*.o Logger/*.o Network/*.o Storage/*.o Utils/*.o WebServer/*.o
	rm -f railcontrol

clean-sqlite-shell:
	make -C Storage/sqlite clean

test:
	make -C test

tools:
	make -C tools

Timestamp.cpp:
	echo "#include <ctime>" > Timestamp.cpp
	echo "#include \"Timestamp.h\"" >> Timestamp.cpp
	echo "time_t GetCompileTime() { return `date +%s`; }" >> Timestamp.cpp

