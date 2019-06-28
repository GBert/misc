AIOR - the All in One Railroad Interface PoC
============================================


Hints
=====

Raspbian ssh
------------
https://www.raspberrypi.org/documentation/remote-access/ssh/
```
touch /boot/ssh
```

Kernel modules
--------------
```
git clone https://github.com/raspberrypi/linux.git
cd linux
make scripts prepare modules_prepare
make -C . M=drivers/net/can
```

CAN
---
/boot/config.txt

```
kernel=zImage
device_tree=ms7/bcm2710-rpi-3-b-plus.dtb
overlay_prefix=ms7/overlays/

dtoverlay=mcp2517fd-can0
dtparam=interrupt=25
dtparam=oscillator=4000000
dtparam=spimaxfrequency=20000000
```

```
# setup 250kBaud
#  time quantum 250ns time quanta 16 => 16*250ns = 4us => 250kb
ip link set can0 up type can fd off tq 250 prop-seg 6 phase-seg1 7 phase-seg2 2 sjw 1 berr-reporting off restart-ms 100
ip -s -d link show can0
```

```
[    3.960113] CAN device driver interface
[    5.011176] mcp25xxfd spi0.0: MCP2517 successfully initialized.
[  543.892050] mcp25xxfd spi0.0: Controller unexpectedly switched from mode 0 to 6
[  543.892207] IPv6: ADDRCONF(NETDEV_CHANGE): can0: link becomes ready
```
'mcp25xxfd spi0.0: Controller unexpectedly switched from mode 0 to 6' is ok
```
#define CAN_CON_MODE_MIXED   0
#define CAN_CON_MODE_CAN2_0  6
```
Programmer Test
--------------

/home/pi/.pickle/config
```
DEBUG=10
DEVICE=RPI2
SLEEP=10
BITRULES=0x5B00
VPP=4
PGM=-1
PGC=14
PGD=15
```

|     |Si8605|
|-----|------|
|MCLR | 3    |
|PGC  | 5    |
|PGD  | 6    |

TODOs
-----

|Function                  | Status   |
|--------------------------|----------|
|RPi I2C                   | untested |
|RPi CAN                   | working  |
|RPi PIC24 Compiler        | working  |
|PIC programming           | working  |
|PIC firmware basic        | blinky   |
|PIC firmware CAN          | TX OK    |
|PIC firmware H-Bridge     |          |
|PIC firmware Track-Signal |          |
|PIC firmware RS485        |          |
|PIC firmware S88          |          |
|PIC firmware SLX          |          |
|PIC firmware Loconet      |          |
|Silabs programming        |          |
|Silabs firmware           |          |
|RDS/mfx                   |          |
|RPi Image                 |          |
