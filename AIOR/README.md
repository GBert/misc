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

Prgrammer Test
--------------

|     |Si8605|
|-----|------|
|MCLR | 3    |
|PGC  | 5    |
|PGD  | 6    |

