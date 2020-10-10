RPI-I2C/CAN Adapter mit galv. Trennung
======================================

[![RPI- MCP2515](https://github.com/GBert/misc/raw/master/RPi-MCP2515/pictures/rpi-mcp2515_01_s.jpg)](https://github.com/GBert/misc/raw/master/RPi-MCP2515/pictures/rpi-mcp2515_01.jpg)

Hinweis
-------
In der Platinen-Version 1.0 hatte sich ein Fehler eingeschlichen so das der Längsregler MC7805 spiegelverkehrt eingebaut werden musste. Das ist mit der Version >= 1.1 behoben, d.h. das der Längsregler wie auf dem Bestückungsdruck ersichtlich eingebaut werden muss.

[Reichelt Liste (ohne P82B715P)](https://www.reichelt.de/my/1344615) Variante mit galvanischer Trennung und externer Spannungsversorgung

Bauteilliste
------------
(galv. Trennung mit ISO1050)
```
C1,C2	22pF
C3-C9	100nF
D1	LED 2mA
P1	Buchsenleiste 2x20
P6	Steckerleiste 1x2
P9	Groove Buchse gewinkelt
P17	Steckerleiste 1x2
R1-R3	4k7 Ohm
R5	1k5 Ohm
R13	120 Ohm
R14,R15	3k3 Ohm
R24,R25	3k3 Ohm
U1	MCP2515
U3	P82B715
U6	ISO1050
U7	LM7805
Y1	16MHz Quarz
```

Aufbau
------
Die CAN-Anbindung erfolgt bei der Nutzung des ISO1050 über P7 mit VCOM (7-24V), GND, CANL und CANH.

Wird ein galv. getrennter DCDC Konverter verwendet so sind nur GND, CANL und CANH zu verbinden. 

Sollte ein MCP2562 eingesetzt werden erfolgt die Anbindung über P3. 

Nutzung
-------

Mit dem I2C Interface kann man die preiswerten [PI02](http://wiki.rocrail.net/doku.php?id=gca_pi02-de) nutzen.
pio-i2c-udp setzt Events in das M\*rklin Format um und stellt sich als S88 dar.

CAN Nutzung vorbereiten
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install can-utils
```
/boot/config.txt editieren
```
; uncomment
dtparam=spi=on
; add
dtoverlay=mcp2515-can0,oscillator=16000000,interrupt=25
```
reboot
```
sudo shutdown -r now
```
CAN Interface konfigurieren
```
sudo ip link set can0 up type can bitrate 250000 restart-ms 100
```
bzw: die Datei /etc/network/interfaces.d/can0 wie folgt anlegen:
```
auto can0
iface can0 inet manual
        pre-up /sbin/ip link set $IFACE type can bitrate 250000 restart-ms 100
        up /sbin/ifconfig $IFACE up
        down /sbin/ifconfig $IFACE down
```
can2lan nutzen
```
sudo apt-get install lighttpd
# zumindest geraet.vrs unter /var/www/html/config/ anlegen:
sudo mkdir -p /var/www/html/config/
sudo cat << EOF > /var/www/html/config/geraet.vrs
[geraet]
version
 .minor=1
geraet
 .sernum=1
 .hardvers=RPi,3
EOF

```
can2lan herunter laden und starten
```
cd
sudo apt-get install git libpcap-dev
git clone https://github.com/GBert/railroad.git
cd railroad/can2udp/src
make
sudo cp can2lan /usr/sbin/
sudo cp can-monitor /usr/bin/
sudo cp ../files/maerklin/config/gleisbild.cs2 /var/www/html/gleisbild.cs2

# Debian  Init Skript installieren und testen
sudo cp ../debian/can2lan.init /etc/init.d
sudo /etc/init.d/can2lan.init start
sudo /etc/init.d/can2lan.init status
sudo /etc/init.d/can2lan.init stop

sudo update-rc.d can2lan.init defaults

# oder manuell im Vordergrund start
can2lan -mvf -c /var/www/html
```
