Maerklin Motorla Decoder -> CS2 CAN
===================================

Setup
-----
CAN on B8 & B9
Sniffer on B4

```
/*
      B12        MICRO USB         GND
      B13                          GND
      B14                          3V3
      B15   RESET    GND    GND  RESET
      A8    BUTTON  *BOOT1 *BOOT0  B11
      A9             3V3    3V3    B10
      A10                           B1
      A11                           B0
      A12                           A7
      A15                           A6
      B3       STM32F103C8T6        A5
Snif  B4                            A4
      B5                            A3
      B6                            A2
      B7              8M            A1
CANRX B8           32768            A0
CANTX B9                           C15
      5V       PC13     POWER      C14
      GND      LED      LED        C13
      3V3           SWD           VBAT
             3V3 DIO  DCLK GND
                 PA13 PA14
```

Typical sequence
----------------
```
     56 ms: MM2 A= 60, F=0, D= 0, X= 7 F4 aus <REP> <REP> <REP>
     83 ms: MM2 A= 60, F=0, D= 0, X=10 V      <REP> <REP> <REP>
    109 ms: MM2 A= 24, F=0, D= 0, X= 7 F4 aus <REP> <REP> <REP>
    135 ms: MM2 A= 24, F=0, D= 0, X=10 V      <REP> <REP> <REP>
    161 ms: MM2 A= 78, F=0, D= 0, X= 3 F1 aus <REP> <REP> <REP>
    188 ms: MM2 A= 78, F=0, D= 0, X=10 V      <REP> <REP> <REP>
    214 ms: MM2 A= 72, F=0, D= 0, X= 3 F1 aus <REP> <REP> <REP>
    240 ms: MM2 A= 72, F=0, D= 0, X=10 V      <REP> <REP> <REP>
```
