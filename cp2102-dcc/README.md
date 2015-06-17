SRCPD using CP2102
==================

timing
------

```
# calculate timing
# baud = (24 000 000 / Prescaleer) / (65536 - x)

bc
obase=16
# DCC
65536-24000000*.000058
FA90.00000
# Maerklin
65536-24000000*.000026
FD90.00000

# using http://cp210x-program.sourceforge.net/
# setting 19200 -> 17241 (DCC)

# save eeprom first

sudo ./cp210x-program -f eeprom.hex

# [baudrate table]
#  38400 = FD8F, FF15, 1 #   38400 Baud, 470 us
#  19200 = FB1E, FE2B, 1 #   19200 Baud, 938 us
# ->
# [baudrate table]
#  38400 = FD90, FF15, 1 #   38462 Baud, 470 us
#  19200 = FA90, FE2B, 1 #   17241 Baud, 938 us

sudo ./cp210x-program -p -F eeprom.hex -w --set-baudrate 19200:FA90,FE2B,1 --set-baudrate 38400:FD90,FF15,1
# testing DCC -> 8.6 kHz square wave
stty -F /dev/ttyUSB0 19200
echo UUUUUUU > /dev/ttyUSB0
