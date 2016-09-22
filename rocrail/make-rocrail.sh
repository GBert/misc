#!/bin/sh

set -x
cd /tmp
tar Jxf ~/projekte/rocrail.tar.xz
cd Rocrail
git pull
cd ..
mv ~/projekte/rocrail.tar.xz ~/projekte/rocrail.tar.xz_save
tar Jcf ~/projekte/rocrail.tar.xz Rocrail
cd Rocrail
make version

REVISION=`git rev-list --count HEAD`
VERSION=`git log -n 1 --pretty=format:%H | cut -b-10`

echo "$REVISION $VERSION"

cp common/version.h /home/gerd/projekte/lede/feeds/railroad/rocrail/common
rm -rf .cdtproject .cproject .git .project .settings
cd ..
sed "s/__VERSION__/${REVISION}/" ~/projekte/Makefile.rocrail > Makefile.r1
sed "s/__REVISION__/${VERSION}/" Makefile.r1 > /home/gerd/projekte/lede/feeds/railroad/rocrail/Makefile
mv Rocrail rocrail
tar zcf rocrail-${REVISION}.tar.gz rocrail
mv rocrail-${REVISION}.tar.gz /home/gerd/projekte/lede/dl/
rm -rf rocrail
cd /home/gerd/projekte/lede/feeds/railroad/rocrail/
git commit -a -m "bump revision" && git push
