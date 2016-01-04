INCLUDEPATH=../../include
LIBPATH=../../lib
ARFLAGS=rc
OBJS=avl_crea.o avl_dele.o avl_dest.o avl_find.o avl_init.o avl_inse.o avl_purg.o avl_wasc.o avl_wdes.o
TARGET=libavl.a


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) > /dev/null 2> /dev/null


avl_crea.o: avl_crea.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_dele.o: avl_dele.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_dest.o: avl_dest.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_find.o: avl_find.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_init.o: avl_init.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_inse.o: avl_inse.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_purg.o: avl_purg.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_wasc.o: avl_wasc.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


avl_wdes.o: avl_wdes.c avl.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


install:
	install -m 664 avl.h $(INCLUDEPATH)
	install -m 664 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET) *.o
