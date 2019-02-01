A5-V11 8 MByte flash
====================

Lede
----
```
git clone https://github.com/lede-project/source.git lede
cd lede
scripts/feeds update -a
scripts/feeds install luci-app-openvpn
scripts/feeds install luci-proto-3g
scripts/feeds install rpcd-mod-rrdns
wget https://github.com/GBert/misc/raw/master/a5-v11-8m/mk.patch
patch -p1 < mk.patch
wget https://github.com/GBert/misc/raw/master/a5-v11-8m/config_a5v11-16m_pkumm
cp config_a5v11-16m_pkumm .config
make dirclean
make menuconfig
make clean ; make -j9
```

Create image
------------
```
# mtd0.img u-boot
# mtd1.img u-boot-Env
# mtd2.img factory

# 8 Mbyte
cat u-boot.img u-boot-env.img factory.img lede-ramips-rt305x-a5-v11-8M-squashfs-sysupgrade.bin > combined_image.img
dd if=/dev/zero ibs=1k count=8192 | tr "\000" "\377" >paddedFile_8m.bin
dd if=combined_image.img of=paddedFile_8m.bin conv=notrunc    
sudo flashrom -c MX25L6406E/MX25L6408E -p ch341a_spi -w paddedFile_8m.bin

# 16 MByte
cat u-boot.img u-boot-env.img factory.img lede-ramips-rt305x-a5-v11-16M-squashfs-sysupgrade.bin > combined_image.img
dd if=/dev/zero ibs=1k count=16384 | tr "\000" "\377" >paddedFile_16m.bin
dd if=combined_image.img of=paddedFile_16m.bin conv=notrunc
sudo flashrom -c MX25L12835F/MX25L12845E/MX25L12865E -p ch341a_spi -w paddedFile_16m.bin
```

Serial connection
-----------------
If you put the PCB on the table with the USB jack to the left and the Ethernet jack to the right (Ralink CPU is on the other side of the board), you see the 4 pads in the bottom-right corner of the PCB. They are (from left to right):

- VCC (3.3V; do not connect)
- TX (connect to RXI on a 3.3V serial USB adapter)
- RX (connect to TXO on a 3.3V serial USB adapter using a 470 Ohm to 1k Ohm resistor)
- GND (connect to GND on a 3.3V serial USB adapter)

Fallback U-Boot update
----------------------

```
setenv ipaddr 192.168.0.199
setenv serverip 192.168.0.9
setenv bootfile openwrt-ramips-rt305x-vmlinux.bin
tftpboot
erase linux
cp.linux
reset
```

Backup
------

supported flash chips by u-boot
```
AT25DF321
AT26DF161
FL016AIF
FL064AIF
MX25L1605D
MX25L3205D
MX25L6406E
MX25L12835F
MX25L25635F
MX25L51245G
S25FL256S
S25FL128P
S25FL129P
S25FL164K
S25FL132K
S25FL032P
S25FL064P
S25FL116K
F25L64QA
F25L32QA
EN25F16
EN25Q32B
EN25F32
EN25F64
EN25Q64
W25Q32BV
W25X32VS
W25Q64BV
W25Q128FV
W25Q256FV
N25Q032A13ESE40F
N25Q064A13ESE40F
N25Q128A13ESE40F
N25Q256A
MT25QL512AB
GD25Q32B
GD25Q64B
GD25Q128C
Pm25LQ032
```

