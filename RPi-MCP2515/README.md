RPI-CAN/I2C Adapter mit galv. Trennung
======================================

![RPI- MCP2515](https://github.com/GBert/misc/raw/master/RPi-MCP2515/pictures/rpi-mcp2515_01_s.jpg)

[Reichelt Liste (ohne P82B715P)](https://www.reichelt.de/my/1344615)

Nutzung
-------

```
sudo apt-get update
sudo apt-get install can-utils
sudo apt-get upgrade
```
/boot/config.txt editieren
\# uncomment
dtparam=spi=on
\# add
dtoverlay=mcp2515-can0,oscillator=16000000,interrupt=25
dtoverlay=spi-bcm2835-overlay
```
reboot
```
sudo shutdown -h now
```
CAN Interface konfigurieren
```
ip link set can0 up type can bitrate 250000 restart-ms 100
```

can2lan nutzen
```
sudo apt-get install lighttpd
# zumindest geraet.vrs unter /var/www/html/config/ anlegen:
mkdir /var/www/html/config/
cat << EOF > /var/www/html/config/geraet.vrs
[geraet]
version
 .minor=1
geraet
 .sernum=1
 .hardvers=RPi,3
EOF
```
can2lan herunterladen und starten
```
cd
git clone https://github.com/GBert/railroad.git
cd rairoad/can2udp/src
make
./can2lan -mvf -c /var/www/html
```
