;------------------------------------------------------------------------------
;
;   The Wellington Boot Loader for PIC18
;
;   Device Specific Constants
;
;   Copyright (c) 2014 - 2015 Darron M Broad
;
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;
;   This file is derived from ds30 Loader
;
;   Copyright (c) 2009-2012 Mikael Gustafsson
;
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;   This file is part of The Wellington Boot Loader.
;
;   The Wellington Boot Loader is free software: you can redistribute it and/or
;   modify it under the terms of the GNU General Public License as published
;   by the Free Software Foundation.
;
;   The Wellington Boot Loader is distributed in the hope that it will be
;   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License along
;   with The Wellington Boot Loader. If not, see http://www.gnu.org/licenses/
;------------------------------------------------------------------------------

                NOLIST
                RADIX       DEC

;------------------------------------------------------------------------------

#IFDEF  __18F248
        LIST        P=PIC18F248
        #INCLUDE    "p18f248.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F258
        LIST        P=PIC18F258
        #INCLUDE    "p18f258.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F448
        LIST        P=PIC18F448
        #INCLUDE    "p18f448.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F452
        LIST        P=PIC18F452
        #INCLUDE    "p18f452.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F458
        LIST        P=PIC18F458
        #INCLUDE    "p18f458.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F1220
        LIST        P=PIC18F1220
        #INCLUDE    "p18f1220.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x1000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F1230
        LIST        P=PIC18F1230
        #INCLUDE    "p18f1230.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x1000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F1320
        LIST        P=PIC18F1320
        #INCLUDE    "p18f1320.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     MAX_EE      128
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F1330
        LIST        P=PIC18F1330
        #INCLUDE    "p18f1330.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     MAX_EE      128
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2220
        LIST        P=PIC18F2220
        #INCLUDE    "p18f2220.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x1000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2221
        LIST        P=PIC18F2221
        #INCLUDE    "p18f2221.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x1000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2320
        LIST        P=PIC18F2320
        #INCLUDE    "p18f2320.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2321
        LIST        P=PIC18F2321
        #INCLUDE    "p18f2321.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2331
        LIST        P=PIC18F2331
        #INCLUDE    "p18f2331.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2420
        LIST        P=PIC18F2420
        #INCLUDE    "p18f2420.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2423
        LIST        P=PIC18F2423
        #INCLUDE    "p18f2423.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2431
        LIST        P=PIC18F2431
        #INCLUDE    "p18f2431.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2450
        LIST        P=PIC18F2450
        #INCLUDE    "p18f2450.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     ROWSIZE     16
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2455
        LIST        P=PIC18F2455
        #INCLUDE    "p18f2455.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x6000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2458
        LIST        P=PIC18F2458
        #INCLUDE    "p18f2458.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x6000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2480
        LIST        P=PIC18F2480
        #INCLUDE    "p18f2480.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2520
        LIST        P=PIC18F2520
        #INCLUDE    "p18f2520.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2523
        LIST        P=PIC18F2523
        #INCLUDE    "p18f2523.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2525
        LIST        P=PIC18F2525
        #INCLUDE    "p18f2525.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0xC000
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2550
        LIST        P=PIC18F2550
        #INCLUDE    "p18f2550.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2553
        LIST        P=PIC18F2553
        #INCLUDE    "p18f2553.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2580
        LIST        P=PIC18F2580
        #INCLUDE    "p18f2580.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2585
        LIST        P=PIC18F2585
        #INCLUDE    "p18f2585.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0xC000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2620
        LIST        P=PIC18F2620
        #INCLUDE    "p18f2620.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2680
        LIST        P=PIC18F2680
        #INCLUDE    "p18f2680.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2682
        LIST        P=PIC18F2682
        #INCLUDE    "p18f2682.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x14000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F2685
        LIST        P=PIC18F2685
        #INCLUDE    "p18f2685.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4220
        LIST        P=PIC18F4220
        #INCLUDE    "p18f4220.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x1000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4221
        LIST        P=PIC18F4221
        #INCLUDE    "p18f4221.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x1000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4320
        LIST        P=PIC18F4320
        #INCLUDE    "p18f4320.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4321
        LIST        P=PIC18F4321
        #INCLUDE    "p18f4321.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4331
        LIST        P=PIC18F4331
        #INCLUDE    "p18f4331.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4420
        LIST        P=PIC18F4420
        #INCLUDE    "p18f4420.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4423
        LIST        P=PIC18F4423
        #INCLUDE    "p18f4423.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4431
        LIST        P=PIC18F4431
        #INCLUDE    "p18f4431.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4450
        LIST        P=PIC18F4450
        #INCLUDE    "p18f4450.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     ROWSIZE     16
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4455
        LIST        P=PIC18F4455
        #INCLUDE    "p18f4455.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x6000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4458
        LIST        P=PIC18F4458
        #INCLUDE    "p18f4458.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x6000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4480
        LIST        P=PIC18F4480
        #INCLUDE    "p18f4480.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4520
        LIST        P=PIC18F4520
        #INCLUDE    "p18f4520.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4523
        LIST        P=PIC18F4523
        #INCLUDE    "p18f4523.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4525
        LIST        P=PIC18F4525
        #INCLUDE    "p18f4525.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0xC000
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4550
        LIST        P=PIC18F4550
        #INCLUDE    "p18f4550.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4553
        LIST        P=PIC18F4553
        #INCLUDE    "p18f4553.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4580
        LIST        P=PIC18F4580
        #INCLUDE    "p18f4580.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4585
        LIST        P=PIC18F4585
        #INCLUDE    "p18f4585.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0xC000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4620
        LIST        P=PIC18F4620
        #INCLUDE    "p18f4620.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4680
        LIST        P=PIC18F4680
        #INCLUDE    "p18f4680.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4682
        LIST        P=PIC18F4682
        #INCLUDE    "p18f4682.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x14000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F4685
        LIST        P=PIC18F4685
        #INCLUDE    "p18f4685.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_CAN     1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F6520
        LIST        P=PIC18F6520
        #INCLUDE    "p18f6520.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F6527
        LIST        P=PIC18F6527
        #INCLUDE    "p18f6527.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0xC000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F6620
        LIST        P=PIC18F6620
        #INCLUDE    "p18f6620.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F6622
        LIST        P=PIC18F6622
        #INCLUDE    "p18f6622.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F6627
        LIST        P=PIC18F6627
        #INCLUDE    "p18f6627.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F6628
        LIST        P=PIC18F6628
        #INCLUDE    "p18f6628.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F6720
        LIST        P=PIC18F6720
        #INCLUDE    "p18f6720.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F6722
        LIST        P=PIC18F6722
        #INCLUDE    "p18f6722.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F6723
        LIST        P=PIC18F6723
        #INCLUDE    "p18f6723.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F8520
        LIST        P=PIC18F8520
        #INCLUDE    "p18f8520.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F8527
        LIST        P=PIC18F8527
        #INCLUDE    "p18f8527.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0xC000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F8620
        LIST        P=PIC18F8620
        #INCLUDE    "p18f8620.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F8622
        LIST        P=PIC18F8622
        #INCLUDE    "p18f8622.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F8627
        LIST        P=PIC18F8627
        #INCLUDE    "p18f8627.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F8628
        LIST        P=PIC18F8628
        #INCLUDE    "p18f8628.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F8680
        LIST        P=PIC18F8680
        #INCLUDE    "p18f8680.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F8720
        LIST        P=PIC18F8720
        #INCLUDE    "p18f8720.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F8722
        LIST        P=PIC18F8722
        #INCLUDE    "p18f8722.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F8723
        LIST        P=PIC18F8723
        #INCLUDE    "p18f8723.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F13K22
        LIST        P=PIC18F13K22
        #INCLUDE    "p18f13k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F13K50
        LIST        P=PIC18F13K50
        #INCLUDE    "p18f13k50.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     8
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F14K22
        LIST        P=PIC18F14K22
        #INCLUDE    "p18f14k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     16
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F14K50
        LIST        P=PIC18F14K50
        #INCLUDE    "p18f14k50.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     16
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F23K20
        LIST        P=PIC18F23K20
        #INCLUDE    "p18f23k20.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     16
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F23K22
        LIST        P=PIC18F23K22
        #INCLUDE    "p18f23k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F24K20
        LIST        P=PIC18F24K20
        #INCLUDE    "p18f24k20.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F24K22
        LIST        P=PIC18F24K22
        #INCLUDE    "p18f24k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F24K50
        LIST        P=PIC18F24K50
        #INCLUDE    "p18f24k50.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F25K20
        LIST        P=PIC18F25K20
        #INCLUDE    "p18f25k20.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F25K22
        LIST        P=PIC18F25K22
        #INCLUDE    "p18f25k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F25K50
        LIST        P=PIC18F25K50
        #INCLUDE    "p18f25k50.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F25K80
        LIST        P=PIC18F25K80
        #INCLUDE    "p18f25k80.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_CAN     1
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F26K20
        LIST        P=PIC18F26K20
        #INCLUDE    "p18f26k20.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F26K22
        LIST        P=PIC18F26K22
        #INCLUDE    "p18f26k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F26K80
        LIST        P=PIC18F26K80
        #INCLUDE    "p18f26k80.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_CAN     1
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F43K20
        LIST        P=PIC18F43K20
        #INCLUDE    "p18f43k20.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     16
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F43K22
        LIST        P=PIC18F43K22
        #INCLUDE    "p18f43k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F44K20
        LIST        P=PIC18F44K20
        #INCLUDE    "p18f44k20.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F44K22
        LIST        P=PIC18F44K22
        #INCLUDE    "p18f44k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F45K20
        LIST        P=PIC18F45K20
        #INCLUDE    "p18f45k20.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     32
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F45K22
        LIST        P=PIC18F45K22
        #INCLUDE    "p18f45k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F45K50
        LIST        P=PIC18F45K50
        #INCLUDE    "p18f45k50.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     MAX_EE      256
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F45K80
        LIST        P=PIC18F45K80
        #INCLUDE    "p18f45k80.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_CAN     1
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F46K20
        LIST        P=PIC18F46K20
        #INCLUDE    "p18f46k20.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F46K22
        LIST        P=PIC18F46K22
        #INCLUDE    "p18f46k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F46K80
        LIST        P=PIC18F46K80
        #INCLUDE    "p18f46k80.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_CAN     1
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F65K22
        LIST        P=PIC18F65K22
        #INCLUDE    "p18f65k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F65K80
        LIST        P=PIC18F65K80
        #INCLUDE    "p18f65k80.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_CAN     1
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F66K22
        LIST        P=PIC18F66K22
        #INCLUDE    "p18f66k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F66K80
        LIST        P=PIC18F66K80
        #INCLUDE    "p18f66k80.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_CAN     1
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F67K22
        LIST        P=PIC18F67K22
        #INCLUDE    "p18f67k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     128
        #DEFINE     ERASESIZE   128
#ENDIF
#IFDEF  __18F85K22
        LIST        P=PIC18F85K22
        #INCLUDE    "p18f85k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F86K22
        LIST        P=PIC18F86K22
        #INCLUDE    "p18f86k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   64
#ENDIF
#IFDEF  __18F87K22
        LIST        P=PIC18F87K22
        #INCLUDE    "p18f87k22.inc"
        NOLIST
        #DEFINE     IS_PIC18F   1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     MAX_EE      1024
        #DEFINE     ROWSIZE     128
        #DEFINE     ERASESIZE   128
#ENDIF

;------------------------------------------------------------------------------
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
