INCLUDEPATH=../include
LIBPATH=../lib
ARFLAGS=rc
OBJS=scan_create.o scan_destroy.o scan_init.o scan_exit.o scan_scan.o scan_suche.o scan_eol.o scan_ueberlesen.o
TARGET=libscanner.a


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) > /dev/null 2> /dev/null


scan_create.o: scan_create.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_destroy.o: scan_destroy.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_init.o: scan_init.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_exit.o: scan_exit.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_scan.o: scan_scan.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_suche.o: scan_suche.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_eol.o: scan_eol.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_ueberlesen.o: scan_ueberlesen.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


install:
	install -m 644 scanner.h $(INCLUDEPATH)
	install -m 644 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET) *.o
