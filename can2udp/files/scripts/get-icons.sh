#!/bin/sh

# get M*rklins CS2 image and extract some useful data
set -e

FILE=cs2update.img
LINK=http://www.maerklin.de/fileadmin/media/service/software-updates/$FILE
MNT_POINT=/tmp/mnt
WEB_DIR=/www

cd /root

set -x

wget $LINK
mkdir -p $MNT_POINT
mount -o loop $FILE $MNT_POINT

cp -r $MNT_POINT/home/cs2/fcticons $WEB_DIR
cp -r $MNT_POINT/home/cs2/icons $WEB_DIR
cp -r $MNT_POINT/home/cs2/magicons_ $WEB_DIR
# cp -r $(MNT_POINT)/home/cs2/spiel $WEB_DIR

umount $MNT_POINT

