S88 Interface using FTDI in SYNC-BITBANG mode
=============================================

Why ?:
It's easy to use GPIOs connecting S88-Bus (http://s88udp.de/).
But some boards don't have accessible GPIOs.

With FTDI's synch bitbanging USB to serial converter you can
build a lightweight S88 interface. It's only copying buffers
so it dosn't need much CPU cycles.

The programm sends M\*rklin S88 events on UDP port 15730.
The datagrams are encapsulated CAN-Frames.

Plan:
- debouncing events
