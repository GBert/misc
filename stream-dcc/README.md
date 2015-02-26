Stream-DCC PoC
==============

goal: cheap DCC interface with PIC16F1704 / inline programmer based on CP2104

Why PIC16F1704 ?
----------------
- UART
- CCP
- accurate internal clock -+1%
- 14 pin PDIP
- ADC (feedback)
- cheap
- fits perfect

How ?
-----
create an universal bit stream device: UART -> bit stream
maybe sniffer for feedback

Links
-----
http://www.nicolinux.eu/2014/08/raspdcc-the-software-for-the-pic/

