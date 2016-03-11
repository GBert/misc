; LF-INTOSC Low-power Enable bit:
;               CONFIG    INTOSCSEL=LOW
; Extended Instruction Set:
                CONFIG    XINST=OFF
; PLL x4 Enable bit:
                CONFIG    OSC=HSPLL
; Fail-Safe Clock Monitor:
                CONFIG    FCMEN=OFF
; Internal External Oscillator Switch Over Mode:
                CONFIG    IESO=OFF
; Power-up Timer:
                CONFIG    PWRT=OFF
; Brown-out Detect:
                CONFIG    BOREN=OFF
; Brown-out Reset Voltage bits:
                CONFIG    BORV=1
; BORMV Power level:
;               CONFIG    BORPWR=LOW
; Watchdog Timer:
; TODO
;                CONFIG    WRTD=OFF
; Watchdog Postscaler:
                CONFIG    WDTPS=1024
; Master Clear Enable:
                CONFIG    MCLRE=ON
; Stack Overflow Reset:
                CONFIG    STVREN=ON
; Code Protect 00800-03FFF:
                CONFIG    CP0=OFF
; Code Protect 04000-07FFF:
                CONFIG    CP1=OFF
; Code Protect 08000-0BFFF:
                CONFIG    CP2=OFF
; Code Protect 0C000-0FFFF:
                CONFIG    CP3=OFF
; Code Protect Boot:
                CONFIG    CPB=OFF
; Data EE Read Protect:
                CONFIG    CPD=OFF
; Table Write Protect 00800-03FFF:
                CONFIG    WRT0=OFF
; Table Write Protect 04000-07FFF:
                CONFIG    WRT1=OFF
; Table Write Protect 08000-0BFFF:
                CONFIG    WRT2=OFF
; Table Write Protect 0C000-0FFFF:
                CONFIG    WRT3=OFF
; Config. Write Protect:
                CONFIG    WRTC=OFF
; Table Write Protect Boot:
                CONFIG    WRTB=OFF
; Data EE Write Protect:
                CONFIG    WRTD=OFF
; Table Read Protect 00800-03FFF:
                CONFIG    EBTR0=OFF
; Table Read Protect 04000-07FFF:
                CONFIG    EBTR1=OFF
; Table Read Protect 08000-0BFFF:
                CONFIG    EBTR2=OFF
; Table Read Protect 0C000-0FFFF:
                CONFIG    EBTR3=OFF
; Table Read Protect Boot:
                CONFIG    EBTRB=OFF

