TARGET=testlib
OBJS=main.o
LOCALLIBS=-lmr_can -lm -lbytestream

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o $@ $(OBJS) $(LDLIBS) $(LOCALLIBS)

main.o: main.c

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
