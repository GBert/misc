RPI-I2C/CAN Adapter with optional I2C Bus Extender
==================================================

RPi2 Setup
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

