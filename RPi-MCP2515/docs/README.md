LTV817 GPIO Test
================

```
RPi is capable of sink 8 mA standard -> use IF 5 mA
Rled = (3.3V - 1.1V(forwarding voltage LED)) / 5mA = 440 Ohm
-> 390 Ohm

IF = (3.3V - 1.1V) / 390 Ohm = 5.64 mA
CTR = 50 %
IC = IF * CTR = 5.6 mA * 0.5 = 2.8 mA

Rce = 5V / 2.8mA = 1.8 kOhm
```


![RPI- MCP2515](https://github.com/GBert/misc/raw/master/RPi-MCP2515/docs/rpi_gpio-ltv817.jpg)


[LTspice Model](https://github.com/GBert/misc/raw/master/RPi-MCP2515/docs/RPi-LTV817.asc)

