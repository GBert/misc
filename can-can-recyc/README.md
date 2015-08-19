CAN CAN Interface Recycling
===========================

```
JP1 (SLCAN Platine)
          -----
   VCC 14 |O O| 13          
       12 |O O| 11          
       10 |O O| 9  PGD(RxD) 
        8 |O O| 7  PGC(TxD) 
        6 |O O| 5  MCLR
        4 |O O| 3  RTS        
   GND  2 |O 0| 1          
          -----

FTDI Verbindungen zur SCLAN Platine
           ---
       DTR |O|  <-> Jumper -> MCLR
       RxT |O|  <-> PGC(TxD)
       TxD |O|  <-> PGD(RxD)
       VCC |O|  <-> VCC
       CTS |O|  <-> RTS
       GND |O|  <-> GND
           ---
```




