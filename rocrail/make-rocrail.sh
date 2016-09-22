#!/bin/sh

TMPDIR=/tmp
HOME=/home/gerd/projekte
LEDEDIR=${HOME}/lede

set -x
cd ${TMPDIR}
tar Jxf ${HOME}/rocrail.tar.xz
cd Rocrail
git pull
cd ..
mv ${HOME}/rocrail.tar.xz ${HOME}/rocrail.tar.xz_save
tar Jcf ${HOME}/rocrail.tar.xz Rocrail
cd Rocrail
make version

REVISION=`git rev-list --count HEAD`
VERSION=`git log -n 1 --pretty=format:%H | cut -b-10`

cp common/version.h ${LEDEDIR}/feeds/railroad/rocrail/common
rm -rf .cdtproject .cproject .git .project .settings
cd ..
sed "s/__VERSION__/${REVISION}/" ${HOME}/Makefile.rocrail > Makefile.r1
sed "s/__REVISION__/${VERSION}/" Makefile.r1 > ${LEDEDIR}/feeds/railroad/rocrail/Makefile
mv Rocrail rocrail
tar zcf rocrail-${REVISION}.tar.gz rocrail
mv rocrail-${REVISION}.tar.gz ${LEDEDIR}/dl/
rm -rf rocrail
cd ${LEDEDIR}/feeds/railroad/rocrail/
git commit -a -m "bump revision" && git push
