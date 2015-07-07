MCU=16f1614
CC=sdcc
CFLAGS=--use-non-free -mpic14 -p$(MCU) #-DDEBUG=1
CLIB=

MAIN=latency-test
TARGET=$(MAIN).hex
SOURCE=$(MAIN).c
OBJECT=$(SOURCE:.c=.o)
# HEADER=$(SOURCE:.c=.h)

build:$(TARGET)

$(TARGET):$(OBJECT)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT) -o $(TARGET) $(CLIB) -m$(MCU)

$(OBJECT):$(HEADER) Makefile

install:build

uninstall:clean

clean:
	find . -regextype posix-extended -regex ".*\.(o|asm|cod|err|hex|log|lst)" -delete

%.o:%.c
	@echo -n "[CC]   "
	$(CC) $(CFLAGS) -c $< -o $@

upload: ${TARGET}
	pk2cmd -B/usr/share/pk2/ -PPIC$(MCU) -F$(TARGET) -M -R -T

power: ${TARGET}
	pk2cmd -B/usr/share/pk2/ -PPIC$(MCU) -T -R

serial: ${TARGET}
	./pk2serial -r -p -b 57600
