LTV817 GPIO Test
================

```
RPi is capable of sink 8 mA standard -> use IF 5 mA
Rled = (3.3V - 1.1V(forwarding voltage LED)) / 5mA = 440 Ohm
-> 390 Ohm

IF = (3.3V - 1.1V) / 390 Ohm = 5.64 mA
CTR = 50 %
IC = IF * CTR = 5.6 mA * 0.5 = 2.8 mA

Rce  = 5V  / 2.8mA = 1.8 kOhm
Rce' = 12V / 2.8mA = 4.4 kOhm -> 4.7KOhm # frequency still OK ?
```

5kHz schould be fine:

![RPI- MCP2515](https://github.com/GBert/misc/raw/master/RPi-MCP2515/docs/rpi_gpio-ltv817.jpg)


[LTspice Model](https://github.com/GBert/misc/raw/master/RPi-MCP2515/docs/RPi-LTV817.asc)

IO -> RPi
---------
```
IF = 5 mA
5V
R1  = (5V - 1.1V) / 5mA = 780 -> 680 Ohm

12V
R1' = (12 - 1.1V) / 5mA = 2.2 kOhm

CTR = 50%

R2 = 3.3V / 2.5mA = 1320 = 1k2 kOhm
```
