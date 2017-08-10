Banana Pi USB OTG Serial
========================

Kernel
------
```
CONFIG_MUSB_PIO_ONLY=y
CONFIG_NOP_USB_XCEIV=y
CONFIG_USB_F_ACM=y
CONFIG_USB_F_OBEX=y
CONFIG_USB_F_SERIAL=y
CONFIG_USB_GADGET=y
CONFIG_USB_G_SERIAL=y
# CONFIG_USB_MUSB_GADGET is not set
CONFIG_USB_MUSB_HDRC=y
# CONFIG_USB_MUSB_HOST is not set
CONFIG_USB_MUSB_SUNXI=y
CONFIG_USB_OTG=y
CONFIG_USB_PHY=y
CONFIG_USB_U_SERIAL=y
```

dmesg output
```
[    0.206945] sun4i-usb-phy 1c13400.phy: could not find pctldev for node /soc@01c00000/pinctrl@01c20800/usb0_id_detect_pin@0, deferring probe
[    1.126419] ehci_hcd: USB 2.0 'Enhanced' Host Controller (EHCI) Driver
[    1.138719] ohci_hcd: USB 1.1 'Open' Host Controller (OHCI) Driver
[    1.150755] usbcore: registered new interface driver usb-storage
[    1.531631] ehci-platform 1c14000.usb: EHCI Host Controller
[    1.537251] ehci-platform 1c14000.usb: new USB bus registered, assigned bus number 1
[    1.545209] ehci-platform 1c14000.usb: irq 28, io mem 0x01c14000
[    1.569567] ehci-platform 1c14000.usb: USB 2.0 started, EHCI 1.00
[    1.575879] usb usb1: New USB device found, idVendor=1d6b, idProduct=0002
[    1.582720] usb usb1: New USB device strings: Mfr=3, Product=2, SerialNumber=1
[    1.589974] usb usb1: Product: EHCI Host Controller
[    1.594850] usb usb1: Manufacturer: Linux 4.4.79 ehci_hcd
[    1.600280] usb usb1: SerialNumber: 1c14000.usb
[    1.605676] hub 1-0:1.0: USB hub found
[    1.614260] ehci-platform 1c1c000.usb: EHCI Host Controller
[    1.619964] ehci-platform 1c1c000.usb: new USB bus registered, assigned bus number 2
[    1.627854] ehci-platform 1c1c000.usb: irq 32, io mem 0x01c1c000
[    1.649527] ehci-platform 1c1c000.usb: USB 2.0 started, EHCI 1.00
[    1.655807] usb usb2: New USB device found, idVendor=1d6b, idProduct=0002
[    1.662631] usb usb2: New USB device strings: Mfr=3, Product=2, SerialNumber=1
[    1.669867] usb usb2: Product: EHCI Host Controller
[    1.674742] usb usb2: Manufacturer: Linux 4.4.79 ehci_hcd
[    1.680149] usb usb2: SerialNumber: 1c1c000.usb
[    1.685547] hub 2-0:1.0: USB hub found
[    1.694094] ohci-platform 1c14400.usb: Generic Platform OHCI controller
[    1.700786] ohci-platform 1c14400.usb: new USB bus registered, assigned bus number 3
[    1.708687] ohci-platform 1c14400.usb: irq 29, io mem 0x01c14400
[    1.773687] usb usb3: New USB device found, idVendor=1d6b, idProduct=0001
[    1.780506] usb usb3: New USB device strings: Mfr=3, Product=2, SerialNumber=1
[    1.787722] usb usb3: Product: Generic Platform OHCI controller
[    1.793941] usb usb3: Manufacturer: Linux 4.4.79 ohci_hcd
[    1.799338] usb usb3: SerialNumber: 1c14400.usb
[    1.804777] hub 3-0:1.0: USB hub found
[    1.813347] ohci-platform 1c1c400.usb: Generic Platform OHCI controller
[    1.820050] ohci-platform 1c1c400.usb: new USB bus registered, assigned bus number 4
[    1.827964] ohci-platform 1c1c400.usb: irq 33, io mem 0x01c1c400
[    1.893697] usb usb4: New USB device found, idVendor=1d6b, idProduct=0001
[    1.900511] usb usb4: New USB device strings: Mfr=3, Product=2, SerialNumber=1
[    1.907726] usb usb4: Product: Generic Platform OHCI controller
[    1.913783] usb usb4: Manufacturer: Linux 4.4.79 ohci_hcd
[    1.919180] usb usb4: SerialNumber: 1c1c400.usb
[    1.924614] hub 4-0:1.0: USB hub found
[    1.933372] usb_phy_generic.0.auto supply vcc not found, using dummy regulator
[    1.941274] musb-hdrc: ConfigData=0xde (UTMI-8, dyn FIFOs, bulk combine, bulk split, HB-ISO Rx, HB-ISO Tx, SoftConn)
[    1.941293] musb-hdrc: MHDRC RTL version 0.0 
[    1.941312] musb-hdrc: 11/11 max ep, 5184/8192 memory
[    1.941525] musb-hdrc musb-hdrc.1.auto: MUSB HDRC host driver
[    1.947460] musb-hdrc musb-hdrc.1.auto: new USB bus registered, assigned bus number 5
[    1.955531] usb usb5: New USB device found, idVendor=1d6b, idProduct=0002
[    1.962351] usb usb5: New USB device strings: Mfr=3, Product=2, SerialNumber=1
[    1.969583] usb usb5: Product: MUSB HDRC host driver
[    1.974545] usb usb5: Manufacturer: Linux 4.4.79 musb-hcd
[    1.979960] usb usb5: SerialNumber: musb-hdrc.1.auto
[    1.985760] hub 5-0:1.0: USB hub found
```

configfs
--------
```
#!/bin/sh
#
set -x
mount -t configfs none /sys/kernel/config

CONFIGFS=/sys/kernel/config/usb_gadget
GADGET=$CONFIGFS/g1

# Create a gadget
mkdir $GADGET
# Set the VID/PID
echo 0x0525 > $GADGET/idVendor
echo 0xa4a7 > $GADGET/idProduct

# Set strings - 0x409 is a magic number in the USB spec meaning "English (United States)"
mkdir $GADGET/strings/0x409
echo "00000001" > $GADGET/strings/0x409/serialnumber
echo "CdB" > $GADGET/strings/0x409/manufacturer
echo "CC-Schnitte" > $GADGET/strings/0x409/product

# Create a config called 'c.1'
mkdir $GADGET/configs/c.1
mkdir $GADGET/configs/c.1/strings/0x409
echo "Config 1" > $GADGET/configs/c.1/strings/0x409/configuration
echo 500 > $GADGET/configs/c.1/MaxPower

# The 'acm' part of the directory name is important, they set the type of function.
mkdir $GADGET/functions/acm.usb0
ln -s $GADGET/functions/acm.usb0 $GADGET/configs/c.1

# Finally, enable the gadget by setting its USB Device Controller.
echo "musb-hdrc.1.auto" > $GADGET/UDC
```

