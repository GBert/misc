KiCad
=====

Offline footprints/3D
---------------------
```
mv ~/.config/kicad/fp-lib-table ~/.config/kicad/fp-lib-table_old
wget  https://raw.githubusercontent.com/KiCad/kicad-library/master/template/fp-lib-table.for-pretty -O ~/.config/kicad/fp-lib-table

mkdir /tmp/pretty/
cd /tmp/pretty
for i in `cut -d"/" -f2 ~/.config/kicad/fp-lib-table | cut -d")" -f1` ; do ; git clone --depth=1 https://github.com/KiCad/$i $i ; done

find /tmp/pretty -name .git -exec rm -rf {} \;

cp -rv /tmp/pretty/* /usr/local/share/kicad/modules
```

Using
-----

Howto fix cursor warp on X forwarding ?

Use "Do not center and warp cursor on zoom" in preference

How do I make PCB mounting holes?

A hole can be seen as a footprint, with only one pad. You can add one (or many) footprint
(right toolbar, button "add module") like the "1pin" footprint existing in library. Edit
the pad in order to have the correct size (pad diameter and drill value).

