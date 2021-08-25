INCLUDEPATH=../../include
LIBPATH=../../lib
ARFLAGS=rc
OBJS=liste_ah.o liste_ap.o liste_cr.o liste_de.o liste_df.o liste_ds.o liste_em.o liste_fi.o liste_fr.o liste_in.o liste_is.o liste_ne.o
TARGET=libliste.a


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) 2> /dev/null


liste_ah.o: liste_ah.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_ap.o: liste_ap.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_cr.o: liste_cr.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_de.o: liste_de.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_df.o: liste_df.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_ds.o: liste_ds.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_em.o: liste_em.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_fi.o: liste_fi.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_fr.o: liste_fr.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_in.o: liste_in.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_is.o: liste_is.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


liste_ne.o: liste_ne.c liste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


install:
	install -m 644 liste.h $(INCLUDEPATH)
	install -m 644 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET) *.o
