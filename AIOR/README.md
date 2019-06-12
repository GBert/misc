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

Prgrammer Test
--------------

|     |Si8605|
|-----|------|
|MCLR | 3    |
|PGC  | 5    |
|PGD  | 6    |

