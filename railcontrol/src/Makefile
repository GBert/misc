#CC=gcc
#CPP=g++

CPPFLAGS+=-I. -g -O2 -Wall -Wextra -Werror -std=c++11
LDFLAGS+=-g -Wl,--export-dynamic
LIBS=-lpthread -ldl

TMPDIR=RailControl

CXXOBJ= $(patsubst %.cpp,%.o,$(sort Timestamp.cpp $(wildcard *.cpp)) $(wildcard WebServer/*.cpp DataModel/*.cpp Hardware/*.cpp Logger/*.cpp Network/*.cpp Storage/*.cpp Utils/*.cpp))
COBJ= $(patsubst %.c,%.o,$(wildcard Hardware/zlib/*.c))
OBJ=Storage/sqlite/sqlite3.o $(CXXOBJ) $(COBJ)

all: $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o railcontrol $(LIBS)

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

Version.o: Version.cpp Version.h
	$(CXX) $(CPPFLAGS) -c -o $@ $<

Hardware/zlib/%.o: Hardware/zlib/%.c Hardware/zlib/*.h
	$(CC) $(CFLAGSZLIB) -c -o $@ $<

Storage/sqlite/sqlite3.o: Storage/sqlite/sqlite3.c Storage/sqlite/sqlite3.h
	$(CC) $(CFLAGSSQLITE) -c -o $@ $<

%.o: %.cpp *.h DataModel/*.h Hardware/*.h Logger/*.h Network/*.h Storage/*.h Utils/*.h WebServer/*.h
	$(CXX) $(CPPFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f *.o DataModel/*.o Hardware/*.o Hardware/zlib/*.o Logger/*.o Network/*.o Storage/*.o Storage/sqlite/*.o Utils/*.o WebServer/*.o
	rm -f railcontrol

clean-sqlite-shell:
	make -C Storage/sqlite clean

test:
	make -C test

tools:
	make -C tools

.PHONY: Version.cpp
Version.cpp: Version.cpp.in
	sed s/@COMPILE_TIMESTAMP@/`date +%s`/ < Version.cpp.in | sed s/@GIT_HASH@/`git log -1 --format=%H`/ | sed s/@GIT_TIMESTAMP@/`git log -1 --format=%at`/ | sed s/@RAILCONTROL_VERSION@/$(RAILCONTROL_VERSION)/ > Version.cpp

