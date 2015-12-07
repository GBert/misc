INCLUDEPATH=../../include
LIBPATH=../../lib
ARFLAGS=rc
OBJS=baum_create.o baum_delete.o baum_destroy.o baum_finde.o baum_init.o baum_insert.o baum_winorder.o baum_wpraeorder.o baum_wpostorder.o
TARGET=libbaum.a


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) > /dev/null 2>/dev/null


baum_create.o: baum_create.c baum.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


baum_delete.o: baum_delete.c baum.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


baum_destroy.o: baum_destroy.c baum.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


baum_finde.o: baum_finde.c baum.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


baum_init.o: baum_init.c baum.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


baum_insert.o: baum_insert.c baum.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


baum_winorder.o: baum_winorder.c baum.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


baum_wpraeorder.o: baum_wpraeorder.c baum.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


baum_wpostorder.o: baum_wpostorder.c baum.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


install:
	install -m 664 baum.h $(INCLUDEPATH)
	install -m 644 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET) *.o
