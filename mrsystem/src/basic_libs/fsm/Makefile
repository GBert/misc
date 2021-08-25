TARGET=libfsm.a
ARFLAGS=rc
OBJS=fsm_create.o fsm_destroy.o fsm_init.o fsm_exit.o fsm_do.o
INCLUDEPATH=../include
LIBPATH=../lib


%.o: %.c
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $< -I$(INCLUDEPATH)


all: $(TARGET)


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) > /dev/null 2>/dev/null


fsm_create.o: fsm_create.c fsm.h

fsm_destroy.o: fsm_destroy.c fsm.h

fsm_init.o: fsm_init.c fsm.h

fsm_exit.o: fsm_exit.c fsm.h

fsm_do.o: fsm_do.c fsm.h


install: $(TARGET)
	install -m 644 fsm.h $(INCLUDEPATH)
	install -m 644 $(TARGET) $(LIBPATH)


clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
