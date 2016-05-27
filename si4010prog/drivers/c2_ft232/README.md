# FT232 C2 bus master
This is a C2 Bus master implementation for the FTDI FT232. Downside of this bus
master is that it is relatively slow. This bus master has been tested to work
with a FT232H. It does not work with the genuine FT232R(L) chip. It seems that
it does work with Chinees FT232RL chips, but no guarantee there.

## Connections
The SI4010 is connected to the FT232 according to the following schematic:

    FT232H                SI4010

       3v3 -------------- VCC
       DTR ----/\1K/\---- C2CLK(/GPIO5)
       CTS ----/\1K/\---- C2DAT(/GPIO4)
       GND -------------- GND

## FTDI device URI
If you have multiple FTDI chips connected to your host or your FTDI doesn't use
the standard USB VID:PID of the FT232H chip you'll have to specify the device
to use. This can be done with the '-d' option of si4010prog.

The device URI can have one of the following formats:
 * ft232://d:<bus#>/<device#>
   Bus and device number as show by lsusb and used in /dev/bus/usb/. Numbers
   are prepended with 0s to 3 digits (eg. "003/001").
 * ft232://i:<vendor>:<product>
   First device with given vendor and product id, ids can be decimal, octal
   (preceded by "0") or hex (preceded by "0x")
 * ft232://i:<vendor>:<product>:<index>
   As above with index being the number of the device (starting with 0) if
   there are more than one
 * ft232://s:<vendor>:<product>:<serial>
   First device with given vendor id, product id and serial string

Some examples:
 * "ftdi://i:0x0403:0x6014" uses the first FT232H device found. This is the
   default
 * "ft232://i:0x0403:0x6014:2" use the third FT232H device connected
 * "ft232://d:003/006" use USB device 6 on bus 3

## Device permissions
By default only root has enough permissions to use the FT232 device. To allow
normal users to use si4010prog with an FT232 you can add an udev rule to change
the device permissions. This can be done by copying 99-ft232h.rules to
/etc/udev/rules.d/.
