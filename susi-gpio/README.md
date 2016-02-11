#c2tool-gpio module

This kernel module is used in conjunction with c2t-new (modified c2tool from Dirk Eibach).

##Using the module
```
insmod c2tool-gpio c2ck=19 c2d=20 # using GPIO 19&20`
mknod /dev/c2tool-gpio c 180 0
chmod 666 /dev/c2tool-gpio
```
##Links
* [C2 Interface Specification](http://www.silabs.com/Support%20Documents/TechnicalDocs/C2spec.pdf)
