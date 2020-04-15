

CPPFLAGS+=-g -fpic -O2 -Wall -Wextra -Werror -std=c++11 -I..
LDFLAGS+=-g -shared -Wl,-soname,
CFLAGSSQLITE=-g -fpic -O2 -DSQLITE_ENABLE_FTS4 -DSQLITE_ENABLE_JSON1 -DSQLITE_ENABLE_RTREE -DHAVE_USLEEP

LIBS= \
	Sqlite.so

all: $(LIBS)

Sqlite.so: Sqlite.o sqlite/sqlite3.o *.h ../Logger/*.h ../Hardware/HardwareParams.h ../DataModel/*.h ../Languages.h
	$(CXX) $(CPPFLAGS) $(LDFLAGS)$@ -o $@ $<  sqlite/sqlite3.o

sqlite/sqlite3.o: sqlite/sqlite3.c sqlite/sqlite3.h
	$(CC) $(CPPFLAGS) $(CFLAGSSQLITE) -c -o $@ $<

%.o: %.cpp *.h ../*.h ../DataModel/*.h ../Logger/*.h ../Hardware/HardwareParams.h ../Languages.h
	$(CXX) $(CPPFLAGS) -c -o $@ $<

%.so: %.o *.h ../*.h ../DataModel/*.h ../Languages.h
	$(CPP) -o $@ $< $(LDFLAGS)$@

clean:
	rm -f *.o *.so sqlite/*.o

