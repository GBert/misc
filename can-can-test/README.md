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


Backup
------
```
minimal Standard Frame Format (SFF) Bits = 47 + n*8
minimal Extended Frame Format (EFF) Bits = 67 + n*8

min SFF SLCAN API : t1230\r  (6 chars)
max EFF SLCAN API : T1234567881122334455667788\r  (27 chars)

UART 1 Byte = 1 startbit + 8 bits + 2 stopbit = 11 bits
min SFF UART time =  6 * 11 =  66 Bits -> 2MBaud -> 33us   | min SFF @ 1MBit ->  47 us
min SFF UART time = 27 * 11 = 292 Bits -> 2MBaud -> 146us  | max EFF @ 1MBit -> 131 us

CPU Cycles for min SFF@1MBit with 16 MHz = 4 * (47 + 0*8) = 4 *  47 cycles
CPU Cycles for max EFF@1MBit with 16 MHz = 4 * (67 + 8*8) = 4 * 131 cycles

PUTC            5 cycles
BIN2ASC        25 cycles
UART Tx IRQ  (15) cycles

Estimation for EFF (DLC 0) and SFF (DLC8)
-----------------------------------------

SFF t1230

            6 6 25 6 6  = 49 cycles + 6 * 15 cycles (IRQ) = 139 Cycles
SFF no Data t 1 23 0\r


EFF T1234567881122334455667788
            6 6 13 25 25 25 6 25 25 25 25 25 25 25 25 6  = 314 cycles + 27 * 15 cycles (IRQ) = 717 cycles 
EFF 8 Bytes T 1 2  34 56 78 8 11 22 33 44 55 66 77 88 \r

```
