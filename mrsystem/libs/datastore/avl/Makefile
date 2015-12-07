INCLUDEPATH=../../include
LIBPATH=../../lib
ARFLAGS=rc
OBJS=avl_create.o avl_delete.o avl_destroy.o avl_finde.o avl_init.o avl_insert.o avl_purge.o avl_wascend.o avl_wdescend.o
TARGET=libavl.a


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) > /dev/null 2> /dev/null


avl_create.o: avl_create.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_delete.o: avl_delete.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_destroy.o: avl_destroy.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_finde.o: avl_finde.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_init.o: avl_init.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_insert.o: avl_insert.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_purge.o: avl_purge.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_wascend.o: avl_wascend.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_wdescend.o: avl_wdescend.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


install:
	install -m 664 avl.h $(INCLUDEPATH)
	install -m 664 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET) *.o
