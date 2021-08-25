#!/bin/sh

insmod susi-gpio susiclk=267 susidat=269 # BPi PI11 and PI13
mknod /dev/susi-gpio c 181 0
chmod 666 /dev/susi-gpio
