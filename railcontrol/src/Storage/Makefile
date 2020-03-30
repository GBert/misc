
CC=gcc
CPP=g++
CCRASPI=aarch64-linux-gcc
CPPRASPI=aarch64-linux-g++

CFLAGSRASPI=--sysroot=/home/teddy/buildroot-2018.11/output/host/aarch64-buildroot-linux-gnu/sysroot/
CPPFLAGS=-g -fpic -O2 -Wall -Wextra -Werror -std=c++11 -I..
CPPFLAGSRASPI=-g -fpic -O2 -Wall --Wextra -Werror std=c++11 -I.. --sysroot=/home/teddy/buildroot-2018.11/output/host/aarch64-buildroot-linux-gnu/sysroot/
LDFLAGS=-g -shared -Wl,-soname,
CFLAGSSQLITE=-g -fpic -O2 -DSQLITE_ENABLE_FTS4 -DSQLITE_ENABLE_JSON1 -DSQLITE_ENABLE_RTREE -DHAVE_USLEEP

LIBS= \
	Sqlite.so

all: $(LIBS)

Sqlite.so: Sqlite.o sqlite/sqlite3.o *.h ../Logger/*.h ../Hardware/HardwareParams.h ../DataModel/*.h ../Languages.h
	$(CPP) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ $<  sqlite/sqlite3.o

sqlite/sqlite3.o: sqlite/sqlite3.c sqlite/sqlite3.h
	$(CC) $(CFLAGSSQLITE) -c -o $@ $<

amalgamation: sqlite/sqlite3.c sqlite/sqlite3.h
	$(CC) $(CFLAGSSQLITE) -c -o sqlite/sqlite3.o sqlite/sqlite3.c

raspi: sqlite/sqlite3.c sqlite/sqlite3.h
	$(CCRASPI) $(CFLAGSRASPI) $(CFLAGSSQLITE) -c -o sqlite/sqlite3.o sqlite/sqlite3.c

%.so: %.cpp *.h ../*.h ../DataModel/*.h ../Languages.h
	$(CPP) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ $<

%.so: %.o *.h ../*.h ../DataModel/*.h ../Languages.h
	$(CPP) -o $@ $< $(LDFLAGS)$@

clean:
	rm -f *.o *.so sqlite/*.o

