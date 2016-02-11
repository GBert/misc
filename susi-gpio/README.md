#susi-gpio kernel module

##Using the module
```
insmod susi-gpio susiclk=268 susidat=270 # PI12 and PI14 on BPi
mknod /dev/susi-gpio c 181 0
chmod 666 /dev/susi-gpio
```
##Links
* http://www.nmra.org/sites/default/files/standards/sandrp/pdf/ti-9.2.3_susi_05_03.pdf
