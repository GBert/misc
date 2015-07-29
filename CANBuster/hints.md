#!/bin/sh

# offline footprints

mv ~/.config/kicad/fp-lib-table ~/.config/kicad/fp-lib-table_old
wget  https://raw.githubusercontent.com/KiCad/kicad-library/master/template/fp-lib-table.for-pretty -O ~/.config/kicad/fp-lib-table

mkdir /tmp/pretty/
cd /tmp/pretty
for i in `cut -d"/" -f2 ~/.config/kicad/fp-lib-table | cut -d")" -f1`
do
   git clone --depth==1 https://github.com/KiCad/$i $i
done

find /tmp/pretty -name .git -exec rm -rf {} \;

