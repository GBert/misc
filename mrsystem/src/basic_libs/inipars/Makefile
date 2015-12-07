INCLUDEPATH=../include
LIBPATH=../lib
ARFLAGS=rc
OBJS=ip_create.o ip_destroy.o ip_init.o ip_exit.o ip_read.o
TARGET=libinipars.a


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) > /dev/null 2> /dev/null


ip_create.o: ip_create.c inipars.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


ip_destroy.o: ip_destroy.c inipars.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


ip_init.o: ip_init.c inipars.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


ip_exit.o: ip_exit.c inipars.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


ip_read.o: ip_read.c inipars.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


parstest: parstest.o
	$(CC) $(LDFLAGS) -o parsetest parstest.o -linipars -lscanner -lmap -lavl


parstest.o: parstest.c
	$(CC)  $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


install:
	install -m 644 inipars.h $(INCLUDEPATH)
	install -m 644 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET) *.o
