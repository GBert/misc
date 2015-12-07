INCLUDEPATH=../../include
LIBPATH=../../lib
ARFLAGS=rc
OBJS=liste_ahead.o liste_append.o liste_create.o liste_dfirst.o liste_delete.o liste_dstr.o liste_finde.o liste_frst.o liste_empty.o liste_init.o liste_isert.o liste_next.o
TARGET=libliste.a


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) 2> /dev/null


liste_ahead.o: liste_ahead.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_append.o: liste_append.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_create.o: liste_create.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_dfirst.o: liste_dfirst.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_delete.o: liste_delete.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_dstr.o: liste_dstr.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_finde.o: liste_finde.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_frst.o: liste_frst.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_empty.o: liste_empty.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_init.o: liste_init.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_isert.o: liste_isert.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_next.o: liste_next.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


install:
	install -m 644 liste.h $(INCLUDEPATH)
	install -m 644 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET) *.o
