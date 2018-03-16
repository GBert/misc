INCLUDEPATH=../../include
LIBPATH=../../lib
ARFLAGS=rc
OBJS=menge_add.o menge_create.o menge_destroy.o menge_first.o menge_idel.o menge_init.o menge_next.o menge_remove.o menge_citer.o menge_iiter.o menge_diter.o
TARGET=libmenge.a


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) > /dev/null 2> /dev/null


menge_add.o: menge_add.c menge.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


menge_create.o: menge_create.c menge.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


menge_destroy.o: menge_destroy.c menge.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


menge_first.o: menge_first.c menge.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


menge_idel.o: menge_idel.c menge.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


menge_init.o: menge_init.c menge.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


menge_next.o: menge_next.c menge.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


menge_remove.o: menge_remove.c menge.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


menge_citer.o: menge_citer.c menge.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


menge_iiter.o: menge_iiter.c menge.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


menge_diter.o: menge_diter.c menge.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


install:
	install -m 644 menge.h $(INCLUDEPATH)
	install -m 644 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET) *.o
