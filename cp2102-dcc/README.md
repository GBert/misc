SRCPD using CP2102
==================

timing
------

```

bc -l
obase=16
65536-24000000*.000058
^D

sudo ./cp210x-program -f eeprom.hex
sudo ./cp210x-program -p -F eeprom.hex -w --set-baudrate 19200:FA90,FE2B,1
# testing
stty -F /dev/ttyUSB0 19200
echo UUUUUUU > /dev/ttyUSB0
```
