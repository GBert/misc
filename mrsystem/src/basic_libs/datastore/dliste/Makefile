INCLUDEPATH=../../include
LIBPATH=../../lib
ARFLAGS=rc
OBJS=dlist_ah.o dlist_ap.o dlist_ci.o dlist_cr.o dlist_da.o dlist_de.o dlist_df.o dlist_di.o dlist_dl.o dlist_ds.o dlist_fi.o dlist_fr.o dlist_ii.o dlist_in.o dlist_is.o dlist_la.o dlist_ne.o dlist_ns.o dlist_pr.o dlist_rm.o
TARGET=libdliste.a


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) > /dev/null 2> /dev/null


dlist_ah.o: dlist_ah.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_ap.o: dlist_ap.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_ci.o: dlist_ci.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_cr.o: dlist_cr.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_da.o: dlist_da.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_de.o: dlist_de.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_df.o: dlist_df.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_di.o: dlist_di.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_dl.o: dlist_dl.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_ds.o: dlist_ds.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_fi.o: dlist_fi.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_fr.o: dlist_fr.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_ii.o: dlist_ii.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_in.o: dlist_in.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_is.o: dlist_is.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_la.o: dlist_la.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_ne.o: dlist_ne.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_ns.o: dlist_ns.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_pr.o: dlist_pr.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


dlist_rm.o: dlist_rm.c dliste.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


install:
	install -m 644 dliste.h $(INCLUDEPATH)
	install -m 644 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET) *.o
