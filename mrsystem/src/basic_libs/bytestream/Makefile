INCLUDEPATH=../include
LIBPATH=../lib
OBJS=get_long.o get_int.o set_long.o set_int.o
TARGET=libbytestream.a

%.o: %.c
	$(CC) -Wall -pedantic $(CFLAGS) -c $< -I$(INCLUDEPATH)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+

get_long.o: get_long.c bytestream.h

get_int.o: get_int.c bytestream.h

set_long.o: get_long.c bytestream.h

set_int.o: get_int.c bytestream.h

install: $(TARGET)
	install -m 644 $(TARGET) $(LIBPATH)
	install -m 644 bytestream.h $(INCLUDEPATH)

clean:
	rm -f $(TARGET) $(OBJS)
