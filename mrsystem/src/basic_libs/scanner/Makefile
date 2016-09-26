INCLUDEPATH=../include
LIBPATH=../lib
ARFLAGS=rc
OBJS=scan_cre.o scan_des.o scan_ini.o scan_exi.o scan_sca.o scan_suc.o scan_eol.o scan_ueb.o
TARGET=libscanner.a


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) > /dev/null 2> /dev/null


scan_cre.o: scan_cre.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_des.o: scan_des.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_ini.o: scan_ini.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_exi.o: scan_exi.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_sca.o: scan_sca.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_suc.o: scan_suc.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_eol.o: scan_eol.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


scan_ueb.o: scan_ueb.c scanner.h
	$(CC) $(CFLAGS) $(CCFLAGS) -I$(INCLUDEPATH) -c -o $@ $<


install:
	install -m 644 scanner.h $(INCLUDEPATH)
	install -m 644 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET) *.o
