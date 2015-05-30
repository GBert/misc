[CAN-CAN](http://wiki.kewl.org/dokuwiki/projects:can-can) Performance Test
========================
on Beaglebone Black

using [Pickle](http://wiki.kewl.org/dokuwiki/projects:pickle) to programm the PIC18F25K80

SLCAN (CAN-CAN) is connected via PL2303HX (allowing 4MBit UART)

Result
------

CAN-CAN and PIC18F@64MHz can forward all CAN messages @1Mbaud without any loss if UART is fast enough.
A PIC18F@16MHz is able to handle roughly 50% percent of a fully saturated CAN@1MBit.

Conclusion
----------
CAN-CAN and PIC18F@64MHz is at 50-60% workload on a saturated CAN@1Mbaud.
PIC18F@64MHz at CAN@250k has plenty of free CPU cycles.
