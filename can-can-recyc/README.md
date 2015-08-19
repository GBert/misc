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
Notwendige Verbindungen zwischen SLCAN-Platine und Gleisbox
```
    SLCAN Gleisbox
      5V   am Laengsregler 7805
     GND   am Laengsregler 7805
    CANL   tbd
    CANH   tbd
```
Installation (Linux only)
-------------------------
```
# PIC Programmer uebersetzen
mkdir ~/tmp
cd ~/tmp
git clone https://github.com/GBert/misc.git
cd misc/pickle-ftdi/src
make
ln -s pickle p16
mkdir ~/.pickle
cp dotconf/ftdi-ft232 ~/.pickle/config
# PIC Firmware holen
wget http://lnxpps.de/openwrt/wr841/bin/can-can_uart500_can250.hex
# PIC Firmware flashen (Adapter Platine Jumper links)
./p16 lvp p can-can_uart500_can250.hex
# PIC Firmware uerberpruefen
./p16 lvp v can-can_uart500_can250.hex
# Adapter Platine Jumper rechts - fertig
```

CAN Utils
```
cd ~/tmp
git clone https://github.com/linux-can/can-utils
cd can-utils
./autogen.sh
make
sudo make install
```
Jetzt kann der Adapter genutzt werden:
```
sudo slcand -S500000 /dev/ttyUSB0 can0
sudo ifconfig can0 up
```

Test mit angeschlossener Gleisbox
```
# erstes Terminal
candump -tA -xe can0,0:0,#FFFFFFF

#zweites Terminal
cansend can0 00360301#0000000011
cansend can0 00304711#

# Ergebnis aehnlich
candump -tA -xe can0,0:0,#FFFFFFF
 (2015-08-18 11:05:51.927377)  can0  TX - -  00360301   [5]  00 00 00 00 11
 (2015-08-18 11:06:31.302496)  can0  TX - -  00304711   [0]
 (2015-08-18 11:06:31.302651)  can0  RX - -  00319B32   [8]  47 43 DE AD 01 27 00 10
```
Wenn die Gleisbox antwortet ist alles soweit vorbereitet un der Adapter kann verwendet werden.

