#!/bin/sh

# nodejs
./scripts/feeds update node
rm ./package/feeds/packages/node
rm ./package/feeds/packages/node-*
./scripts/feeds install -a -p node
make defconfig

scripts/feeds update -a ; git pull
scripts/feeds install -a -p luci

./scripts/feeds install libxml2 libpam libftdi1 liblzma libnetsnmp libwebsockets php8 php8-cgi libsqlite3
./scripts/feeds install canutils can2udp can-monitor ms1relay maecan-c maecan-server mrsystem railcontrol z21emu libpcap perl-cgi simple-com nano joe websockify minicom picocom dfu-util easy-loader io lsof tree xxd shellinabox usbutils
./scripts/feeds install openssh-sftp-server python-evdev libpcap-dev libxml2-dev
