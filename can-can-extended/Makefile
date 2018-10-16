CFLAGS ?= -g -O2 -Wall -Wextra -D_FORTIFY_SOURCE=2 -fstack-protector --param ssp-buffer-size=4

all: read-dev-usbmon

read-dev-usbmon: read-dev-usbmon.c

.PHONY: all clean install-home install install-udevrule uninstall
clean:
	rm -f read-dev-usbmon

