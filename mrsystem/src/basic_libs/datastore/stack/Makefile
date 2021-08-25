INCLUDEPATH=../../include
LIBPATH=../../lib
ARFLAGS=rc
OBJS=stack_cr.o stack_de.o stack_in.o stack_is.o stack_po.o stack_pu.o
TARGET=libstack.a


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) > /dev/null 2> /dev/null


stack_cr.o: stack_cr.c stack.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


stack_de.o: stack_de.c stack.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


stack_in.o: stack_in.c stack.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


stack_is.o: stack_is.c stack.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


stack_po.o: stack_po.c stack.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


stack_pu.o: stack_pu.c stack.h
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


install:
	install -m 644 stack.h $(INCLUDEPATH)
	install -m 644 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET) *.o
