Maerklin Motorla Decoder -> CS2 CAN
===================================

Setup
-----
CAN on B8 & B9
Motorola Input B3

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
MM    B3       STM32F103C8T6        A5
      B4                            A4
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
