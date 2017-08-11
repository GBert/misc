#!/bin/sh
#
# set -x
mount -t configfs none /sys/kernel/config

CONFIGFS=/sys/kernel/config/usb_gadget
GADGET=$CONFIGFS/g1

# Create a gadget
mkdir $GADGET
# Set the VID/PID - this is a pid.codes test code. I'll get a proper one later
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

# Create ACM function and add them to config 'c.1'
# The 'acm' part of the directory name are important, they set the type of function
mkdir $GADGET/functions/acm.usb0
ln -s $GADGET/functions/acm.usb0 $GADGET/configs/c.1

# Finally, enable the gadget by setting its USB Device Controller. The musb-hdrc.1.auto is the name of the (only) UDC on the Banana Pi
echo "musb-hdrc.1.auto" > $GADGET/UDC

