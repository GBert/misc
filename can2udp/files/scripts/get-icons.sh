#!/bin/sh

# get M*rklins CS2 image and extract some useful data
set -e

FILE=cs2update.img
FS_TYPE=reiserfs
LINK=https://cloud.maerklin.de/owncloud/index.php/s/s9VYomomybeHTnD/download
MNT_POINT=/tmp/mnt
WEB_DIR=/www

cd /root

set -x

wget $LINK -O $FILE
mkdir -p $MNT_POINT
mount -o loop -t $FS_TYPE $FILE $MNT_POINT

cp -r $MNT_POINT/home/cs2/fcticons $WEB_DIR
cp -r $MNT_POINT/home/cs2/icons $WEB_DIR
cp -r $MNT_POINT/home/cs2/magicons_ $WEB_DIR
mkdir -p $WEB_DIR/update
cp $MNT_POINT/home/cs2/update/*.bin $WEB_DIR/update
cp $MNT_POINT/home/cs2/update/*.ms2 $WEB_DIR/update
# cp -r $MNT_POINT/home/cs2/spiel $WEB_DIR
cp -r $MNT_POINT/home/cs2/doc $WEB_DIR

umount $MNT_POINT

