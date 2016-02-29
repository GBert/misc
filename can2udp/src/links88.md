# LinkS88 Zugriff

Beispiel:
Laenge Bus 1 (Speicher 02 Laenge 00 03)
```
 (2016-02-27 19:53:24.899901)  can0  TX - -  00000300   [8]  53 38 40 39 0B 02 00 03
### Quittung Speicher 02 (Laenge Bus 1) OK 01
 (2016-02-27 19:53:24.900510)  can0  RX - -  00019B01   [7]  53 38 40 39 0B 02 01
```

```
 1  Auswertung einzeln => 0  Tastaturmatrix => 1
 2  Laenge Bus 1 (RJ45-1) max 31
 3  Laenge Bus 2 (RJ45-1) max 31
 4  Laenge Bus 3 (6-Polig) max 31
 5  Zykluszeit Bus 1 [ms] min 10 default 100 max 1000
 6  Zykluszeit Bus 2 [ms] min 10 default 100 max 1000
 7  Zykluszeit Bus 3 [ms] min 10 default 100 max 1000
 8  Bit Zeit S88     [us] min 100 default 167
 9  Zykluszeit 1 - 16 [ms] min 10 default 100
10  Zykluszeit Tastatur [ms] min 10 default 100
11  Spalten Tastatur max 8
12  Zeilen  Tastatur max 8
```

```
Kontakte (Gleis): 1-16
Tasteradressen: 101-164
Bus 1: 1001 - 1496 (z.B. 1005 = 1. Modul, Kontakt 5)
Bus 2: 2001 - 2496
Bus 3: 3001 - 3496
```

```
root@BananaPi:~# wake-up-links88 -c "B3=1"
bus 3 length 1
02:58:11.930       CAN->  CANID 0x00300300   [0]
02:58:11.941       CAN<-  CANID 0x00360300   [0]
02:58:11.941       CAN->  CANID 0x0031533C   [8] 47 43 7b 11 01 27 00 10  GC{..'..
02:58:11.942       CAN->  CANID 0x00379B01   [8] 53 38 40 39 01 00 00 40  S8@9...@
Found LinkS88 ID: 0x39
   sending wake-up sequence
02:58:11.942       CAN<-  CANID 0x00360300   [5] 53 38 40 39 e4           S8@9.
02:58:11.952       CAN<-  CANID 0x00360300   [5] 53 38 40 39 11           S8@9.
02:58:11.963       CAN<-  CANID 0x00010300   [7] 53 38 40 39 0c 00 39     S8@9..9
02:58:11.973       CAN<-  CANID 0x00000300   [8] 53 38 40 39 0b 04 00 01  S8@9....
02:58:11.974       CAN->  CANID 0x00019B01   [7] 53 38 40 39 0b 04 01     S8@9...


root@BananaPi:~# wake-up-links88 -c "B3=1"
bus 3 length 1
02:58:44.764       CAN->  CANID 0x00300300   [0]
02:58:44.775       CAN<-  CANID 0x00360300   [0]
02:58:44.775       CAN->  CANID 0x0031533C   [8] 47 43 7b 11 01 27 00 10  GC{..'..
02:58:44.775       CAN->  CANID 0x00319B01   [8] 53 38 40 39 01 00 00 40  S8@9...@
inserting known LinkS88 ID 0x39
```

### postives ACK
wake-up-links88 -c "B3=1"


```
 (2016-02-29 19:35:49.553238)  can0  TX - -  00000300   [8]  53 38 40 39 0B 04 00 01
 (2016-02-29 19:35:49.553835)  can0  RX - -  00019B01   [7]  53 38 40 39 0B 04 01
```
### negatives ACK 
wake-up-links88 -c "B3=55"
```
 (2016-02-29 19:36:18.693424)  can0  TX - -  00000300   [8]  53 38 40 39 0B 04 00 37
 (2016-02-29 19:36:18.694010)  can0  RX - -  00019B01   [7]  53 38 40 39 0B 04 00
```

