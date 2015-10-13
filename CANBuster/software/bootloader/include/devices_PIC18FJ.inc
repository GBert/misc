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

#IFDEF  __18F24J10
        LIST        P=PIC18F24J10
        #INCLUDE    "p18f24j10.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F24J11
        LIST        P=PIC18F24J11
        #INCLUDE    "p18f24j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F24J50
        LIST        P=PIC18F24J50
        #INCLUDE    "p18f24j50.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F25J10
        LIST        P=PIC18F25J10
        #INCLUDE    "p18f25j10.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F25J11
        LIST        P=PIC18F25J11
        #INCLUDE    "p18f25j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F25J50
        LIST        P=PIC18F25J50
        #INCLUDE    "p18f25j50.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F26J11
        LIST        P=PIC18F26J11
        #INCLUDE    "p18f26j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F26J13
        LIST        P=PIC18F26J13
        #INCLUDE    "p18f26j13.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F26J50
        LIST        P=PIC18F26J50
        #INCLUDE    "p18f26j50.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F26J53
        LIST        P=PIC18F26J53
        #INCLUDE    "p18f26j53.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F27J13
        LIST        P=PIC18F27J13
        #INCLUDE    "p18f27j13.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F27J53
        LIST        P=PIC18F27J53
        #INCLUDE    "p18f27j53.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F44J10
        LIST        P=PIC18F44J10
        #INCLUDE    "p18f44j10.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F44J11
        LIST        P=PIC18F44J11
        #INCLUDE    "p18f44j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F44J50
        LIST        P=PIC18F44J50
        #INCLUDE    "p18f44j50.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F45J10
        LIST        P=PIC18F45J10
        #INCLUDE    "p18f45j10.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F45J11
        LIST        P=PIC18F45J11
        #INCLUDE    "p18f45j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F45J50
        LIST        P=PIC18F45J50
        #INCLUDE    "p18f45j50.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F46J11
        LIST        P=PIC18F46J11
        #INCLUDE    "p18f46j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F46J13
        LIST        P=PIC18F46J13
        #INCLUDE    "p18f46j13.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F46J50
        LIST        P=PIC18F46J50
        #INCLUDE    "p18f46j50.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F46J53
        LIST        P=PIC18F46J53
        #INCLUDE    "p18f46j53.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F47J13
        LIST        P=PIC18F47J13
        #INCLUDE    "p18f47j13.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F47J53
        LIST        P=PIC18F47J53
        #INCLUDE    "p18f47j53.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
        #DEFINE     HAS_PPS     1
#ENDIF
#IFDEF  __18F63J11
        LIST        P=PIC18F63J11
        #INCLUDE    "p18f63j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F63J90
        LIST        P=PIC18F63J90
        #INCLUDE    "p18f63j90.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F64J11
        LIST        P=PIC18F64J11
        #INCLUDE    "p18f64j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F64J90
        LIST        P=PIC18F64J90
        #INCLUDE    "p18f64j90.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F65J10
        LIST        P=PIC18F65J10
        #INCLUDE    "p18f65j10.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F65J11
        LIST        P=PIC18F65J11
        #INCLUDE    "p18f65j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F65J15
        LIST        P=PIC18F65J15
        #INCLUDE    "p18f65j15.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0xC000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F65J50
        LIST        P=PIC18F65J50
        #INCLUDE    "p18f65j50.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F65J90
        LIST        P=PIC18F65J90
        #INCLUDE    "p18f65j90.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F66J10
        LIST        P=PIC18F66J10
        #INCLUDE    "p18f66j10.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F66J11
        LIST        P=PIC18F66J11
        #INCLUDE    "p18f66j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F66J15
        LIST        P=PIC18F66J15
        #INCLUDE    "p18f66j15.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F66J16
        LIST        P=PIC18F66J16
        #INCLUDE    "p18f66j16.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F66J50
        LIST        P=PIC18F66J50
        #INCLUDE    "p18f66j50.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F66J55
        LIST        P=PIC18F66J55
        #INCLUDE    "p18f66j55.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F66J60
        LIST        P=PIC18F66J60
        #INCLUDE    "p18f66j60.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F66J65
        LIST        P=PIC18F66J65
        #INCLUDE    "p18f66j65.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F66J90
        LIST        P=PIC18F66J90
        #INCLUDE    "p18f66j90.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F66J93
        LIST        P=PIC18F66J93
        #INCLUDE    "p18f66j93.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F67J10
        LIST        P=PIC18F67J10
        #INCLUDE    "p18f67j10.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F67J11
        LIST        P=PIC18F67J11
        #INCLUDE    "p18f67j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F67J50
        LIST        P=PIC18F67J50
        #INCLUDE    "p18f67j50.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F67J60
        LIST        P=PIC18F67J60
        #INCLUDE    "p18f67j60.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F67J90
        LIST        P=PIC18F67J90
        #INCLUDE    "p18f67j90.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F67J93
        LIST        P=PIC18F67J93
        #INCLUDE    "p18f67j93.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F83J11
        LIST        P=PIC18F83J11
        #INCLUDE    "p18f83j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F83J90
        LIST        P=PIC18F83J90
        #INCLUDE    "p18f83j90.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x2000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F84J11
        LIST        P=PIC18F84J11
        #INCLUDE    "p18f84j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F84J90
        LIST        P=PIC18F84J90
        #INCLUDE    "p18f84j90.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x4000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F85J10
        LIST        P=PIC18F85J10
        #INCLUDE    "p18f85j10.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F85J11
        LIST        P=PIC18F85J11
        #INCLUDE    "p18f85j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F85J15
        LIST        P=PIC18F85J15
        #INCLUDE    "p18f85j15.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0xC000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F85J50
        LIST        P=PIC18F85J50
        #INCLUDE    "p18f85j50.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F85J90
        LIST        P=PIC18F85J90
        #INCLUDE    "p18f85j90.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x8000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F86J10
        LIST        P=PIC18F86J10
        #INCLUDE    "p18f86j10.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F86J11
        LIST        P=PIC18F86J11
        #INCLUDE    "p18f86j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F86J15
        LIST        P=PIC18F86J15
        #INCLUDE    "p18f86j15.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F86J16
        LIST        P=PIC18F86J16
        #INCLUDE    "p18f86j16.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F86J50
        LIST        P=PIC18F86J50
        #INCLUDE    "p18f86j50.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F86J55
        LIST        P=PIC18F86J55
        #INCLUDE    "p18f86j55.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F86J60
        LIST        P=PIC18F86J60
        #INCLUDE    "p18f86j60.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F86J65
        LIST        P=PIC18F86J65
        #INCLUDE    "p18f86j65.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F86J90
        LIST        P=PIC18F86J90
        #INCLUDE    "p18f86j90.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F86J93
        LIST        P=PIC18F86J93
        #INCLUDE    "p18f86j93.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F87J10
        LIST        P=PIC18F87J10
        #INCLUDE    "p18f87j10.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F87J11
        LIST        P=PIC18F87J11
        #INCLUDE    "p18f87j11.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F87J50
        LIST        P=PIC18F87J50
        #INCLUDE    "p18f87j50.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F87J60
        LIST        P=PIC18F87J60
        #INCLUDE    "p18f87j60.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F87J90
        LIST        P=PIC18F87J90
        #INCLUDE    "p18f87j90.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F87J93
        LIST        P=PIC18F87J93
        #INCLUDE    "p18f87j93.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F96J60
        LIST        P=PIC18F96J60
        #INCLUDE    "p18f96j60.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x10000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F96J65
        LIST        P=PIC18F96J65
        #INCLUDE    "p18f96j65.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x18000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF
#IFDEF  __18F97J60
        LIST        P=PIC18F97J60
        #INCLUDE    "p18f97j60.inc"
        NOLIST
        #DEFINE     IS_PIC18FJ  1
        #DEFINE     MAX_FLASH   0x20000
        #DEFINE     HAS_UART2   1
        #DEFINE     ROWSIZE     64
        #DEFINE     ERASESIZE   1024
#ENDIF

;------------------------------------------------------------------------------
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
