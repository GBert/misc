TARGET=magtest
OBJS=magtest.o ../magnetartikel.o
LOCALLIBS=-lmr_ipc -lmr_can -lcs2_parse -lfsm -lm -lbytestream -lmrconfig -linipars -lscanner -lmap -lmenge -lavl -ldliste -lz

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

magtest.o: magtest.c

../magnetartikel.o: ../magnetartikel.c ../magnetartikel.h
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -o $@ -c $<

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
