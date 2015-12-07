INCLUDEPATH=../../include
LIBPATH=../../lib
ARFLAGS=rc
OBJS=dlist_ahead.o dlist_append.o dlist_create.o dlist_dakt.o dlist_dfirst.o dlist_dlast.o dlist_delete.o dlist_dstroy.o dlist_finde.o dlist_frst.o dlist_init.o dlist_nsert.o dlist_is_empty.o dlist_last.o dlist_next.o dlist_prev.o dlist_citer.o dlist_diter.o dlist_iiter.o
TARGET=libdliste.a


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) > /dev/null 2> /dev/null


dlist_ahead.o: dlist_ahead.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_append.o: dlist_append.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_create.o: dlist_create.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_dakt.o: dlist_dakt.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_dfirst.o: dlist_dfirst.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_dlast.o: dlist_dlast.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_delete.o: dlist_delete.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_dstroy.o: dlist_dstroy.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_finde.o: dlist_finde.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_frst.o: dlist_frst.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_init.o: dlist_init.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_nsert.o: dlist_nsert.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_is_empty.o: dlist_is_empty.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_last.o: dlist_last.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_next.o: dlist_next.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_prev.o: dlist_prev.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_citer.o: dlist_citer.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_diter.o: dlist_diter.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_iiter.o: dlist_iiter.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


install:
	install -m 644 dliste.h $(INCLUDEPATH)
	install -m 644 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET) *.o
