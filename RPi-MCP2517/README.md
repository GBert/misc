RPI-I2C/CAN Adapter with optional I2C Bus Extender
==================================================

```
# Determine RPi Version
cat /sys/firmware/devicetree/base/model
```

RPi2B Setup
----------
```
cd /tmp
wget "https://github.com/GBert/misc/raw/master/RPi-MCP2517/bin/mcp2517fd-rasp1+2+3.tar.gz"
cd / ; tar zxvf /tmp/mcp2517fd-rasp1+2+3.tar.gz

cat << EOF > /boot/config.txt
core_freq=250
kernel=ms7/zImage
device_tree=ms7/bcm2709-rpi-2-b.dtb
overlay_prefix=ms7/overlays/
dtoverlay=mcp2517fd-can0
dtparam=interrupt=25
dtparam=oscillator=4000000
dtparam=i2c_arm=on
EOF

reboot
```

RPi3 Setup
----------
```
cd /tmp
wget "https://github.com/GBert/misc/raw/master/RPi-MCP2517/bin/mcp2517fd-rasp1+2+3.tar.gz"
cd / ; tar zxvf /tmp/mcp2517fd-rasp1+2+3.tar.gz

cat << EOF > /boot/config.txt 
core_freq=250
kernel=ms7/zImage
device_tree=ms7/bcm2710-rpi-3-b.dtb
overlay_prefix=ms7/overlays/
dtoverlay=mcp2517fd-can0
dtparam=interrupt=25
dtparam=oscillator=4000000
dtparam=i2c_arm=on
EOF

reboot
```

Result
------
```
dmesg | egrep -i "can|spi" 
[    4.821482] CAN device driver interface
[    4.840982] mcp25xxfd spi0.0 can0: MCP2517 successfully initialized.
```

CAN Setup
---------
```
# setup 250kBaud
#  time quantum 250ns time quanta 16 => 16*250ns = 4us => 250kb
ip link set can0 up type can fd off tq 250 prop-seg 6 phase-seg1 7 phase-seg2 2 sjw 1 berr-reporting off restart-ms 100
ip -s -d link show can0
```
