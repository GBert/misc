DEST=/usr/local/bin/

LDFLAGS =
INCLUDES = -Ishared/
DEFINES =

CFLAGS = -c -O2 ${INCLUDES} ${DEFINES} -Wall -W \
         -Werror \
         -Wunused -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast \
         -Wcast-align -Wwrite-strings -Waggregate-return \
         -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
         -Wnested-externs -Winline

OBJS = intelhex.o main.o picdata.o shared/serial.o

all:		bootloader
		@echo "### $@ built."

bootloader:		${OBJS}
		@echo "### Linking $@..."
		@${CC} -o $@ ${OBJS} ${LDFLAGS}
		@echo "### $@ built."

clean:		
		@rm -f core bootloader ${OBJS}

.c.o:
		@echo "### Compiling $<..."
		@${CC} ${CFLAGS} ${DEFS} $< -o $@

install: all
		@echo "### Install..."
		install --owner root --mode=a+rx,u+ws bootloader ${DEST}
		@echo "### Install done."
