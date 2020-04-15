
CFLAGS+=-O2 -Wall -I.
LDFLAGS+=-ldl -lncurses -pthread

LIBS=shell.o sqlite.o

sqlite3: $(LIBS)
	$(CC) \
	-o $@ $(LIBS) $(LDFLAGS)

sqlite.o: sqlite3.c
	$(CC) $(CFLAGS) \
	-DSQLITE_THREADSAFE=0 \
	-DSQLITE_ENABLE_FTS4 \
	-DSQLITE_ENABLE_JSON1 \
	-DSQLITE_ENABLE_RTREE \
	-DSQLITE_ENABLE_EXPLAIN_COMMENTS \
	-DHAVE_USLEEP \
	-c -o $@ $<
	#-DSQLITE_ENABLE_FTS5

%.o: %.c
	$(CC) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.so sqlite3

