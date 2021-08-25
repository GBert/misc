#CC=gcc
#CPP=g++

CPPFLAGS+=-I. -g -O2 -Wall -Wextra -Werror -std=c++11
LDFLAGS+=-g -Wl,--export-dynamic
LIBS=-lpthread -ldl

TMPDIR=RailControl

CXXOBJ= $(patsubst %.cpp,%.o,$(sort $(wildcard *.cpp)) $(wildcard WebServer/*.cpp DataModel/*.cpp Hardware/*.cpp Hardware/Protocols/*.cpp Logger/*.cpp Network/*.cpp Storage/*.cpp Utils/*.cpp))
COBJ= $(patsubst %.c,%.o,$(wildcard Hardware/zlib/*.c))
OBJ=Storage/sqlite/sqlite3.o $(CXXOBJ) $(COBJ)

all: $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o railcontrol $(LIBS)

dist: all
	strip railcontrol
	mkdir $(TMPDIR)
	cp -r \
		html \
		railcontrol.conf.dist \
		railcontrol \
		$(TMPDIR)
	( cd $(TMPDIR)/.. && tar cvJf railcontrol.`date +"%Y%m%d"`.tar.xz RailControl/* RailControl/html/* )
	rm -r $(TMPDIR)

sqlite-shell:
	make -C Storage/sqlite

Version.o: Version.cpp Version.h
	$(CXX) $(CPPFLAGS) -c -o $@ $<

Hardware/zlib/%.o: Hardware/zlib/%.c Hardware/zlib/*.h
	$(CC) $(CPPFLAGS) $(CFLAGSZLIB) -c -o $@ $<

Storage/sqlite/sqlite3.o: Storage/sqlite/sqlite3.c Storage/sqlite/sqlite3.h
	$(CC) $(CPPFLAGS) $(CFLAGSSQLITE) -c -o $@ $<

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

