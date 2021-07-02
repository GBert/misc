// Version 2.31
// Generated 13/10/2020 GMT

/*
 * Copyright © 2020, Microchip Technology Inc. and its subsidiaries ("Microchip")
 * All rights reserved.
 * 
 * This software is developed by Microchip Technology Inc. and its subsidiaries ("Microchip").
 * 
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 * 
 *     1. Redistributions of source code must retain the above copyright notice, this list of
 *        conditions and the following disclaimer.
 * 
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list
 *        of conditions and the following disclaimer in the documentation and/or other
 *        materials provided with the distribution. Publication is not required when
 *        this file is used in an embedded application.
 * 
 *     3. Microchip's name may not be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL MICROCHIP BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING BUT NOT LIMITED TO
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWSOEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PIC16F1709_H_
#define _PIC16F1709_H_

/*
 * C Header file for the Microchip PIC Microcontroller
 * PIC16F1709
 */
#ifndef __XC8
#warning Header file pic16f1709.h included directly. Use #include <xc.h> instead.
#endif

#include <__at.h>

/*
 * Register Definitions
 */

// Register: INDF0
#define INDF0 INDF0
extern volatile unsigned char           INDF0               __at(0x000);
#ifndef _LIB_BUILD
asm("INDF0 equ 00h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned INDF0                  :8;
    };
} INDF0bits_t;
extern volatile INDF0bits_t INDF0bits __at(0x000);
// bitfield macros
#define _INDF0_INDF0_POSN                                   0x0
#define _INDF0_INDF0_POSITION                               0x0
#define _INDF0_INDF0_SIZE                                   0x8
#define _INDF0_INDF0_LENGTH                                 0x8
#define _INDF0_INDF0_MASK                                   0xFF

// Register: INDF1
#define INDF1 INDF1
extern volatile unsigned char           INDF1               __at(0x001);
#ifndef _LIB_BUILD
asm("INDF1 equ 01h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned INDF1                  :8;
    };
} INDF1bits_t;
extern volatile INDF1bits_t INDF1bits __at(0x001);
// bitfield macros
#define _INDF1_INDF1_POSN                                   0x0
#define _INDF1_INDF1_POSITION                               0x0
#define _INDF1_INDF1_SIZE                                   0x8
#define _INDF1_INDF1_LENGTH                                 0x8
#define _INDF1_INDF1_MASK                                   0xFF

// Register: PCL
#define PCL PCL
extern volatile unsigned char           PCL                 __at(0x002);
#ifndef _LIB_BUILD
asm("PCL equ 02h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PCL                    :8;
    };
} PCLbits_t;
extern volatile PCLbits_t PCLbits __at(0x002);
// bitfield macros
#define _PCL_PCL_POSN                                       0x0
#define _PCL_PCL_POSITION                                   0x0
#define _PCL_PCL_SIZE                                       0x8
#define _PCL_PCL_LENGTH                                     0x8
#define _PCL_PCL_MASK                                       0xFF

// Register: STATUS
#define STATUS STATUS
extern volatile unsigned char           STATUS              __at(0x003);
#ifndef _LIB_BUILD
asm("STATUS equ 03h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned C                      :1;
        unsigned DC                     :1;
        unsigned Z                      :1;
        unsigned nPD                    :1;
        unsigned nTO                    :1;
    };
    struct {
        unsigned CARRY                  :1;
        unsigned                        :1;
        unsigned ZERO                   :1;
    };
} STATUSbits_t;
extern volatile STATUSbits_t STATUSbits __at(0x003);
// bitfield macros
#define _STATUS_C_POSN                                      0x0
#define _STATUS_C_POSITION                                  0x0
#define _STATUS_C_SIZE                                      0x1
#define _STATUS_C_LENGTH                                    0x1
#define _STATUS_C_MASK                                      0x1
#define _STATUS_DC_POSN                                     0x1
#define _STATUS_DC_POSITION                                 0x1
#define _STATUS_DC_SIZE                                     0x1
#define _STATUS_DC_LENGTH                                   0x1
#define _STATUS_DC_MASK                                     0x2
#define _STATUS_Z_POSN                                      0x2
#define _STATUS_Z_POSITION                                  0x2
#define _STATUS_Z_SIZE                                      0x1
#define _STATUS_Z_LENGTH                                    0x1
#define _STATUS_Z_MASK                                      0x4
#define _STATUS_nPD_POSN                                    0x3
#define _STATUS_nPD_POSITION                                0x3
#define _STATUS_nPD_SIZE                                    0x1
#define _STATUS_nPD_LENGTH                                  0x1
#define _STATUS_nPD_MASK                                    0x8
#define _STATUS_nTO_POSN                                    0x4
#define _STATUS_nTO_POSITION                                0x4
#define _STATUS_nTO_SIZE                                    0x1
#define _STATUS_nTO_LENGTH                                  0x1
#define _STATUS_nTO_MASK                                    0x10
#define _STATUS_CARRY_POSN                                  0x0
#define _STATUS_CARRY_POSITION                              0x0
#define _STATUS_CARRY_SIZE                                  0x1
#define _STATUS_CARRY_LENGTH                                0x1
#define _STATUS_CARRY_MASK                                  0x1
#define _STATUS_ZERO_POSN                                   0x2
#define _STATUS_ZERO_POSITION                               0x2
#define _STATUS_ZERO_SIZE                                   0x1
#define _STATUS_ZERO_LENGTH                                 0x1
#define _STATUS_ZERO_MASK                                   0x4

// Register: FSR0
#define FSR0 FSR0
extern volatile unsigned short          FSR0                __at(0x004);

// Register: FSR0L
#define FSR0L FSR0L
extern volatile unsigned char           FSR0L               __at(0x004);
#ifndef _LIB_BUILD
asm("FSR0L equ 04h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned FSR0L                  :8;
    };
} FSR0Lbits_t;
extern volatile FSR0Lbits_t FSR0Lbits __at(0x004);
// bitfield macros
#define _FSR0L_FSR0L_POSN                                   0x0
#define _FSR0L_FSR0L_POSITION                               0x0
#define _FSR0L_FSR0L_SIZE                                   0x8
#define _FSR0L_FSR0L_LENGTH                                 0x8
#define _FSR0L_FSR0L_MASK                                   0xFF

// Register: FSR0H
#define FSR0H FSR0H
extern volatile unsigned char           FSR0H               __at(0x005);
#ifndef _LIB_BUILD
asm("FSR0H equ 05h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned FSR0H                  :8;
    };
} FSR0Hbits_t;
extern volatile FSR0Hbits_t FSR0Hbits __at(0x005);
// bitfield macros
#define _FSR0H_FSR0H_POSN                                   0x0
#define _FSR0H_FSR0H_POSITION                               0x0
#define _FSR0H_FSR0H_SIZE                                   0x8
#define _FSR0H_FSR0H_LENGTH                                 0x8
#define _FSR0H_FSR0H_MASK                                   0xFF

// Register: FSR1
#define FSR1 FSR1
extern volatile unsigned short          FSR1                __at(0x006);

// Register: FSR1L
#define FSR1L FSR1L
extern volatile unsigned char           FSR1L               __at(0x006);
#ifndef _LIB_BUILD
asm("FSR1L equ 06h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned FSR1L                  :8;
    };
} FSR1Lbits_t;
extern volatile FSR1Lbits_t FSR1Lbits __at(0x006);
// bitfield macros
#define _FSR1L_FSR1L_POSN                                   0x0
#define _FSR1L_FSR1L_POSITION                               0x0
#define _FSR1L_FSR1L_SIZE                                   0x8
#define _FSR1L_FSR1L_LENGTH                                 0x8
#define _FSR1L_FSR1L_MASK                                   0xFF

// Register: FSR1H
#define FSR1H FSR1H
extern volatile unsigned char           FSR1H               __at(0x007);
#ifndef _LIB_BUILD
asm("FSR1H equ 07h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned FSR1H                  :8;
    };
} FSR1Hbits_t;
extern volatile FSR1Hbits_t FSR1Hbits __at(0x007);
// bitfield macros
#define _FSR1H_FSR1H_POSN                                   0x0
#define _FSR1H_FSR1H_POSITION                               0x0
#define _FSR1H_FSR1H_SIZE                                   0x8
#define _FSR1H_FSR1H_LENGTH                                 0x8
#define _FSR1H_FSR1H_MASK                                   0xFF

// Register: BSR
#define BSR BSR
extern volatile unsigned char           BSR                 __at(0x008);
#ifndef _LIB_BUILD
asm("BSR equ 08h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned BSR                    :5;
    };
    struct {
        unsigned BSR0                   :1;
        unsigned BSR1                   :1;
        unsigned BSR2                   :1;
        unsigned BSR3                   :1;
        unsigned BSR4                   :1;
    };
} BSRbits_t;
extern volatile BSRbits_t BSRbits __at(0x008);
// bitfield macros
#define _BSR_BSR_POSN                                       0x0
#define _BSR_BSR_POSITION                                   0x0
#define _BSR_BSR_SIZE                                       0x5
#define _BSR_BSR_LENGTH                                     0x5
#define _BSR_BSR_MASK                                       0x1F
#define _BSR_BSR0_POSN                                      0x0
#define _BSR_BSR0_POSITION                                  0x0
#define _BSR_BSR0_SIZE                                      0x1
#define _BSR_BSR0_LENGTH                                    0x1
#define _BSR_BSR0_MASK                                      0x1
#define _BSR_BSR1_POSN                                      0x1
#define _BSR_BSR1_POSITION                                  0x1
#define _BSR_BSR1_SIZE                                      0x1
#define _BSR_BSR1_LENGTH                                    0x1
#define _BSR_BSR1_MASK                                      0x2
#define _BSR_BSR2_POSN                                      0x2
#define _BSR_BSR2_POSITION                                  0x2
#define _BSR_BSR2_SIZE                                      0x1
#define _BSR_BSR2_LENGTH                                    0x1
#define _BSR_BSR2_MASK                                      0x4
#define _BSR_BSR3_POSN                                      0x3
#define _BSR_BSR3_POSITION                                  0x3
#define _BSR_BSR3_SIZE                                      0x1
#define _BSR_BSR3_LENGTH                                    0x1
#define _BSR_BSR3_MASK                                      0x8
#define _BSR_BSR4_POSN                                      0x4
#define _BSR_BSR4_POSITION                                  0x4
#define _BSR_BSR4_SIZE                                      0x1
#define _BSR_BSR4_LENGTH                                    0x1
#define _BSR_BSR4_MASK                                      0x10

// Register: WREG
#define WREG WREG
extern volatile unsigned char           WREG                __at(0x009);
#ifndef _LIB_BUILD
asm("WREG equ 09h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned WREG0                  :8;
    };
} WREGbits_t;
extern volatile WREGbits_t WREGbits __at(0x009);
// bitfield macros
#define _WREG_WREG0_POSN                                    0x0
#define _WREG_WREG0_POSITION                                0x0
#define _WREG_WREG0_SIZE                                    0x8
#define _WREG_WREG0_LENGTH                                  0x8
#define _WREG_WREG0_MASK                                    0xFF

// Register: PCLATH
#define PCLATH PCLATH
extern volatile unsigned char           PCLATH              __at(0x00A);
#ifndef _LIB_BUILD
asm("PCLATH equ 0Ah");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PCLATH                 :7;
    };
} PCLATHbits_t;
extern volatile PCLATHbits_t PCLATHbits __at(0x00A);
// bitfield macros
#define _PCLATH_PCLATH_POSN                                 0x0
#define _PCLATH_PCLATH_POSITION                             0x0
#define _PCLATH_PCLATH_SIZE                                 0x7
#define _PCLATH_PCLATH_LENGTH                               0x7
#define _PCLATH_PCLATH_MASK                                 0x7F

// Register: INTCON
#define INTCON INTCON
extern volatile unsigned char           INTCON              __at(0x00B);
#ifndef _LIB_BUILD
asm("INTCON equ 0Bh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned IOCIF                  :1;
        unsigned INTF                   :1;
        unsigned TMR0IF                 :1;
        unsigned IOCIE                  :1;
        unsigned INTE                   :1;
        unsigned TMR0IE                 :1;
        unsigned PEIE                   :1;
        unsigned GIE                    :1;
    };
    struct {
        unsigned                        :2;
        unsigned T0IF                   :1;
        unsigned                        :2;
        unsigned T0IE                   :1;
    };
} INTCONbits_t;
extern volatile INTCONbits_t INTCONbits __at(0x00B);
// bitfield macros
#define _INTCON_IOCIF_POSN                                  0x0
#define _INTCON_IOCIF_POSITION                              0x0
#define _INTCON_IOCIF_SIZE                                  0x1
#define _INTCON_IOCIF_LENGTH                                0x1
#define _INTCON_IOCIF_MASK                                  0x1
#define _INTCON_INTF_POSN                                   0x1
#define _INTCON_INTF_POSITION                               0x1
#define _INTCON_INTF_SIZE                                   0x1
#define _INTCON_INTF_LENGTH                                 0x1
#define _INTCON_INTF_MASK                                   0x2
#define _INTCON_TMR0IF_POSN                                 0x2
#define _INTCON_TMR0IF_POSITION                             0x2
#define _INTCON_TMR0IF_SIZE                                 0x1
#define _INTCON_TMR0IF_LENGTH                               0x1
#define _INTCON_TMR0IF_MASK                                 0x4
#define _INTCON_IOCIE_POSN                                  0x3
#define _INTCON_IOCIE_POSITION                              0x3
#define _INTCON_IOCIE_SIZE                                  0x1
#define _INTCON_IOCIE_LENGTH                                0x1
#define _INTCON_IOCIE_MASK                                  0x8
#define _INTCON_INTE_POSN                                   0x4
#define _INTCON_INTE_POSITION                               0x4
#define _INTCON_INTE_SIZE                                   0x1
#define _INTCON_INTE_LENGTH                                 0x1
#define _INTCON_INTE_MASK                                   0x10
#define _INTCON_TMR0IE_POSN                                 0x5
#define _INTCON_TMR0IE_POSITION                             0x5
#define _INTCON_TMR0IE_SIZE                                 0x1
#define _INTCON_TMR0IE_LENGTH                               0x1
#define _INTCON_TMR0IE_MASK                                 0x20
#define _INTCON_PEIE_POSN                                   0x6
#define _INTCON_PEIE_POSITION                               0x6
#define _INTCON_PEIE_SIZE                                   0x1
#define _INTCON_PEIE_LENGTH                                 0x1
#define _INTCON_PEIE_MASK                                   0x40
#define _INTCON_GIE_POSN                                    0x7
#define _INTCON_GIE_POSITION                                0x7
#define _INTCON_GIE_SIZE                                    0x1
#define _INTCON_GIE_LENGTH                                  0x1
#define _INTCON_GIE_MASK                                    0x80
#define _INTCON_T0IF_POSN                                   0x2
#define _INTCON_T0IF_POSITION                               0x2
#define _INTCON_T0IF_SIZE                                   0x1
#define _INTCON_T0IF_LENGTH                                 0x1
#define _INTCON_T0IF_MASK                                   0x4
#define _INTCON_T0IE_POSN                                   0x5
#define _INTCON_T0IE_POSITION                               0x5
#define _INTCON_T0IE_SIZE                                   0x1
#define _INTCON_T0IE_LENGTH                                 0x1
#define _INTCON_T0IE_MASK                                   0x20

// Register: PORTA
#define PORTA PORTA
extern volatile unsigned char           PORTA               __at(0x00C);
#ifndef _LIB_BUILD
asm("PORTA equ 0Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RA0                    :1;
        unsigned RA1                    :1;
        unsigned RA2                    :1;
        unsigned RA3                    :1;
        unsigned RA4                    :1;
        unsigned RA5                    :1;
    };
} PORTAbits_t;
extern volatile PORTAbits_t PORTAbits __at(0x00C);
// bitfield macros
#define _PORTA_RA0_POSN                                     0x0
#define _PORTA_RA0_POSITION                                 0x0
#define _PORTA_RA0_SIZE                                     0x1
#define _PORTA_RA0_LENGTH                                   0x1
#define _PORTA_RA0_MASK                                     0x1
#define _PORTA_RA1_POSN                                     0x1
#define _PORTA_RA1_POSITION                                 0x1
#define _PORTA_RA1_SIZE                                     0x1
#define _PORTA_RA1_LENGTH                                   0x1
#define _PORTA_RA1_MASK                                     0x2
#define _PORTA_RA2_POSN                                     0x2
#define _PORTA_RA2_POSITION                                 0x2
#define _PORTA_RA2_SIZE                                     0x1
#define _PORTA_RA2_LENGTH                                   0x1
#define _PORTA_RA2_MASK                                     0x4
#define _PORTA_RA3_POSN                                     0x3
#define _PORTA_RA3_POSITION                                 0x3
#define _PORTA_RA3_SIZE                                     0x1
#define _PORTA_RA3_LENGTH                                   0x1
#define _PORTA_RA3_MASK                                     0x8
#define _PORTA_RA4_POSN                                     0x4
#define _PORTA_RA4_POSITION                                 0x4
#define _PORTA_RA4_SIZE                                     0x1
#define _PORTA_RA4_LENGTH                                   0x1
#define _PORTA_RA4_MASK                                     0x10
#define _PORTA_RA5_POSN                                     0x5
#define _PORTA_RA5_POSITION                                 0x5
#define _PORTA_RA5_SIZE                                     0x1
#define _PORTA_RA5_LENGTH                                   0x1
#define _PORTA_RA5_MASK                                     0x20

// Register: PORTB
#define PORTB PORTB
extern volatile unsigned char           PORTB               __at(0x00D);
#ifndef _LIB_BUILD
asm("PORTB equ 0Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned RB4                    :1;
        unsigned RB5                    :1;
        unsigned RB6                    :1;
        unsigned RB7                    :1;
    };
} PORTBbits_t;
extern volatile PORTBbits_t PORTBbits __at(0x00D);
// bitfield macros
#define _PORTB_RB4_POSN                                     0x4
#define _PORTB_RB4_POSITION                                 0x4
#define _PORTB_RB4_SIZE                                     0x1
#define _PORTB_RB4_LENGTH                                   0x1
#define _PORTB_RB4_MASK                                     0x10
#define _PORTB_RB5_POSN                                     0x5
#define _PORTB_RB5_POSITION                                 0x5
#define _PORTB_RB5_SIZE                                     0x1
#define _PORTB_RB5_LENGTH                                   0x1
#define _PORTB_RB5_MASK                                     0x20
#define _PORTB_RB6_POSN                                     0x6
#define _PORTB_RB6_POSITION                                 0x6
#define _PORTB_RB6_SIZE                                     0x1
#define _PORTB_RB6_LENGTH                                   0x1
#define _PORTB_RB6_MASK                                     0x40
#define _PORTB_RB7_POSN                                     0x7
#define _PORTB_RB7_POSITION                                 0x7
#define _PORTB_RB7_SIZE                                     0x1
#define _PORTB_RB7_LENGTH                                   0x1
#define _PORTB_RB7_MASK                                     0x80

// Register: PORTC
#define PORTC PORTC
extern volatile unsigned char           PORTC               __at(0x00E);
#ifndef _LIB_BUILD
asm("PORTC equ 0Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RC0                    :1;
        unsigned RC1                    :1;
        unsigned RC2                    :1;
        unsigned RC3                    :1;
        unsigned RC4                    :1;
        unsigned RC5                    :1;
        unsigned RC6                    :1;
        unsigned RC7                    :1;
    };
} PORTCbits_t;
extern volatile PORTCbits_t PORTCbits __at(0x00E);
// bitfield macros
#define _PORTC_RC0_POSN                                     0x0
#define _PORTC_RC0_POSITION                                 0x0
#define _PORTC_RC0_SIZE                                     0x1
#define _PORTC_RC0_LENGTH                                   0x1
#define _PORTC_RC0_MASK                                     0x1
#define _PORTC_RC1_POSN                                     0x1
#define _PORTC_RC1_POSITION                                 0x1
#define _PORTC_RC1_SIZE                                     0x1
#define _PORTC_RC1_LENGTH                                   0x1
#define _PORTC_RC1_MASK                                     0x2
#define _PORTC_RC2_POSN                                     0x2
#define _PORTC_RC2_POSITION                                 0x2
#define _PORTC_RC2_SIZE                                     0x1
#define _PORTC_RC2_LENGTH                                   0x1
#define _PORTC_RC2_MASK                                     0x4
#define _PORTC_RC3_POSN                                     0x3
#define _PORTC_RC3_POSITION                                 0x3
#define _PORTC_RC3_SIZE                                     0x1
#define _PORTC_RC3_LENGTH                                   0x1
#define _PORTC_RC3_MASK                                     0x8
#define _PORTC_RC4_POSN                                     0x4
#define _PORTC_RC4_POSITION                                 0x4
#define _PORTC_RC4_SIZE                                     0x1
#define _PORTC_RC4_LENGTH                                   0x1
#define _PORTC_RC4_MASK                                     0x10
#define _PORTC_RC5_POSN                                     0x5
#define _PORTC_RC5_POSITION                                 0x5
#define _PORTC_RC5_SIZE                                     0x1
#define _PORTC_RC5_LENGTH                                   0x1
#define _PORTC_RC5_MASK                                     0x20
#define _PORTC_RC6_POSN                                     0x6
#define _PORTC_RC6_POSITION                                 0x6
#define _PORTC_RC6_SIZE                                     0x1
#define _PORTC_RC6_LENGTH                                   0x1
#define _PORTC_RC6_MASK                                     0x40
#define _PORTC_RC7_POSN                                     0x7
#define _PORTC_RC7_POSITION                                 0x7
#define _PORTC_RC7_SIZE                                     0x1
#define _PORTC_RC7_LENGTH                                   0x1
#define _PORTC_RC7_MASK                                     0x80

// Register: PIR1
#define PIR1 PIR1
extern volatile unsigned char           PIR1                __at(0x011);
#ifndef _LIB_BUILD
asm("PIR1 equ 011h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TMR1IF                 :1;
        unsigned TMR2IF                 :1;
        unsigned CCP1IF                 :1;
        unsigned SSP1IF                 :1;
        unsigned TXIF                   :1;
        unsigned RCIF                   :1;
        unsigned ADIF                   :1;
        unsigned TMR1GIF                :1;
    };
    struct {
        unsigned                        :2;
        unsigned CCPIF                  :1;
    };
} PIR1bits_t;
extern volatile PIR1bits_t PIR1bits __at(0x011);
// bitfield macros
#define _PIR1_TMR1IF_POSN                                   0x0
#define _PIR1_TMR1IF_POSITION                               0x0
#define _PIR1_TMR1IF_SIZE                                   0x1
#define _PIR1_TMR1IF_LENGTH                                 0x1
#define _PIR1_TMR1IF_MASK                                   0x1
#define _PIR1_TMR2IF_POSN                                   0x1
#define _PIR1_TMR2IF_POSITION                               0x1
#define _PIR1_TMR2IF_SIZE                                   0x1
#define _PIR1_TMR2IF_LENGTH                                 0x1
#define _PIR1_TMR2IF_MASK                                   0x2
#define _PIR1_CCP1IF_POSN                                   0x2
#define _PIR1_CCP1IF_POSITION                               0x2
#define _PIR1_CCP1IF_SIZE                                   0x1
#define _PIR1_CCP1IF_LENGTH                                 0x1
#define _PIR1_CCP1IF_MASK                                   0x4
#define _PIR1_SSP1IF_POSN                                   0x3
#define _PIR1_SSP1IF_POSITION                               0x3
#define _PIR1_SSP1IF_SIZE                                   0x1
#define _PIR1_SSP1IF_LENGTH                                 0x1
#define _PIR1_SSP1IF_MASK                                   0x8
#define _PIR1_TXIF_POSN                                     0x4
#define _PIR1_TXIF_POSITION                                 0x4
#define _PIR1_TXIF_SIZE                                     0x1
#define _PIR1_TXIF_LENGTH                                   0x1
#define _PIR1_TXIF_MASK                                     0x10
#define _PIR1_RCIF_POSN                                     0x5
#define _PIR1_RCIF_POSITION                                 0x5
#define _PIR1_RCIF_SIZE                                     0x1
#define _PIR1_RCIF_LENGTH                                   0x1
#define _PIR1_RCIF_MASK                                     0x20
#define _PIR1_ADIF_POSN                                     0x6
#define _PIR1_ADIF_POSITION                                 0x6
#define _PIR1_ADIF_SIZE                                     0x1
#define _PIR1_ADIF_LENGTH                                   0x1
#define _PIR1_ADIF_MASK                                     0x40
#define _PIR1_TMR1GIF_POSN                                  0x7
#define _PIR1_TMR1GIF_POSITION                              0x7
#define _PIR1_TMR1GIF_SIZE                                  0x1
#define _PIR1_TMR1GIF_LENGTH                                0x1
#define _PIR1_TMR1GIF_MASK                                  0x80
#define _PIR1_CCPIF_POSN                                    0x2
#define _PIR1_CCPIF_POSITION                                0x2
#define _PIR1_CCPIF_SIZE                                    0x1
#define _PIR1_CCPIF_LENGTH                                  0x1
#define _PIR1_CCPIF_MASK                                    0x4

// Register: PIR2
#define PIR2 PIR2
extern volatile unsigned char           PIR2                __at(0x012);
#ifndef _LIB_BUILD
asm("PIR2 equ 012h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CCP2IF                 :1;
        unsigned TMR4IF                 :1;
        unsigned TMR6IF                 :1;
        unsigned BCL1IF                 :1;
        unsigned                        :1;
        unsigned C1IF                   :1;
        unsigned C2IF                   :1;
        unsigned OSFIF                  :1;
    };
} PIR2bits_t;
extern volatile PIR2bits_t PIR2bits __at(0x012);
// bitfield macros
#define _PIR2_CCP2IF_POSN                                   0x0
#define _PIR2_CCP2IF_POSITION                               0x0
#define _PIR2_CCP2IF_SIZE                                   0x1
#define _PIR2_CCP2IF_LENGTH                                 0x1
#define _PIR2_CCP2IF_MASK                                   0x1
#define _PIR2_TMR4IF_POSN                                   0x1
#define _PIR2_TMR4IF_POSITION                               0x1
#define _PIR2_TMR4IF_SIZE                                   0x1
#define _PIR2_TMR4IF_LENGTH                                 0x1
#define _PIR2_TMR4IF_MASK                                   0x2
#define _PIR2_TMR6IF_POSN                                   0x2
#define _PIR2_TMR6IF_POSITION                               0x2
#define _PIR2_TMR6IF_SIZE                                   0x1
#define _PIR2_TMR6IF_LENGTH                                 0x1
#define _PIR2_TMR6IF_MASK                                   0x4
#define _PIR2_BCL1IF_POSN                                   0x3
#define _PIR2_BCL1IF_POSITION                               0x3
#define _PIR2_BCL1IF_SIZE                                   0x1
#define _PIR2_BCL1IF_LENGTH                                 0x1
#define _PIR2_BCL1IF_MASK                                   0x8
#define _PIR2_C1IF_POSN                                     0x5
#define _PIR2_C1IF_POSITION                                 0x5
#define _PIR2_C1IF_SIZE                                     0x1
#define _PIR2_C1IF_LENGTH                                   0x1
#define _PIR2_C1IF_MASK                                     0x20
#define _PIR2_C2IF_POSN                                     0x6
#define _PIR2_C2IF_POSITION                                 0x6
#define _PIR2_C2IF_SIZE                                     0x1
#define _PIR2_C2IF_LENGTH                                   0x1
#define _PIR2_C2IF_MASK                                     0x40
#define _PIR2_OSFIF_POSN                                    0x7
#define _PIR2_OSFIF_POSITION                                0x7
#define _PIR2_OSFIF_SIZE                                    0x1
#define _PIR2_OSFIF_LENGTH                                  0x1
#define _PIR2_OSFIF_MASK                                    0x80

// Register: PIR3
#define PIR3 PIR3
extern volatile unsigned char           PIR3                __at(0x013);
#ifndef _LIB_BUILD
asm("PIR3 equ 013h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CLC1IF                 :1;
        unsigned CLC2IF                 :1;
        unsigned CLC3IF                 :1;
        unsigned                        :1;
        unsigned ZCDIF                  :1;
        unsigned COGIF                  :1;
    };
} PIR3bits_t;
extern volatile PIR3bits_t PIR3bits __at(0x013);
// bitfield macros
#define _PIR3_CLC1IF_POSN                                   0x0
#define _PIR3_CLC1IF_POSITION                               0x0
#define _PIR3_CLC1IF_SIZE                                   0x1
#define _PIR3_CLC1IF_LENGTH                                 0x1
#define _PIR3_CLC1IF_MASK                                   0x1
#define _PIR3_CLC2IF_POSN                                   0x1
#define _PIR3_CLC2IF_POSITION                               0x1
#define _PIR3_CLC2IF_SIZE                                   0x1
#define _PIR3_CLC2IF_LENGTH                                 0x1
#define _PIR3_CLC2IF_MASK                                   0x2
#define _PIR3_CLC3IF_POSN                                   0x2
#define _PIR3_CLC3IF_POSITION                               0x2
#define _PIR3_CLC3IF_SIZE                                   0x1
#define _PIR3_CLC3IF_LENGTH                                 0x1
#define _PIR3_CLC3IF_MASK                                   0x4
#define _PIR3_ZCDIF_POSN                                    0x4
#define _PIR3_ZCDIF_POSITION                                0x4
#define _PIR3_ZCDIF_SIZE                                    0x1
#define _PIR3_ZCDIF_LENGTH                                  0x1
#define _PIR3_ZCDIF_MASK                                    0x10
#define _PIR3_COGIF_POSN                                    0x5
#define _PIR3_COGIF_POSITION                                0x5
#define _PIR3_COGIF_SIZE                                    0x1
#define _PIR3_COGIF_LENGTH                                  0x1
#define _PIR3_COGIF_MASK                                    0x20

// Register: TMR0
#define TMR0 TMR0
extern volatile unsigned char           TMR0                __at(0x015);
#ifndef _LIB_BUILD
asm("TMR0 equ 015h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TMR0                   :8;
    };
} TMR0bits_t;
extern volatile TMR0bits_t TMR0bits __at(0x015);
// bitfield macros
#define _TMR0_TMR0_POSN                                     0x0
#define _TMR0_TMR0_POSITION                                 0x0
#define _TMR0_TMR0_SIZE                                     0x8
#define _TMR0_TMR0_LENGTH                                   0x8
#define _TMR0_TMR0_MASK                                     0xFF

// Register: TMR1
#define TMR1 TMR1
extern volatile unsigned short          TMR1                __at(0x016);
#ifndef _LIB_BUILD
asm("TMR1 equ 016h");
#endif

// Register: TMR1L
#define TMR1L TMR1L
extern volatile unsigned char           TMR1L               __at(0x016);
#ifndef _LIB_BUILD
asm("TMR1L equ 016h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TMR1L                  :8;
    };
} TMR1Lbits_t;
extern volatile TMR1Lbits_t TMR1Lbits __at(0x016);
// bitfield macros
#define _TMR1L_TMR1L_POSN                                   0x0
#define _TMR1L_TMR1L_POSITION                               0x0
#define _TMR1L_TMR1L_SIZE                                   0x8
#define _TMR1L_TMR1L_LENGTH                                 0x8
#define _TMR1L_TMR1L_MASK                                   0xFF

// Register: TMR1H
#define TMR1H TMR1H
extern volatile unsigned char           TMR1H               __at(0x017);
#ifndef _LIB_BUILD
asm("TMR1H equ 017h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TMR1H                  :8;
    };
} TMR1Hbits_t;
extern volatile TMR1Hbits_t TMR1Hbits __at(0x017);
// bitfield macros
#define _TMR1H_TMR1H_POSN                                   0x0
#define _TMR1H_TMR1H_POSITION                               0x0
#define _TMR1H_TMR1H_SIZE                                   0x8
#define _TMR1H_TMR1H_LENGTH                                 0x8
#define _TMR1H_TMR1H_MASK                                   0xFF

// Register: T1CON
#define T1CON T1CON
extern volatile unsigned char           T1CON               __at(0x018);
#ifndef _LIB_BUILD
asm("T1CON equ 018h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TMR1ON                 :1;
        unsigned                        :1;
        unsigned nT1SYNC                :1;
        unsigned T1OSCEN                :1;
        unsigned T1CKPS                 :2;
        unsigned TMR1CS                 :2;
    };
    struct {
        unsigned                        :4;
        unsigned T1CKPS0                :1;
        unsigned T1CKPS1                :1;
        unsigned TMR1CS0                :1;
        unsigned TMR1CS1                :1;
    };
} T1CONbits_t;
extern volatile T1CONbits_t T1CONbits __at(0x018);
// bitfield macros
#define _T1CON_TMR1ON_POSN                                  0x0
#define _T1CON_TMR1ON_POSITION                              0x0
#define _T1CON_TMR1ON_SIZE                                  0x1
#define _T1CON_TMR1ON_LENGTH                                0x1
#define _T1CON_TMR1ON_MASK                                  0x1
#define _T1CON_nT1SYNC_POSN                                 0x2
#define _T1CON_nT1SYNC_POSITION                             0x2
#define _T1CON_nT1SYNC_SIZE                                 0x1
#define _T1CON_nT1SYNC_LENGTH                               0x1
#define _T1CON_nT1SYNC_MASK                                 0x4
#define _T1CON_T1OSCEN_POSN                                 0x3
#define _T1CON_T1OSCEN_POSITION                             0x3
#define _T1CON_T1OSCEN_SIZE                                 0x1
#define _T1CON_T1OSCEN_LENGTH                               0x1
#define _T1CON_T1OSCEN_MASK                                 0x8
#define _T1CON_T1CKPS_POSN                                  0x4
#define _T1CON_T1CKPS_POSITION                              0x4
#define _T1CON_T1CKPS_SIZE                                  0x2
#define _T1CON_T1CKPS_LENGTH                                0x2
#define _T1CON_T1CKPS_MASK                                  0x30
#define _T1CON_TMR1CS_POSN                                  0x6
#define _T1CON_TMR1CS_POSITION                              0x6
#define _T1CON_TMR1CS_SIZE                                  0x2
#define _T1CON_TMR1CS_LENGTH                                0x2
#define _T1CON_TMR1CS_MASK                                  0xC0
#define _T1CON_T1CKPS0_POSN                                 0x4
#define _T1CON_T1CKPS0_POSITION                             0x4
#define _T1CON_T1CKPS0_SIZE                                 0x1
#define _T1CON_T1CKPS0_LENGTH                               0x1
#define _T1CON_T1CKPS0_MASK                                 0x10
#define _T1CON_T1CKPS1_POSN                                 0x5
#define _T1CON_T1CKPS1_POSITION                             0x5
#define _T1CON_T1CKPS1_SIZE                                 0x1
#define _T1CON_T1CKPS1_LENGTH                               0x1
#define _T1CON_T1CKPS1_MASK                                 0x20
#define _T1CON_TMR1CS0_POSN                                 0x6
#define _T1CON_TMR1CS0_POSITION                             0x6
#define _T1CON_TMR1CS0_SIZE                                 0x1
#define _T1CON_TMR1CS0_LENGTH                               0x1
#define _T1CON_TMR1CS0_MASK                                 0x40
#define _T1CON_TMR1CS1_POSN                                 0x7
#define _T1CON_TMR1CS1_POSITION                             0x7
#define _T1CON_TMR1CS1_SIZE                                 0x1
#define _T1CON_TMR1CS1_LENGTH                               0x1
#define _T1CON_TMR1CS1_MASK                                 0x80

// Register: T1GCON
#define T1GCON T1GCON
extern volatile unsigned char           T1GCON              __at(0x019);
#ifndef _LIB_BUILD
asm("T1GCON equ 019h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned T1GSS                  :2;
        unsigned T1GVAL                 :1;
        unsigned T1GGO_nDONE            :1;
        unsigned T1GSPM                 :1;
        unsigned T1GTM                  :1;
        unsigned T1GPOL                 :1;
        unsigned TMR1GE                 :1;
    };
    struct {
        unsigned T1GSS0                 :1;
        unsigned T1GSS1                 :1;
    };
} T1GCONbits_t;
extern volatile T1GCONbits_t T1GCONbits __at(0x019);
// bitfield macros
#define _T1GCON_T1GSS_POSN                                  0x0
#define _T1GCON_T1GSS_POSITION                              0x0
#define _T1GCON_T1GSS_SIZE                                  0x2
#define _T1GCON_T1GSS_LENGTH                                0x2
#define _T1GCON_T1GSS_MASK                                  0x3
#define _T1GCON_T1GVAL_POSN                                 0x2
#define _T1GCON_T1GVAL_POSITION                             0x2
#define _T1GCON_T1GVAL_SIZE                                 0x1
#define _T1GCON_T1GVAL_LENGTH                               0x1
#define _T1GCON_T1GVAL_MASK                                 0x4
#define _T1GCON_T1GGO_nDONE_POSN                            0x3
#define _T1GCON_T1GGO_nDONE_POSITION                        0x3
#define _T1GCON_T1GGO_nDONE_SIZE                            0x1
#define _T1GCON_T1GGO_nDONE_LENGTH                          0x1
#define _T1GCON_T1GGO_nDONE_MASK                            0x8
#define _T1GCON_T1GSPM_POSN                                 0x4
#define _T1GCON_T1GSPM_POSITION                             0x4
#define _T1GCON_T1GSPM_SIZE                                 0x1
#define _T1GCON_T1GSPM_LENGTH                               0x1
#define _T1GCON_T1GSPM_MASK                                 0x10
#define _T1GCON_T1GTM_POSN                                  0x5
#define _T1GCON_T1GTM_POSITION                              0x5
#define _T1GCON_T1GTM_SIZE                                  0x1
#define _T1GCON_T1GTM_LENGTH                                0x1
#define _T1GCON_T1GTM_MASK                                  0x20
#define _T1GCON_T1GPOL_POSN                                 0x6
#define _T1GCON_T1GPOL_POSITION                             0x6
#define _T1GCON_T1GPOL_SIZE                                 0x1
#define _T1GCON_T1GPOL_LENGTH                               0x1
#define _T1GCON_T1GPOL_MASK                                 0x40
#define _T1GCON_TMR1GE_POSN                                 0x7
#define _T1GCON_TMR1GE_POSITION                             0x7
#define _T1GCON_TMR1GE_SIZE                                 0x1
#define _T1GCON_TMR1GE_LENGTH                               0x1
#define _T1GCON_TMR1GE_MASK                                 0x80
#define _T1GCON_T1GSS0_POSN                                 0x0
#define _T1GCON_T1GSS0_POSITION                             0x0
#define _T1GCON_T1GSS0_SIZE                                 0x1
#define _T1GCON_T1GSS0_LENGTH                               0x1
#define _T1GCON_T1GSS0_MASK                                 0x1
#define _T1GCON_T1GSS1_POSN                                 0x1
#define _T1GCON_T1GSS1_POSITION                             0x1
#define _T1GCON_T1GSS1_SIZE                                 0x1
#define _T1GCON_T1GSS1_LENGTH                               0x1
#define _T1GCON_T1GSS1_MASK                                 0x2

// Register: TMR2
#define TMR2 TMR2
extern volatile unsigned char           TMR2                __at(0x01A);
#ifndef _LIB_BUILD
asm("TMR2 equ 01Ah");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TMR2                   :8;
    };
} TMR2bits_t;
extern volatile TMR2bits_t TMR2bits __at(0x01A);
// bitfield macros
#define _TMR2_TMR2_POSN                                     0x0
#define _TMR2_TMR2_POSITION                                 0x0
#define _TMR2_TMR2_SIZE                                     0x8
#define _TMR2_TMR2_LENGTH                                   0x8
#define _TMR2_TMR2_MASK                                     0xFF

// Register: PR2
#define PR2 PR2
extern volatile unsigned char           PR2                 __at(0x01B);
#ifndef _LIB_BUILD
asm("PR2 equ 01Bh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PR2                    :8;
    };
} PR2bits_t;
extern volatile PR2bits_t PR2bits __at(0x01B);
// bitfield macros
#define _PR2_PR2_POSN                                       0x0
#define _PR2_PR2_POSITION                                   0x0
#define _PR2_PR2_SIZE                                       0x8
#define _PR2_PR2_LENGTH                                     0x8
#define _PR2_PR2_MASK                                       0xFF

// Register: T2CON
#define T2CON T2CON
extern volatile unsigned char           T2CON               __at(0x01C);
#ifndef _LIB_BUILD
asm("T2CON equ 01Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned T2CKPS                 :2;
        unsigned TMR2ON                 :1;
        unsigned T2OUTPS                :4;
    };
    struct {
        unsigned T2CKPS0                :1;
        unsigned T2CKPS1                :1;
        unsigned                        :1;
        unsigned T2OUTPS0               :1;
        unsigned T2OUTPS1               :1;
        unsigned T2OUTPS2               :1;
        unsigned T2OUTPS3               :1;
    };
} T2CONbits_t;
extern volatile T2CONbits_t T2CONbits __at(0x01C);
// bitfield macros
#define _T2CON_T2CKPS_POSN                                  0x0
#define _T2CON_T2CKPS_POSITION                              0x0
#define _T2CON_T2CKPS_SIZE                                  0x2
#define _T2CON_T2CKPS_LENGTH                                0x2
#define _T2CON_T2CKPS_MASK                                  0x3
#define _T2CON_TMR2ON_POSN                                  0x2
#define _T2CON_TMR2ON_POSITION                              0x2
#define _T2CON_TMR2ON_SIZE                                  0x1
#define _T2CON_TMR2ON_LENGTH                                0x1
#define _T2CON_TMR2ON_MASK                                  0x4
#define _T2CON_T2OUTPS_POSN                                 0x3
#define _T2CON_T2OUTPS_POSITION                             0x3
#define _T2CON_T2OUTPS_SIZE                                 0x4
#define _T2CON_T2OUTPS_LENGTH                               0x4
#define _T2CON_T2OUTPS_MASK                                 0x78
#define _T2CON_T2CKPS0_POSN                                 0x0
#define _T2CON_T2CKPS0_POSITION                             0x0
#define _T2CON_T2CKPS0_SIZE                                 0x1
#define _T2CON_T2CKPS0_LENGTH                               0x1
#define _T2CON_T2CKPS0_MASK                                 0x1
#define _T2CON_T2CKPS1_POSN                                 0x1
#define _T2CON_T2CKPS1_POSITION                             0x1
#define _T2CON_T2CKPS1_SIZE                                 0x1
#define _T2CON_T2CKPS1_LENGTH                               0x1
#define _T2CON_T2CKPS1_MASK                                 0x2
#define _T2CON_T2OUTPS0_POSN                                0x3
#define _T2CON_T2OUTPS0_POSITION                            0x3
#define _T2CON_T2OUTPS0_SIZE                                0x1
#define _T2CON_T2OUTPS0_LENGTH                              0x1
#define _T2CON_T2OUTPS0_MASK                                0x8
#define _T2CON_T2OUTPS1_POSN                                0x4
#define _T2CON_T2OUTPS1_POSITION                            0x4
#define _T2CON_T2OUTPS1_SIZE                                0x1
#define _T2CON_T2OUTPS1_LENGTH                              0x1
#define _T2CON_T2OUTPS1_MASK                                0x10
#define _T2CON_T2OUTPS2_POSN                                0x5
#define _T2CON_T2OUTPS2_POSITION                            0x5
#define _T2CON_T2OUTPS2_SIZE                                0x1
#define _T2CON_T2OUTPS2_LENGTH                              0x1
#define _T2CON_T2OUTPS2_MASK                                0x20
#define _T2CON_T2OUTPS3_POSN                                0x6
#define _T2CON_T2OUTPS3_POSITION                            0x6
#define _T2CON_T2OUTPS3_SIZE                                0x1
#define _T2CON_T2OUTPS3_LENGTH                              0x1
#define _T2CON_T2OUTPS3_MASK                                0x40

// Register: TRISA
#define TRISA TRISA
extern volatile unsigned char           TRISA               __at(0x08C);
#ifndef _LIB_BUILD
asm("TRISA equ 08Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TRISA0                 :1;
        unsigned TRISA1                 :1;
        unsigned TRISA2                 :1;
        unsigned                        :1;
        unsigned TRISA4                 :1;
        unsigned TRISA5                 :1;
    };
} TRISAbits_t;
extern volatile TRISAbits_t TRISAbits __at(0x08C);
// bitfield macros
#define _TRISA_TRISA0_POSN                                  0x0
#define _TRISA_TRISA0_POSITION                              0x0
#define _TRISA_TRISA0_SIZE                                  0x1
#define _TRISA_TRISA0_LENGTH                                0x1
#define _TRISA_TRISA0_MASK                                  0x1
#define _TRISA_TRISA1_POSN                                  0x1
#define _TRISA_TRISA1_POSITION                              0x1
#define _TRISA_TRISA1_SIZE                                  0x1
#define _TRISA_TRISA1_LENGTH                                0x1
#define _TRISA_TRISA1_MASK                                  0x2
#define _TRISA_TRISA2_POSN                                  0x2
#define _TRISA_TRISA2_POSITION                              0x2
#define _TRISA_TRISA2_SIZE                                  0x1
#define _TRISA_TRISA2_LENGTH                                0x1
#define _TRISA_TRISA2_MASK                                  0x4
#define _TRISA_TRISA4_POSN                                  0x4
#define _TRISA_TRISA4_POSITION                              0x4
#define _TRISA_TRISA4_SIZE                                  0x1
#define _TRISA_TRISA4_LENGTH                                0x1
#define _TRISA_TRISA4_MASK                                  0x10
#define _TRISA_TRISA5_POSN                                  0x5
#define _TRISA_TRISA5_POSITION                              0x5
#define _TRISA_TRISA5_SIZE                                  0x1
#define _TRISA_TRISA5_LENGTH                                0x1
#define _TRISA_TRISA5_MASK                                  0x20

// Register: TRISB
#define TRISB TRISB
extern volatile unsigned char           TRISB               __at(0x08D);
#ifndef _LIB_BUILD
asm("TRISB equ 08Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned TRISB4                 :1;
        unsigned TRISB5                 :1;
        unsigned TRISB6                 :1;
        unsigned TRISB7                 :1;
    };
} TRISBbits_t;
extern volatile TRISBbits_t TRISBbits __at(0x08D);
// bitfield macros
#define _TRISB_TRISB4_POSN                                  0x4
#define _TRISB_TRISB4_POSITION                              0x4
#define _TRISB_TRISB4_SIZE                                  0x1
#define _TRISB_TRISB4_LENGTH                                0x1
#define _TRISB_TRISB4_MASK                                  0x10
#define _TRISB_TRISB5_POSN                                  0x5
#define _TRISB_TRISB5_POSITION                              0x5
#define _TRISB_TRISB5_SIZE                                  0x1
#define _TRISB_TRISB5_LENGTH                                0x1
#define _TRISB_TRISB5_MASK                                  0x20
#define _TRISB_TRISB6_POSN                                  0x6
#define _TRISB_TRISB6_POSITION                              0x6
#define _TRISB_TRISB6_SIZE                                  0x1
#define _TRISB_TRISB6_LENGTH                                0x1
#define _TRISB_TRISB6_MASK                                  0x40
#define _TRISB_TRISB7_POSN                                  0x7
#define _TRISB_TRISB7_POSITION                              0x7
#define _TRISB_TRISB7_SIZE                                  0x1
#define _TRISB_TRISB7_LENGTH                                0x1
#define _TRISB_TRISB7_MASK                                  0x80

// Register: TRISC
#define TRISC TRISC
extern volatile unsigned char           TRISC               __at(0x08E);
#ifndef _LIB_BUILD
asm("TRISC equ 08Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TRISC0                 :1;
        unsigned TRISC1                 :1;
        unsigned TRISC2                 :1;
        unsigned TRISC3                 :1;
        unsigned TRISC4                 :1;
        unsigned TRISC5                 :1;
        unsigned TRISC6                 :1;
        unsigned TRISC7                 :1;
    };
} TRISCbits_t;
extern volatile TRISCbits_t TRISCbits __at(0x08E);
// bitfield macros
#define _TRISC_TRISC0_POSN                                  0x0
#define _TRISC_TRISC0_POSITION                              0x0
#define _TRISC_TRISC0_SIZE                                  0x1
#define _TRISC_TRISC0_LENGTH                                0x1
#define _TRISC_TRISC0_MASK                                  0x1
#define _TRISC_TRISC1_POSN                                  0x1
#define _TRISC_TRISC1_POSITION                              0x1
#define _TRISC_TRISC1_SIZE                                  0x1
#define _TRISC_TRISC1_LENGTH                                0x1
#define _TRISC_TRISC1_MASK                                  0x2
#define _TRISC_TRISC2_POSN                                  0x2
#define _TRISC_TRISC2_POSITION                              0x2
#define _TRISC_TRISC2_SIZE                                  0x1
#define _TRISC_TRISC2_LENGTH                                0x1
#define _TRISC_TRISC2_MASK                                  0x4
#define _TRISC_TRISC3_POSN                                  0x3
#define _TRISC_TRISC3_POSITION                              0x3
#define _TRISC_TRISC3_SIZE                                  0x1
#define _TRISC_TRISC3_LENGTH                                0x1
#define _TRISC_TRISC3_MASK                                  0x8
#define _TRISC_TRISC4_POSN                                  0x4
#define _TRISC_TRISC4_POSITION                              0x4
#define _TRISC_TRISC4_SIZE                                  0x1
#define _TRISC_TRISC4_LENGTH                                0x1
#define _TRISC_TRISC4_MASK                                  0x10
#define _TRISC_TRISC5_POSN                                  0x5
#define _TRISC_TRISC5_POSITION                              0x5
#define _TRISC_TRISC5_SIZE                                  0x1
#define _TRISC_TRISC5_LENGTH                                0x1
#define _TRISC_TRISC5_MASK                                  0x20
#define _TRISC_TRISC6_POSN                                  0x6
#define _TRISC_TRISC6_POSITION                              0x6
#define _TRISC_TRISC6_SIZE                                  0x1
#define _TRISC_TRISC6_LENGTH                                0x1
#define _TRISC_TRISC6_MASK                                  0x40
#define _TRISC_TRISC7_POSN                                  0x7
#define _TRISC_TRISC7_POSITION                              0x7
#define _TRISC_TRISC7_SIZE                                  0x1
#define _TRISC_TRISC7_LENGTH                                0x1
#define _TRISC_TRISC7_MASK                                  0x80

// Register: PIE1
#define PIE1 PIE1
extern volatile unsigned char           PIE1                __at(0x091);
#ifndef _LIB_BUILD
asm("PIE1 equ 091h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TMR1IE                 :1;
        unsigned TMR2IE                 :1;
        unsigned CCP1IE                 :1;
        unsigned SSP1IE                 :1;
        unsigned TXIE                   :1;
        unsigned RCIE                   :1;
        unsigned ADIE                   :1;
        unsigned TMR1GIE                :1;
    };
    struct {
        unsigned                        :2;
        unsigned CCPIE                  :1;
    };
} PIE1bits_t;
extern volatile PIE1bits_t PIE1bits __at(0x091);
// bitfield macros
#define _PIE1_TMR1IE_POSN                                   0x0
#define _PIE1_TMR1IE_POSITION                               0x0
#define _PIE1_TMR1IE_SIZE                                   0x1
#define _PIE1_TMR1IE_LENGTH                                 0x1
#define _PIE1_TMR1IE_MASK                                   0x1
#define _PIE1_TMR2IE_POSN                                   0x1
#define _PIE1_TMR2IE_POSITION                               0x1
#define _PIE1_TMR2IE_SIZE                                   0x1
#define _PIE1_TMR2IE_LENGTH                                 0x1
#define _PIE1_TMR2IE_MASK                                   0x2
#define _PIE1_CCP1IE_POSN                                   0x2
#define _PIE1_CCP1IE_POSITION                               0x2
#define _PIE1_CCP1IE_SIZE                                   0x1
#define _PIE1_CCP1IE_LENGTH                                 0x1
#define _PIE1_CCP1IE_MASK                                   0x4
#define _PIE1_SSP1IE_POSN                                   0x3
#define _PIE1_SSP1IE_POSITION                               0x3
#define _PIE1_SSP1IE_SIZE                                   0x1
#define _PIE1_SSP1IE_LENGTH                                 0x1
#define _PIE1_SSP1IE_MASK                                   0x8
#define _PIE1_TXIE_POSN                                     0x4
#define _PIE1_TXIE_POSITION                                 0x4
#define _PIE1_TXIE_SIZE                                     0x1
#define _PIE1_TXIE_LENGTH                                   0x1
#define _PIE1_TXIE_MASK                                     0x10
#define _PIE1_RCIE_POSN                                     0x5
#define _PIE1_RCIE_POSITION                                 0x5
#define _PIE1_RCIE_SIZE                                     0x1
#define _PIE1_RCIE_LENGTH                                   0x1
#define _PIE1_RCIE_MASK                                     0x20
#define _PIE1_ADIE_POSN                                     0x6
#define _PIE1_ADIE_POSITION                                 0x6
#define _PIE1_ADIE_SIZE                                     0x1
#define _PIE1_ADIE_LENGTH                                   0x1
#define _PIE1_ADIE_MASK                                     0x40
#define _PIE1_TMR1GIE_POSN                                  0x7
#define _PIE1_TMR1GIE_POSITION                              0x7
#define _PIE1_TMR1GIE_SIZE                                  0x1
#define _PIE1_TMR1GIE_LENGTH                                0x1
#define _PIE1_TMR1GIE_MASK                                  0x80
#define _PIE1_CCPIE_POSN                                    0x2
#define _PIE1_CCPIE_POSITION                                0x2
#define _PIE1_CCPIE_SIZE                                    0x1
#define _PIE1_CCPIE_LENGTH                                  0x1
#define _PIE1_CCPIE_MASK                                    0x4

// Register: PIE2
#define PIE2 PIE2
extern volatile unsigned char           PIE2                __at(0x092);
#ifndef _LIB_BUILD
asm("PIE2 equ 092h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CCP2IE                 :1;
        unsigned TMR4IE                 :1;
        unsigned TMR6IE                 :1;
        unsigned BCL1IE                 :1;
        unsigned                        :1;
        unsigned C1IE                   :1;
        unsigned C2IE                   :1;
        unsigned OSFIE                  :1;
    };
} PIE2bits_t;
extern volatile PIE2bits_t PIE2bits __at(0x092);
// bitfield macros
#define _PIE2_CCP2IE_POSN                                   0x0
#define _PIE2_CCP2IE_POSITION                               0x0
#define _PIE2_CCP2IE_SIZE                                   0x1
#define _PIE2_CCP2IE_LENGTH                                 0x1
#define _PIE2_CCP2IE_MASK                                   0x1
#define _PIE2_TMR4IE_POSN                                   0x1
#define _PIE2_TMR4IE_POSITION                               0x1
#define _PIE2_TMR4IE_SIZE                                   0x1
#define _PIE2_TMR4IE_LENGTH                                 0x1
#define _PIE2_TMR4IE_MASK                                   0x2
#define _PIE2_TMR6IE_POSN                                   0x2
#define _PIE2_TMR6IE_POSITION                               0x2
#define _PIE2_TMR6IE_SIZE                                   0x1
#define _PIE2_TMR6IE_LENGTH                                 0x1
#define _PIE2_TMR6IE_MASK                                   0x4
#define _PIE2_BCL1IE_POSN                                   0x3
#define _PIE2_BCL1IE_POSITION                               0x3
#define _PIE2_BCL1IE_SIZE                                   0x1
#define _PIE2_BCL1IE_LENGTH                                 0x1
#define _PIE2_BCL1IE_MASK                                   0x8
#define _PIE2_C1IE_POSN                                     0x5
#define _PIE2_C1IE_POSITION                                 0x5
#define _PIE2_C1IE_SIZE                                     0x1
#define _PIE2_C1IE_LENGTH                                   0x1
#define _PIE2_C1IE_MASK                                     0x20
#define _PIE2_C2IE_POSN                                     0x6
#define _PIE2_C2IE_POSITION                                 0x6
#define _PIE2_C2IE_SIZE                                     0x1
#define _PIE2_C2IE_LENGTH                                   0x1
#define _PIE2_C2IE_MASK                                     0x40
#define _PIE2_OSFIE_POSN                                    0x7
#define _PIE2_OSFIE_POSITION                                0x7
#define _PIE2_OSFIE_SIZE                                    0x1
#define _PIE2_OSFIE_LENGTH                                  0x1
#define _PIE2_OSFIE_MASK                                    0x80

// Register: PIE3
#define PIE3 PIE3
extern volatile unsigned char           PIE3                __at(0x093);
#ifndef _LIB_BUILD
asm("PIE3 equ 093h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CLC1IE                 :1;
        unsigned CLC2IE                 :1;
        unsigned CLC3IE                 :1;
        unsigned                        :1;
        unsigned ZCDIE                  :1;
        unsigned COGIE                  :1;
    };
} PIE3bits_t;
extern volatile PIE3bits_t PIE3bits __at(0x093);
// bitfield macros
#define _PIE3_CLC1IE_POSN                                   0x0
#define _PIE3_CLC1IE_POSITION                               0x0
#define _PIE3_CLC1IE_SIZE                                   0x1
#define _PIE3_CLC1IE_LENGTH                                 0x1
#define _PIE3_CLC1IE_MASK                                   0x1
#define _PIE3_CLC2IE_POSN                                   0x1
#define _PIE3_CLC2IE_POSITION                               0x1
#define _PIE3_CLC2IE_SIZE                                   0x1
#define _PIE3_CLC2IE_LENGTH                                 0x1
#define _PIE3_CLC2IE_MASK                                   0x2
#define _PIE3_CLC3IE_POSN                                   0x2
#define _PIE3_CLC3IE_POSITION                               0x2
#define _PIE3_CLC3IE_SIZE                                   0x1
#define _PIE3_CLC3IE_LENGTH                                 0x1
#define _PIE3_CLC3IE_MASK                                   0x4
#define _PIE3_ZCDIE_POSN                                    0x4
#define _PIE3_ZCDIE_POSITION                                0x4
#define _PIE3_ZCDIE_SIZE                                    0x1
#define _PIE3_ZCDIE_LENGTH                                  0x1
#define _PIE3_ZCDIE_MASK                                    0x10
#define _PIE3_COGIE_POSN                                    0x5
#define _PIE3_COGIE_POSITION                                0x5
#define _PIE3_COGIE_SIZE                                    0x1
#define _PIE3_COGIE_LENGTH                                  0x1
#define _PIE3_COGIE_MASK                                    0x20

// Register: OPTION_REG
#define OPTION_REG OPTION_REG
extern volatile unsigned char           OPTION_REG          __at(0x095);
#ifndef _LIB_BUILD
asm("OPTION_REG equ 095h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PS                     :3;
        unsigned PSA                    :1;
        unsigned TMR0SE                 :1;
        unsigned TMR0CS                 :1;
        unsigned INTEDG                 :1;
        unsigned nWPUEN                 :1;
    };
    struct {
        unsigned PS0                    :1;
        unsigned PS1                    :1;
        unsigned PS2                    :1;
        unsigned                        :1;
        unsigned T0SE                   :1;
        unsigned T0CS                   :1;
    };
} OPTION_REGbits_t;
extern volatile OPTION_REGbits_t OPTION_REGbits __at(0x095);
// bitfield macros
#define _OPTION_REG_PS_POSN                                 0x0
#define _OPTION_REG_PS_POSITION                             0x0
#define _OPTION_REG_PS_SIZE                                 0x3
#define _OPTION_REG_PS_LENGTH                               0x3
#define _OPTION_REG_PS_MASK                                 0x7
#define _OPTION_REG_PSA_POSN                                0x3
#define _OPTION_REG_PSA_POSITION                            0x3
#define _OPTION_REG_PSA_SIZE                                0x1
#define _OPTION_REG_PSA_LENGTH                              0x1
#define _OPTION_REG_PSA_MASK                                0x8
#define _OPTION_REG_TMR0SE_POSN                             0x4
#define _OPTION_REG_TMR0SE_POSITION                         0x4
#define _OPTION_REG_TMR0SE_SIZE                             0x1
#define _OPTION_REG_TMR0SE_LENGTH                           0x1
#define _OPTION_REG_TMR0SE_MASK                             0x10
#define _OPTION_REG_TMR0CS_POSN                             0x5
#define _OPTION_REG_TMR0CS_POSITION                         0x5
#define _OPTION_REG_TMR0CS_SIZE                             0x1
#define _OPTION_REG_TMR0CS_LENGTH                           0x1
#define _OPTION_REG_TMR0CS_MASK                             0x20
#define _OPTION_REG_INTEDG_POSN                             0x6
#define _OPTION_REG_INTEDG_POSITION                         0x6
#define _OPTION_REG_INTEDG_SIZE                             0x1
#define _OPTION_REG_INTEDG_LENGTH                           0x1
#define _OPTION_REG_INTEDG_MASK                             0x40
#define _OPTION_REG_nWPUEN_POSN                             0x7
#define _OPTION_REG_nWPUEN_POSITION                         0x7
#define _OPTION_REG_nWPUEN_SIZE                             0x1
#define _OPTION_REG_nWPUEN_LENGTH                           0x1
#define _OPTION_REG_nWPUEN_MASK                             0x80
#define _OPTION_REG_PS0_POSN                                0x0
#define _OPTION_REG_PS0_POSITION                            0x0
#define _OPTION_REG_PS0_SIZE                                0x1
#define _OPTION_REG_PS0_LENGTH                              0x1
#define _OPTION_REG_PS0_MASK                                0x1
#define _OPTION_REG_PS1_POSN                                0x1
#define _OPTION_REG_PS1_POSITION                            0x1
#define _OPTION_REG_PS1_SIZE                                0x1
#define _OPTION_REG_PS1_LENGTH                              0x1
#define _OPTION_REG_PS1_MASK                                0x2
#define _OPTION_REG_PS2_POSN                                0x2
#define _OPTION_REG_PS2_POSITION                            0x2
#define _OPTION_REG_PS2_SIZE                                0x1
#define _OPTION_REG_PS2_LENGTH                              0x1
#define _OPTION_REG_PS2_MASK                                0x4
#define _OPTION_REG_T0SE_POSN                               0x4
#define _OPTION_REG_T0SE_POSITION                           0x4
#define _OPTION_REG_T0SE_SIZE                               0x1
#define _OPTION_REG_T0SE_LENGTH                             0x1
#define _OPTION_REG_T0SE_MASK                               0x10
#define _OPTION_REG_T0CS_POSN                               0x5
#define _OPTION_REG_T0CS_POSITION                           0x5
#define _OPTION_REG_T0CS_SIZE                               0x1
#define _OPTION_REG_T0CS_LENGTH                             0x1
#define _OPTION_REG_T0CS_MASK                               0x20

// Register: PCON
#define PCON PCON
extern volatile unsigned char           PCON                __at(0x096);
#ifndef _LIB_BUILD
asm("PCON equ 096h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned nBOR                   :1;
        unsigned nPOR                   :1;
        unsigned nRI                    :1;
        unsigned nRMCLR                 :1;
        unsigned nRWDT                  :1;
        unsigned                        :1;
        unsigned STKUNF                 :1;
        unsigned STKOVF                 :1;
    };
} PCONbits_t;
extern volatile PCONbits_t PCONbits __at(0x096);
// bitfield macros
#define _PCON_nBOR_POSN                                     0x0
#define _PCON_nBOR_POSITION                                 0x0
#define _PCON_nBOR_SIZE                                     0x1
#define _PCON_nBOR_LENGTH                                   0x1
#define _PCON_nBOR_MASK                                     0x1
#define _PCON_nPOR_POSN                                     0x1
#define _PCON_nPOR_POSITION                                 0x1
#define _PCON_nPOR_SIZE                                     0x1
#define _PCON_nPOR_LENGTH                                   0x1
#define _PCON_nPOR_MASK                                     0x2
#define _PCON_nRI_POSN                                      0x2
#define _PCON_nRI_POSITION                                  0x2
#define _PCON_nRI_SIZE                                      0x1
#define _PCON_nRI_LENGTH                                    0x1
#define _PCON_nRI_MASK                                      0x4
#define _PCON_nRMCLR_POSN                                   0x3
#define _PCON_nRMCLR_POSITION                               0x3
#define _PCON_nRMCLR_SIZE                                   0x1
#define _PCON_nRMCLR_LENGTH                                 0x1
#define _PCON_nRMCLR_MASK                                   0x8
#define _PCON_nRWDT_POSN                                    0x4
#define _PCON_nRWDT_POSITION                                0x4
#define _PCON_nRWDT_SIZE                                    0x1
#define _PCON_nRWDT_LENGTH                                  0x1
#define _PCON_nRWDT_MASK                                    0x10
#define _PCON_STKUNF_POSN                                   0x6
#define _PCON_STKUNF_POSITION                               0x6
#define _PCON_STKUNF_SIZE                                   0x1
#define _PCON_STKUNF_LENGTH                                 0x1
#define _PCON_STKUNF_MASK                                   0x40
#define _PCON_STKOVF_POSN                                   0x7
#define _PCON_STKOVF_POSITION                               0x7
#define _PCON_STKOVF_SIZE                                   0x1
#define _PCON_STKOVF_LENGTH                                 0x1
#define _PCON_STKOVF_MASK                                   0x80

// Register: WDTCON
#define WDTCON WDTCON
extern volatile unsigned char           WDTCON              __at(0x097);
#ifndef _LIB_BUILD
asm("WDTCON equ 097h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SWDTEN                 :1;
        unsigned WDTPS                  :5;
    };
    struct {
        unsigned                        :1;
        unsigned WDTPS0                 :1;
        unsigned WDTPS1                 :1;
        unsigned WDTPS2                 :1;
        unsigned WDTPS3                 :1;
        unsigned WDTPS4                 :1;
    };
} WDTCONbits_t;
extern volatile WDTCONbits_t WDTCONbits __at(0x097);
// bitfield macros
#define _WDTCON_SWDTEN_POSN                                 0x0
#define _WDTCON_SWDTEN_POSITION                             0x0
#define _WDTCON_SWDTEN_SIZE                                 0x1
#define _WDTCON_SWDTEN_LENGTH                               0x1
#define _WDTCON_SWDTEN_MASK                                 0x1
#define _WDTCON_WDTPS_POSN                                  0x1
#define _WDTCON_WDTPS_POSITION                              0x1
#define _WDTCON_WDTPS_SIZE                                  0x5
#define _WDTCON_WDTPS_LENGTH                                0x5
#define _WDTCON_WDTPS_MASK                                  0x3E
#define _WDTCON_WDTPS0_POSN                                 0x1
#define _WDTCON_WDTPS0_POSITION                             0x1
#define _WDTCON_WDTPS0_SIZE                                 0x1
#define _WDTCON_WDTPS0_LENGTH                               0x1
#define _WDTCON_WDTPS0_MASK                                 0x2
#define _WDTCON_WDTPS1_POSN                                 0x2
#define _WDTCON_WDTPS1_POSITION                             0x2
#define _WDTCON_WDTPS1_SIZE                                 0x1
#define _WDTCON_WDTPS1_LENGTH                               0x1
#define _WDTCON_WDTPS1_MASK                                 0x4
#define _WDTCON_WDTPS2_POSN                                 0x3
#define _WDTCON_WDTPS2_POSITION                             0x3
#define _WDTCON_WDTPS2_SIZE                                 0x1
#define _WDTCON_WDTPS2_LENGTH                               0x1
#define _WDTCON_WDTPS2_MASK                                 0x8
#define _WDTCON_WDTPS3_POSN                                 0x4
#define _WDTCON_WDTPS3_POSITION                             0x4
#define _WDTCON_WDTPS3_SIZE                                 0x1
#define _WDTCON_WDTPS3_LENGTH                               0x1
#define _WDTCON_WDTPS3_MASK                                 0x10
#define _WDTCON_WDTPS4_POSN                                 0x5
#define _WDTCON_WDTPS4_POSITION                             0x5
#define _WDTCON_WDTPS4_SIZE                                 0x1
#define _WDTCON_WDTPS4_LENGTH                               0x1
#define _WDTCON_WDTPS4_MASK                                 0x20

// Register: OSCTUNE
#define OSCTUNE OSCTUNE
extern volatile unsigned char           OSCTUNE             __at(0x098);
#ifndef _LIB_BUILD
asm("OSCTUNE equ 098h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TUN                    :6;
    };
    struct {
        unsigned TUN0                   :1;
        unsigned TUN1                   :1;
        unsigned TUN2                   :1;
        unsigned TUN3                   :1;
        unsigned TUN4                   :1;
        unsigned TUN5                   :1;
    };
} OSCTUNEbits_t;
extern volatile OSCTUNEbits_t OSCTUNEbits __at(0x098);
// bitfield macros
#define _OSCTUNE_TUN_POSN                                   0x0
#define _OSCTUNE_TUN_POSITION                               0x0
#define _OSCTUNE_TUN_SIZE                                   0x6
#define _OSCTUNE_TUN_LENGTH                                 0x6
#define _OSCTUNE_TUN_MASK                                   0x3F
#define _OSCTUNE_TUN0_POSN                                  0x0
#define _OSCTUNE_TUN0_POSITION                              0x0
#define _OSCTUNE_TUN0_SIZE                                  0x1
#define _OSCTUNE_TUN0_LENGTH                                0x1
#define _OSCTUNE_TUN0_MASK                                  0x1
#define _OSCTUNE_TUN1_POSN                                  0x1
#define _OSCTUNE_TUN1_POSITION                              0x1
#define _OSCTUNE_TUN1_SIZE                                  0x1
#define _OSCTUNE_TUN1_LENGTH                                0x1
#define _OSCTUNE_TUN1_MASK                                  0x2
#define _OSCTUNE_TUN2_POSN                                  0x2
#define _OSCTUNE_TUN2_POSITION                              0x2
#define _OSCTUNE_TUN2_SIZE                                  0x1
#define _OSCTUNE_TUN2_LENGTH                                0x1
#define _OSCTUNE_TUN2_MASK                                  0x4
#define _OSCTUNE_TUN3_POSN                                  0x3
#define _OSCTUNE_TUN3_POSITION                              0x3
#define _OSCTUNE_TUN3_SIZE                                  0x1
#define _OSCTUNE_TUN3_LENGTH                                0x1
#define _OSCTUNE_TUN3_MASK                                  0x8
#define _OSCTUNE_TUN4_POSN                                  0x4
#define _OSCTUNE_TUN4_POSITION                              0x4
#define _OSCTUNE_TUN4_SIZE                                  0x1
#define _OSCTUNE_TUN4_LENGTH                                0x1
#define _OSCTUNE_TUN4_MASK                                  0x10
#define _OSCTUNE_TUN5_POSN                                  0x5
#define _OSCTUNE_TUN5_POSITION                              0x5
#define _OSCTUNE_TUN5_SIZE                                  0x1
#define _OSCTUNE_TUN5_LENGTH                                0x1
#define _OSCTUNE_TUN5_MASK                                  0x20

// Register: OSCCON
#define OSCCON OSCCON
extern volatile unsigned char           OSCCON              __at(0x099);
#ifndef _LIB_BUILD
asm("OSCCON equ 099h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SCS                    :2;
        unsigned                        :1;
        unsigned IRCF                   :4;
        unsigned SPLLEN                 :1;
    };
    struct {
        unsigned SCS0                   :1;
        unsigned SCS1                   :1;
        unsigned                        :1;
        unsigned IRCF0                  :1;
        unsigned IRCF1                  :1;
        unsigned IRCF2                  :1;
        unsigned IRCF3                  :1;
    };
} OSCCONbits_t;
extern volatile OSCCONbits_t OSCCONbits __at(0x099);
// bitfield macros
#define _OSCCON_SCS_POSN                                    0x0
#define _OSCCON_SCS_POSITION                                0x0
#define _OSCCON_SCS_SIZE                                    0x2
#define _OSCCON_SCS_LENGTH                                  0x2
#define _OSCCON_SCS_MASK                                    0x3
#define _OSCCON_IRCF_POSN                                   0x3
#define _OSCCON_IRCF_POSITION                               0x3
#define _OSCCON_IRCF_SIZE                                   0x4
#define _OSCCON_IRCF_LENGTH                                 0x4
#define _OSCCON_IRCF_MASK                                   0x78
#define _OSCCON_SPLLEN_POSN                                 0x7
#define _OSCCON_SPLLEN_POSITION                             0x7
#define _OSCCON_SPLLEN_SIZE                                 0x1
#define _OSCCON_SPLLEN_LENGTH                               0x1
#define _OSCCON_SPLLEN_MASK                                 0x80
#define _OSCCON_SCS0_POSN                                   0x0
#define _OSCCON_SCS0_POSITION                               0x0
#define _OSCCON_SCS0_SIZE                                   0x1
#define _OSCCON_SCS0_LENGTH                                 0x1
#define _OSCCON_SCS0_MASK                                   0x1
#define _OSCCON_SCS1_POSN                                   0x1
#define _OSCCON_SCS1_POSITION                               0x1
#define _OSCCON_SCS1_SIZE                                   0x1
#define _OSCCON_SCS1_LENGTH                                 0x1
#define _OSCCON_SCS1_MASK                                   0x2
#define _OSCCON_IRCF0_POSN                                  0x3
#define _OSCCON_IRCF0_POSITION                              0x3
#define _OSCCON_IRCF0_SIZE                                  0x1
#define _OSCCON_IRCF0_LENGTH                                0x1
#define _OSCCON_IRCF0_MASK                                  0x8
#define _OSCCON_IRCF1_POSN                                  0x4
#define _OSCCON_IRCF1_POSITION                              0x4
#define _OSCCON_IRCF1_SIZE                                  0x1
#define _OSCCON_IRCF1_LENGTH                                0x1
#define _OSCCON_IRCF1_MASK                                  0x10
#define _OSCCON_IRCF2_POSN                                  0x5
#define _OSCCON_IRCF2_POSITION                              0x5
#define _OSCCON_IRCF2_SIZE                                  0x1
#define _OSCCON_IRCF2_LENGTH                                0x1
#define _OSCCON_IRCF2_MASK                                  0x20
#define _OSCCON_IRCF3_POSN                                  0x6
#define _OSCCON_IRCF3_POSITION                              0x6
#define _OSCCON_IRCF3_SIZE                                  0x1
#define _OSCCON_IRCF3_LENGTH                                0x1
#define _OSCCON_IRCF3_MASK                                  0x40

// Register: OSCSTAT
#define OSCSTAT OSCSTAT
extern volatile unsigned char           OSCSTAT             __at(0x09A);
#ifndef _LIB_BUILD
asm("OSCSTAT equ 09Ah");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned HFIOFS                 :1;
        unsigned LFIOFR                 :1;
        unsigned MFIOFR                 :1;
        unsigned HFIOFL                 :1;
        unsigned HFIOFR                 :1;
        unsigned OSTS                   :1;
        unsigned PLLR                   :1;
        unsigned SOSCR                  :1;
    };
} OSCSTATbits_t;
extern volatile OSCSTATbits_t OSCSTATbits __at(0x09A);
// bitfield macros
#define _OSCSTAT_HFIOFS_POSN                                0x0
#define _OSCSTAT_HFIOFS_POSITION                            0x0
#define _OSCSTAT_HFIOFS_SIZE                                0x1
#define _OSCSTAT_HFIOFS_LENGTH                              0x1
#define _OSCSTAT_HFIOFS_MASK                                0x1
#define _OSCSTAT_LFIOFR_POSN                                0x1
#define _OSCSTAT_LFIOFR_POSITION                            0x1
#define _OSCSTAT_LFIOFR_SIZE                                0x1
#define _OSCSTAT_LFIOFR_LENGTH                              0x1
#define _OSCSTAT_LFIOFR_MASK                                0x2
#define _OSCSTAT_MFIOFR_POSN                                0x2
#define _OSCSTAT_MFIOFR_POSITION                            0x2
#define _OSCSTAT_MFIOFR_SIZE                                0x1
#define _OSCSTAT_MFIOFR_LENGTH                              0x1
#define _OSCSTAT_MFIOFR_MASK                                0x4
#define _OSCSTAT_HFIOFL_POSN                                0x3
#define _OSCSTAT_HFIOFL_POSITION                            0x3
#define _OSCSTAT_HFIOFL_SIZE                                0x1
#define _OSCSTAT_HFIOFL_LENGTH                              0x1
#define _OSCSTAT_HFIOFL_MASK                                0x8
#define _OSCSTAT_HFIOFR_POSN                                0x4
#define _OSCSTAT_HFIOFR_POSITION                            0x4
#define _OSCSTAT_HFIOFR_SIZE                                0x1
#define _OSCSTAT_HFIOFR_LENGTH                              0x1
#define _OSCSTAT_HFIOFR_MASK                                0x10
#define _OSCSTAT_OSTS_POSN                                  0x5
#define _OSCSTAT_OSTS_POSITION                              0x5
#define _OSCSTAT_OSTS_SIZE                                  0x1
#define _OSCSTAT_OSTS_LENGTH                                0x1
#define _OSCSTAT_OSTS_MASK                                  0x20
#define _OSCSTAT_PLLR_POSN                                  0x6
#define _OSCSTAT_PLLR_POSITION                              0x6
#define _OSCSTAT_PLLR_SIZE                                  0x1
#define _OSCSTAT_PLLR_LENGTH                                0x1
#define _OSCSTAT_PLLR_MASK                                  0x40
#define _OSCSTAT_SOSCR_POSN                                 0x7
#define _OSCSTAT_SOSCR_POSITION                             0x7
#define _OSCSTAT_SOSCR_SIZE                                 0x1
#define _OSCSTAT_SOSCR_LENGTH                               0x1
#define _OSCSTAT_SOSCR_MASK                                 0x80

// Register: ADRES
#define ADRES ADRES
extern volatile unsigned short          ADRES               __at(0x09B);
#ifndef _LIB_BUILD
asm("ADRES equ 09Bh");
#endif

// Register: ADRESL
#define ADRESL ADRESL
extern volatile unsigned char           ADRESL              __at(0x09B);
#ifndef _LIB_BUILD
asm("ADRESL equ 09Bh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned ADRESL                 :8;
    };
} ADRESLbits_t;
extern volatile ADRESLbits_t ADRESLbits __at(0x09B);
// bitfield macros
#define _ADRESL_ADRESL_POSN                                 0x0
#define _ADRESL_ADRESL_POSITION                             0x0
#define _ADRESL_ADRESL_SIZE                                 0x8
#define _ADRESL_ADRESL_LENGTH                               0x8
#define _ADRESL_ADRESL_MASK                                 0xFF

// Register: ADRESH
#define ADRESH ADRESH
extern volatile unsigned char           ADRESH              __at(0x09C);
#ifndef _LIB_BUILD
asm("ADRESH equ 09Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned ADRESH                 :8;
    };
} ADRESHbits_t;
extern volatile ADRESHbits_t ADRESHbits __at(0x09C);
// bitfield macros
#define _ADRESH_ADRESH_POSN                                 0x0
#define _ADRESH_ADRESH_POSITION                             0x0
#define _ADRESH_ADRESH_SIZE                                 0x8
#define _ADRESH_ADRESH_LENGTH                               0x8
#define _ADRESH_ADRESH_MASK                                 0xFF

// Register: ADCON0
#define ADCON0 ADCON0
extern volatile unsigned char           ADCON0              __at(0x09D);
#ifndef _LIB_BUILD
asm("ADCON0 equ 09Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned ADON                   :1;
        unsigned GO_nDONE               :1;
        unsigned CHS                    :5;
    };
    struct {
        unsigned                        :1;
        unsigned ADGO                   :1;
        unsigned CHS0                   :1;
        unsigned CHS1                   :1;
        unsigned CHS2                   :1;
        unsigned CHS3                   :1;
        unsigned CHS4                   :1;
    };
    struct {
        unsigned                        :1;
        unsigned GO                     :1;
    };
} ADCON0bits_t;
extern volatile ADCON0bits_t ADCON0bits __at(0x09D);
// bitfield macros
#define _ADCON0_ADON_POSN                                   0x0
#define _ADCON0_ADON_POSITION                               0x0
#define _ADCON0_ADON_SIZE                                   0x1
#define _ADCON0_ADON_LENGTH                                 0x1
#define _ADCON0_ADON_MASK                                   0x1
#define _ADCON0_GO_nDONE_POSN                               0x1
#define _ADCON0_GO_nDONE_POSITION                           0x1
#define _ADCON0_GO_nDONE_SIZE                               0x1
#define _ADCON0_GO_nDONE_LENGTH                             0x1
#define _ADCON0_GO_nDONE_MASK                               0x2
#define _ADCON0_CHS_POSN                                    0x2
#define _ADCON0_CHS_POSITION                                0x2
#define _ADCON0_CHS_SIZE                                    0x5
#define _ADCON0_CHS_LENGTH                                  0x5
#define _ADCON0_CHS_MASK                                    0x7C
#define _ADCON0_ADGO_POSN                                   0x1
#define _ADCON0_ADGO_POSITION                               0x1
#define _ADCON0_ADGO_SIZE                                   0x1
#define _ADCON0_ADGO_LENGTH                                 0x1
#define _ADCON0_ADGO_MASK                                   0x2
#define _ADCON0_CHS0_POSN                                   0x2
#define _ADCON0_CHS0_POSITION                               0x2
#define _ADCON0_CHS0_SIZE                                   0x1
#define _ADCON0_CHS0_LENGTH                                 0x1
#define _ADCON0_CHS0_MASK                                   0x4
#define _ADCON0_CHS1_POSN                                   0x3
#define _ADCON0_CHS1_POSITION                               0x3
#define _ADCON0_CHS1_SIZE                                   0x1
#define _ADCON0_CHS1_LENGTH                                 0x1
#define _ADCON0_CHS1_MASK                                   0x8
#define _ADCON0_CHS2_POSN                                   0x4
#define _ADCON0_CHS2_POSITION                               0x4
#define _ADCON0_CHS2_SIZE                                   0x1
#define _ADCON0_CHS2_LENGTH                                 0x1
#define _ADCON0_CHS2_MASK                                   0x10
#define _ADCON0_CHS3_POSN                                   0x5
#define _ADCON0_CHS3_POSITION                               0x5
#define _ADCON0_CHS3_SIZE                                   0x1
#define _ADCON0_CHS3_LENGTH                                 0x1
#define _ADCON0_CHS3_MASK                                   0x20
#define _ADCON0_CHS4_POSN                                   0x6
#define _ADCON0_CHS4_POSITION                               0x6
#define _ADCON0_CHS4_SIZE                                   0x1
#define _ADCON0_CHS4_LENGTH                                 0x1
#define _ADCON0_CHS4_MASK                                   0x40
#define _ADCON0_GO_POSN                                     0x1
#define _ADCON0_GO_POSITION                                 0x1
#define _ADCON0_GO_SIZE                                     0x1
#define _ADCON0_GO_LENGTH                                   0x1
#define _ADCON0_GO_MASK                                     0x2

// Register: ADCON1
#define ADCON1 ADCON1
extern volatile unsigned char           ADCON1              __at(0x09E);
#ifndef _LIB_BUILD
asm("ADCON1 equ 09Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned ADPREF                 :2;
        unsigned ADNREF                 :1;
        unsigned                        :1;
        unsigned ADCS                   :3;
        unsigned ADFM                   :1;
    };
    struct {
        unsigned ADPREF0                :1;
        unsigned ADPREF1                :1;
    };
} ADCON1bits_t;
extern volatile ADCON1bits_t ADCON1bits __at(0x09E);
// bitfield macros
#define _ADCON1_ADPREF_POSN                                 0x0
#define _ADCON1_ADPREF_POSITION                             0x0
#define _ADCON1_ADPREF_SIZE                                 0x2
#define _ADCON1_ADPREF_LENGTH                               0x2
#define _ADCON1_ADPREF_MASK                                 0x3
#define _ADCON1_ADNREF_POSN                                 0x2
#define _ADCON1_ADNREF_POSITION                             0x2
#define _ADCON1_ADNREF_SIZE                                 0x1
#define _ADCON1_ADNREF_LENGTH                               0x1
#define _ADCON1_ADNREF_MASK                                 0x4
#define _ADCON1_ADCS_POSN                                   0x4
#define _ADCON1_ADCS_POSITION                               0x4
#define _ADCON1_ADCS_SIZE                                   0x3
#define _ADCON1_ADCS_LENGTH                                 0x3
#define _ADCON1_ADCS_MASK                                   0x70
#define _ADCON1_ADFM_POSN                                   0x7
#define _ADCON1_ADFM_POSITION                               0x7
#define _ADCON1_ADFM_SIZE                                   0x1
#define _ADCON1_ADFM_LENGTH                                 0x1
#define _ADCON1_ADFM_MASK                                   0x80
#define _ADCON1_ADPREF0_POSN                                0x0
#define _ADCON1_ADPREF0_POSITION                            0x0
#define _ADCON1_ADPREF0_SIZE                                0x1
#define _ADCON1_ADPREF0_LENGTH                              0x1
#define _ADCON1_ADPREF0_MASK                                0x1
#define _ADCON1_ADPREF1_POSN                                0x1
#define _ADCON1_ADPREF1_POSITION                            0x1
#define _ADCON1_ADPREF1_SIZE                                0x1
#define _ADCON1_ADPREF1_LENGTH                              0x1
#define _ADCON1_ADPREF1_MASK                                0x2

// Register: ADCON2
#define ADCON2 ADCON2
extern volatile unsigned char           ADCON2              __at(0x09F);
#ifndef _LIB_BUILD
asm("ADCON2 equ 09Fh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned TRIGSEL                :4;
    };
    struct {
        unsigned                        :4;
        unsigned TRIGSEL0               :1;
        unsigned TRIGSEL1               :1;
        unsigned TRIGSEL2               :1;
        unsigned TRIGSEL3               :1;
    };
} ADCON2bits_t;
extern volatile ADCON2bits_t ADCON2bits __at(0x09F);
// bitfield macros
#define _ADCON2_TRIGSEL_POSN                                0x4
#define _ADCON2_TRIGSEL_POSITION                            0x4
#define _ADCON2_TRIGSEL_SIZE                                0x4
#define _ADCON2_TRIGSEL_LENGTH                              0x4
#define _ADCON2_TRIGSEL_MASK                                0xF0
#define _ADCON2_TRIGSEL0_POSN                               0x4
#define _ADCON2_TRIGSEL0_POSITION                           0x4
#define _ADCON2_TRIGSEL0_SIZE                               0x1
#define _ADCON2_TRIGSEL0_LENGTH                             0x1
#define _ADCON2_TRIGSEL0_MASK                               0x10
#define _ADCON2_TRIGSEL1_POSN                               0x5
#define _ADCON2_TRIGSEL1_POSITION                           0x5
#define _ADCON2_TRIGSEL1_SIZE                               0x1
#define _ADCON2_TRIGSEL1_LENGTH                             0x1
#define _ADCON2_TRIGSEL1_MASK                               0x20
#define _ADCON2_TRIGSEL2_POSN                               0x6
#define _ADCON2_TRIGSEL2_POSITION                           0x6
#define _ADCON2_TRIGSEL2_SIZE                               0x1
#define _ADCON2_TRIGSEL2_LENGTH                             0x1
#define _ADCON2_TRIGSEL2_MASK                               0x40
#define _ADCON2_TRIGSEL3_POSN                               0x7
#define _ADCON2_TRIGSEL3_POSITION                           0x7
#define _ADCON2_TRIGSEL3_SIZE                               0x1
#define _ADCON2_TRIGSEL3_LENGTH                             0x1
#define _ADCON2_TRIGSEL3_MASK                               0x80

// Register: LATA
#define LATA LATA
extern volatile unsigned char           LATA                __at(0x10C);
#ifndef _LIB_BUILD
asm("LATA equ 010Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LATA0                  :1;
        unsigned LATA1                  :1;
        unsigned LATA2                  :1;
        unsigned                        :1;
        unsigned LATA4                  :1;
        unsigned LATA5                  :1;
    };
} LATAbits_t;
extern volatile LATAbits_t LATAbits __at(0x10C);
// bitfield macros
#define _LATA_LATA0_POSN                                    0x0
#define _LATA_LATA0_POSITION                                0x0
#define _LATA_LATA0_SIZE                                    0x1
#define _LATA_LATA0_LENGTH                                  0x1
#define _LATA_LATA0_MASK                                    0x1
#define _LATA_LATA1_POSN                                    0x1
#define _LATA_LATA1_POSITION                                0x1
#define _LATA_LATA1_SIZE                                    0x1
#define _LATA_LATA1_LENGTH                                  0x1
#define _LATA_LATA1_MASK                                    0x2
#define _LATA_LATA2_POSN                                    0x2
#define _LATA_LATA2_POSITION                                0x2
#define _LATA_LATA2_SIZE                                    0x1
#define _LATA_LATA2_LENGTH                                  0x1
#define _LATA_LATA2_MASK                                    0x4
#define _LATA_LATA4_POSN                                    0x4
#define _LATA_LATA4_POSITION                                0x4
#define _LATA_LATA4_SIZE                                    0x1
#define _LATA_LATA4_LENGTH                                  0x1
#define _LATA_LATA4_MASK                                    0x10
#define _LATA_LATA5_POSN                                    0x5
#define _LATA_LATA5_POSITION                                0x5
#define _LATA_LATA5_SIZE                                    0x1
#define _LATA_LATA5_LENGTH                                  0x1
#define _LATA_LATA5_MASK                                    0x20

// Register: LATB
#define LATB LATB
extern volatile unsigned char           LATB                __at(0x10D);
#ifndef _LIB_BUILD
asm("LATB equ 010Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned LATB4                  :1;
        unsigned LATB5                  :1;
        unsigned LATB6                  :1;
        unsigned LATB7                  :1;
    };
} LATBbits_t;
extern volatile LATBbits_t LATBbits __at(0x10D);
// bitfield macros
#define _LATB_LATB4_POSN                                    0x4
#define _LATB_LATB4_POSITION                                0x4
#define _LATB_LATB4_SIZE                                    0x1
#define _LATB_LATB4_LENGTH                                  0x1
#define _LATB_LATB4_MASK                                    0x10
#define _LATB_LATB5_POSN                                    0x5
#define _LATB_LATB5_POSITION                                0x5
#define _LATB_LATB5_SIZE                                    0x1
#define _LATB_LATB5_LENGTH                                  0x1
#define _LATB_LATB5_MASK                                    0x20
#define _LATB_LATB6_POSN                                    0x6
#define _LATB_LATB6_POSITION                                0x6
#define _LATB_LATB6_SIZE                                    0x1
#define _LATB_LATB6_LENGTH                                  0x1
#define _LATB_LATB6_MASK                                    0x40
#define _LATB_LATB7_POSN                                    0x7
#define _LATB_LATB7_POSITION                                0x7
#define _LATB_LATB7_SIZE                                    0x1
#define _LATB_LATB7_LENGTH                                  0x1
#define _LATB_LATB7_MASK                                    0x80

// Register: LATC
#define LATC LATC
extern volatile unsigned char           LATC                __at(0x10E);
#ifndef _LIB_BUILD
asm("LATC equ 010Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LATC0                  :1;
        unsigned LATC1                  :1;
        unsigned LATC2                  :1;
        unsigned LATC3                  :1;
        unsigned LATC4                  :1;
        unsigned LATC5                  :1;
        unsigned LATC6                  :1;
        unsigned LATC7                  :1;
    };
} LATCbits_t;
extern volatile LATCbits_t LATCbits __at(0x10E);
// bitfield macros
#define _LATC_LATC0_POSN                                    0x0
#define _LATC_LATC0_POSITION                                0x0
#define _LATC_LATC0_SIZE                                    0x1
#define _LATC_LATC0_LENGTH                                  0x1
#define _LATC_LATC0_MASK                                    0x1
#define _LATC_LATC1_POSN                                    0x1
#define _LATC_LATC1_POSITION                                0x1
#define _LATC_LATC1_SIZE                                    0x1
#define _LATC_LATC1_LENGTH                                  0x1
#define _LATC_LATC1_MASK                                    0x2
#define _LATC_LATC2_POSN                                    0x2
#define _LATC_LATC2_POSITION                                0x2
#define _LATC_LATC2_SIZE                                    0x1
#define _LATC_LATC2_LENGTH                                  0x1
#define _LATC_LATC2_MASK                                    0x4
#define _LATC_LATC3_POSN                                    0x3
#define _LATC_LATC3_POSITION                                0x3
#define _LATC_LATC3_SIZE                                    0x1
#define _LATC_LATC3_LENGTH                                  0x1
#define _LATC_LATC3_MASK                                    0x8
#define _LATC_LATC4_POSN                                    0x4
#define _LATC_LATC4_POSITION                                0x4
#define _LATC_LATC4_SIZE                                    0x1
#define _LATC_LATC4_LENGTH                                  0x1
#define _LATC_LATC4_MASK                                    0x10
#define _LATC_LATC5_POSN                                    0x5
#define _LATC_LATC5_POSITION                                0x5
#define _LATC_LATC5_SIZE                                    0x1
#define _LATC_LATC5_LENGTH                                  0x1
#define _LATC_LATC5_MASK                                    0x20
#define _LATC_LATC6_POSN                                    0x6
#define _LATC_LATC6_POSITION                                0x6
#define _LATC_LATC6_SIZE                                    0x1
#define _LATC_LATC6_LENGTH                                  0x1
#define _LATC_LATC6_MASK                                    0x40
#define _LATC_LATC7_POSN                                    0x7
#define _LATC_LATC7_POSITION                                0x7
#define _LATC_LATC7_SIZE                                    0x1
#define _LATC_LATC7_LENGTH                                  0x1
#define _LATC_LATC7_MASK                                    0x80

// Register: CM1CON0
#define CM1CON0 CM1CON0
extern volatile unsigned char           CM1CON0             __at(0x111);
#ifndef _LIB_BUILD
asm("CM1CON0 equ 0111h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned C1SYNC                 :1;
        unsigned C1HYS                  :1;
        unsigned C1SP                   :1;
        unsigned C1ZLF                  :1;
        unsigned C1POL                  :1;
        unsigned                        :1;
        unsigned C1OUT                  :1;
        unsigned C1ON                   :1;
    };
} CM1CON0bits_t;
extern volatile CM1CON0bits_t CM1CON0bits __at(0x111);
// bitfield macros
#define _CM1CON0_C1SYNC_POSN                                0x0
#define _CM1CON0_C1SYNC_POSITION                            0x0
#define _CM1CON0_C1SYNC_SIZE                                0x1
#define _CM1CON0_C1SYNC_LENGTH                              0x1
#define _CM1CON0_C1SYNC_MASK                                0x1
#define _CM1CON0_C1HYS_POSN                                 0x1
#define _CM1CON0_C1HYS_POSITION                             0x1
#define _CM1CON0_C1HYS_SIZE                                 0x1
#define _CM1CON0_C1HYS_LENGTH                               0x1
#define _CM1CON0_C1HYS_MASK                                 0x2
#define _CM1CON0_C1SP_POSN                                  0x2
#define _CM1CON0_C1SP_POSITION                              0x2
#define _CM1CON0_C1SP_SIZE                                  0x1
#define _CM1CON0_C1SP_LENGTH                                0x1
#define _CM1CON0_C1SP_MASK                                  0x4
#define _CM1CON0_C1ZLF_POSN                                 0x3
#define _CM1CON0_C1ZLF_POSITION                             0x3
#define _CM1CON0_C1ZLF_SIZE                                 0x1
#define _CM1CON0_C1ZLF_LENGTH                               0x1
#define _CM1CON0_C1ZLF_MASK                                 0x8
#define _CM1CON0_C1POL_POSN                                 0x4
#define _CM1CON0_C1POL_POSITION                             0x4
#define _CM1CON0_C1POL_SIZE                                 0x1
#define _CM1CON0_C1POL_LENGTH                               0x1
#define _CM1CON0_C1POL_MASK                                 0x10
#define _CM1CON0_C1OUT_POSN                                 0x6
#define _CM1CON0_C1OUT_POSITION                             0x6
#define _CM1CON0_C1OUT_SIZE                                 0x1
#define _CM1CON0_C1OUT_LENGTH                               0x1
#define _CM1CON0_C1OUT_MASK                                 0x40
#define _CM1CON0_C1ON_POSN                                  0x7
#define _CM1CON0_C1ON_POSITION                              0x7
#define _CM1CON0_C1ON_SIZE                                  0x1
#define _CM1CON0_C1ON_LENGTH                                0x1
#define _CM1CON0_C1ON_MASK                                  0x80

// Register: CM1CON1
#define CM1CON1 CM1CON1
extern volatile unsigned char           CM1CON1             __at(0x112);
#ifndef _LIB_BUILD
asm("CM1CON1 equ 0112h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned C1NCH                  :3;
        unsigned C1PCH                  :3;
        unsigned C1INTN                 :1;
        unsigned C1INTP                 :1;
    };
    struct {
        unsigned C1NCH0                 :1;
        unsigned C1NCH1                 :1;
        unsigned C1NCH2                 :1;
        unsigned C1PCH0                 :1;
        unsigned C1PCH1                 :1;
        unsigned C1PCH2                 :1;
    };
} CM1CON1bits_t;
extern volatile CM1CON1bits_t CM1CON1bits __at(0x112);
// bitfield macros
#define _CM1CON1_C1NCH_POSN                                 0x0
#define _CM1CON1_C1NCH_POSITION                             0x0
#define _CM1CON1_C1NCH_SIZE                                 0x3
#define _CM1CON1_C1NCH_LENGTH                               0x3
#define _CM1CON1_C1NCH_MASK                                 0x7
#define _CM1CON1_C1PCH_POSN                                 0x3
#define _CM1CON1_C1PCH_POSITION                             0x3
#define _CM1CON1_C1PCH_SIZE                                 0x3
#define _CM1CON1_C1PCH_LENGTH                               0x3
#define _CM1CON1_C1PCH_MASK                                 0x38
#define _CM1CON1_C1INTN_POSN                                0x6
#define _CM1CON1_C1INTN_POSITION                            0x6
#define _CM1CON1_C1INTN_SIZE                                0x1
#define _CM1CON1_C1INTN_LENGTH                              0x1
#define _CM1CON1_C1INTN_MASK                                0x40
#define _CM1CON1_C1INTP_POSN                                0x7
#define _CM1CON1_C1INTP_POSITION                            0x7
#define _CM1CON1_C1INTP_SIZE                                0x1
#define _CM1CON1_C1INTP_LENGTH                              0x1
#define _CM1CON1_C1INTP_MASK                                0x80
#define _CM1CON1_C1NCH0_POSN                                0x0
#define _CM1CON1_C1NCH0_POSITION                            0x0
#define _CM1CON1_C1NCH0_SIZE                                0x1
#define _CM1CON1_C1NCH0_LENGTH                              0x1
#define _CM1CON1_C1NCH0_MASK                                0x1
#define _CM1CON1_C1NCH1_POSN                                0x1
#define _CM1CON1_C1NCH1_POSITION                            0x1
#define _CM1CON1_C1NCH1_SIZE                                0x1
#define _CM1CON1_C1NCH1_LENGTH                              0x1
#define _CM1CON1_C1NCH1_MASK                                0x2
#define _CM1CON1_C1NCH2_POSN                                0x2
#define _CM1CON1_C1NCH2_POSITION                            0x2
#define _CM1CON1_C1NCH2_SIZE                                0x1
#define _CM1CON1_C1NCH2_LENGTH                              0x1
#define _CM1CON1_C1NCH2_MASK                                0x4
#define _CM1CON1_C1PCH0_POSN                                0x3
#define _CM1CON1_C1PCH0_POSITION                            0x3
#define _CM1CON1_C1PCH0_SIZE                                0x1
#define _CM1CON1_C1PCH0_LENGTH                              0x1
#define _CM1CON1_C1PCH0_MASK                                0x8
#define _CM1CON1_C1PCH1_POSN                                0x4
#define _CM1CON1_C1PCH1_POSITION                            0x4
#define _CM1CON1_C1PCH1_SIZE                                0x1
#define _CM1CON1_C1PCH1_LENGTH                              0x1
#define _CM1CON1_C1PCH1_MASK                                0x10
#define _CM1CON1_C1PCH2_POSN                                0x5
#define _CM1CON1_C1PCH2_POSITION                            0x5
#define _CM1CON1_C1PCH2_SIZE                                0x1
#define _CM1CON1_C1PCH2_LENGTH                              0x1
#define _CM1CON1_C1PCH2_MASK                                0x20

// Register: CM2CON0
#define CM2CON0 CM2CON0
extern volatile unsigned char           CM2CON0             __at(0x113);
#ifndef _LIB_BUILD
asm("CM2CON0 equ 0113h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned C2SYNC                 :1;
        unsigned C2HYS                  :1;
        unsigned C2SP                   :1;
        unsigned C2ZLF                  :1;
        unsigned C2POL                  :1;
        unsigned                        :1;
        unsigned C2OUT                  :1;
        unsigned C2ON                   :1;
    };
} CM2CON0bits_t;
extern volatile CM2CON0bits_t CM2CON0bits __at(0x113);
// bitfield macros
#define _CM2CON0_C2SYNC_POSN                                0x0
#define _CM2CON0_C2SYNC_POSITION                            0x0
#define _CM2CON0_C2SYNC_SIZE                                0x1
#define _CM2CON0_C2SYNC_LENGTH                              0x1
#define _CM2CON0_C2SYNC_MASK                                0x1
#define _CM2CON0_C2HYS_POSN                                 0x1
#define _CM2CON0_C2HYS_POSITION                             0x1
#define _CM2CON0_C2HYS_SIZE                                 0x1
#define _CM2CON0_C2HYS_LENGTH                               0x1
#define _CM2CON0_C2HYS_MASK                                 0x2
#define _CM2CON0_C2SP_POSN                                  0x2
#define _CM2CON0_C2SP_POSITION                              0x2
#define _CM2CON0_C2SP_SIZE                                  0x1
#define _CM2CON0_C2SP_LENGTH                                0x1
#define _CM2CON0_C2SP_MASK                                  0x4
#define _CM2CON0_C2ZLF_POSN                                 0x3
#define _CM2CON0_C2ZLF_POSITION                             0x3
#define _CM2CON0_C2ZLF_SIZE                                 0x1
#define _CM2CON0_C2ZLF_LENGTH                               0x1
#define _CM2CON0_C2ZLF_MASK                                 0x8
#define _CM2CON0_C2POL_POSN                                 0x4
#define _CM2CON0_C2POL_POSITION                             0x4
#define _CM2CON0_C2POL_SIZE                                 0x1
#define _CM2CON0_C2POL_LENGTH                               0x1
#define _CM2CON0_C2POL_MASK                                 0x10
#define _CM2CON0_C2OUT_POSN                                 0x6
#define _CM2CON0_C2OUT_POSITION                             0x6
#define _CM2CON0_C2OUT_SIZE                                 0x1
#define _CM2CON0_C2OUT_LENGTH                               0x1
#define _CM2CON0_C2OUT_MASK                                 0x40
#define _CM2CON0_C2ON_POSN                                  0x7
#define _CM2CON0_C2ON_POSITION                              0x7
#define _CM2CON0_C2ON_SIZE                                  0x1
#define _CM2CON0_C2ON_LENGTH                                0x1
#define _CM2CON0_C2ON_MASK                                  0x80

// Register: CM2CON1
#define CM2CON1 CM2CON1
extern volatile unsigned char           CM2CON1             __at(0x114);
#ifndef _LIB_BUILD
asm("CM2CON1 equ 0114h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned C2NCH                  :3;
        unsigned C2PCH                  :3;
        unsigned C2INTN                 :1;
        unsigned C2INTP                 :1;
    };
    struct {
        unsigned C2NCH0                 :1;
        unsigned C2NCH1                 :1;
        unsigned C2NCH2                 :1;
        unsigned C2PCH0                 :1;
        unsigned C2PCH1                 :1;
        unsigned C2PCH2                 :1;
    };
} CM2CON1bits_t;
extern volatile CM2CON1bits_t CM2CON1bits __at(0x114);
// bitfield macros
#define _CM2CON1_C2NCH_POSN                                 0x0
#define _CM2CON1_C2NCH_POSITION                             0x0
#define _CM2CON1_C2NCH_SIZE                                 0x3
#define _CM2CON1_C2NCH_LENGTH                               0x3
#define _CM2CON1_C2NCH_MASK                                 0x7
#define _CM2CON1_C2PCH_POSN                                 0x3
#define _CM2CON1_C2PCH_POSITION                             0x3
#define _CM2CON1_C2PCH_SIZE                                 0x3
#define _CM2CON1_C2PCH_LENGTH                               0x3
#define _CM2CON1_C2PCH_MASK                                 0x38
#define _CM2CON1_C2INTN_POSN                                0x6
#define _CM2CON1_C2INTN_POSITION                            0x6
#define _CM2CON1_C2INTN_SIZE                                0x1
#define _CM2CON1_C2INTN_LENGTH                              0x1
#define _CM2CON1_C2INTN_MASK                                0x40
#define _CM2CON1_C2INTP_POSN                                0x7
#define _CM2CON1_C2INTP_POSITION                            0x7
#define _CM2CON1_C2INTP_SIZE                                0x1
#define _CM2CON1_C2INTP_LENGTH                              0x1
#define _CM2CON1_C2INTP_MASK                                0x80
#define _CM2CON1_C2NCH0_POSN                                0x0
#define _CM2CON1_C2NCH0_POSITION                            0x0
#define _CM2CON1_C2NCH0_SIZE                                0x1
#define _CM2CON1_C2NCH0_LENGTH                              0x1
#define _CM2CON1_C2NCH0_MASK                                0x1
#define _CM2CON1_C2NCH1_POSN                                0x1
#define _CM2CON1_C2NCH1_POSITION                            0x1
#define _CM2CON1_C2NCH1_SIZE                                0x1
#define _CM2CON1_C2NCH1_LENGTH                              0x1
#define _CM2CON1_C2NCH1_MASK                                0x2
#define _CM2CON1_C2NCH2_POSN                                0x2
#define _CM2CON1_C2NCH2_POSITION                            0x2
#define _CM2CON1_C2NCH2_SIZE                                0x1
#define _CM2CON1_C2NCH2_LENGTH                              0x1
#define _CM2CON1_C2NCH2_MASK                                0x4
#define _CM2CON1_C2PCH0_POSN                                0x3
#define _CM2CON1_C2PCH0_POSITION                            0x3
#define _CM2CON1_C2PCH0_SIZE                                0x1
#define _CM2CON1_C2PCH0_LENGTH                              0x1
#define _CM2CON1_C2PCH0_MASK                                0x8
#define _CM2CON1_C2PCH1_POSN                                0x4
#define _CM2CON1_C2PCH1_POSITION                            0x4
#define _CM2CON1_C2PCH1_SIZE                                0x1
#define _CM2CON1_C2PCH1_LENGTH                              0x1
#define _CM2CON1_C2PCH1_MASK                                0x10
#define _CM2CON1_C2PCH2_POSN                                0x5
#define _CM2CON1_C2PCH2_POSITION                            0x5
#define _CM2CON1_C2PCH2_SIZE                                0x1
#define _CM2CON1_C2PCH2_LENGTH                              0x1
#define _CM2CON1_C2PCH2_MASK                                0x20

// Register: CMOUT
#define CMOUT CMOUT
extern volatile unsigned char           CMOUT               __at(0x115);
#ifndef _LIB_BUILD
asm("CMOUT equ 0115h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned MC1OUT                 :1;
        unsigned MC2OUT                 :1;
    };
} CMOUTbits_t;
extern volatile CMOUTbits_t CMOUTbits __at(0x115);
// bitfield macros
#define _CMOUT_MC1OUT_POSN                                  0x0
#define _CMOUT_MC1OUT_POSITION                              0x0
#define _CMOUT_MC1OUT_SIZE                                  0x1
#define _CMOUT_MC1OUT_LENGTH                                0x1
#define _CMOUT_MC1OUT_MASK                                  0x1
#define _CMOUT_MC2OUT_POSN                                  0x1
#define _CMOUT_MC2OUT_POSITION                              0x1
#define _CMOUT_MC2OUT_SIZE                                  0x1
#define _CMOUT_MC2OUT_LENGTH                                0x1
#define _CMOUT_MC2OUT_MASK                                  0x2

// Register: BORCON
#define BORCON BORCON
extern volatile unsigned char           BORCON              __at(0x116);
#ifndef _LIB_BUILD
asm("BORCON equ 0116h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned BORRDY                 :1;
        unsigned                        :5;
        unsigned BORFS                  :1;
        unsigned SBOREN                 :1;
    };
} BORCONbits_t;
extern volatile BORCONbits_t BORCONbits __at(0x116);
// bitfield macros
#define _BORCON_BORRDY_POSN                                 0x0
#define _BORCON_BORRDY_POSITION                             0x0
#define _BORCON_BORRDY_SIZE                                 0x1
#define _BORCON_BORRDY_LENGTH                               0x1
#define _BORCON_BORRDY_MASK                                 0x1
#define _BORCON_BORFS_POSN                                  0x6
#define _BORCON_BORFS_POSITION                              0x6
#define _BORCON_BORFS_SIZE                                  0x1
#define _BORCON_BORFS_LENGTH                                0x1
#define _BORCON_BORFS_MASK                                  0x40
#define _BORCON_SBOREN_POSN                                 0x7
#define _BORCON_SBOREN_POSITION                             0x7
#define _BORCON_SBOREN_SIZE                                 0x1
#define _BORCON_SBOREN_LENGTH                               0x1
#define _BORCON_SBOREN_MASK                                 0x80

// Register: FVRCON
#define FVRCON FVRCON
extern volatile unsigned char           FVRCON              __at(0x117);
#ifndef _LIB_BUILD
asm("FVRCON equ 0117h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned ADFVR                  :2;
        unsigned CDAFVR                 :2;
        unsigned TSRNG                  :1;
        unsigned TSEN                   :1;
        unsigned FVRRDY                 :1;
        unsigned FVREN                  :1;
    };
    struct {
        unsigned ADFVR0                 :1;
        unsigned ADFVR1                 :1;
        unsigned CDAFVR0                :1;
        unsigned CDAFVR1                :1;
    };
} FVRCONbits_t;
extern volatile FVRCONbits_t FVRCONbits __at(0x117);
// bitfield macros
#define _FVRCON_ADFVR_POSN                                  0x0
#define _FVRCON_ADFVR_POSITION                              0x0
#define _FVRCON_ADFVR_SIZE                                  0x2
#define _FVRCON_ADFVR_LENGTH                                0x2
#define _FVRCON_ADFVR_MASK                                  0x3
#define _FVRCON_CDAFVR_POSN                                 0x2
#define _FVRCON_CDAFVR_POSITION                             0x2
#define _FVRCON_CDAFVR_SIZE                                 0x2
#define _FVRCON_CDAFVR_LENGTH                               0x2
#define _FVRCON_CDAFVR_MASK                                 0xC
#define _FVRCON_TSRNG_POSN                                  0x4
#define _FVRCON_TSRNG_POSITION                              0x4
#define _FVRCON_TSRNG_SIZE                                  0x1
#define _FVRCON_TSRNG_LENGTH                                0x1
#define _FVRCON_TSRNG_MASK                                  0x10
#define _FVRCON_TSEN_POSN                                   0x5
#define _FVRCON_TSEN_POSITION                               0x5
#define _FVRCON_TSEN_SIZE                                   0x1
#define _FVRCON_TSEN_LENGTH                                 0x1
#define _FVRCON_TSEN_MASK                                   0x20
#define _FVRCON_FVRRDY_POSN                                 0x6
#define _FVRCON_FVRRDY_POSITION                             0x6
#define _FVRCON_FVRRDY_SIZE                                 0x1
#define _FVRCON_FVRRDY_LENGTH                               0x1
#define _FVRCON_FVRRDY_MASK                                 0x40
#define _FVRCON_FVREN_POSN                                  0x7
#define _FVRCON_FVREN_POSITION                              0x7
#define _FVRCON_FVREN_SIZE                                  0x1
#define _FVRCON_FVREN_LENGTH                                0x1
#define _FVRCON_FVREN_MASK                                  0x80
#define _FVRCON_ADFVR0_POSN                                 0x0
#define _FVRCON_ADFVR0_POSITION                             0x0
#define _FVRCON_ADFVR0_SIZE                                 0x1
#define _FVRCON_ADFVR0_LENGTH                               0x1
#define _FVRCON_ADFVR0_MASK                                 0x1
#define _FVRCON_ADFVR1_POSN                                 0x1
#define _FVRCON_ADFVR1_POSITION                             0x1
#define _FVRCON_ADFVR1_SIZE                                 0x1
#define _FVRCON_ADFVR1_LENGTH                               0x1
#define _FVRCON_ADFVR1_MASK                                 0x2
#define _FVRCON_CDAFVR0_POSN                                0x2
#define _FVRCON_CDAFVR0_POSITION                            0x2
#define _FVRCON_CDAFVR0_SIZE                                0x1
#define _FVRCON_CDAFVR0_LENGTH                              0x1
#define _FVRCON_CDAFVR0_MASK                                0x4
#define _FVRCON_CDAFVR1_POSN                                0x3
#define _FVRCON_CDAFVR1_POSITION                            0x3
#define _FVRCON_CDAFVR1_SIZE                                0x1
#define _FVRCON_CDAFVR1_LENGTH                              0x1
#define _FVRCON_CDAFVR1_MASK                                0x8

// Register: DAC1CON0
#define DAC1CON0 DAC1CON0
extern volatile unsigned char           DAC1CON0            __at(0x118);
#ifndef _LIB_BUILD
asm("DAC1CON0 equ 0118h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned DAC1NSS                :1;
        unsigned                        :1;
        unsigned DAC1PSS                :2;
        unsigned DAC1OE2                :1;
        unsigned DAC1OE1                :1;
        unsigned                        :1;
        unsigned DAC1EN                 :1;
    };
    struct {
        unsigned                        :2;
        unsigned DAC1PSS0               :1;
        unsigned DAC1PSS1               :1;
    };
    struct {
        unsigned DACNSS                 :1;
        unsigned                        :1;
        unsigned DACPSS                 :2;
        unsigned DACOE0                 :1;
        unsigned DACOE1                 :1;
        unsigned                        :1;
        unsigned DACEN                  :1;
    };
    struct {
        unsigned                        :2;
        unsigned DACPSS0                :1;
        unsigned DACPSS1                :1;
    };
} DAC1CON0bits_t;
extern volatile DAC1CON0bits_t DAC1CON0bits __at(0x118);
// bitfield macros
#define _DAC1CON0_DAC1NSS_POSN                              0x0
#define _DAC1CON0_DAC1NSS_POSITION                          0x0
#define _DAC1CON0_DAC1NSS_SIZE                              0x1
#define _DAC1CON0_DAC1NSS_LENGTH                            0x1
#define _DAC1CON0_DAC1NSS_MASK                              0x1
#define _DAC1CON0_DAC1PSS_POSN                              0x2
#define _DAC1CON0_DAC1PSS_POSITION                          0x2
#define _DAC1CON0_DAC1PSS_SIZE                              0x2
#define _DAC1CON0_DAC1PSS_LENGTH                            0x2
#define _DAC1CON0_DAC1PSS_MASK                              0xC
#define _DAC1CON0_DAC1OE2_POSN                              0x4
#define _DAC1CON0_DAC1OE2_POSITION                          0x4
#define _DAC1CON0_DAC1OE2_SIZE                              0x1
#define _DAC1CON0_DAC1OE2_LENGTH                            0x1
#define _DAC1CON0_DAC1OE2_MASK                              0x10
#define _DAC1CON0_DAC1OE1_POSN                              0x5
#define _DAC1CON0_DAC1OE1_POSITION                          0x5
#define _DAC1CON0_DAC1OE1_SIZE                              0x1
#define _DAC1CON0_DAC1OE1_LENGTH                            0x1
#define _DAC1CON0_DAC1OE1_MASK                              0x20
#define _DAC1CON0_DAC1EN_POSN                               0x7
#define _DAC1CON0_DAC1EN_POSITION                           0x7
#define _DAC1CON0_DAC1EN_SIZE                               0x1
#define _DAC1CON0_DAC1EN_LENGTH                             0x1
#define _DAC1CON0_DAC1EN_MASK                               0x80
#define _DAC1CON0_DAC1PSS0_POSN                             0x2
#define _DAC1CON0_DAC1PSS0_POSITION                         0x2
#define _DAC1CON0_DAC1PSS0_SIZE                             0x1
#define _DAC1CON0_DAC1PSS0_LENGTH                           0x1
#define _DAC1CON0_DAC1PSS0_MASK                             0x4
#define _DAC1CON0_DAC1PSS1_POSN                             0x3
#define _DAC1CON0_DAC1PSS1_POSITION                         0x3
#define _DAC1CON0_DAC1PSS1_SIZE                             0x1
#define _DAC1CON0_DAC1PSS1_LENGTH                           0x1
#define _DAC1CON0_DAC1PSS1_MASK                             0x8
#define _DAC1CON0_DACNSS_POSN                               0x0
#define _DAC1CON0_DACNSS_POSITION                           0x0
#define _DAC1CON0_DACNSS_SIZE                               0x1
#define _DAC1CON0_DACNSS_LENGTH                             0x1
#define _DAC1CON0_DACNSS_MASK                               0x1
#define _DAC1CON0_DACPSS_POSN                               0x2
#define _DAC1CON0_DACPSS_POSITION                           0x2
#define _DAC1CON0_DACPSS_SIZE                               0x2
#define _DAC1CON0_DACPSS_LENGTH                             0x2
#define _DAC1CON0_DACPSS_MASK                               0xC
#define _DAC1CON0_DACOE0_POSN                               0x4
#define _DAC1CON0_DACOE0_POSITION                           0x4
#define _DAC1CON0_DACOE0_SIZE                               0x1
#define _DAC1CON0_DACOE0_LENGTH                             0x1
#define _DAC1CON0_DACOE0_MASK                               0x10
#define _DAC1CON0_DACOE1_POSN                               0x5
#define _DAC1CON0_DACOE1_POSITION                           0x5
#define _DAC1CON0_DACOE1_SIZE                               0x1
#define _DAC1CON0_DACOE1_LENGTH                             0x1
#define _DAC1CON0_DACOE1_MASK                               0x20
#define _DAC1CON0_DACEN_POSN                                0x7
#define _DAC1CON0_DACEN_POSITION                            0x7
#define _DAC1CON0_DACEN_SIZE                                0x1
#define _DAC1CON0_DACEN_LENGTH                              0x1
#define _DAC1CON0_DACEN_MASK                                0x80
#define _DAC1CON0_DACPSS0_POSN                              0x2
#define _DAC1CON0_DACPSS0_POSITION                          0x2
#define _DAC1CON0_DACPSS0_SIZE                              0x1
#define _DAC1CON0_DACPSS0_LENGTH                            0x1
#define _DAC1CON0_DACPSS0_MASK                              0x4
#define _DAC1CON0_DACPSS1_POSN                              0x3
#define _DAC1CON0_DACPSS1_POSITION                          0x3
#define _DAC1CON0_DACPSS1_SIZE                              0x1
#define _DAC1CON0_DACPSS1_LENGTH                            0x1
#define _DAC1CON0_DACPSS1_MASK                              0x8

// Register: DAC1CON1
#define DAC1CON1 DAC1CON1
extern volatile unsigned char           DAC1CON1            __at(0x119);
#ifndef _LIB_BUILD
asm("DAC1CON1 equ 0119h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned DAC1R                  :8;
    };
    struct {
        unsigned DAC1R0                 :1;
        unsigned DAC1R1                 :1;
        unsigned DAC1R2                 :1;
        unsigned DAC1R3                 :1;
        unsigned DAC1R4                 :1;
        unsigned DAC1R5                 :1;
        unsigned DAC1R6                 :1;
        unsigned DAC1R7                 :1;
    };
    struct {
        unsigned DACR0                  :1;
        unsigned DACR1                  :1;
        unsigned DACR2                  :1;
        unsigned DACR3                  :1;
        unsigned DACR4                  :1;
        unsigned DACR5                  :1;
        unsigned DACR6                  :1;
        unsigned DACR7                  :1;
    };
} DAC1CON1bits_t;
extern volatile DAC1CON1bits_t DAC1CON1bits __at(0x119);
// bitfield macros
#define _DAC1CON1_DAC1R_POSN                                0x0
#define _DAC1CON1_DAC1R_POSITION                            0x0
#define _DAC1CON1_DAC1R_SIZE                                0x8
#define _DAC1CON1_DAC1R_LENGTH                              0x8
#define _DAC1CON1_DAC1R_MASK                                0xFF
#define _DAC1CON1_DAC1R0_POSN                               0x0
#define _DAC1CON1_DAC1R0_POSITION                           0x0
#define _DAC1CON1_DAC1R0_SIZE                               0x1
#define _DAC1CON1_DAC1R0_LENGTH                             0x1
#define _DAC1CON1_DAC1R0_MASK                               0x1
#define _DAC1CON1_DAC1R1_POSN                               0x1
#define _DAC1CON1_DAC1R1_POSITION                           0x1
#define _DAC1CON1_DAC1R1_SIZE                               0x1
#define _DAC1CON1_DAC1R1_LENGTH                             0x1
#define _DAC1CON1_DAC1R1_MASK                               0x2
#define _DAC1CON1_DAC1R2_POSN                               0x2
#define _DAC1CON1_DAC1R2_POSITION                           0x2
#define _DAC1CON1_DAC1R2_SIZE                               0x1
#define _DAC1CON1_DAC1R2_LENGTH                             0x1
#define _DAC1CON1_DAC1R2_MASK                               0x4
#define _DAC1CON1_DAC1R3_POSN                               0x3
#define _DAC1CON1_DAC1R3_POSITION                           0x3
#define _DAC1CON1_DAC1R3_SIZE                               0x1
#define _DAC1CON1_DAC1R3_LENGTH                             0x1
#define _DAC1CON1_DAC1R3_MASK                               0x8
#define _DAC1CON1_DAC1R4_POSN                               0x4
#define _DAC1CON1_DAC1R4_POSITION                           0x4
#define _DAC1CON1_DAC1R4_SIZE                               0x1
#define _DAC1CON1_DAC1R4_LENGTH                             0x1
#define _DAC1CON1_DAC1R4_MASK                               0x10
#define _DAC1CON1_DAC1R5_POSN                               0x5
#define _DAC1CON1_DAC1R5_POSITION                           0x5
#define _DAC1CON1_DAC1R5_SIZE                               0x1
#define _DAC1CON1_DAC1R5_LENGTH                             0x1
#define _DAC1CON1_DAC1R5_MASK                               0x20
#define _DAC1CON1_DAC1R6_POSN                               0x6
#define _DAC1CON1_DAC1R6_POSITION                           0x6
#define _DAC1CON1_DAC1R6_SIZE                               0x1
#define _DAC1CON1_DAC1R6_LENGTH                             0x1
#define _DAC1CON1_DAC1R6_MASK                               0x40
#define _DAC1CON1_DAC1R7_POSN                               0x7
#define _DAC1CON1_DAC1R7_POSITION                           0x7
#define _DAC1CON1_DAC1R7_SIZE                               0x1
#define _DAC1CON1_DAC1R7_LENGTH                             0x1
#define _DAC1CON1_DAC1R7_MASK                               0x80
#define _DAC1CON1_DACR0_POSN                                0x0
#define _DAC1CON1_DACR0_POSITION                            0x0
#define _DAC1CON1_DACR0_SIZE                                0x1
#define _DAC1CON1_DACR0_LENGTH                              0x1
#define _DAC1CON1_DACR0_MASK                                0x1
#define _DAC1CON1_DACR1_POSN                                0x1
#define _DAC1CON1_DACR1_POSITION                            0x1
#define _DAC1CON1_DACR1_SIZE                                0x1
#define _DAC1CON1_DACR1_LENGTH                              0x1
#define _DAC1CON1_DACR1_MASK                                0x2
#define _DAC1CON1_DACR2_POSN                                0x2
#define _DAC1CON1_DACR2_POSITION                            0x2
#define _DAC1CON1_DACR2_SIZE                                0x1
#define _DAC1CON1_DACR2_LENGTH                              0x1
#define _DAC1CON1_DACR2_MASK                                0x4
#define _DAC1CON1_DACR3_POSN                                0x3
#define _DAC1CON1_DACR3_POSITION                            0x3
#define _DAC1CON1_DACR3_SIZE                                0x1
#define _DAC1CON1_DACR3_LENGTH                              0x1
#define _DAC1CON1_DACR3_MASK                                0x8
#define _DAC1CON1_DACR4_POSN                                0x4
#define _DAC1CON1_DACR4_POSITION                            0x4
#define _DAC1CON1_DACR4_SIZE                                0x1
#define _DAC1CON1_DACR4_LENGTH                              0x1
#define _DAC1CON1_DACR4_MASK                                0x10
#define _DAC1CON1_DACR5_POSN                                0x5
#define _DAC1CON1_DACR5_POSITION                            0x5
#define _DAC1CON1_DACR5_SIZE                                0x1
#define _DAC1CON1_DACR5_LENGTH                              0x1
#define _DAC1CON1_DACR5_MASK                                0x20
#define _DAC1CON1_DACR6_POSN                                0x6
#define _DAC1CON1_DACR6_POSITION                            0x6
#define _DAC1CON1_DACR6_SIZE                                0x1
#define _DAC1CON1_DACR6_LENGTH                              0x1
#define _DAC1CON1_DACR6_MASK                                0x40
#define _DAC1CON1_DACR7_POSN                                0x7
#define _DAC1CON1_DACR7_POSITION                            0x7
#define _DAC1CON1_DACR7_SIZE                                0x1
#define _DAC1CON1_DACR7_LENGTH                              0x1
#define _DAC1CON1_DACR7_MASK                                0x80

// Register: ZCD1CON
#define ZCD1CON ZCD1CON
extern volatile unsigned char           ZCD1CON             __at(0x11C);
#ifndef _LIB_BUILD
asm("ZCD1CON equ 011Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned ZCD1INTN               :1;
        unsigned ZCD1INTP               :1;
        unsigned                        :2;
        unsigned ZCD1POL                :1;
        unsigned ZCD1OUT                :1;
        unsigned                        :1;
        unsigned ZCD1EN                 :1;
    };
} ZCD1CONbits_t;
extern volatile ZCD1CONbits_t ZCD1CONbits __at(0x11C);
// bitfield macros
#define _ZCD1CON_ZCD1INTN_POSN                              0x0
#define _ZCD1CON_ZCD1INTN_POSITION                          0x0
#define _ZCD1CON_ZCD1INTN_SIZE                              0x1
#define _ZCD1CON_ZCD1INTN_LENGTH                            0x1
#define _ZCD1CON_ZCD1INTN_MASK                              0x1
#define _ZCD1CON_ZCD1INTP_POSN                              0x1
#define _ZCD1CON_ZCD1INTP_POSITION                          0x1
#define _ZCD1CON_ZCD1INTP_SIZE                              0x1
#define _ZCD1CON_ZCD1INTP_LENGTH                            0x1
#define _ZCD1CON_ZCD1INTP_MASK                              0x2
#define _ZCD1CON_ZCD1POL_POSN                               0x4
#define _ZCD1CON_ZCD1POL_POSITION                           0x4
#define _ZCD1CON_ZCD1POL_SIZE                               0x1
#define _ZCD1CON_ZCD1POL_LENGTH                             0x1
#define _ZCD1CON_ZCD1POL_MASK                               0x10
#define _ZCD1CON_ZCD1OUT_POSN                               0x5
#define _ZCD1CON_ZCD1OUT_POSITION                           0x5
#define _ZCD1CON_ZCD1OUT_SIZE                               0x1
#define _ZCD1CON_ZCD1OUT_LENGTH                             0x1
#define _ZCD1CON_ZCD1OUT_MASK                               0x20
#define _ZCD1CON_ZCD1EN_POSN                                0x7
#define _ZCD1CON_ZCD1EN_POSITION                            0x7
#define _ZCD1CON_ZCD1EN_SIZE                                0x1
#define _ZCD1CON_ZCD1EN_LENGTH                              0x1
#define _ZCD1CON_ZCD1EN_MASK                                0x80

// Register: ANSELA
#define ANSELA ANSELA
extern volatile unsigned char           ANSELA              __at(0x18C);
#ifndef _LIB_BUILD
asm("ANSELA equ 018Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned ANSA0                  :1;
        unsigned ANSA1                  :1;
        unsigned ANSA2                  :1;
        unsigned                        :1;
        unsigned ANSA4                  :1;
        unsigned ANS5                   :1;
    };
} ANSELAbits_t;
extern volatile ANSELAbits_t ANSELAbits __at(0x18C);
// bitfield macros
#define _ANSELA_ANSA0_POSN                                  0x0
#define _ANSELA_ANSA0_POSITION                              0x0
#define _ANSELA_ANSA0_SIZE                                  0x1
#define _ANSELA_ANSA0_LENGTH                                0x1
#define _ANSELA_ANSA0_MASK                                  0x1
#define _ANSELA_ANSA1_POSN                                  0x1
#define _ANSELA_ANSA1_POSITION                              0x1
#define _ANSELA_ANSA1_SIZE                                  0x1
#define _ANSELA_ANSA1_LENGTH                                0x1
#define _ANSELA_ANSA1_MASK                                  0x2
#define _ANSELA_ANSA2_POSN                                  0x2
#define _ANSELA_ANSA2_POSITION                              0x2
#define _ANSELA_ANSA2_SIZE                                  0x1
#define _ANSELA_ANSA2_LENGTH                                0x1
#define _ANSELA_ANSA2_MASK                                  0x4
#define _ANSELA_ANSA4_POSN                                  0x4
#define _ANSELA_ANSA4_POSITION                              0x4
#define _ANSELA_ANSA4_SIZE                                  0x1
#define _ANSELA_ANSA4_LENGTH                                0x1
#define _ANSELA_ANSA4_MASK                                  0x10
#define _ANSELA_ANS5_POSN                                   0x5
#define _ANSELA_ANS5_POSITION                               0x5
#define _ANSELA_ANS5_SIZE                                   0x1
#define _ANSELA_ANS5_LENGTH                                 0x1
#define _ANSELA_ANS5_MASK                                   0x20

// Register: ANSELB
#define ANSELB ANSELB
extern volatile unsigned char           ANSELB              __at(0x18D);
#ifndef _LIB_BUILD
asm("ANSELB equ 018Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned ANSB4                  :1;
        unsigned ANSB5                  :1;
    };
} ANSELBbits_t;
extern volatile ANSELBbits_t ANSELBbits __at(0x18D);
// bitfield macros
#define _ANSELB_ANSB4_POSN                                  0x4
#define _ANSELB_ANSB4_POSITION                              0x4
#define _ANSELB_ANSB4_SIZE                                  0x1
#define _ANSELB_ANSB4_LENGTH                                0x1
#define _ANSELB_ANSB4_MASK                                  0x10
#define _ANSELB_ANSB5_POSN                                  0x5
#define _ANSELB_ANSB5_POSITION                              0x5
#define _ANSELB_ANSB5_SIZE                                  0x1
#define _ANSELB_ANSB5_LENGTH                                0x1
#define _ANSELB_ANSB5_MASK                                  0x20

// Register: ANSELC
#define ANSELC ANSELC
extern volatile unsigned char           ANSELC              __at(0x18E);
#ifndef _LIB_BUILD
asm("ANSELC equ 018Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned ANSC0                  :1;
        unsigned ANSC1                  :1;
        unsigned ANSC2                  :1;
        unsigned ANSC3                  :1;
        unsigned                        :2;
        unsigned ANSC6                  :1;
        unsigned ANSC7                  :1;
    };
} ANSELCbits_t;
extern volatile ANSELCbits_t ANSELCbits __at(0x18E);
// bitfield macros
#define _ANSELC_ANSC0_POSN                                  0x0
#define _ANSELC_ANSC0_POSITION                              0x0
#define _ANSELC_ANSC0_SIZE                                  0x1
#define _ANSELC_ANSC0_LENGTH                                0x1
#define _ANSELC_ANSC0_MASK                                  0x1
#define _ANSELC_ANSC1_POSN                                  0x1
#define _ANSELC_ANSC1_POSITION                              0x1
#define _ANSELC_ANSC1_SIZE                                  0x1
#define _ANSELC_ANSC1_LENGTH                                0x1
#define _ANSELC_ANSC1_MASK                                  0x2
#define _ANSELC_ANSC2_POSN                                  0x2
#define _ANSELC_ANSC2_POSITION                              0x2
#define _ANSELC_ANSC2_SIZE                                  0x1
#define _ANSELC_ANSC2_LENGTH                                0x1
#define _ANSELC_ANSC2_MASK                                  0x4
#define _ANSELC_ANSC3_POSN                                  0x3
#define _ANSELC_ANSC3_POSITION                              0x3
#define _ANSELC_ANSC3_SIZE                                  0x1
#define _ANSELC_ANSC3_LENGTH                                0x1
#define _ANSELC_ANSC3_MASK                                  0x8
#define _ANSELC_ANSC6_POSN                                  0x6
#define _ANSELC_ANSC6_POSITION                              0x6
#define _ANSELC_ANSC6_SIZE                                  0x1
#define _ANSELC_ANSC6_LENGTH                                0x1
#define _ANSELC_ANSC6_MASK                                  0x40
#define _ANSELC_ANSC7_POSN                                  0x7
#define _ANSELC_ANSC7_POSITION                              0x7
#define _ANSELC_ANSC7_SIZE                                  0x1
#define _ANSELC_ANSC7_LENGTH                                0x1
#define _ANSELC_ANSC7_MASK                                  0x80

// Register: PMADR
#define PMADR PMADR
extern volatile unsigned short          PMADR               __at(0x191);
#ifndef _LIB_BUILD
asm("PMADR equ 0191h");
#endif

// Register: PMADRL
#define PMADRL PMADRL
extern volatile unsigned char           PMADRL              __at(0x191);
#ifndef _LIB_BUILD
asm("PMADRL equ 0191h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PMADRL                 :8;
    };
} PMADRLbits_t;
extern volatile PMADRLbits_t PMADRLbits __at(0x191);
// bitfield macros
#define _PMADRL_PMADRL_POSN                                 0x0
#define _PMADRL_PMADRL_POSITION                             0x0
#define _PMADRL_PMADRL_SIZE                                 0x8
#define _PMADRL_PMADRL_LENGTH                               0x8
#define _PMADRL_PMADRL_MASK                                 0xFF

// Register: PMADRH
#define PMADRH PMADRH
extern volatile unsigned char           PMADRH              __at(0x192);
#ifndef _LIB_BUILD
asm("PMADRH equ 0192h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PMADRH                 :7;
    };
} PMADRHbits_t;
extern volatile PMADRHbits_t PMADRHbits __at(0x192);
// bitfield macros
#define _PMADRH_PMADRH_POSN                                 0x0
#define _PMADRH_PMADRH_POSITION                             0x0
#define _PMADRH_PMADRH_SIZE                                 0x7
#define _PMADRH_PMADRH_LENGTH                               0x7
#define _PMADRH_PMADRH_MASK                                 0x7F

// Register: PMDAT
#define PMDAT PMDAT
extern volatile unsigned short          PMDAT               __at(0x193);
#ifndef _LIB_BUILD
asm("PMDAT equ 0193h");
#endif

// Register: PMDATL
#define PMDATL PMDATL
extern volatile unsigned char           PMDATL              __at(0x193);
#ifndef _LIB_BUILD
asm("PMDATL equ 0193h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PMDATL                 :8;
    };
} PMDATLbits_t;
extern volatile PMDATLbits_t PMDATLbits __at(0x193);
// bitfield macros
#define _PMDATL_PMDATL_POSN                                 0x0
#define _PMDATL_PMDATL_POSITION                             0x0
#define _PMDATL_PMDATL_SIZE                                 0x8
#define _PMDATL_PMDATL_LENGTH                               0x8
#define _PMDATL_PMDATL_MASK                                 0xFF

// Register: PMDATH
#define PMDATH PMDATH
extern volatile unsigned char           PMDATH              __at(0x194);
#ifndef _LIB_BUILD
asm("PMDATH equ 0194h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PMDATH                 :6;
    };
} PMDATHbits_t;
extern volatile PMDATHbits_t PMDATHbits __at(0x194);
// bitfield macros
#define _PMDATH_PMDATH_POSN                                 0x0
#define _PMDATH_PMDATH_POSITION                             0x0
#define _PMDATH_PMDATH_SIZE                                 0x6
#define _PMDATH_PMDATH_LENGTH                               0x6
#define _PMDATH_PMDATH_MASK                                 0x3F

// Register: PMCON1
#define PMCON1 PMCON1
extern volatile unsigned char           PMCON1              __at(0x195);
#ifndef _LIB_BUILD
asm("PMCON1 equ 0195h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RD                     :1;
        unsigned WR                     :1;
        unsigned WREN                   :1;
        unsigned WRERR                  :1;
        unsigned FREE                   :1;
        unsigned LWLO                   :1;
        unsigned CFGS                   :1;
    };
} PMCON1bits_t;
extern volatile PMCON1bits_t PMCON1bits __at(0x195);
// bitfield macros
#define _PMCON1_RD_POSN                                     0x0
#define _PMCON1_RD_POSITION                                 0x0
#define _PMCON1_RD_SIZE                                     0x1
#define _PMCON1_RD_LENGTH                                   0x1
#define _PMCON1_RD_MASK                                     0x1
#define _PMCON1_WR_POSN                                     0x1
#define _PMCON1_WR_POSITION                                 0x1
#define _PMCON1_WR_SIZE                                     0x1
#define _PMCON1_WR_LENGTH                                   0x1
#define _PMCON1_WR_MASK                                     0x2
#define _PMCON1_WREN_POSN                                   0x2
#define _PMCON1_WREN_POSITION                               0x2
#define _PMCON1_WREN_SIZE                                   0x1
#define _PMCON1_WREN_LENGTH                                 0x1
#define _PMCON1_WREN_MASK                                   0x4
#define _PMCON1_WRERR_POSN                                  0x3
#define _PMCON1_WRERR_POSITION                              0x3
#define _PMCON1_WRERR_SIZE                                  0x1
#define _PMCON1_WRERR_LENGTH                                0x1
#define _PMCON1_WRERR_MASK                                  0x8
#define _PMCON1_FREE_POSN                                   0x4
#define _PMCON1_FREE_POSITION                               0x4
#define _PMCON1_FREE_SIZE                                   0x1
#define _PMCON1_FREE_LENGTH                                 0x1
#define _PMCON1_FREE_MASK                                   0x10
#define _PMCON1_LWLO_POSN                                   0x5
#define _PMCON1_LWLO_POSITION                               0x5
#define _PMCON1_LWLO_SIZE                                   0x1
#define _PMCON1_LWLO_LENGTH                                 0x1
#define _PMCON1_LWLO_MASK                                   0x20
#define _PMCON1_CFGS_POSN                                   0x6
#define _PMCON1_CFGS_POSITION                               0x6
#define _PMCON1_CFGS_SIZE                                   0x1
#define _PMCON1_CFGS_LENGTH                                 0x1
#define _PMCON1_CFGS_MASK                                   0x40

// Register: PMCON2
#define PMCON2 PMCON2
extern volatile unsigned char           PMCON2              __at(0x196);
#ifndef _LIB_BUILD
asm("PMCON2 equ 0196h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PMCON2                 :8;
    };
} PMCON2bits_t;
extern volatile PMCON2bits_t PMCON2bits __at(0x196);
// bitfield macros
#define _PMCON2_PMCON2_POSN                                 0x0
#define _PMCON2_PMCON2_POSITION                             0x0
#define _PMCON2_PMCON2_SIZE                                 0x8
#define _PMCON2_PMCON2_LENGTH                               0x8
#define _PMCON2_PMCON2_MASK                                 0xFF

// Register: VREGCON
#define VREGCON VREGCON
extern volatile unsigned char           VREGCON             __at(0x197);
#ifndef _LIB_BUILD
asm("VREGCON equ 0197h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :1;
        unsigned VREGPM                 :1;
    };
} VREGCONbits_t;
extern volatile VREGCONbits_t VREGCONbits __at(0x197);
// bitfield macros
#define _VREGCON_VREGPM_POSN                                0x1
#define _VREGCON_VREGPM_POSITION                            0x1
#define _VREGCON_VREGPM_SIZE                                0x1
#define _VREGCON_VREGPM_LENGTH                              0x1
#define _VREGCON_VREGPM_MASK                                0x2

// Register: RC1REG
#define RC1REG RC1REG
extern volatile unsigned char           RC1REG              __at(0x199);
#ifndef _LIB_BUILD
asm("RC1REG equ 0199h");
#endif
// aliases
extern volatile unsigned char           RCREG               __at(0x199);
#ifndef _LIB_BUILD
asm("RCREG equ 0199h");
#endif
extern volatile unsigned char           RCREG1              __at(0x199);
#ifndef _LIB_BUILD
asm("RCREG1 equ 0199h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RC1REG                 :8;
    };
} RC1REGbits_t;
extern volatile RC1REGbits_t RC1REGbits __at(0x199);
// bitfield macros
#define _RC1REG_RC1REG_POSN                                 0x0
#define _RC1REG_RC1REG_POSITION                             0x0
#define _RC1REG_RC1REG_SIZE                                 0x8
#define _RC1REG_RC1REG_LENGTH                               0x8
#define _RC1REG_RC1REG_MASK                                 0xFF
// alias bitfield definitions
typedef union {
    struct {
        unsigned RC1REG                 :8;
    };
} RCREGbits_t;
extern volatile RCREGbits_t RCREGbits __at(0x199);
// bitfield macros
#define _RCREG_RC1REG_POSN                                  0x0
#define _RCREG_RC1REG_POSITION                              0x0
#define _RCREG_RC1REG_SIZE                                  0x8
#define _RCREG_RC1REG_LENGTH                                0x8
#define _RCREG_RC1REG_MASK                                  0xFF
typedef union {
    struct {
        unsigned RC1REG                 :8;
    };
} RCREG1bits_t;
extern volatile RCREG1bits_t RCREG1bits __at(0x199);
// bitfield macros
#define _RCREG1_RC1REG_POSN                                 0x0
#define _RCREG1_RC1REG_POSITION                             0x0
#define _RCREG1_RC1REG_SIZE                                 0x8
#define _RCREG1_RC1REG_LENGTH                               0x8
#define _RCREG1_RC1REG_MASK                                 0xFF

// Register: TX1REG
#define TX1REG TX1REG
extern volatile unsigned char           TX1REG              __at(0x19A);
#ifndef _LIB_BUILD
asm("TX1REG equ 019Ah");
#endif
// aliases
extern volatile unsigned char           TXREG1              __at(0x19A);
#ifndef _LIB_BUILD
asm("TXREG1 equ 019Ah");
#endif
extern volatile unsigned char           TXREG               __at(0x19A);
#ifndef _LIB_BUILD
asm("TXREG equ 019Ah");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TX1REG                 :8;
    };
} TX1REGbits_t;
extern volatile TX1REGbits_t TX1REGbits __at(0x19A);
// bitfield macros
#define _TX1REG_TX1REG_POSN                                 0x0
#define _TX1REG_TX1REG_POSITION                             0x0
#define _TX1REG_TX1REG_SIZE                                 0x8
#define _TX1REG_TX1REG_LENGTH                               0x8
#define _TX1REG_TX1REG_MASK                                 0xFF
// alias bitfield definitions
typedef union {
    struct {
        unsigned TX1REG                 :8;
    };
} TXREG1bits_t;
extern volatile TXREG1bits_t TXREG1bits __at(0x19A);
// bitfield macros
#define _TXREG1_TX1REG_POSN                                 0x0
#define _TXREG1_TX1REG_POSITION                             0x0
#define _TXREG1_TX1REG_SIZE                                 0x8
#define _TXREG1_TX1REG_LENGTH                               0x8
#define _TXREG1_TX1REG_MASK                                 0xFF
typedef union {
    struct {
        unsigned TX1REG                 :8;
    };
} TXREGbits_t;
extern volatile TXREGbits_t TXREGbits __at(0x19A);
// bitfield macros
#define _TXREG_TX1REG_POSN                                  0x0
#define _TXREG_TX1REG_POSITION                              0x0
#define _TXREG_TX1REG_SIZE                                  0x8
#define _TXREG_TX1REG_LENGTH                                0x8
#define _TXREG_TX1REG_MASK                                  0xFF

// Register: SP1BRG
#define SP1BRG SP1BRG
extern volatile unsigned short          SP1BRG              __at(0x19B);
#ifndef _LIB_BUILD
asm("SP1BRG equ 019Bh");
#endif

// Register: SP1BRGL
#define SP1BRGL SP1BRGL
extern volatile unsigned char           SP1BRGL             __at(0x19B);
#ifndef _LIB_BUILD
asm("SP1BRGL equ 019Bh");
#endif
// aliases
extern volatile unsigned char           SPBRG               __at(0x19B);
#ifndef _LIB_BUILD
asm("SPBRG equ 019Bh");
#endif
extern volatile unsigned char           SPBRG1              __at(0x19B);
#ifndef _LIB_BUILD
asm("SPBRG1 equ 019Bh");
#endif
extern volatile unsigned char           SPBRGL              __at(0x19B);
#ifndef _LIB_BUILD
asm("SPBRGL equ 019Bh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SP1BRGL                :8;
    };
} SP1BRGLbits_t;
extern volatile SP1BRGLbits_t SP1BRGLbits __at(0x19B);
// bitfield macros
#define _SP1BRGL_SP1BRGL_POSN                               0x0
#define _SP1BRGL_SP1BRGL_POSITION                           0x0
#define _SP1BRGL_SP1BRGL_SIZE                               0x8
#define _SP1BRGL_SP1BRGL_LENGTH                             0x8
#define _SP1BRGL_SP1BRGL_MASK                               0xFF
// alias bitfield definitions
typedef union {
    struct {
        unsigned SP1BRGL                :8;
    };
} SPBRGbits_t;
extern volatile SPBRGbits_t SPBRGbits __at(0x19B);
// bitfield macros
#define _SPBRG_SP1BRGL_POSN                                 0x0
#define _SPBRG_SP1BRGL_POSITION                             0x0
#define _SPBRG_SP1BRGL_SIZE                                 0x8
#define _SPBRG_SP1BRGL_LENGTH                               0x8
#define _SPBRG_SP1BRGL_MASK                                 0xFF
typedef union {
    struct {
        unsigned SP1BRGL                :8;
    };
} SPBRG1bits_t;
extern volatile SPBRG1bits_t SPBRG1bits __at(0x19B);
// bitfield macros
#define _SPBRG1_SP1BRGL_POSN                                0x0
#define _SPBRG1_SP1BRGL_POSITION                            0x0
#define _SPBRG1_SP1BRGL_SIZE                                0x8
#define _SPBRG1_SP1BRGL_LENGTH                              0x8
#define _SPBRG1_SP1BRGL_MASK                                0xFF
typedef union {
    struct {
        unsigned SP1BRGL                :8;
    };
} SPBRGLbits_t;
extern volatile SPBRGLbits_t SPBRGLbits __at(0x19B);
// bitfield macros
#define _SPBRGL_SP1BRGL_POSN                                0x0
#define _SPBRGL_SP1BRGL_POSITION                            0x0
#define _SPBRGL_SP1BRGL_SIZE                                0x8
#define _SPBRGL_SP1BRGL_LENGTH                              0x8
#define _SPBRGL_SP1BRGL_MASK                                0xFF

// Register: SP1BRGH
#define SP1BRGH SP1BRGH
extern volatile unsigned char           SP1BRGH             __at(0x19C);
#ifndef _LIB_BUILD
asm("SP1BRGH equ 019Ch");
#endif
// aliases
extern volatile unsigned char           SPBRGH              __at(0x19C);
#ifndef _LIB_BUILD
asm("SPBRGH equ 019Ch");
#endif
extern volatile unsigned char           SPBRGH1             __at(0x19C);
#ifndef _LIB_BUILD
asm("SPBRGH1 equ 019Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SP1BRGH                :8;
    };
} SP1BRGHbits_t;
extern volatile SP1BRGHbits_t SP1BRGHbits __at(0x19C);
// bitfield macros
#define _SP1BRGH_SP1BRGH_POSN                               0x0
#define _SP1BRGH_SP1BRGH_POSITION                           0x0
#define _SP1BRGH_SP1BRGH_SIZE                               0x8
#define _SP1BRGH_SP1BRGH_LENGTH                             0x8
#define _SP1BRGH_SP1BRGH_MASK                               0xFF
// alias bitfield definitions
typedef union {
    struct {
        unsigned SP1BRGH                :8;
    };
} SPBRGHbits_t;
extern volatile SPBRGHbits_t SPBRGHbits __at(0x19C);
// bitfield macros
#define _SPBRGH_SP1BRGH_POSN                                0x0
#define _SPBRGH_SP1BRGH_POSITION                            0x0
#define _SPBRGH_SP1BRGH_SIZE                                0x8
#define _SPBRGH_SP1BRGH_LENGTH                              0x8
#define _SPBRGH_SP1BRGH_MASK                                0xFF
typedef union {
    struct {
        unsigned SP1BRGH                :8;
    };
} SPBRGH1bits_t;
extern volatile SPBRGH1bits_t SPBRGH1bits __at(0x19C);
// bitfield macros
#define _SPBRGH1_SP1BRGH_POSN                               0x0
#define _SPBRGH1_SP1BRGH_POSITION                           0x0
#define _SPBRGH1_SP1BRGH_SIZE                               0x8
#define _SPBRGH1_SP1BRGH_LENGTH                             0x8
#define _SPBRGH1_SP1BRGH_MASK                               0xFF

// Register: RC1STA
#define RC1STA RC1STA
extern volatile unsigned char           RC1STA              __at(0x19D);
#ifndef _LIB_BUILD
asm("RC1STA equ 019Dh");
#endif
// aliases
extern volatile unsigned char           RCSTA1              __at(0x19D);
#ifndef _LIB_BUILD
asm("RCSTA1 equ 019Dh");
#endif
extern volatile unsigned char           RCSTA               __at(0x19D);
#ifndef _LIB_BUILD
asm("RCSTA equ 019Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RX9D                   :1;
        unsigned OERR                   :1;
        unsigned FERR                   :1;
        unsigned ADDEN                  :1;
        unsigned CREN                   :1;
        unsigned SREN                   :1;
        unsigned RX9                    :1;
        unsigned SPEN                   :1;
    };
} RC1STAbits_t;
extern volatile RC1STAbits_t RC1STAbits __at(0x19D);
// bitfield macros
#define _RC1STA_RX9D_POSN                                   0x0
#define _RC1STA_RX9D_POSITION                               0x0
#define _RC1STA_RX9D_SIZE                                   0x1
#define _RC1STA_RX9D_LENGTH                                 0x1
#define _RC1STA_RX9D_MASK                                   0x1
#define _RC1STA_OERR_POSN                                   0x1
#define _RC1STA_OERR_POSITION                               0x1
#define _RC1STA_OERR_SIZE                                   0x1
#define _RC1STA_OERR_LENGTH                                 0x1
#define _RC1STA_OERR_MASK                                   0x2
#define _RC1STA_FERR_POSN                                   0x2
#define _RC1STA_FERR_POSITION                               0x2
#define _RC1STA_FERR_SIZE                                   0x1
#define _RC1STA_FERR_LENGTH                                 0x1
#define _RC1STA_FERR_MASK                                   0x4
#define _RC1STA_ADDEN_POSN                                  0x3
#define _RC1STA_ADDEN_POSITION                              0x3
#define _RC1STA_ADDEN_SIZE                                  0x1
#define _RC1STA_ADDEN_LENGTH                                0x1
#define _RC1STA_ADDEN_MASK                                  0x8
#define _RC1STA_CREN_POSN                                   0x4
#define _RC1STA_CREN_POSITION                               0x4
#define _RC1STA_CREN_SIZE                                   0x1
#define _RC1STA_CREN_LENGTH                                 0x1
#define _RC1STA_CREN_MASK                                   0x10
#define _RC1STA_SREN_POSN                                   0x5
#define _RC1STA_SREN_POSITION                               0x5
#define _RC1STA_SREN_SIZE                                   0x1
#define _RC1STA_SREN_LENGTH                                 0x1
#define _RC1STA_SREN_MASK                                   0x20
#define _RC1STA_RX9_POSN                                    0x6
#define _RC1STA_RX9_POSITION                                0x6
#define _RC1STA_RX9_SIZE                                    0x1
#define _RC1STA_RX9_LENGTH                                  0x1
#define _RC1STA_RX9_MASK                                    0x40
#define _RC1STA_SPEN_POSN                                   0x7
#define _RC1STA_SPEN_POSITION                               0x7
#define _RC1STA_SPEN_SIZE                                   0x1
#define _RC1STA_SPEN_LENGTH                                 0x1
#define _RC1STA_SPEN_MASK                                   0x80
// alias bitfield definitions
typedef union {
    struct {
        unsigned RX9D                   :1;
        unsigned OERR                   :1;
        unsigned FERR                   :1;
        unsigned ADDEN                  :1;
        unsigned CREN                   :1;
        unsigned SREN                   :1;
        unsigned RX9                    :1;
        unsigned SPEN                   :1;
    };
} RCSTA1bits_t;
extern volatile RCSTA1bits_t RCSTA1bits __at(0x19D);
// bitfield macros
#define _RCSTA1_RX9D_POSN                                   0x0
#define _RCSTA1_RX9D_POSITION                               0x0
#define _RCSTA1_RX9D_SIZE                                   0x1
#define _RCSTA1_RX9D_LENGTH                                 0x1
#define _RCSTA1_RX9D_MASK                                   0x1
#define _RCSTA1_OERR_POSN                                   0x1
#define _RCSTA1_OERR_POSITION                               0x1
#define _RCSTA1_OERR_SIZE                                   0x1
#define _RCSTA1_OERR_LENGTH                                 0x1
#define _RCSTA1_OERR_MASK                                   0x2
#define _RCSTA1_FERR_POSN                                   0x2
#define _RCSTA1_FERR_POSITION                               0x2
#define _RCSTA1_FERR_SIZE                                   0x1
#define _RCSTA1_FERR_LENGTH                                 0x1
#define _RCSTA1_FERR_MASK                                   0x4
#define _RCSTA1_ADDEN_POSN                                  0x3
#define _RCSTA1_ADDEN_POSITION                              0x3
#define _RCSTA1_ADDEN_SIZE                                  0x1
#define _RCSTA1_ADDEN_LENGTH                                0x1
#define _RCSTA1_ADDEN_MASK                                  0x8
#define _RCSTA1_CREN_POSN                                   0x4
#define _RCSTA1_CREN_POSITION                               0x4
#define _RCSTA1_CREN_SIZE                                   0x1
#define _RCSTA1_CREN_LENGTH                                 0x1
#define _RCSTA1_CREN_MASK                                   0x10
#define _RCSTA1_SREN_POSN                                   0x5
#define _RCSTA1_SREN_POSITION                               0x5
#define _RCSTA1_SREN_SIZE                                   0x1
#define _RCSTA1_SREN_LENGTH                                 0x1
#define _RCSTA1_SREN_MASK                                   0x20
#define _RCSTA1_RX9_POSN                                    0x6
#define _RCSTA1_RX9_POSITION                                0x6
#define _RCSTA1_RX9_SIZE                                    0x1
#define _RCSTA1_RX9_LENGTH                                  0x1
#define _RCSTA1_RX9_MASK                                    0x40
#define _RCSTA1_SPEN_POSN                                   0x7
#define _RCSTA1_SPEN_POSITION                               0x7
#define _RCSTA1_SPEN_SIZE                                   0x1
#define _RCSTA1_SPEN_LENGTH                                 0x1
#define _RCSTA1_SPEN_MASK                                   0x80
typedef union {
    struct {
        unsigned RX9D                   :1;
        unsigned OERR                   :1;
        unsigned FERR                   :1;
        unsigned ADDEN                  :1;
        unsigned CREN                   :1;
        unsigned SREN                   :1;
        unsigned RX9                    :1;
        unsigned SPEN                   :1;
    };
} RCSTAbits_t;
extern volatile RCSTAbits_t RCSTAbits __at(0x19D);
// bitfield macros
#define _RCSTA_RX9D_POSN                                    0x0
#define _RCSTA_RX9D_POSITION                                0x0
#define _RCSTA_RX9D_SIZE                                    0x1
#define _RCSTA_RX9D_LENGTH                                  0x1
#define _RCSTA_RX9D_MASK                                    0x1
#define _RCSTA_OERR_POSN                                    0x1
#define _RCSTA_OERR_POSITION                                0x1
#define _RCSTA_OERR_SIZE                                    0x1
#define _RCSTA_OERR_LENGTH                                  0x1
#define _RCSTA_OERR_MASK                                    0x2
#define _RCSTA_FERR_POSN                                    0x2
#define _RCSTA_FERR_POSITION                                0x2
#define _RCSTA_FERR_SIZE                                    0x1
#define _RCSTA_FERR_LENGTH                                  0x1
#define _RCSTA_FERR_MASK                                    0x4
#define _RCSTA_ADDEN_POSN                                   0x3
#define _RCSTA_ADDEN_POSITION                               0x3
#define _RCSTA_ADDEN_SIZE                                   0x1
#define _RCSTA_ADDEN_LENGTH                                 0x1
#define _RCSTA_ADDEN_MASK                                   0x8
#define _RCSTA_CREN_POSN                                    0x4
#define _RCSTA_CREN_POSITION                                0x4
#define _RCSTA_CREN_SIZE                                    0x1
#define _RCSTA_CREN_LENGTH                                  0x1
#define _RCSTA_CREN_MASK                                    0x10
#define _RCSTA_SREN_POSN                                    0x5
#define _RCSTA_SREN_POSITION                                0x5
#define _RCSTA_SREN_SIZE                                    0x1
#define _RCSTA_SREN_LENGTH                                  0x1
#define _RCSTA_SREN_MASK                                    0x20
#define _RCSTA_RX9_POSN                                     0x6
#define _RCSTA_RX9_POSITION                                 0x6
#define _RCSTA_RX9_SIZE                                     0x1
#define _RCSTA_RX9_LENGTH                                   0x1
#define _RCSTA_RX9_MASK                                     0x40
#define _RCSTA_SPEN_POSN                                    0x7
#define _RCSTA_SPEN_POSITION                                0x7
#define _RCSTA_SPEN_SIZE                                    0x1
#define _RCSTA_SPEN_LENGTH                                  0x1
#define _RCSTA_SPEN_MASK                                    0x80

// Register: TX1STA
#define TX1STA TX1STA
extern volatile unsigned char           TX1STA              __at(0x19E);
#ifndef _LIB_BUILD
asm("TX1STA equ 019Eh");
#endif
// aliases
extern volatile unsigned char           TXSTA1              __at(0x19E);
#ifndef _LIB_BUILD
asm("TXSTA1 equ 019Eh");
#endif
extern volatile unsigned char           TXSTA               __at(0x19E);
#ifndef _LIB_BUILD
asm("TXSTA equ 019Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TX9D                   :1;
        unsigned TRMT                   :1;
        unsigned BRGH                   :1;
        unsigned SENDB                  :1;
        unsigned SYNC                   :1;
        unsigned TXEN                   :1;
        unsigned TX9                    :1;
        unsigned CSRC                   :1;
    };
} TX1STAbits_t;
extern volatile TX1STAbits_t TX1STAbits __at(0x19E);
// bitfield macros
#define _TX1STA_TX9D_POSN                                   0x0
#define _TX1STA_TX9D_POSITION                               0x0
#define _TX1STA_TX9D_SIZE                                   0x1
#define _TX1STA_TX9D_LENGTH                                 0x1
#define _TX1STA_TX9D_MASK                                   0x1
#define _TX1STA_TRMT_POSN                                   0x1
#define _TX1STA_TRMT_POSITION                               0x1
#define _TX1STA_TRMT_SIZE                                   0x1
#define _TX1STA_TRMT_LENGTH                                 0x1
#define _TX1STA_TRMT_MASK                                   0x2
#define _TX1STA_BRGH_POSN                                   0x2
#define _TX1STA_BRGH_POSITION                               0x2
#define _TX1STA_BRGH_SIZE                                   0x1
#define _TX1STA_BRGH_LENGTH                                 0x1
#define _TX1STA_BRGH_MASK                                   0x4
#define _TX1STA_SENDB_POSN                                  0x3
#define _TX1STA_SENDB_POSITION                              0x3
#define _TX1STA_SENDB_SIZE                                  0x1
#define _TX1STA_SENDB_LENGTH                                0x1
#define _TX1STA_SENDB_MASK                                  0x8
#define _TX1STA_SYNC_POSN                                   0x4
#define _TX1STA_SYNC_POSITION                               0x4
#define _TX1STA_SYNC_SIZE                                   0x1
#define _TX1STA_SYNC_LENGTH                                 0x1
#define _TX1STA_SYNC_MASK                                   0x10
#define _TX1STA_TXEN_POSN                                   0x5
#define _TX1STA_TXEN_POSITION                               0x5
#define _TX1STA_TXEN_SIZE                                   0x1
#define _TX1STA_TXEN_LENGTH                                 0x1
#define _TX1STA_TXEN_MASK                                   0x20
#define _TX1STA_TX9_POSN                                    0x6
#define _TX1STA_TX9_POSITION                                0x6
#define _TX1STA_TX9_SIZE                                    0x1
#define _TX1STA_TX9_LENGTH                                  0x1
#define _TX1STA_TX9_MASK                                    0x40
#define _TX1STA_CSRC_POSN                                   0x7
#define _TX1STA_CSRC_POSITION                               0x7
#define _TX1STA_CSRC_SIZE                                   0x1
#define _TX1STA_CSRC_LENGTH                                 0x1
#define _TX1STA_CSRC_MASK                                   0x80
// alias bitfield definitions
typedef union {
    struct {
        unsigned TX9D                   :1;
        unsigned TRMT                   :1;
        unsigned BRGH                   :1;
        unsigned SENDB                  :1;
        unsigned SYNC                   :1;
        unsigned TXEN                   :1;
        unsigned TX9                    :1;
        unsigned CSRC                   :1;
    };
} TXSTA1bits_t;
extern volatile TXSTA1bits_t TXSTA1bits __at(0x19E);
// bitfield macros
#define _TXSTA1_TX9D_POSN                                   0x0
#define _TXSTA1_TX9D_POSITION                               0x0
#define _TXSTA1_TX9D_SIZE                                   0x1
#define _TXSTA1_TX9D_LENGTH                                 0x1
#define _TXSTA1_TX9D_MASK                                   0x1
#define _TXSTA1_TRMT_POSN                                   0x1
#define _TXSTA1_TRMT_POSITION                               0x1
#define _TXSTA1_TRMT_SIZE                                   0x1
#define _TXSTA1_TRMT_LENGTH                                 0x1
#define _TXSTA1_TRMT_MASK                                   0x2
#define _TXSTA1_BRGH_POSN                                   0x2
#define _TXSTA1_BRGH_POSITION                               0x2
#define _TXSTA1_BRGH_SIZE                                   0x1
#define _TXSTA1_BRGH_LENGTH                                 0x1
#define _TXSTA1_BRGH_MASK                                   0x4
#define _TXSTA1_SENDB_POSN                                  0x3
#define _TXSTA1_SENDB_POSITION                              0x3
#define _TXSTA1_SENDB_SIZE                                  0x1
#define _TXSTA1_SENDB_LENGTH                                0x1
#define _TXSTA1_SENDB_MASK                                  0x8
#define _TXSTA1_SYNC_POSN                                   0x4
#define _TXSTA1_SYNC_POSITION                               0x4
#define _TXSTA1_SYNC_SIZE                                   0x1
#define _TXSTA1_SYNC_LENGTH                                 0x1
#define _TXSTA1_SYNC_MASK                                   0x10
#define _TXSTA1_TXEN_POSN                                   0x5
#define _TXSTA1_TXEN_POSITION                               0x5
#define _TXSTA1_TXEN_SIZE                                   0x1
#define _TXSTA1_TXEN_LENGTH                                 0x1
#define _TXSTA1_TXEN_MASK                                   0x20
#define _TXSTA1_TX9_POSN                                    0x6
#define _TXSTA1_TX9_POSITION                                0x6
#define _TXSTA1_TX9_SIZE                                    0x1
#define _TXSTA1_TX9_LENGTH                                  0x1
#define _TXSTA1_TX9_MASK                                    0x40
#define _TXSTA1_CSRC_POSN                                   0x7
#define _TXSTA1_CSRC_POSITION                               0x7
#define _TXSTA1_CSRC_SIZE                                   0x1
#define _TXSTA1_CSRC_LENGTH                                 0x1
#define _TXSTA1_CSRC_MASK                                   0x80
typedef union {
    struct {
        unsigned TX9D                   :1;
        unsigned TRMT                   :1;
        unsigned BRGH                   :1;
        unsigned SENDB                  :1;
        unsigned SYNC                   :1;
        unsigned TXEN                   :1;
        unsigned TX9                    :1;
        unsigned CSRC                   :1;
    };
} TXSTAbits_t;
extern volatile TXSTAbits_t TXSTAbits __at(0x19E);
// bitfield macros
#define _TXSTA_TX9D_POSN                                    0x0
#define _TXSTA_TX9D_POSITION                                0x0
#define _TXSTA_TX9D_SIZE                                    0x1
#define _TXSTA_TX9D_LENGTH                                  0x1
#define _TXSTA_TX9D_MASK                                    0x1
#define _TXSTA_TRMT_POSN                                    0x1
#define _TXSTA_TRMT_POSITION                                0x1
#define _TXSTA_TRMT_SIZE                                    0x1
#define _TXSTA_TRMT_LENGTH                                  0x1
#define _TXSTA_TRMT_MASK                                    0x2
#define _TXSTA_BRGH_POSN                                    0x2
#define _TXSTA_BRGH_POSITION                                0x2
#define _TXSTA_BRGH_SIZE                                    0x1
#define _TXSTA_BRGH_LENGTH                                  0x1
#define _TXSTA_BRGH_MASK                                    0x4
#define _TXSTA_SENDB_POSN                                   0x3
#define _TXSTA_SENDB_POSITION                               0x3
#define _TXSTA_SENDB_SIZE                                   0x1
#define _TXSTA_SENDB_LENGTH                                 0x1
#define _TXSTA_SENDB_MASK                                   0x8
#define _TXSTA_SYNC_POSN                                    0x4
#define _TXSTA_SYNC_POSITION                                0x4
#define _TXSTA_SYNC_SIZE                                    0x1
#define _TXSTA_SYNC_LENGTH                                  0x1
#define _TXSTA_SYNC_MASK                                    0x10
#define _TXSTA_TXEN_POSN                                    0x5
#define _TXSTA_TXEN_POSITION                                0x5
#define _TXSTA_TXEN_SIZE                                    0x1
#define _TXSTA_TXEN_LENGTH                                  0x1
#define _TXSTA_TXEN_MASK                                    0x20
#define _TXSTA_TX9_POSN                                     0x6
#define _TXSTA_TX9_POSITION                                 0x6
#define _TXSTA_TX9_SIZE                                     0x1
#define _TXSTA_TX9_LENGTH                                   0x1
#define _TXSTA_TX9_MASK                                     0x40
#define _TXSTA_CSRC_POSN                                    0x7
#define _TXSTA_CSRC_POSITION                                0x7
#define _TXSTA_CSRC_SIZE                                    0x1
#define _TXSTA_CSRC_LENGTH                                  0x1
#define _TXSTA_CSRC_MASK                                    0x80

// Register: BAUD1CON
#define BAUD1CON BAUD1CON
extern volatile unsigned char           BAUD1CON            __at(0x19F);
#ifndef _LIB_BUILD
asm("BAUD1CON equ 019Fh");
#endif
// aliases
extern volatile unsigned char           BAUDCON1            __at(0x19F);
#ifndef _LIB_BUILD
asm("BAUDCON1 equ 019Fh");
#endif
extern volatile unsigned char           BAUDCTL1            __at(0x19F);
#ifndef _LIB_BUILD
asm("BAUDCTL1 equ 019Fh");
#endif
extern volatile unsigned char           BAUDCON             __at(0x19F);
#ifndef _LIB_BUILD
asm("BAUDCON equ 019Fh");
#endif
extern volatile unsigned char           BAUDCTL             __at(0x19F);
#ifndef _LIB_BUILD
asm("BAUDCTL equ 019Fh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned ABDEN                  :1;
        unsigned WUE                    :1;
        unsigned                        :1;
        unsigned BRG16                  :1;
        unsigned SCKP                   :1;
        unsigned                        :1;
        unsigned RCIDL                  :1;
        unsigned ABDOVF                 :1;
    };
} BAUD1CONbits_t;
extern volatile BAUD1CONbits_t BAUD1CONbits __at(0x19F);
// bitfield macros
#define _BAUD1CON_ABDEN_POSN                                0x0
#define _BAUD1CON_ABDEN_POSITION                            0x0
#define _BAUD1CON_ABDEN_SIZE                                0x1
#define _BAUD1CON_ABDEN_LENGTH                              0x1
#define _BAUD1CON_ABDEN_MASK                                0x1
#define _BAUD1CON_WUE_POSN                                  0x1
#define _BAUD1CON_WUE_POSITION                              0x1
#define _BAUD1CON_WUE_SIZE                                  0x1
#define _BAUD1CON_WUE_LENGTH                                0x1
#define _BAUD1CON_WUE_MASK                                  0x2
#define _BAUD1CON_BRG16_POSN                                0x3
#define _BAUD1CON_BRG16_POSITION                            0x3
#define _BAUD1CON_BRG16_SIZE                                0x1
#define _BAUD1CON_BRG16_LENGTH                              0x1
#define _BAUD1CON_BRG16_MASK                                0x8
#define _BAUD1CON_SCKP_POSN                                 0x4
#define _BAUD1CON_SCKP_POSITION                             0x4
#define _BAUD1CON_SCKP_SIZE                                 0x1
#define _BAUD1CON_SCKP_LENGTH                               0x1
#define _BAUD1CON_SCKP_MASK                                 0x10
#define _BAUD1CON_RCIDL_POSN                                0x6
#define _BAUD1CON_RCIDL_POSITION                            0x6
#define _BAUD1CON_RCIDL_SIZE                                0x1
#define _BAUD1CON_RCIDL_LENGTH                              0x1
#define _BAUD1CON_RCIDL_MASK                                0x40
#define _BAUD1CON_ABDOVF_POSN                               0x7
#define _BAUD1CON_ABDOVF_POSITION                           0x7
#define _BAUD1CON_ABDOVF_SIZE                               0x1
#define _BAUD1CON_ABDOVF_LENGTH                             0x1
#define _BAUD1CON_ABDOVF_MASK                               0x80
// alias bitfield definitions
typedef union {
    struct {
        unsigned ABDEN                  :1;
        unsigned WUE                    :1;
        unsigned                        :1;
        unsigned BRG16                  :1;
        unsigned SCKP                   :1;
        unsigned                        :1;
        unsigned RCIDL                  :1;
        unsigned ABDOVF                 :1;
    };
} BAUDCON1bits_t;
extern volatile BAUDCON1bits_t BAUDCON1bits __at(0x19F);
// bitfield macros
#define _BAUDCON1_ABDEN_POSN                                0x0
#define _BAUDCON1_ABDEN_POSITION                            0x0
#define _BAUDCON1_ABDEN_SIZE                                0x1
#define _BAUDCON1_ABDEN_LENGTH                              0x1
#define _BAUDCON1_ABDEN_MASK                                0x1
#define _BAUDCON1_WUE_POSN                                  0x1
#define _BAUDCON1_WUE_POSITION                              0x1
#define _BAUDCON1_WUE_SIZE                                  0x1
#define _BAUDCON1_WUE_LENGTH                                0x1
#define _BAUDCON1_WUE_MASK                                  0x2
#define _BAUDCON1_BRG16_POSN                                0x3
#define _BAUDCON1_BRG16_POSITION                            0x3
#define _BAUDCON1_BRG16_SIZE                                0x1
#define _BAUDCON1_BRG16_LENGTH                              0x1
#define _BAUDCON1_BRG16_MASK                                0x8
#define _BAUDCON1_SCKP_POSN                                 0x4
#define _BAUDCON1_SCKP_POSITION                             0x4
#define _BAUDCON1_SCKP_SIZE                                 0x1
#define _BAUDCON1_SCKP_LENGTH                               0x1
#define _BAUDCON1_SCKP_MASK                                 0x10
#define _BAUDCON1_RCIDL_POSN                                0x6
#define _BAUDCON1_RCIDL_POSITION                            0x6
#define _BAUDCON1_RCIDL_SIZE                                0x1
#define _BAUDCON1_RCIDL_LENGTH                              0x1
#define _BAUDCON1_RCIDL_MASK                                0x40
#define _BAUDCON1_ABDOVF_POSN                               0x7
#define _BAUDCON1_ABDOVF_POSITION                           0x7
#define _BAUDCON1_ABDOVF_SIZE                               0x1
#define _BAUDCON1_ABDOVF_LENGTH                             0x1
#define _BAUDCON1_ABDOVF_MASK                               0x80
typedef union {
    struct {
        unsigned ABDEN                  :1;
        unsigned WUE                    :1;
        unsigned                        :1;
        unsigned BRG16                  :1;
        unsigned SCKP                   :1;
        unsigned                        :1;
        unsigned RCIDL                  :1;
        unsigned ABDOVF                 :1;
    };
} BAUDCTL1bits_t;
extern volatile BAUDCTL1bits_t BAUDCTL1bits __at(0x19F);
// bitfield macros
#define _BAUDCTL1_ABDEN_POSN                                0x0
#define _BAUDCTL1_ABDEN_POSITION                            0x0
#define _BAUDCTL1_ABDEN_SIZE                                0x1
#define _BAUDCTL1_ABDEN_LENGTH                              0x1
#define _BAUDCTL1_ABDEN_MASK                                0x1
#define _BAUDCTL1_WUE_POSN                                  0x1
#define _BAUDCTL1_WUE_POSITION                              0x1
#define _BAUDCTL1_WUE_SIZE                                  0x1
#define _BAUDCTL1_WUE_LENGTH                                0x1
#define _BAUDCTL1_WUE_MASK                                  0x2
#define _BAUDCTL1_BRG16_POSN                                0x3
#define _BAUDCTL1_BRG16_POSITION                            0x3
#define _BAUDCTL1_BRG16_SIZE                                0x1
#define _BAUDCTL1_BRG16_LENGTH                              0x1
#define _BAUDCTL1_BRG16_MASK                                0x8
#define _BAUDCTL1_SCKP_POSN                                 0x4
#define _BAUDCTL1_SCKP_POSITION                             0x4
#define _BAUDCTL1_SCKP_SIZE                                 0x1
#define _BAUDCTL1_SCKP_LENGTH                               0x1
#define _BAUDCTL1_SCKP_MASK                                 0x10
#define _BAUDCTL1_RCIDL_POSN                                0x6
#define _BAUDCTL1_RCIDL_POSITION                            0x6
#define _BAUDCTL1_RCIDL_SIZE                                0x1
#define _BAUDCTL1_RCIDL_LENGTH                              0x1
#define _BAUDCTL1_RCIDL_MASK                                0x40
#define _BAUDCTL1_ABDOVF_POSN                               0x7
#define _BAUDCTL1_ABDOVF_POSITION                           0x7
#define _BAUDCTL1_ABDOVF_SIZE                               0x1
#define _BAUDCTL1_ABDOVF_LENGTH                             0x1
#define _BAUDCTL1_ABDOVF_MASK                               0x80
typedef union {
    struct {
        unsigned ABDEN                  :1;
        unsigned WUE                    :1;
        unsigned                        :1;
        unsigned BRG16                  :1;
        unsigned SCKP                   :1;
        unsigned                        :1;
        unsigned RCIDL                  :1;
        unsigned ABDOVF                 :1;
    };
} BAUDCONbits_t;
extern volatile BAUDCONbits_t BAUDCONbits __at(0x19F);
// bitfield macros
#define _BAUDCON_ABDEN_POSN                                 0x0
#define _BAUDCON_ABDEN_POSITION                             0x0
#define _BAUDCON_ABDEN_SIZE                                 0x1
#define _BAUDCON_ABDEN_LENGTH                               0x1
#define _BAUDCON_ABDEN_MASK                                 0x1
#define _BAUDCON_WUE_POSN                                   0x1
#define _BAUDCON_WUE_POSITION                               0x1
#define _BAUDCON_WUE_SIZE                                   0x1
#define _BAUDCON_WUE_LENGTH                                 0x1
#define _BAUDCON_WUE_MASK                                   0x2
#define _BAUDCON_BRG16_POSN                                 0x3
#define _BAUDCON_BRG16_POSITION                             0x3
#define _BAUDCON_BRG16_SIZE                                 0x1
#define _BAUDCON_BRG16_LENGTH                               0x1
#define _BAUDCON_BRG16_MASK                                 0x8
#define _BAUDCON_SCKP_POSN                                  0x4
#define _BAUDCON_SCKP_POSITION                              0x4
#define _BAUDCON_SCKP_SIZE                                  0x1
#define _BAUDCON_SCKP_LENGTH                                0x1
#define _BAUDCON_SCKP_MASK                                  0x10
#define _BAUDCON_RCIDL_POSN                                 0x6
#define _BAUDCON_RCIDL_POSITION                             0x6
#define _BAUDCON_RCIDL_SIZE                                 0x1
#define _BAUDCON_RCIDL_LENGTH                               0x1
#define _BAUDCON_RCIDL_MASK                                 0x40
#define _BAUDCON_ABDOVF_POSN                                0x7
#define _BAUDCON_ABDOVF_POSITION                            0x7
#define _BAUDCON_ABDOVF_SIZE                                0x1
#define _BAUDCON_ABDOVF_LENGTH                              0x1
#define _BAUDCON_ABDOVF_MASK                                0x80
typedef union {
    struct {
        unsigned ABDEN                  :1;
        unsigned WUE                    :1;
        unsigned                        :1;
        unsigned BRG16                  :1;
        unsigned SCKP                   :1;
        unsigned                        :1;
        unsigned RCIDL                  :1;
        unsigned ABDOVF                 :1;
    };
} BAUDCTLbits_t;
extern volatile BAUDCTLbits_t BAUDCTLbits __at(0x19F);
// bitfield macros
#define _BAUDCTL_ABDEN_POSN                                 0x0
#define _BAUDCTL_ABDEN_POSITION                             0x0
#define _BAUDCTL_ABDEN_SIZE                                 0x1
#define _BAUDCTL_ABDEN_LENGTH                               0x1
#define _BAUDCTL_ABDEN_MASK                                 0x1
#define _BAUDCTL_WUE_POSN                                   0x1
#define _BAUDCTL_WUE_POSITION                               0x1
#define _BAUDCTL_WUE_SIZE                                   0x1
#define _BAUDCTL_WUE_LENGTH                                 0x1
#define _BAUDCTL_WUE_MASK                                   0x2
#define _BAUDCTL_BRG16_POSN                                 0x3
#define _BAUDCTL_BRG16_POSITION                             0x3
#define _BAUDCTL_BRG16_SIZE                                 0x1
#define _BAUDCTL_BRG16_LENGTH                               0x1
#define _BAUDCTL_BRG16_MASK                                 0x8
#define _BAUDCTL_SCKP_POSN                                  0x4
#define _BAUDCTL_SCKP_POSITION                              0x4
#define _BAUDCTL_SCKP_SIZE                                  0x1
#define _BAUDCTL_SCKP_LENGTH                                0x1
#define _BAUDCTL_SCKP_MASK                                  0x10
#define _BAUDCTL_RCIDL_POSN                                 0x6
#define _BAUDCTL_RCIDL_POSITION                             0x6
#define _BAUDCTL_RCIDL_SIZE                                 0x1
#define _BAUDCTL_RCIDL_LENGTH                               0x1
#define _BAUDCTL_RCIDL_MASK                                 0x40
#define _BAUDCTL_ABDOVF_POSN                                0x7
#define _BAUDCTL_ABDOVF_POSITION                            0x7
#define _BAUDCTL_ABDOVF_SIZE                                0x1
#define _BAUDCTL_ABDOVF_LENGTH                              0x1
#define _BAUDCTL_ABDOVF_MASK                                0x80

// Register: WPUA
#define WPUA WPUA
extern volatile unsigned char           WPUA                __at(0x20C);
#ifndef _LIB_BUILD
asm("WPUA equ 020Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned WPUA0                  :1;
        unsigned WPUA1                  :1;
        unsigned WPUA2                  :1;
        unsigned WPUA3                  :1;
        unsigned WPUA4                  :1;
        unsigned WPUA5                  :1;
    };
} WPUAbits_t;
extern volatile WPUAbits_t WPUAbits __at(0x20C);
// bitfield macros
#define _WPUA_WPUA0_POSN                                    0x0
#define _WPUA_WPUA0_POSITION                                0x0
#define _WPUA_WPUA0_SIZE                                    0x1
#define _WPUA_WPUA0_LENGTH                                  0x1
#define _WPUA_WPUA0_MASK                                    0x1
#define _WPUA_WPUA1_POSN                                    0x1
#define _WPUA_WPUA1_POSITION                                0x1
#define _WPUA_WPUA1_SIZE                                    0x1
#define _WPUA_WPUA1_LENGTH                                  0x1
#define _WPUA_WPUA1_MASK                                    0x2
#define _WPUA_WPUA2_POSN                                    0x2
#define _WPUA_WPUA2_POSITION                                0x2
#define _WPUA_WPUA2_SIZE                                    0x1
#define _WPUA_WPUA2_LENGTH                                  0x1
#define _WPUA_WPUA2_MASK                                    0x4
#define _WPUA_WPUA3_POSN                                    0x3
#define _WPUA_WPUA3_POSITION                                0x3
#define _WPUA_WPUA3_SIZE                                    0x1
#define _WPUA_WPUA3_LENGTH                                  0x1
#define _WPUA_WPUA3_MASK                                    0x8
#define _WPUA_WPUA4_POSN                                    0x4
#define _WPUA_WPUA4_POSITION                                0x4
#define _WPUA_WPUA4_SIZE                                    0x1
#define _WPUA_WPUA4_LENGTH                                  0x1
#define _WPUA_WPUA4_MASK                                    0x10
#define _WPUA_WPUA5_POSN                                    0x5
#define _WPUA_WPUA5_POSITION                                0x5
#define _WPUA_WPUA5_SIZE                                    0x1
#define _WPUA_WPUA5_LENGTH                                  0x1
#define _WPUA_WPUA5_MASK                                    0x20

// Register: WPUB
#define WPUB WPUB
extern volatile unsigned char           WPUB                __at(0x20D);
#ifndef _LIB_BUILD
asm("WPUB equ 020Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned WPUB4                  :1;
        unsigned WPUB5                  :1;
        unsigned WPUB6                  :1;
        unsigned WPUB7                  :1;
    };
} WPUBbits_t;
extern volatile WPUBbits_t WPUBbits __at(0x20D);
// bitfield macros
#define _WPUB_WPUB4_POSN                                    0x4
#define _WPUB_WPUB4_POSITION                                0x4
#define _WPUB_WPUB4_SIZE                                    0x1
#define _WPUB_WPUB4_LENGTH                                  0x1
#define _WPUB_WPUB4_MASK                                    0x10
#define _WPUB_WPUB5_POSN                                    0x5
#define _WPUB_WPUB5_POSITION                                0x5
#define _WPUB_WPUB5_SIZE                                    0x1
#define _WPUB_WPUB5_LENGTH                                  0x1
#define _WPUB_WPUB5_MASK                                    0x20
#define _WPUB_WPUB6_POSN                                    0x6
#define _WPUB_WPUB6_POSITION                                0x6
#define _WPUB_WPUB6_SIZE                                    0x1
#define _WPUB_WPUB6_LENGTH                                  0x1
#define _WPUB_WPUB6_MASK                                    0x40
#define _WPUB_WPUB7_POSN                                    0x7
#define _WPUB_WPUB7_POSITION                                0x7
#define _WPUB_WPUB7_SIZE                                    0x1
#define _WPUB_WPUB7_LENGTH                                  0x1
#define _WPUB_WPUB7_MASK                                    0x80

// Register: WPUC
#define WPUC WPUC
extern volatile unsigned char           WPUC                __at(0x20E);
#ifndef _LIB_BUILD
asm("WPUC equ 020Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned WPUC0                  :1;
        unsigned WPUC1                  :1;
        unsigned WPUC2                  :1;
        unsigned WPUC3                  :1;
        unsigned WPUC4                  :1;
        unsigned WPUC5                  :1;
        unsigned WPUC6                  :1;
        unsigned WPUC7                  :1;
    };
} WPUCbits_t;
extern volatile WPUCbits_t WPUCbits __at(0x20E);
// bitfield macros
#define _WPUC_WPUC0_POSN                                    0x0
#define _WPUC_WPUC0_POSITION                                0x0
#define _WPUC_WPUC0_SIZE                                    0x1
#define _WPUC_WPUC0_LENGTH                                  0x1
#define _WPUC_WPUC0_MASK                                    0x1
#define _WPUC_WPUC1_POSN                                    0x1
#define _WPUC_WPUC1_POSITION                                0x1
#define _WPUC_WPUC1_SIZE                                    0x1
#define _WPUC_WPUC1_LENGTH                                  0x1
#define _WPUC_WPUC1_MASK                                    0x2
#define _WPUC_WPUC2_POSN                                    0x2
#define _WPUC_WPUC2_POSITION                                0x2
#define _WPUC_WPUC2_SIZE                                    0x1
#define _WPUC_WPUC2_LENGTH                                  0x1
#define _WPUC_WPUC2_MASK                                    0x4
#define _WPUC_WPUC3_POSN                                    0x3
#define _WPUC_WPUC3_POSITION                                0x3
#define _WPUC_WPUC3_SIZE                                    0x1
#define _WPUC_WPUC3_LENGTH                                  0x1
#define _WPUC_WPUC3_MASK                                    0x8
#define _WPUC_WPUC4_POSN                                    0x4
#define _WPUC_WPUC4_POSITION                                0x4
#define _WPUC_WPUC4_SIZE                                    0x1
#define _WPUC_WPUC4_LENGTH                                  0x1
#define _WPUC_WPUC4_MASK                                    0x10
#define _WPUC_WPUC5_POSN                                    0x5
#define _WPUC_WPUC5_POSITION                                0x5
#define _WPUC_WPUC5_SIZE                                    0x1
#define _WPUC_WPUC5_LENGTH                                  0x1
#define _WPUC_WPUC5_MASK                                    0x20
#define _WPUC_WPUC6_POSN                                    0x6
#define _WPUC_WPUC6_POSITION                                0x6
#define _WPUC_WPUC6_SIZE                                    0x1
#define _WPUC_WPUC6_LENGTH                                  0x1
#define _WPUC_WPUC6_MASK                                    0x40
#define _WPUC_WPUC7_POSN                                    0x7
#define _WPUC_WPUC7_POSITION                                0x7
#define _WPUC_WPUC7_SIZE                                    0x1
#define _WPUC_WPUC7_LENGTH                                  0x1
#define _WPUC_WPUC7_MASK                                    0x80

// Register: SSP1BUF
#define SSP1BUF SSP1BUF
extern volatile unsigned char           SSP1BUF             __at(0x211);
#ifndef _LIB_BUILD
asm("SSP1BUF equ 0211h");
#endif
// aliases
extern volatile unsigned char           SSPBUF              __at(0x211);
#ifndef _LIB_BUILD
asm("SSPBUF equ 0211h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SSP1BUF0               :1;
        unsigned SSP1BUF1               :1;
        unsigned SSP1BUF2               :1;
        unsigned SSP1BUF3               :1;
        unsigned SSP1BUF4               :1;
        unsigned SSP1BUF5               :1;
        unsigned SSP1BUF6               :1;
        unsigned SSP1BUF7               :1;
    };
    struct {
        unsigned BUF                    :8;
    };
    struct {
        unsigned BUF0                   :1;
        unsigned BUF1                   :1;
        unsigned BUF2                   :1;
        unsigned BUF3                   :1;
        unsigned BUF4                   :1;
        unsigned BUF5                   :1;
        unsigned BUF6                   :1;
        unsigned BUF7                   :1;
    };
    struct {
        unsigned SSP1BUF                :8;
    };
} SSP1BUFbits_t;
extern volatile SSP1BUFbits_t SSP1BUFbits __at(0x211);
// bitfield macros
#define _SSP1BUF_SSP1BUF0_POSN                              0x0
#define _SSP1BUF_SSP1BUF0_POSITION                          0x0
#define _SSP1BUF_SSP1BUF0_SIZE                              0x1
#define _SSP1BUF_SSP1BUF0_LENGTH                            0x1
#define _SSP1BUF_SSP1BUF0_MASK                              0x1
#define _SSP1BUF_SSP1BUF1_POSN                              0x1
#define _SSP1BUF_SSP1BUF1_POSITION                          0x1
#define _SSP1BUF_SSP1BUF1_SIZE                              0x1
#define _SSP1BUF_SSP1BUF1_LENGTH                            0x1
#define _SSP1BUF_SSP1BUF1_MASK                              0x2
#define _SSP1BUF_SSP1BUF2_POSN                              0x2
#define _SSP1BUF_SSP1BUF2_POSITION                          0x2
#define _SSP1BUF_SSP1BUF2_SIZE                              0x1
#define _SSP1BUF_SSP1BUF2_LENGTH                            0x1
#define _SSP1BUF_SSP1BUF2_MASK                              0x4
#define _SSP1BUF_SSP1BUF3_POSN                              0x3
#define _SSP1BUF_SSP1BUF3_POSITION                          0x3
#define _SSP1BUF_SSP1BUF3_SIZE                              0x1
#define _SSP1BUF_SSP1BUF3_LENGTH                            0x1
#define _SSP1BUF_SSP1BUF3_MASK                              0x8
#define _SSP1BUF_SSP1BUF4_POSN                              0x4
#define _SSP1BUF_SSP1BUF4_POSITION                          0x4
#define _SSP1BUF_SSP1BUF4_SIZE                              0x1
#define _SSP1BUF_SSP1BUF4_LENGTH                            0x1
#define _SSP1BUF_SSP1BUF4_MASK                              0x10
#define _SSP1BUF_SSP1BUF5_POSN                              0x5
#define _SSP1BUF_SSP1BUF5_POSITION                          0x5
#define _SSP1BUF_SSP1BUF5_SIZE                              0x1
#define _SSP1BUF_SSP1BUF5_LENGTH                            0x1
#define _SSP1BUF_SSP1BUF5_MASK                              0x20
#define _SSP1BUF_SSP1BUF6_POSN                              0x6
#define _SSP1BUF_SSP1BUF6_POSITION                          0x6
#define _SSP1BUF_SSP1BUF6_SIZE                              0x1
#define _SSP1BUF_SSP1BUF6_LENGTH                            0x1
#define _SSP1BUF_SSP1BUF6_MASK                              0x40
#define _SSP1BUF_SSP1BUF7_POSN                              0x7
#define _SSP1BUF_SSP1BUF7_POSITION                          0x7
#define _SSP1BUF_SSP1BUF7_SIZE                              0x1
#define _SSP1BUF_SSP1BUF7_LENGTH                            0x1
#define _SSP1BUF_SSP1BUF7_MASK                              0x80
#define _SSP1BUF_BUF_POSN                                   0x0
#define _SSP1BUF_BUF_POSITION                               0x0
#define _SSP1BUF_BUF_SIZE                                   0x8
#define _SSP1BUF_BUF_LENGTH                                 0x8
#define _SSP1BUF_BUF_MASK                                   0xFF
#define _SSP1BUF_BUF0_POSN                                  0x0
#define _SSP1BUF_BUF0_POSITION                              0x0
#define _SSP1BUF_BUF0_SIZE                                  0x1
#define _SSP1BUF_BUF0_LENGTH                                0x1
#define _SSP1BUF_BUF0_MASK                                  0x1
#define _SSP1BUF_BUF1_POSN                                  0x1
#define _SSP1BUF_BUF1_POSITION                              0x1
#define _SSP1BUF_BUF1_SIZE                                  0x1
#define _SSP1BUF_BUF1_LENGTH                                0x1
#define _SSP1BUF_BUF1_MASK                                  0x2
#define _SSP1BUF_BUF2_POSN                                  0x2
#define _SSP1BUF_BUF2_POSITION                              0x2
#define _SSP1BUF_BUF2_SIZE                                  0x1
#define _SSP1BUF_BUF2_LENGTH                                0x1
#define _SSP1BUF_BUF2_MASK                                  0x4
#define _SSP1BUF_BUF3_POSN                                  0x3
#define _SSP1BUF_BUF3_POSITION                              0x3
#define _SSP1BUF_BUF3_SIZE                                  0x1
#define _SSP1BUF_BUF3_LENGTH                                0x1
#define _SSP1BUF_BUF3_MASK                                  0x8
#define _SSP1BUF_BUF4_POSN                                  0x4
#define _SSP1BUF_BUF4_POSITION                              0x4
#define _SSP1BUF_BUF4_SIZE                                  0x1
#define _SSP1BUF_BUF4_LENGTH                                0x1
#define _SSP1BUF_BUF4_MASK                                  0x10
#define _SSP1BUF_BUF5_POSN                                  0x5
#define _SSP1BUF_BUF5_POSITION                              0x5
#define _SSP1BUF_BUF5_SIZE                                  0x1
#define _SSP1BUF_BUF5_LENGTH                                0x1
#define _SSP1BUF_BUF5_MASK                                  0x20
#define _SSP1BUF_BUF6_POSN                                  0x6
#define _SSP1BUF_BUF6_POSITION                              0x6
#define _SSP1BUF_BUF6_SIZE                                  0x1
#define _SSP1BUF_BUF6_LENGTH                                0x1
#define _SSP1BUF_BUF6_MASK                                  0x40
#define _SSP1BUF_BUF7_POSN                                  0x7
#define _SSP1BUF_BUF7_POSITION                              0x7
#define _SSP1BUF_BUF7_SIZE                                  0x1
#define _SSP1BUF_BUF7_LENGTH                                0x1
#define _SSP1BUF_BUF7_MASK                                  0x80
#define _SSP1BUF_SSP1BUF_POSN                               0x0
#define _SSP1BUF_SSP1BUF_POSITION                           0x0
#define _SSP1BUF_SSP1BUF_SIZE                               0x8
#define _SSP1BUF_SSP1BUF_LENGTH                             0x8
#define _SSP1BUF_SSP1BUF_MASK                               0xFF
// alias bitfield definitions
typedef union {
    struct {
        unsigned SSP1BUF0               :1;
        unsigned SSP1BUF1               :1;
        unsigned SSP1BUF2               :1;
        unsigned SSP1BUF3               :1;
        unsigned SSP1BUF4               :1;
        unsigned SSP1BUF5               :1;
        unsigned SSP1BUF6               :1;
        unsigned SSP1BUF7               :1;
    };
    struct {
        unsigned BUF                    :8;
    };
    struct {
        unsigned BUF0                   :1;
        unsigned BUF1                   :1;
        unsigned BUF2                   :1;
        unsigned BUF3                   :1;
        unsigned BUF4                   :1;
        unsigned BUF5                   :1;
        unsigned BUF6                   :1;
        unsigned BUF7                   :1;
    };
    struct {
        unsigned SSP1BUF                :8;
    };
} SSPBUFbits_t;
extern volatile SSPBUFbits_t SSPBUFbits __at(0x211);
// bitfield macros
#define _SSPBUF_SSP1BUF0_POSN                               0x0
#define _SSPBUF_SSP1BUF0_POSITION                           0x0
#define _SSPBUF_SSP1BUF0_SIZE                               0x1
#define _SSPBUF_SSP1BUF0_LENGTH                             0x1
#define _SSPBUF_SSP1BUF0_MASK                               0x1
#define _SSPBUF_SSP1BUF1_POSN                               0x1
#define _SSPBUF_SSP1BUF1_POSITION                           0x1
#define _SSPBUF_SSP1BUF1_SIZE                               0x1
#define _SSPBUF_SSP1BUF1_LENGTH                             0x1
#define _SSPBUF_SSP1BUF1_MASK                               0x2
#define _SSPBUF_SSP1BUF2_POSN                               0x2
#define _SSPBUF_SSP1BUF2_POSITION                           0x2
#define _SSPBUF_SSP1BUF2_SIZE                               0x1
#define _SSPBUF_SSP1BUF2_LENGTH                             0x1
#define _SSPBUF_SSP1BUF2_MASK                               0x4
#define _SSPBUF_SSP1BUF3_POSN                               0x3
#define _SSPBUF_SSP1BUF3_POSITION                           0x3
#define _SSPBUF_SSP1BUF3_SIZE                               0x1
#define _SSPBUF_SSP1BUF3_LENGTH                             0x1
#define _SSPBUF_SSP1BUF3_MASK                               0x8
#define _SSPBUF_SSP1BUF4_POSN                               0x4
#define _SSPBUF_SSP1BUF4_POSITION                           0x4
#define _SSPBUF_SSP1BUF4_SIZE                               0x1
#define _SSPBUF_SSP1BUF4_LENGTH                             0x1
#define _SSPBUF_SSP1BUF4_MASK                               0x10
#define _SSPBUF_SSP1BUF5_POSN                               0x5
#define _SSPBUF_SSP1BUF5_POSITION                           0x5
#define _SSPBUF_SSP1BUF5_SIZE                               0x1
#define _SSPBUF_SSP1BUF5_LENGTH                             0x1
#define _SSPBUF_SSP1BUF5_MASK                               0x20
#define _SSPBUF_SSP1BUF6_POSN                               0x6
#define _SSPBUF_SSP1BUF6_POSITION                           0x6
#define _SSPBUF_SSP1BUF6_SIZE                               0x1
#define _SSPBUF_SSP1BUF6_LENGTH                             0x1
#define _SSPBUF_SSP1BUF6_MASK                               0x40
#define _SSPBUF_SSP1BUF7_POSN                               0x7
#define _SSPBUF_SSP1BUF7_POSITION                           0x7
#define _SSPBUF_SSP1BUF7_SIZE                               0x1
#define _SSPBUF_SSP1BUF7_LENGTH                             0x1
#define _SSPBUF_SSP1BUF7_MASK                               0x80
#define _SSPBUF_BUF_POSN                                    0x0
#define _SSPBUF_BUF_POSITION                                0x0
#define _SSPBUF_BUF_SIZE                                    0x8
#define _SSPBUF_BUF_LENGTH                                  0x8
#define _SSPBUF_BUF_MASK                                    0xFF
#define _SSPBUF_BUF0_POSN                                   0x0
#define _SSPBUF_BUF0_POSITION                               0x0
#define _SSPBUF_BUF0_SIZE                                   0x1
#define _SSPBUF_BUF0_LENGTH                                 0x1
#define _SSPBUF_BUF0_MASK                                   0x1
#define _SSPBUF_BUF1_POSN                                   0x1
#define _SSPBUF_BUF1_POSITION                               0x1
#define _SSPBUF_BUF1_SIZE                                   0x1
#define _SSPBUF_BUF1_LENGTH                                 0x1
#define _SSPBUF_BUF1_MASK                                   0x2
#define _SSPBUF_BUF2_POSN                                   0x2
#define _SSPBUF_BUF2_POSITION                               0x2
#define _SSPBUF_BUF2_SIZE                                   0x1
#define _SSPBUF_BUF2_LENGTH                                 0x1
#define _SSPBUF_BUF2_MASK                                   0x4
#define _SSPBUF_BUF3_POSN                                   0x3
#define _SSPBUF_BUF3_POSITION                               0x3
#define _SSPBUF_BUF3_SIZE                                   0x1
#define _SSPBUF_BUF3_LENGTH                                 0x1
#define _SSPBUF_BUF3_MASK                                   0x8
#define _SSPBUF_BUF4_POSN                                   0x4
#define _SSPBUF_BUF4_POSITION                               0x4
#define _SSPBUF_BUF4_SIZE                                   0x1
#define _SSPBUF_BUF4_LENGTH                                 0x1
#define _SSPBUF_BUF4_MASK                                   0x10
#define _SSPBUF_BUF5_POSN                                   0x5
#define _SSPBUF_BUF5_POSITION                               0x5
#define _SSPBUF_BUF5_SIZE                                   0x1
#define _SSPBUF_BUF5_LENGTH                                 0x1
#define _SSPBUF_BUF5_MASK                                   0x20
#define _SSPBUF_BUF6_POSN                                   0x6
#define _SSPBUF_BUF6_POSITION                               0x6
#define _SSPBUF_BUF6_SIZE                                   0x1
#define _SSPBUF_BUF6_LENGTH                                 0x1
#define _SSPBUF_BUF6_MASK                                   0x40
#define _SSPBUF_BUF7_POSN                                   0x7
#define _SSPBUF_BUF7_POSITION                               0x7
#define _SSPBUF_BUF7_SIZE                                   0x1
#define _SSPBUF_BUF7_LENGTH                                 0x1
#define _SSPBUF_BUF7_MASK                                   0x80
#define _SSPBUF_SSP1BUF_POSN                                0x0
#define _SSPBUF_SSP1BUF_POSITION                            0x0
#define _SSPBUF_SSP1BUF_SIZE                                0x8
#define _SSPBUF_SSP1BUF_LENGTH                              0x8
#define _SSPBUF_SSP1BUF_MASK                                0xFF

// Register: SSP1ADD
#define SSP1ADD SSP1ADD
extern volatile unsigned char           SSP1ADD             __at(0x212);
#ifndef _LIB_BUILD
asm("SSP1ADD equ 0212h");
#endif
// aliases
extern volatile unsigned char           SSPADD              __at(0x212);
#ifndef _LIB_BUILD
asm("SSPADD equ 0212h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SSP1ADD0               :1;
        unsigned SSP1ADD1               :1;
        unsigned SSP1ADD2               :1;
        unsigned SSP1ADD3               :1;
        unsigned SSP1ADD4               :1;
        unsigned SSP1ADD5               :1;
        unsigned SSP1ADD6               :1;
        unsigned SSP1ADD7               :1;
    };
    struct {
        unsigned ADD                    :8;
    };
    struct {
        unsigned ADD0                   :1;
        unsigned ADD1                   :1;
        unsigned ADD2                   :1;
        unsigned ADD3                   :1;
        unsigned ADD4                   :1;
        unsigned ADD5                   :1;
        unsigned ADD6                   :1;
        unsigned ADD7                   :1;
    };
    struct {
        unsigned SSP1ADD                :8;
    };
} SSP1ADDbits_t;
extern volatile SSP1ADDbits_t SSP1ADDbits __at(0x212);
// bitfield macros
#define _SSP1ADD_SSP1ADD0_POSN                              0x0
#define _SSP1ADD_SSP1ADD0_POSITION                          0x0
#define _SSP1ADD_SSP1ADD0_SIZE                              0x1
#define _SSP1ADD_SSP1ADD0_LENGTH                            0x1
#define _SSP1ADD_SSP1ADD0_MASK                              0x1
#define _SSP1ADD_SSP1ADD1_POSN                              0x1
#define _SSP1ADD_SSP1ADD1_POSITION                          0x1
#define _SSP1ADD_SSP1ADD1_SIZE                              0x1
#define _SSP1ADD_SSP1ADD1_LENGTH                            0x1
#define _SSP1ADD_SSP1ADD1_MASK                              0x2
#define _SSP1ADD_SSP1ADD2_POSN                              0x2
#define _SSP1ADD_SSP1ADD2_POSITION                          0x2
#define _SSP1ADD_SSP1ADD2_SIZE                              0x1
#define _SSP1ADD_SSP1ADD2_LENGTH                            0x1
#define _SSP1ADD_SSP1ADD2_MASK                              0x4
#define _SSP1ADD_SSP1ADD3_POSN                              0x3
#define _SSP1ADD_SSP1ADD3_POSITION                          0x3
#define _SSP1ADD_SSP1ADD3_SIZE                              0x1
#define _SSP1ADD_SSP1ADD3_LENGTH                            0x1
#define _SSP1ADD_SSP1ADD3_MASK                              0x8
#define _SSP1ADD_SSP1ADD4_POSN                              0x4
#define _SSP1ADD_SSP1ADD4_POSITION                          0x4
#define _SSP1ADD_SSP1ADD4_SIZE                              0x1
#define _SSP1ADD_SSP1ADD4_LENGTH                            0x1
#define _SSP1ADD_SSP1ADD4_MASK                              0x10
#define _SSP1ADD_SSP1ADD5_POSN                              0x5
#define _SSP1ADD_SSP1ADD5_POSITION                          0x5
#define _SSP1ADD_SSP1ADD5_SIZE                              0x1
#define _SSP1ADD_SSP1ADD5_LENGTH                            0x1
#define _SSP1ADD_SSP1ADD5_MASK                              0x20
#define _SSP1ADD_SSP1ADD6_POSN                              0x6
#define _SSP1ADD_SSP1ADD6_POSITION                          0x6
#define _SSP1ADD_SSP1ADD6_SIZE                              0x1
#define _SSP1ADD_SSP1ADD6_LENGTH                            0x1
#define _SSP1ADD_SSP1ADD6_MASK                              0x40
#define _SSP1ADD_SSP1ADD7_POSN                              0x7
#define _SSP1ADD_SSP1ADD7_POSITION                          0x7
#define _SSP1ADD_SSP1ADD7_SIZE                              0x1
#define _SSP1ADD_SSP1ADD7_LENGTH                            0x1
#define _SSP1ADD_SSP1ADD7_MASK                              0x80
#define _SSP1ADD_ADD_POSN                                   0x0
#define _SSP1ADD_ADD_POSITION                               0x0
#define _SSP1ADD_ADD_SIZE                                   0x8
#define _SSP1ADD_ADD_LENGTH                                 0x8
#define _SSP1ADD_ADD_MASK                                   0xFF
#define _SSP1ADD_ADD0_POSN                                  0x0
#define _SSP1ADD_ADD0_POSITION                              0x0
#define _SSP1ADD_ADD0_SIZE                                  0x1
#define _SSP1ADD_ADD0_LENGTH                                0x1
#define _SSP1ADD_ADD0_MASK                                  0x1
#define _SSP1ADD_ADD1_POSN                                  0x1
#define _SSP1ADD_ADD1_POSITION                              0x1
#define _SSP1ADD_ADD1_SIZE                                  0x1
#define _SSP1ADD_ADD1_LENGTH                                0x1
#define _SSP1ADD_ADD1_MASK                                  0x2
#define _SSP1ADD_ADD2_POSN                                  0x2
#define _SSP1ADD_ADD2_POSITION                              0x2
#define _SSP1ADD_ADD2_SIZE                                  0x1
#define _SSP1ADD_ADD2_LENGTH                                0x1
#define _SSP1ADD_ADD2_MASK                                  0x4
#define _SSP1ADD_ADD3_POSN                                  0x3
#define _SSP1ADD_ADD3_POSITION                              0x3
#define _SSP1ADD_ADD3_SIZE                                  0x1
#define _SSP1ADD_ADD3_LENGTH                                0x1
#define _SSP1ADD_ADD3_MASK                                  0x8
#define _SSP1ADD_ADD4_POSN                                  0x4
#define _SSP1ADD_ADD4_POSITION                              0x4
#define _SSP1ADD_ADD4_SIZE                                  0x1
#define _SSP1ADD_ADD4_LENGTH                                0x1
#define _SSP1ADD_ADD4_MASK                                  0x10
#define _SSP1ADD_ADD5_POSN                                  0x5
#define _SSP1ADD_ADD5_POSITION                              0x5
#define _SSP1ADD_ADD5_SIZE                                  0x1
#define _SSP1ADD_ADD5_LENGTH                                0x1
#define _SSP1ADD_ADD5_MASK                                  0x20
#define _SSP1ADD_ADD6_POSN                                  0x6
#define _SSP1ADD_ADD6_POSITION                              0x6
#define _SSP1ADD_ADD6_SIZE                                  0x1
#define _SSP1ADD_ADD6_LENGTH                                0x1
#define _SSP1ADD_ADD6_MASK                                  0x40
#define _SSP1ADD_ADD7_POSN                                  0x7
#define _SSP1ADD_ADD7_POSITION                              0x7
#define _SSP1ADD_ADD7_SIZE                                  0x1
#define _SSP1ADD_ADD7_LENGTH                                0x1
#define _SSP1ADD_ADD7_MASK                                  0x80
#define _SSP1ADD_SSP1ADD_POSN                               0x0
#define _SSP1ADD_SSP1ADD_POSITION                           0x0
#define _SSP1ADD_SSP1ADD_SIZE                               0x8
#define _SSP1ADD_SSP1ADD_LENGTH                             0x8
#define _SSP1ADD_SSP1ADD_MASK                               0xFF
// alias bitfield definitions
typedef union {
    struct {
        unsigned SSP1ADD0               :1;
        unsigned SSP1ADD1               :1;
        unsigned SSP1ADD2               :1;
        unsigned SSP1ADD3               :1;
        unsigned SSP1ADD4               :1;
        unsigned SSP1ADD5               :1;
        unsigned SSP1ADD6               :1;
        unsigned SSP1ADD7               :1;
    };
    struct {
        unsigned ADD                    :8;
    };
    struct {
        unsigned ADD0                   :1;
        unsigned ADD1                   :1;
        unsigned ADD2                   :1;
        unsigned ADD3                   :1;
        unsigned ADD4                   :1;
        unsigned ADD5                   :1;
        unsigned ADD6                   :1;
        unsigned ADD7                   :1;
    };
    struct {
        unsigned SSP1ADD                :8;
    };
} SSPADDbits_t;
extern volatile SSPADDbits_t SSPADDbits __at(0x212);
// bitfield macros
#define _SSPADD_SSP1ADD0_POSN                               0x0
#define _SSPADD_SSP1ADD0_POSITION                           0x0
#define _SSPADD_SSP1ADD0_SIZE                               0x1
#define _SSPADD_SSP1ADD0_LENGTH                             0x1
#define _SSPADD_SSP1ADD0_MASK                               0x1
#define _SSPADD_SSP1ADD1_POSN                               0x1
#define _SSPADD_SSP1ADD1_POSITION                           0x1
#define _SSPADD_SSP1ADD1_SIZE                               0x1
#define _SSPADD_SSP1ADD1_LENGTH                             0x1
#define _SSPADD_SSP1ADD1_MASK                               0x2
#define _SSPADD_SSP1ADD2_POSN                               0x2
#define _SSPADD_SSP1ADD2_POSITION                           0x2
#define _SSPADD_SSP1ADD2_SIZE                               0x1
#define _SSPADD_SSP1ADD2_LENGTH                             0x1
#define _SSPADD_SSP1ADD2_MASK                               0x4
#define _SSPADD_SSP1ADD3_POSN                               0x3
#define _SSPADD_SSP1ADD3_POSITION                           0x3
#define _SSPADD_SSP1ADD3_SIZE                               0x1
#define _SSPADD_SSP1ADD3_LENGTH                             0x1
#define _SSPADD_SSP1ADD3_MASK                               0x8
#define _SSPADD_SSP1ADD4_POSN                               0x4
#define _SSPADD_SSP1ADD4_POSITION                           0x4
#define _SSPADD_SSP1ADD4_SIZE                               0x1
#define _SSPADD_SSP1ADD4_LENGTH                             0x1
#define _SSPADD_SSP1ADD4_MASK                               0x10
#define _SSPADD_SSP1ADD5_POSN                               0x5
#define _SSPADD_SSP1ADD5_POSITION                           0x5
#define _SSPADD_SSP1ADD5_SIZE                               0x1
#define _SSPADD_SSP1ADD5_LENGTH                             0x1
#define _SSPADD_SSP1ADD5_MASK                               0x20
#define _SSPADD_SSP1ADD6_POSN                               0x6
#define _SSPADD_SSP1ADD6_POSITION                           0x6
#define _SSPADD_SSP1ADD6_SIZE                               0x1
#define _SSPADD_SSP1ADD6_LENGTH                             0x1
#define _SSPADD_SSP1ADD6_MASK                               0x40
#define _SSPADD_SSP1ADD7_POSN                               0x7
#define _SSPADD_SSP1ADD7_POSITION                           0x7
#define _SSPADD_SSP1ADD7_SIZE                               0x1
#define _SSPADD_SSP1ADD7_LENGTH                             0x1
#define _SSPADD_SSP1ADD7_MASK                               0x80
#define _SSPADD_ADD_POSN                                    0x0
#define _SSPADD_ADD_POSITION                                0x0
#define _SSPADD_ADD_SIZE                                    0x8
#define _SSPADD_ADD_LENGTH                                  0x8
#define _SSPADD_ADD_MASK                                    0xFF
#define _SSPADD_ADD0_POSN                                   0x0
#define _SSPADD_ADD0_POSITION                               0x0
#define _SSPADD_ADD0_SIZE                                   0x1
#define _SSPADD_ADD0_LENGTH                                 0x1
#define _SSPADD_ADD0_MASK                                   0x1
#define _SSPADD_ADD1_POSN                                   0x1
#define _SSPADD_ADD1_POSITION                               0x1
#define _SSPADD_ADD1_SIZE                                   0x1
#define _SSPADD_ADD1_LENGTH                                 0x1
#define _SSPADD_ADD1_MASK                                   0x2
#define _SSPADD_ADD2_POSN                                   0x2
#define _SSPADD_ADD2_POSITION                               0x2
#define _SSPADD_ADD2_SIZE                                   0x1
#define _SSPADD_ADD2_LENGTH                                 0x1
#define _SSPADD_ADD2_MASK                                   0x4
#define _SSPADD_ADD3_POSN                                   0x3
#define _SSPADD_ADD3_POSITION                               0x3
#define _SSPADD_ADD3_SIZE                                   0x1
#define _SSPADD_ADD3_LENGTH                                 0x1
#define _SSPADD_ADD3_MASK                                   0x8
#define _SSPADD_ADD4_POSN                                   0x4
#define _SSPADD_ADD4_POSITION                               0x4
#define _SSPADD_ADD4_SIZE                                   0x1
#define _SSPADD_ADD4_LENGTH                                 0x1
#define _SSPADD_ADD4_MASK                                   0x10
#define _SSPADD_ADD5_POSN                                   0x5
#define _SSPADD_ADD5_POSITION                               0x5
#define _SSPADD_ADD5_SIZE                                   0x1
#define _SSPADD_ADD5_LENGTH                                 0x1
#define _SSPADD_ADD5_MASK                                   0x20
#define _SSPADD_ADD6_POSN                                   0x6
#define _SSPADD_ADD6_POSITION                               0x6
#define _SSPADD_ADD6_SIZE                                   0x1
#define _SSPADD_ADD6_LENGTH                                 0x1
#define _SSPADD_ADD6_MASK                                   0x40
#define _SSPADD_ADD7_POSN                                   0x7
#define _SSPADD_ADD7_POSITION                               0x7
#define _SSPADD_ADD7_SIZE                                   0x1
#define _SSPADD_ADD7_LENGTH                                 0x1
#define _SSPADD_ADD7_MASK                                   0x80
#define _SSPADD_SSP1ADD_POSN                                0x0
#define _SSPADD_SSP1ADD_POSITION                            0x0
#define _SSPADD_SSP1ADD_SIZE                                0x8
#define _SSPADD_SSP1ADD_LENGTH                              0x8
#define _SSPADD_SSP1ADD_MASK                                0xFF

// Register: SSP1MSK
#define SSP1MSK SSP1MSK
extern volatile unsigned char           SSP1MSK             __at(0x213);
#ifndef _LIB_BUILD
asm("SSP1MSK equ 0213h");
#endif
// aliases
extern volatile unsigned char           SSPMSK              __at(0x213);
#ifndef _LIB_BUILD
asm("SSPMSK equ 0213h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SSP1MSK0               :1;
        unsigned SSP1MSK1               :1;
        unsigned SSP1MSK2               :1;
        unsigned SSP1MSK3               :1;
        unsigned SSP1MSK4               :1;
        unsigned SSP1MSK5               :1;
        unsigned SSP1MSK6               :1;
        unsigned SSP1MSK7               :1;
    };
    struct {
        unsigned MSK                    :8;
    };
    struct {
        unsigned MSK0                   :1;
        unsigned MSK1                   :1;
        unsigned MSK2                   :1;
        unsigned MSK3                   :1;
        unsigned MSK4                   :1;
        unsigned MSK5                   :1;
        unsigned MSK6                   :1;
        unsigned MSK7                   :1;
    };
    struct {
        unsigned SSP1MSK                :8;
    };
} SSP1MSKbits_t;
extern volatile SSP1MSKbits_t SSP1MSKbits __at(0x213);
// bitfield macros
#define _SSP1MSK_SSP1MSK0_POSN                              0x0
#define _SSP1MSK_SSP1MSK0_POSITION                          0x0
#define _SSP1MSK_SSP1MSK0_SIZE                              0x1
#define _SSP1MSK_SSP1MSK0_LENGTH                            0x1
#define _SSP1MSK_SSP1MSK0_MASK                              0x1
#define _SSP1MSK_SSP1MSK1_POSN                              0x1
#define _SSP1MSK_SSP1MSK1_POSITION                          0x1
#define _SSP1MSK_SSP1MSK1_SIZE                              0x1
#define _SSP1MSK_SSP1MSK1_LENGTH                            0x1
#define _SSP1MSK_SSP1MSK1_MASK                              0x2
#define _SSP1MSK_SSP1MSK2_POSN                              0x2
#define _SSP1MSK_SSP1MSK2_POSITION                          0x2
#define _SSP1MSK_SSP1MSK2_SIZE                              0x1
#define _SSP1MSK_SSP1MSK2_LENGTH                            0x1
#define _SSP1MSK_SSP1MSK2_MASK                              0x4
#define _SSP1MSK_SSP1MSK3_POSN                              0x3
#define _SSP1MSK_SSP1MSK3_POSITION                          0x3
#define _SSP1MSK_SSP1MSK3_SIZE                              0x1
#define _SSP1MSK_SSP1MSK3_LENGTH                            0x1
#define _SSP1MSK_SSP1MSK3_MASK                              0x8
#define _SSP1MSK_SSP1MSK4_POSN                              0x4
#define _SSP1MSK_SSP1MSK4_POSITION                          0x4
#define _SSP1MSK_SSP1MSK4_SIZE                              0x1
#define _SSP1MSK_SSP1MSK4_LENGTH                            0x1
#define _SSP1MSK_SSP1MSK4_MASK                              0x10
#define _SSP1MSK_SSP1MSK5_POSN                              0x5
#define _SSP1MSK_SSP1MSK5_POSITION                          0x5
#define _SSP1MSK_SSP1MSK5_SIZE                              0x1
#define _SSP1MSK_SSP1MSK5_LENGTH                            0x1
#define _SSP1MSK_SSP1MSK5_MASK                              0x20
#define _SSP1MSK_SSP1MSK6_POSN                              0x6
#define _SSP1MSK_SSP1MSK6_POSITION                          0x6
#define _SSP1MSK_SSP1MSK6_SIZE                              0x1
#define _SSP1MSK_SSP1MSK6_LENGTH                            0x1
#define _SSP1MSK_SSP1MSK6_MASK                              0x40
#define _SSP1MSK_SSP1MSK7_POSN                              0x7
#define _SSP1MSK_SSP1MSK7_POSITION                          0x7
#define _SSP1MSK_SSP1MSK7_SIZE                              0x1
#define _SSP1MSK_SSP1MSK7_LENGTH                            0x1
#define _SSP1MSK_SSP1MSK7_MASK                              0x80
#define _SSP1MSK_MSK_POSN                                   0x0
#define _SSP1MSK_MSK_POSITION                               0x0
#define _SSP1MSK_MSK_SIZE                                   0x8
#define _SSP1MSK_MSK_LENGTH                                 0x8
#define _SSP1MSK_MSK_MASK                                   0xFF
#define _SSP1MSK_MSK0_POSN                                  0x0
#define _SSP1MSK_MSK0_POSITION                              0x0
#define _SSP1MSK_MSK0_SIZE                                  0x1
#define _SSP1MSK_MSK0_LENGTH                                0x1
#define _SSP1MSK_MSK0_MASK                                  0x1
#define _SSP1MSK_MSK1_POSN                                  0x1
#define _SSP1MSK_MSK1_POSITION                              0x1
#define _SSP1MSK_MSK1_SIZE                                  0x1
#define _SSP1MSK_MSK1_LENGTH                                0x1
#define _SSP1MSK_MSK1_MASK                                  0x2
#define _SSP1MSK_MSK2_POSN                                  0x2
#define _SSP1MSK_MSK2_POSITION                              0x2
#define _SSP1MSK_MSK2_SIZE                                  0x1
#define _SSP1MSK_MSK2_LENGTH                                0x1
#define _SSP1MSK_MSK2_MASK                                  0x4
#define _SSP1MSK_MSK3_POSN                                  0x3
#define _SSP1MSK_MSK3_POSITION                              0x3
#define _SSP1MSK_MSK3_SIZE                                  0x1
#define _SSP1MSK_MSK3_LENGTH                                0x1
#define _SSP1MSK_MSK3_MASK                                  0x8
#define _SSP1MSK_MSK4_POSN                                  0x4
#define _SSP1MSK_MSK4_POSITION                              0x4
#define _SSP1MSK_MSK4_SIZE                                  0x1
#define _SSP1MSK_MSK4_LENGTH                                0x1
#define _SSP1MSK_MSK4_MASK                                  0x10
#define _SSP1MSK_MSK5_POSN                                  0x5
#define _SSP1MSK_MSK5_POSITION                              0x5
#define _SSP1MSK_MSK5_SIZE                                  0x1
#define _SSP1MSK_MSK5_LENGTH                                0x1
#define _SSP1MSK_MSK5_MASK                                  0x20
#define _SSP1MSK_MSK6_POSN                                  0x6
#define _SSP1MSK_MSK6_POSITION                              0x6
#define _SSP1MSK_MSK6_SIZE                                  0x1
#define _SSP1MSK_MSK6_LENGTH                                0x1
#define _SSP1MSK_MSK6_MASK                                  0x40
#define _SSP1MSK_MSK7_POSN                                  0x7
#define _SSP1MSK_MSK7_POSITION                              0x7
#define _SSP1MSK_MSK7_SIZE                                  0x1
#define _SSP1MSK_MSK7_LENGTH                                0x1
#define _SSP1MSK_MSK7_MASK                                  0x80
#define _SSP1MSK_SSP1MSK_POSN                               0x0
#define _SSP1MSK_SSP1MSK_POSITION                           0x0
#define _SSP1MSK_SSP1MSK_SIZE                               0x8
#define _SSP1MSK_SSP1MSK_LENGTH                             0x8
#define _SSP1MSK_SSP1MSK_MASK                               0xFF
// alias bitfield definitions
typedef union {
    struct {
        unsigned SSP1MSK0               :1;
        unsigned SSP1MSK1               :1;
        unsigned SSP1MSK2               :1;
        unsigned SSP1MSK3               :1;
        unsigned SSP1MSK4               :1;
        unsigned SSP1MSK5               :1;
        unsigned SSP1MSK6               :1;
        unsigned SSP1MSK7               :1;
    };
    struct {
        unsigned MSK                    :8;
    };
    struct {
        unsigned MSK0                   :1;
        unsigned MSK1                   :1;
        unsigned MSK2                   :1;
        unsigned MSK3                   :1;
        unsigned MSK4                   :1;
        unsigned MSK5                   :1;
        unsigned MSK6                   :1;
        unsigned MSK7                   :1;
    };
    struct {
        unsigned SSP1MSK                :8;
    };
} SSPMSKbits_t;
extern volatile SSPMSKbits_t SSPMSKbits __at(0x213);
// bitfield macros
#define _SSPMSK_SSP1MSK0_POSN                               0x0
#define _SSPMSK_SSP1MSK0_POSITION                           0x0
#define _SSPMSK_SSP1MSK0_SIZE                               0x1
#define _SSPMSK_SSP1MSK0_LENGTH                             0x1
#define _SSPMSK_SSP1MSK0_MASK                               0x1
#define _SSPMSK_SSP1MSK1_POSN                               0x1
#define _SSPMSK_SSP1MSK1_POSITION                           0x1
#define _SSPMSK_SSP1MSK1_SIZE                               0x1
#define _SSPMSK_SSP1MSK1_LENGTH                             0x1
#define _SSPMSK_SSP1MSK1_MASK                               0x2
#define _SSPMSK_SSP1MSK2_POSN                               0x2
#define _SSPMSK_SSP1MSK2_POSITION                           0x2
#define _SSPMSK_SSP1MSK2_SIZE                               0x1
#define _SSPMSK_SSP1MSK2_LENGTH                             0x1
#define _SSPMSK_SSP1MSK2_MASK                               0x4
#define _SSPMSK_SSP1MSK3_POSN                               0x3
#define _SSPMSK_SSP1MSK3_POSITION                           0x3
#define _SSPMSK_SSP1MSK3_SIZE                               0x1
#define _SSPMSK_SSP1MSK3_LENGTH                             0x1
#define _SSPMSK_SSP1MSK3_MASK                               0x8
#define _SSPMSK_SSP1MSK4_POSN                               0x4
#define _SSPMSK_SSP1MSK4_POSITION                           0x4
#define _SSPMSK_SSP1MSK4_SIZE                               0x1
#define _SSPMSK_SSP1MSK4_LENGTH                             0x1
#define _SSPMSK_SSP1MSK4_MASK                               0x10
#define _SSPMSK_SSP1MSK5_POSN                               0x5
#define _SSPMSK_SSP1MSK5_POSITION                           0x5
#define _SSPMSK_SSP1MSK5_SIZE                               0x1
#define _SSPMSK_SSP1MSK5_LENGTH                             0x1
#define _SSPMSK_SSP1MSK5_MASK                               0x20
#define _SSPMSK_SSP1MSK6_POSN                               0x6
#define _SSPMSK_SSP1MSK6_POSITION                           0x6
#define _SSPMSK_SSP1MSK6_SIZE                               0x1
#define _SSPMSK_SSP1MSK6_LENGTH                             0x1
#define _SSPMSK_SSP1MSK6_MASK                               0x40
#define _SSPMSK_SSP1MSK7_POSN                               0x7
#define _SSPMSK_SSP1MSK7_POSITION                           0x7
#define _SSPMSK_SSP1MSK7_SIZE                               0x1
#define _SSPMSK_SSP1MSK7_LENGTH                             0x1
#define _SSPMSK_SSP1MSK7_MASK                               0x80
#define _SSPMSK_MSK_POSN                                    0x0
#define _SSPMSK_MSK_POSITION                                0x0
#define _SSPMSK_MSK_SIZE                                    0x8
#define _SSPMSK_MSK_LENGTH                                  0x8
#define _SSPMSK_MSK_MASK                                    0xFF
#define _SSPMSK_MSK0_POSN                                   0x0
#define _SSPMSK_MSK0_POSITION                               0x0
#define _SSPMSK_MSK0_SIZE                                   0x1
#define _SSPMSK_MSK0_LENGTH                                 0x1
#define _SSPMSK_MSK0_MASK                                   0x1
#define _SSPMSK_MSK1_POSN                                   0x1
#define _SSPMSK_MSK1_POSITION                               0x1
#define _SSPMSK_MSK1_SIZE                                   0x1
#define _SSPMSK_MSK1_LENGTH                                 0x1
#define _SSPMSK_MSK1_MASK                                   0x2
#define _SSPMSK_MSK2_POSN                                   0x2
#define _SSPMSK_MSK2_POSITION                               0x2
#define _SSPMSK_MSK2_SIZE                                   0x1
#define _SSPMSK_MSK2_LENGTH                                 0x1
#define _SSPMSK_MSK2_MASK                                   0x4
#define _SSPMSK_MSK3_POSN                                   0x3
#define _SSPMSK_MSK3_POSITION                               0x3
#define _SSPMSK_MSK3_SIZE                                   0x1
#define _SSPMSK_MSK3_LENGTH                                 0x1
#define _SSPMSK_MSK3_MASK                                   0x8
#define _SSPMSK_MSK4_POSN                                   0x4
#define _SSPMSK_MSK4_POSITION                               0x4
#define _SSPMSK_MSK4_SIZE                                   0x1
#define _SSPMSK_MSK4_LENGTH                                 0x1
#define _SSPMSK_MSK4_MASK                                   0x10
#define _SSPMSK_MSK5_POSN                                   0x5
#define _SSPMSK_MSK5_POSITION                               0x5
#define _SSPMSK_MSK5_SIZE                                   0x1
#define _SSPMSK_MSK5_LENGTH                                 0x1
#define _SSPMSK_MSK5_MASK                                   0x20
#define _SSPMSK_MSK6_POSN                                   0x6
#define _SSPMSK_MSK6_POSITION                               0x6
#define _SSPMSK_MSK6_SIZE                                   0x1
#define _SSPMSK_MSK6_LENGTH                                 0x1
#define _SSPMSK_MSK6_MASK                                   0x40
#define _SSPMSK_MSK7_POSN                                   0x7
#define _SSPMSK_MSK7_POSITION                               0x7
#define _SSPMSK_MSK7_SIZE                                   0x1
#define _SSPMSK_MSK7_LENGTH                                 0x1
#define _SSPMSK_MSK7_MASK                                   0x80
#define _SSPMSK_SSP1MSK_POSN                                0x0
#define _SSPMSK_SSP1MSK_POSITION                            0x0
#define _SSPMSK_SSP1MSK_SIZE                                0x8
#define _SSPMSK_SSP1MSK_LENGTH                              0x8
#define _SSPMSK_SSP1MSK_MASK                                0xFF

// Register: SSP1STAT
#define SSP1STAT SSP1STAT
extern volatile unsigned char           SSP1STAT            __at(0x214);
#ifndef _LIB_BUILD
asm("SSP1STAT equ 0214h");
#endif
// aliases
extern volatile unsigned char           SSPSTAT             __at(0x214);
#ifndef _LIB_BUILD
asm("SSPSTAT equ 0214h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned BF                     :1;
        unsigned UA                     :1;
        unsigned R_nW                   :1;
        unsigned S                      :1;
        unsigned P                      :1;
        unsigned D_nA                   :1;
        unsigned CKE                    :1;
        unsigned SMP                    :1;
    };
} SSP1STATbits_t;
extern volatile SSP1STATbits_t SSP1STATbits __at(0x214);
// bitfield macros
#define _SSP1STAT_BF_POSN                                   0x0
#define _SSP1STAT_BF_POSITION                               0x0
#define _SSP1STAT_BF_SIZE                                   0x1
#define _SSP1STAT_BF_LENGTH                                 0x1
#define _SSP1STAT_BF_MASK                                   0x1
#define _SSP1STAT_UA_POSN                                   0x1
#define _SSP1STAT_UA_POSITION                               0x1
#define _SSP1STAT_UA_SIZE                                   0x1
#define _SSP1STAT_UA_LENGTH                                 0x1
#define _SSP1STAT_UA_MASK                                   0x2
#define _SSP1STAT_R_nW_POSN                                 0x2
#define _SSP1STAT_R_nW_POSITION                             0x2
#define _SSP1STAT_R_nW_SIZE                                 0x1
#define _SSP1STAT_R_nW_LENGTH                               0x1
#define _SSP1STAT_R_nW_MASK                                 0x4
#define _SSP1STAT_S_POSN                                    0x3
#define _SSP1STAT_S_POSITION                                0x3
#define _SSP1STAT_S_SIZE                                    0x1
#define _SSP1STAT_S_LENGTH                                  0x1
#define _SSP1STAT_S_MASK                                    0x8
#define _SSP1STAT_P_POSN                                    0x4
#define _SSP1STAT_P_POSITION                                0x4
#define _SSP1STAT_P_SIZE                                    0x1
#define _SSP1STAT_P_LENGTH                                  0x1
#define _SSP1STAT_P_MASK                                    0x10
#define _SSP1STAT_D_nA_POSN                                 0x5
#define _SSP1STAT_D_nA_POSITION                             0x5
#define _SSP1STAT_D_nA_SIZE                                 0x1
#define _SSP1STAT_D_nA_LENGTH                               0x1
#define _SSP1STAT_D_nA_MASK                                 0x20
#define _SSP1STAT_CKE_POSN                                  0x6
#define _SSP1STAT_CKE_POSITION                              0x6
#define _SSP1STAT_CKE_SIZE                                  0x1
#define _SSP1STAT_CKE_LENGTH                                0x1
#define _SSP1STAT_CKE_MASK                                  0x40
#define _SSP1STAT_SMP_POSN                                  0x7
#define _SSP1STAT_SMP_POSITION                              0x7
#define _SSP1STAT_SMP_SIZE                                  0x1
#define _SSP1STAT_SMP_LENGTH                                0x1
#define _SSP1STAT_SMP_MASK                                  0x80
// alias bitfield definitions
typedef union {
    struct {
        unsigned BF                     :1;
        unsigned UA                     :1;
        unsigned R_nW                   :1;
        unsigned S                      :1;
        unsigned P                      :1;
        unsigned D_nA                   :1;
        unsigned CKE                    :1;
        unsigned SMP                    :1;
    };
} SSPSTATbits_t;
extern volatile SSPSTATbits_t SSPSTATbits __at(0x214);
// bitfield macros
#define _SSPSTAT_BF_POSN                                    0x0
#define _SSPSTAT_BF_POSITION                                0x0
#define _SSPSTAT_BF_SIZE                                    0x1
#define _SSPSTAT_BF_LENGTH                                  0x1
#define _SSPSTAT_BF_MASK                                    0x1
#define _SSPSTAT_UA_POSN                                    0x1
#define _SSPSTAT_UA_POSITION                                0x1
#define _SSPSTAT_UA_SIZE                                    0x1
#define _SSPSTAT_UA_LENGTH                                  0x1
#define _SSPSTAT_UA_MASK                                    0x2
#define _SSPSTAT_R_nW_POSN                                  0x2
#define _SSPSTAT_R_nW_POSITION                              0x2
#define _SSPSTAT_R_nW_SIZE                                  0x1
#define _SSPSTAT_R_nW_LENGTH                                0x1
#define _SSPSTAT_R_nW_MASK                                  0x4
#define _SSPSTAT_S_POSN                                     0x3
#define _SSPSTAT_S_POSITION                                 0x3
#define _SSPSTAT_S_SIZE                                     0x1
#define _SSPSTAT_S_LENGTH                                   0x1
#define _SSPSTAT_S_MASK                                     0x8
#define _SSPSTAT_P_POSN                                     0x4
#define _SSPSTAT_P_POSITION                                 0x4
#define _SSPSTAT_P_SIZE                                     0x1
#define _SSPSTAT_P_LENGTH                                   0x1
#define _SSPSTAT_P_MASK                                     0x10
#define _SSPSTAT_D_nA_POSN                                  0x5
#define _SSPSTAT_D_nA_POSITION                              0x5
#define _SSPSTAT_D_nA_SIZE                                  0x1
#define _SSPSTAT_D_nA_LENGTH                                0x1
#define _SSPSTAT_D_nA_MASK                                  0x20
#define _SSPSTAT_CKE_POSN                                   0x6
#define _SSPSTAT_CKE_POSITION                               0x6
#define _SSPSTAT_CKE_SIZE                                   0x1
#define _SSPSTAT_CKE_LENGTH                                 0x1
#define _SSPSTAT_CKE_MASK                                   0x40
#define _SSPSTAT_SMP_POSN                                   0x7
#define _SSPSTAT_SMP_POSITION                               0x7
#define _SSPSTAT_SMP_SIZE                                   0x1
#define _SSPSTAT_SMP_LENGTH                                 0x1
#define _SSPSTAT_SMP_MASK                                   0x80

// Register: SSP1CON1
#define SSP1CON1 SSP1CON1
extern volatile unsigned char           SSP1CON1            __at(0x215);
#ifndef _LIB_BUILD
asm("SSP1CON1 equ 0215h");
#endif
// aliases
extern volatile unsigned char           SSPCON              __at(0x215);
#ifndef _LIB_BUILD
asm("SSPCON equ 0215h");
#endif
extern volatile unsigned char           SSPCON1             __at(0x215);
#ifndef _LIB_BUILD
asm("SSPCON1 equ 0215h");
#endif
extern volatile unsigned char           SSP1CON             __at(0x215);
#ifndef _LIB_BUILD
asm("SSP1CON equ 0215h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SSPM                   :4;
        unsigned CKP                    :1;
        unsigned SSPEN                  :1;
        unsigned SSPOV                  :1;
        unsigned WCOL                   :1;
    };
    struct {
        unsigned SSPM0                  :1;
        unsigned SSPM1                  :1;
        unsigned SSPM2                  :1;
        unsigned SSPM3                  :1;
    };
} SSP1CON1bits_t;
extern volatile SSP1CON1bits_t SSP1CON1bits __at(0x215);
// bitfield macros
#define _SSP1CON1_SSPM_POSN                                 0x0
#define _SSP1CON1_SSPM_POSITION                             0x0
#define _SSP1CON1_SSPM_SIZE                                 0x4
#define _SSP1CON1_SSPM_LENGTH                               0x4
#define _SSP1CON1_SSPM_MASK                                 0xF
#define _SSP1CON1_CKP_POSN                                  0x4
#define _SSP1CON1_CKP_POSITION                              0x4
#define _SSP1CON1_CKP_SIZE                                  0x1
#define _SSP1CON1_CKP_LENGTH                                0x1
#define _SSP1CON1_CKP_MASK                                  0x10
#define _SSP1CON1_SSPEN_POSN                                0x5
#define _SSP1CON1_SSPEN_POSITION                            0x5
#define _SSP1CON1_SSPEN_SIZE                                0x1
#define _SSP1CON1_SSPEN_LENGTH                              0x1
#define _SSP1CON1_SSPEN_MASK                                0x20
#define _SSP1CON1_SSPOV_POSN                                0x6
#define _SSP1CON1_SSPOV_POSITION                            0x6
#define _SSP1CON1_SSPOV_SIZE                                0x1
#define _SSP1CON1_SSPOV_LENGTH                              0x1
#define _SSP1CON1_SSPOV_MASK                                0x40
#define _SSP1CON1_WCOL_POSN                                 0x7
#define _SSP1CON1_WCOL_POSITION                             0x7
#define _SSP1CON1_WCOL_SIZE                                 0x1
#define _SSP1CON1_WCOL_LENGTH                               0x1
#define _SSP1CON1_WCOL_MASK                                 0x80
#define _SSP1CON1_SSPM0_POSN                                0x0
#define _SSP1CON1_SSPM0_POSITION                            0x0
#define _SSP1CON1_SSPM0_SIZE                                0x1
#define _SSP1CON1_SSPM0_LENGTH                              0x1
#define _SSP1CON1_SSPM0_MASK                                0x1
#define _SSP1CON1_SSPM1_POSN                                0x1
#define _SSP1CON1_SSPM1_POSITION                            0x1
#define _SSP1CON1_SSPM1_SIZE                                0x1
#define _SSP1CON1_SSPM1_LENGTH                              0x1
#define _SSP1CON1_SSPM1_MASK                                0x2
#define _SSP1CON1_SSPM2_POSN                                0x2
#define _SSP1CON1_SSPM2_POSITION                            0x2
#define _SSP1CON1_SSPM2_SIZE                                0x1
#define _SSP1CON1_SSPM2_LENGTH                              0x1
#define _SSP1CON1_SSPM2_MASK                                0x4
#define _SSP1CON1_SSPM3_POSN                                0x3
#define _SSP1CON1_SSPM3_POSITION                            0x3
#define _SSP1CON1_SSPM3_SIZE                                0x1
#define _SSP1CON1_SSPM3_LENGTH                              0x1
#define _SSP1CON1_SSPM3_MASK                                0x8
// alias bitfield definitions
typedef union {
    struct {
        unsigned SSPM                   :4;
        unsigned CKP                    :1;
        unsigned SSPEN                  :1;
        unsigned SSPOV                  :1;
        unsigned WCOL                   :1;
    };
    struct {
        unsigned SSPM0                  :1;
        unsigned SSPM1                  :1;
        unsigned SSPM2                  :1;
        unsigned SSPM3                  :1;
    };
} SSPCONbits_t;
extern volatile SSPCONbits_t SSPCONbits __at(0x215);
// bitfield macros
#define _SSPCON_SSPM_POSN                                   0x0
#define _SSPCON_SSPM_POSITION                               0x0
#define _SSPCON_SSPM_SIZE                                   0x4
#define _SSPCON_SSPM_LENGTH                                 0x4
#define _SSPCON_SSPM_MASK                                   0xF
#define _SSPCON_CKP_POSN                                    0x4
#define _SSPCON_CKP_POSITION                                0x4
#define _SSPCON_CKP_SIZE                                    0x1
#define _SSPCON_CKP_LENGTH                                  0x1
#define _SSPCON_CKP_MASK                                    0x10
#define _SSPCON_SSPEN_POSN                                  0x5
#define _SSPCON_SSPEN_POSITION                              0x5
#define _SSPCON_SSPEN_SIZE                                  0x1
#define _SSPCON_SSPEN_LENGTH                                0x1
#define _SSPCON_SSPEN_MASK                                  0x20
#define _SSPCON_SSPOV_POSN                                  0x6
#define _SSPCON_SSPOV_POSITION                              0x6
#define _SSPCON_SSPOV_SIZE                                  0x1
#define _SSPCON_SSPOV_LENGTH                                0x1
#define _SSPCON_SSPOV_MASK                                  0x40
#define _SSPCON_WCOL_POSN                                   0x7
#define _SSPCON_WCOL_POSITION                               0x7
#define _SSPCON_WCOL_SIZE                                   0x1
#define _SSPCON_WCOL_LENGTH                                 0x1
#define _SSPCON_WCOL_MASK                                   0x80
#define _SSPCON_SSPM0_POSN                                  0x0
#define _SSPCON_SSPM0_POSITION                              0x0
#define _SSPCON_SSPM0_SIZE                                  0x1
#define _SSPCON_SSPM0_LENGTH                                0x1
#define _SSPCON_SSPM0_MASK                                  0x1
#define _SSPCON_SSPM1_POSN                                  0x1
#define _SSPCON_SSPM1_POSITION                              0x1
#define _SSPCON_SSPM1_SIZE                                  0x1
#define _SSPCON_SSPM1_LENGTH                                0x1
#define _SSPCON_SSPM1_MASK                                  0x2
#define _SSPCON_SSPM2_POSN                                  0x2
#define _SSPCON_SSPM2_POSITION                              0x2
#define _SSPCON_SSPM2_SIZE                                  0x1
#define _SSPCON_SSPM2_LENGTH                                0x1
#define _SSPCON_SSPM2_MASK                                  0x4
#define _SSPCON_SSPM3_POSN                                  0x3
#define _SSPCON_SSPM3_POSITION                              0x3
#define _SSPCON_SSPM3_SIZE                                  0x1
#define _SSPCON_SSPM3_LENGTH                                0x1
#define _SSPCON_SSPM3_MASK                                  0x8
typedef union {
    struct {
        unsigned SSPM                   :4;
        unsigned CKP                    :1;
        unsigned SSPEN                  :1;
        unsigned SSPOV                  :1;
        unsigned WCOL                   :1;
    };
    struct {
        unsigned SSPM0                  :1;
        unsigned SSPM1                  :1;
        unsigned SSPM2                  :1;
        unsigned SSPM3                  :1;
    };
} SSPCON1bits_t;
extern volatile SSPCON1bits_t SSPCON1bits __at(0x215);
// bitfield macros
#define _SSPCON1_SSPM_POSN                                  0x0
#define _SSPCON1_SSPM_POSITION                              0x0
#define _SSPCON1_SSPM_SIZE                                  0x4
#define _SSPCON1_SSPM_LENGTH                                0x4
#define _SSPCON1_SSPM_MASK                                  0xF
#define _SSPCON1_CKP_POSN                                   0x4
#define _SSPCON1_CKP_POSITION                               0x4
#define _SSPCON1_CKP_SIZE                                   0x1
#define _SSPCON1_CKP_LENGTH                                 0x1
#define _SSPCON1_CKP_MASK                                   0x10
#define _SSPCON1_SSPEN_POSN                                 0x5
#define _SSPCON1_SSPEN_POSITION                             0x5
#define _SSPCON1_SSPEN_SIZE                                 0x1
#define _SSPCON1_SSPEN_LENGTH                               0x1
#define _SSPCON1_SSPEN_MASK                                 0x20
#define _SSPCON1_SSPOV_POSN                                 0x6
#define _SSPCON1_SSPOV_POSITION                             0x6
#define _SSPCON1_SSPOV_SIZE                                 0x1
#define _SSPCON1_SSPOV_LENGTH                               0x1
#define _SSPCON1_SSPOV_MASK                                 0x40
#define _SSPCON1_WCOL_POSN                                  0x7
#define _SSPCON1_WCOL_POSITION                              0x7
#define _SSPCON1_WCOL_SIZE                                  0x1
#define _SSPCON1_WCOL_LENGTH                                0x1
#define _SSPCON1_WCOL_MASK                                  0x80
#define _SSPCON1_SSPM0_POSN                                 0x0
#define _SSPCON1_SSPM0_POSITION                             0x0
#define _SSPCON1_SSPM0_SIZE                                 0x1
#define _SSPCON1_SSPM0_LENGTH                               0x1
#define _SSPCON1_SSPM0_MASK                                 0x1
#define _SSPCON1_SSPM1_POSN                                 0x1
#define _SSPCON1_SSPM1_POSITION                             0x1
#define _SSPCON1_SSPM1_SIZE                                 0x1
#define _SSPCON1_SSPM1_LENGTH                               0x1
#define _SSPCON1_SSPM1_MASK                                 0x2
#define _SSPCON1_SSPM2_POSN                                 0x2
#define _SSPCON1_SSPM2_POSITION                             0x2
#define _SSPCON1_SSPM2_SIZE                                 0x1
#define _SSPCON1_SSPM2_LENGTH                               0x1
#define _SSPCON1_SSPM2_MASK                                 0x4
#define _SSPCON1_SSPM3_POSN                                 0x3
#define _SSPCON1_SSPM3_POSITION                             0x3
#define _SSPCON1_SSPM3_SIZE                                 0x1
#define _SSPCON1_SSPM3_LENGTH                               0x1
#define _SSPCON1_SSPM3_MASK                                 0x8
typedef union {
    struct {
        unsigned SSPM                   :4;
        unsigned CKP                    :1;
        unsigned SSPEN                  :1;
        unsigned SSPOV                  :1;
        unsigned WCOL                   :1;
    };
    struct {
        unsigned SSPM0                  :1;
        unsigned SSPM1                  :1;
        unsigned SSPM2                  :1;
        unsigned SSPM3                  :1;
    };
} SSP1CONbits_t;
extern volatile SSP1CONbits_t SSP1CONbits __at(0x215);
// bitfield macros
#define _SSP1CON_SSPM_POSN                                  0x0
#define _SSP1CON_SSPM_POSITION                              0x0
#define _SSP1CON_SSPM_SIZE                                  0x4
#define _SSP1CON_SSPM_LENGTH                                0x4
#define _SSP1CON_SSPM_MASK                                  0xF
#define _SSP1CON_CKP_POSN                                   0x4
#define _SSP1CON_CKP_POSITION                               0x4
#define _SSP1CON_CKP_SIZE                                   0x1
#define _SSP1CON_CKP_LENGTH                                 0x1
#define _SSP1CON_CKP_MASK                                   0x10
#define _SSP1CON_SSPEN_POSN                                 0x5
#define _SSP1CON_SSPEN_POSITION                             0x5
#define _SSP1CON_SSPEN_SIZE                                 0x1
#define _SSP1CON_SSPEN_LENGTH                               0x1
#define _SSP1CON_SSPEN_MASK                                 0x20
#define _SSP1CON_SSPOV_POSN                                 0x6
#define _SSP1CON_SSPOV_POSITION                             0x6
#define _SSP1CON_SSPOV_SIZE                                 0x1
#define _SSP1CON_SSPOV_LENGTH                               0x1
#define _SSP1CON_SSPOV_MASK                                 0x40
#define _SSP1CON_WCOL_POSN                                  0x7
#define _SSP1CON_WCOL_POSITION                              0x7
#define _SSP1CON_WCOL_SIZE                                  0x1
#define _SSP1CON_WCOL_LENGTH                                0x1
#define _SSP1CON_WCOL_MASK                                  0x80
#define _SSP1CON_SSPM0_POSN                                 0x0
#define _SSP1CON_SSPM0_POSITION                             0x0
#define _SSP1CON_SSPM0_SIZE                                 0x1
#define _SSP1CON_SSPM0_LENGTH                               0x1
#define _SSP1CON_SSPM0_MASK                                 0x1
#define _SSP1CON_SSPM1_POSN                                 0x1
#define _SSP1CON_SSPM1_POSITION                             0x1
#define _SSP1CON_SSPM1_SIZE                                 0x1
#define _SSP1CON_SSPM1_LENGTH                               0x1
#define _SSP1CON_SSPM1_MASK                                 0x2
#define _SSP1CON_SSPM2_POSN                                 0x2
#define _SSP1CON_SSPM2_POSITION                             0x2
#define _SSP1CON_SSPM2_SIZE                                 0x1
#define _SSP1CON_SSPM2_LENGTH                               0x1
#define _SSP1CON_SSPM2_MASK                                 0x4
#define _SSP1CON_SSPM3_POSN                                 0x3
#define _SSP1CON_SSPM3_POSITION                             0x3
#define _SSP1CON_SSPM3_SIZE                                 0x1
#define _SSP1CON_SSPM3_LENGTH                               0x1
#define _SSP1CON_SSPM3_MASK                                 0x8

// Register: SSP1CON2
#define SSP1CON2 SSP1CON2
extern volatile unsigned char           SSP1CON2            __at(0x216);
#ifndef _LIB_BUILD
asm("SSP1CON2 equ 0216h");
#endif
// aliases
extern volatile unsigned char           SSPCON2             __at(0x216);
#ifndef _LIB_BUILD
asm("SSPCON2 equ 0216h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SEN                    :1;
        unsigned RSEN                   :1;
        unsigned PEN                    :1;
        unsigned RCEN                   :1;
        unsigned ACKEN                  :1;
        unsigned ACKDT                  :1;
        unsigned ACKSTAT                :1;
        unsigned GCEN                   :1;
    };
} SSP1CON2bits_t;
extern volatile SSP1CON2bits_t SSP1CON2bits __at(0x216);
// bitfield macros
#define _SSP1CON2_SEN_POSN                                  0x0
#define _SSP1CON2_SEN_POSITION                              0x0
#define _SSP1CON2_SEN_SIZE                                  0x1
#define _SSP1CON2_SEN_LENGTH                                0x1
#define _SSP1CON2_SEN_MASK                                  0x1
#define _SSP1CON2_RSEN_POSN                                 0x1
#define _SSP1CON2_RSEN_POSITION                             0x1
#define _SSP1CON2_RSEN_SIZE                                 0x1
#define _SSP1CON2_RSEN_LENGTH                               0x1
#define _SSP1CON2_RSEN_MASK                                 0x2
#define _SSP1CON2_PEN_POSN                                  0x2
#define _SSP1CON2_PEN_POSITION                              0x2
#define _SSP1CON2_PEN_SIZE                                  0x1
#define _SSP1CON2_PEN_LENGTH                                0x1
#define _SSP1CON2_PEN_MASK                                  0x4
#define _SSP1CON2_RCEN_POSN                                 0x3
#define _SSP1CON2_RCEN_POSITION                             0x3
#define _SSP1CON2_RCEN_SIZE                                 0x1
#define _SSP1CON2_RCEN_LENGTH                               0x1
#define _SSP1CON2_RCEN_MASK                                 0x8
#define _SSP1CON2_ACKEN_POSN                                0x4
#define _SSP1CON2_ACKEN_POSITION                            0x4
#define _SSP1CON2_ACKEN_SIZE                                0x1
#define _SSP1CON2_ACKEN_LENGTH                              0x1
#define _SSP1CON2_ACKEN_MASK                                0x10
#define _SSP1CON2_ACKDT_POSN                                0x5
#define _SSP1CON2_ACKDT_POSITION                            0x5
#define _SSP1CON2_ACKDT_SIZE                                0x1
#define _SSP1CON2_ACKDT_LENGTH                              0x1
#define _SSP1CON2_ACKDT_MASK                                0x20
#define _SSP1CON2_ACKSTAT_POSN                              0x6
#define _SSP1CON2_ACKSTAT_POSITION                          0x6
#define _SSP1CON2_ACKSTAT_SIZE                              0x1
#define _SSP1CON2_ACKSTAT_LENGTH                            0x1
#define _SSP1CON2_ACKSTAT_MASK                              0x40
#define _SSP1CON2_GCEN_POSN                                 0x7
#define _SSP1CON2_GCEN_POSITION                             0x7
#define _SSP1CON2_GCEN_SIZE                                 0x1
#define _SSP1CON2_GCEN_LENGTH                               0x1
#define _SSP1CON2_GCEN_MASK                                 0x80
// alias bitfield definitions
typedef union {
    struct {
        unsigned SEN                    :1;
        unsigned RSEN                   :1;
        unsigned PEN                    :1;
        unsigned RCEN                   :1;
        unsigned ACKEN                  :1;
        unsigned ACKDT                  :1;
        unsigned ACKSTAT                :1;
        unsigned GCEN                   :1;
    };
} SSPCON2bits_t;
extern volatile SSPCON2bits_t SSPCON2bits __at(0x216);
// bitfield macros
#define _SSPCON2_SEN_POSN                                   0x0
#define _SSPCON2_SEN_POSITION                               0x0
#define _SSPCON2_SEN_SIZE                                   0x1
#define _SSPCON2_SEN_LENGTH                                 0x1
#define _SSPCON2_SEN_MASK                                   0x1
#define _SSPCON2_RSEN_POSN                                  0x1
#define _SSPCON2_RSEN_POSITION                              0x1
#define _SSPCON2_RSEN_SIZE                                  0x1
#define _SSPCON2_RSEN_LENGTH                                0x1
#define _SSPCON2_RSEN_MASK                                  0x2
#define _SSPCON2_PEN_POSN                                   0x2
#define _SSPCON2_PEN_POSITION                               0x2
#define _SSPCON2_PEN_SIZE                                   0x1
#define _SSPCON2_PEN_LENGTH                                 0x1
#define _SSPCON2_PEN_MASK                                   0x4
#define _SSPCON2_RCEN_POSN                                  0x3
#define _SSPCON2_RCEN_POSITION                              0x3
#define _SSPCON2_RCEN_SIZE                                  0x1
#define _SSPCON2_RCEN_LENGTH                                0x1
#define _SSPCON2_RCEN_MASK                                  0x8
#define _SSPCON2_ACKEN_POSN                                 0x4
#define _SSPCON2_ACKEN_POSITION                             0x4
#define _SSPCON2_ACKEN_SIZE                                 0x1
#define _SSPCON2_ACKEN_LENGTH                               0x1
#define _SSPCON2_ACKEN_MASK                                 0x10
#define _SSPCON2_ACKDT_POSN                                 0x5
#define _SSPCON2_ACKDT_POSITION                             0x5
#define _SSPCON2_ACKDT_SIZE                                 0x1
#define _SSPCON2_ACKDT_LENGTH                               0x1
#define _SSPCON2_ACKDT_MASK                                 0x20
#define _SSPCON2_ACKSTAT_POSN                               0x6
#define _SSPCON2_ACKSTAT_POSITION                           0x6
#define _SSPCON2_ACKSTAT_SIZE                               0x1
#define _SSPCON2_ACKSTAT_LENGTH                             0x1
#define _SSPCON2_ACKSTAT_MASK                               0x40
#define _SSPCON2_GCEN_POSN                                  0x7
#define _SSPCON2_GCEN_POSITION                              0x7
#define _SSPCON2_GCEN_SIZE                                  0x1
#define _SSPCON2_GCEN_LENGTH                                0x1
#define _SSPCON2_GCEN_MASK                                  0x80

// Register: SSP1CON3
#define SSP1CON3 SSP1CON3
extern volatile unsigned char           SSP1CON3            __at(0x217);
#ifndef _LIB_BUILD
asm("SSP1CON3 equ 0217h");
#endif
// aliases
extern volatile unsigned char           SSPCON3             __at(0x217);
#ifndef _LIB_BUILD
asm("SSPCON3 equ 0217h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned DHEN                   :1;
        unsigned AHEN                   :1;
        unsigned SBCDE                  :1;
        unsigned SDAHT                  :1;
        unsigned BOEN                   :1;
        unsigned SCIE                   :1;
        unsigned PCIE                   :1;
        unsigned ACKTIM                 :1;
    };
} SSP1CON3bits_t;
extern volatile SSP1CON3bits_t SSP1CON3bits __at(0x217);
// bitfield macros
#define _SSP1CON3_DHEN_POSN                                 0x0
#define _SSP1CON3_DHEN_POSITION                             0x0
#define _SSP1CON3_DHEN_SIZE                                 0x1
#define _SSP1CON3_DHEN_LENGTH                               0x1
#define _SSP1CON3_DHEN_MASK                                 0x1
#define _SSP1CON3_AHEN_POSN                                 0x1
#define _SSP1CON3_AHEN_POSITION                             0x1
#define _SSP1CON3_AHEN_SIZE                                 0x1
#define _SSP1CON3_AHEN_LENGTH                               0x1
#define _SSP1CON3_AHEN_MASK                                 0x2
#define _SSP1CON3_SBCDE_POSN                                0x2
#define _SSP1CON3_SBCDE_POSITION                            0x2
#define _SSP1CON3_SBCDE_SIZE                                0x1
#define _SSP1CON3_SBCDE_LENGTH                              0x1
#define _SSP1CON3_SBCDE_MASK                                0x4
#define _SSP1CON3_SDAHT_POSN                                0x3
#define _SSP1CON3_SDAHT_POSITION                            0x3
#define _SSP1CON3_SDAHT_SIZE                                0x1
#define _SSP1CON3_SDAHT_LENGTH                              0x1
#define _SSP1CON3_SDAHT_MASK                                0x8
#define _SSP1CON3_BOEN_POSN                                 0x4
#define _SSP1CON3_BOEN_POSITION                             0x4
#define _SSP1CON3_BOEN_SIZE                                 0x1
#define _SSP1CON3_BOEN_LENGTH                               0x1
#define _SSP1CON3_BOEN_MASK                                 0x10
#define _SSP1CON3_SCIE_POSN                                 0x5
#define _SSP1CON3_SCIE_POSITION                             0x5
#define _SSP1CON3_SCIE_SIZE                                 0x1
#define _SSP1CON3_SCIE_LENGTH                               0x1
#define _SSP1CON3_SCIE_MASK                                 0x20
#define _SSP1CON3_PCIE_POSN                                 0x6
#define _SSP1CON3_PCIE_POSITION                             0x6
#define _SSP1CON3_PCIE_SIZE                                 0x1
#define _SSP1CON3_PCIE_LENGTH                               0x1
#define _SSP1CON3_PCIE_MASK                                 0x40
#define _SSP1CON3_ACKTIM_POSN                               0x7
#define _SSP1CON3_ACKTIM_POSITION                           0x7
#define _SSP1CON3_ACKTIM_SIZE                               0x1
#define _SSP1CON3_ACKTIM_LENGTH                             0x1
#define _SSP1CON3_ACKTIM_MASK                               0x80
// alias bitfield definitions
typedef union {
    struct {
        unsigned DHEN                   :1;
        unsigned AHEN                   :1;
        unsigned SBCDE                  :1;
        unsigned SDAHT                  :1;
        unsigned BOEN                   :1;
        unsigned SCIE                   :1;
        unsigned PCIE                   :1;
        unsigned ACKTIM                 :1;
    };
} SSPCON3bits_t;
extern volatile SSPCON3bits_t SSPCON3bits __at(0x217);
// bitfield macros
#define _SSPCON3_DHEN_POSN                                  0x0
#define _SSPCON3_DHEN_POSITION                              0x0
#define _SSPCON3_DHEN_SIZE                                  0x1
#define _SSPCON3_DHEN_LENGTH                                0x1
#define _SSPCON3_DHEN_MASK                                  0x1
#define _SSPCON3_AHEN_POSN                                  0x1
#define _SSPCON3_AHEN_POSITION                              0x1
#define _SSPCON3_AHEN_SIZE                                  0x1
#define _SSPCON3_AHEN_LENGTH                                0x1
#define _SSPCON3_AHEN_MASK                                  0x2
#define _SSPCON3_SBCDE_POSN                                 0x2
#define _SSPCON3_SBCDE_POSITION                             0x2
#define _SSPCON3_SBCDE_SIZE                                 0x1
#define _SSPCON3_SBCDE_LENGTH                               0x1
#define _SSPCON3_SBCDE_MASK                                 0x4
#define _SSPCON3_SDAHT_POSN                                 0x3
#define _SSPCON3_SDAHT_POSITION                             0x3
#define _SSPCON3_SDAHT_SIZE                                 0x1
#define _SSPCON3_SDAHT_LENGTH                               0x1
#define _SSPCON3_SDAHT_MASK                                 0x8
#define _SSPCON3_BOEN_POSN                                  0x4
#define _SSPCON3_BOEN_POSITION                              0x4
#define _SSPCON3_BOEN_SIZE                                  0x1
#define _SSPCON3_BOEN_LENGTH                                0x1
#define _SSPCON3_BOEN_MASK                                  0x10
#define _SSPCON3_SCIE_POSN                                  0x5
#define _SSPCON3_SCIE_POSITION                              0x5
#define _SSPCON3_SCIE_SIZE                                  0x1
#define _SSPCON3_SCIE_LENGTH                                0x1
#define _SSPCON3_SCIE_MASK                                  0x20
#define _SSPCON3_PCIE_POSN                                  0x6
#define _SSPCON3_PCIE_POSITION                              0x6
#define _SSPCON3_PCIE_SIZE                                  0x1
#define _SSPCON3_PCIE_LENGTH                                0x1
#define _SSPCON3_PCIE_MASK                                  0x40
#define _SSPCON3_ACKTIM_POSN                                0x7
#define _SSPCON3_ACKTIM_POSITION                            0x7
#define _SSPCON3_ACKTIM_SIZE                                0x1
#define _SSPCON3_ACKTIM_LENGTH                              0x1
#define _SSPCON3_ACKTIM_MASK                                0x80

// Register: ODCONA
#define ODCONA ODCONA
extern volatile unsigned char           ODCONA              __at(0x28C);
#ifndef _LIB_BUILD
asm("ODCONA equ 028Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned ODA0                   :1;
        unsigned ODA1                   :1;
        unsigned ODA2                   :1;
        unsigned                        :1;
        unsigned ODA4                   :1;
        unsigned ODA5                   :1;
    };
} ODCONAbits_t;
extern volatile ODCONAbits_t ODCONAbits __at(0x28C);
// bitfield macros
#define _ODCONA_ODA0_POSN                                   0x0
#define _ODCONA_ODA0_POSITION                               0x0
#define _ODCONA_ODA0_SIZE                                   0x1
#define _ODCONA_ODA0_LENGTH                                 0x1
#define _ODCONA_ODA0_MASK                                   0x1
#define _ODCONA_ODA1_POSN                                   0x1
#define _ODCONA_ODA1_POSITION                               0x1
#define _ODCONA_ODA1_SIZE                                   0x1
#define _ODCONA_ODA1_LENGTH                                 0x1
#define _ODCONA_ODA1_MASK                                   0x2
#define _ODCONA_ODA2_POSN                                   0x2
#define _ODCONA_ODA2_POSITION                               0x2
#define _ODCONA_ODA2_SIZE                                   0x1
#define _ODCONA_ODA2_LENGTH                                 0x1
#define _ODCONA_ODA2_MASK                                   0x4
#define _ODCONA_ODA4_POSN                                   0x4
#define _ODCONA_ODA4_POSITION                               0x4
#define _ODCONA_ODA4_SIZE                                   0x1
#define _ODCONA_ODA4_LENGTH                                 0x1
#define _ODCONA_ODA4_MASK                                   0x10
#define _ODCONA_ODA5_POSN                                   0x5
#define _ODCONA_ODA5_POSITION                               0x5
#define _ODCONA_ODA5_SIZE                                   0x1
#define _ODCONA_ODA5_LENGTH                                 0x1
#define _ODCONA_ODA5_MASK                                   0x20

// Register: ODCONB
#define ODCONB ODCONB
extern volatile unsigned char           ODCONB              __at(0x28D);
#ifndef _LIB_BUILD
asm("ODCONB equ 028Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned ODB4                   :1;
        unsigned ODB5                   :1;
        unsigned ODB6                   :1;
        unsigned ODB7                   :1;
    };
} ODCONBbits_t;
extern volatile ODCONBbits_t ODCONBbits __at(0x28D);
// bitfield macros
#define _ODCONB_ODB4_POSN                                   0x4
#define _ODCONB_ODB4_POSITION                               0x4
#define _ODCONB_ODB4_SIZE                                   0x1
#define _ODCONB_ODB4_LENGTH                                 0x1
#define _ODCONB_ODB4_MASK                                   0x10
#define _ODCONB_ODB5_POSN                                   0x5
#define _ODCONB_ODB5_POSITION                               0x5
#define _ODCONB_ODB5_SIZE                                   0x1
#define _ODCONB_ODB5_LENGTH                                 0x1
#define _ODCONB_ODB5_MASK                                   0x20
#define _ODCONB_ODB6_POSN                                   0x6
#define _ODCONB_ODB6_POSITION                               0x6
#define _ODCONB_ODB6_SIZE                                   0x1
#define _ODCONB_ODB6_LENGTH                                 0x1
#define _ODCONB_ODB6_MASK                                   0x40
#define _ODCONB_ODB7_POSN                                   0x7
#define _ODCONB_ODB7_POSITION                               0x7
#define _ODCONB_ODB7_SIZE                                   0x1
#define _ODCONB_ODB7_LENGTH                                 0x1
#define _ODCONB_ODB7_MASK                                   0x80

// Register: ODCONC
#define ODCONC ODCONC
extern volatile unsigned char           ODCONC              __at(0x28E);
#ifndef _LIB_BUILD
asm("ODCONC equ 028Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned ODC0                   :1;
        unsigned ODC1                   :1;
        unsigned ODC2                   :1;
        unsigned ODC3                   :1;
        unsigned ODC4                   :1;
        unsigned ODC5                   :1;
        unsigned ODC6                   :1;
        unsigned ODC7                   :1;
    };
} ODCONCbits_t;
extern volatile ODCONCbits_t ODCONCbits __at(0x28E);
// bitfield macros
#define _ODCONC_ODC0_POSN                                   0x0
#define _ODCONC_ODC0_POSITION                               0x0
#define _ODCONC_ODC0_SIZE                                   0x1
#define _ODCONC_ODC0_LENGTH                                 0x1
#define _ODCONC_ODC0_MASK                                   0x1
#define _ODCONC_ODC1_POSN                                   0x1
#define _ODCONC_ODC1_POSITION                               0x1
#define _ODCONC_ODC1_SIZE                                   0x1
#define _ODCONC_ODC1_LENGTH                                 0x1
#define _ODCONC_ODC1_MASK                                   0x2
#define _ODCONC_ODC2_POSN                                   0x2
#define _ODCONC_ODC2_POSITION                               0x2
#define _ODCONC_ODC2_SIZE                                   0x1
#define _ODCONC_ODC2_LENGTH                                 0x1
#define _ODCONC_ODC2_MASK                                   0x4
#define _ODCONC_ODC3_POSN                                   0x3
#define _ODCONC_ODC3_POSITION                               0x3
#define _ODCONC_ODC3_SIZE                                   0x1
#define _ODCONC_ODC3_LENGTH                                 0x1
#define _ODCONC_ODC3_MASK                                   0x8
#define _ODCONC_ODC4_POSN                                   0x4
#define _ODCONC_ODC4_POSITION                               0x4
#define _ODCONC_ODC4_SIZE                                   0x1
#define _ODCONC_ODC4_LENGTH                                 0x1
#define _ODCONC_ODC4_MASK                                   0x10
#define _ODCONC_ODC5_POSN                                   0x5
#define _ODCONC_ODC5_POSITION                               0x5
#define _ODCONC_ODC5_SIZE                                   0x1
#define _ODCONC_ODC5_LENGTH                                 0x1
#define _ODCONC_ODC5_MASK                                   0x20
#define _ODCONC_ODC6_POSN                                   0x6
#define _ODCONC_ODC6_POSITION                               0x6
#define _ODCONC_ODC6_SIZE                                   0x1
#define _ODCONC_ODC6_LENGTH                                 0x1
#define _ODCONC_ODC6_MASK                                   0x40
#define _ODCONC_ODC7_POSN                                   0x7
#define _ODCONC_ODC7_POSITION                               0x7
#define _ODCONC_ODC7_SIZE                                   0x1
#define _ODCONC_ODC7_LENGTH                                 0x1
#define _ODCONC_ODC7_MASK                                   0x80

// Register: CCPR1
#define CCPR1 CCPR1
extern volatile unsigned short          CCPR1               __at(0x291);
#ifndef _LIB_BUILD
asm("CCPR1 equ 0291h");
#endif

// Register: CCPR1L
#define CCPR1L CCPR1L
extern volatile unsigned char           CCPR1L              __at(0x291);
#ifndef _LIB_BUILD
asm("CCPR1L equ 0291h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CCPR1L                 :8;
    };
} CCPR1Lbits_t;
extern volatile CCPR1Lbits_t CCPR1Lbits __at(0x291);
// bitfield macros
#define _CCPR1L_CCPR1L_POSN                                 0x0
#define _CCPR1L_CCPR1L_POSITION                             0x0
#define _CCPR1L_CCPR1L_SIZE                                 0x8
#define _CCPR1L_CCPR1L_LENGTH                               0x8
#define _CCPR1L_CCPR1L_MASK                                 0xFF

// Register: CCPR1H
#define CCPR1H CCPR1H
extern volatile unsigned char           CCPR1H              __at(0x292);
#ifndef _LIB_BUILD
asm("CCPR1H equ 0292h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CCPR1H                 :8;
    };
} CCPR1Hbits_t;
extern volatile CCPR1Hbits_t CCPR1Hbits __at(0x292);
// bitfield macros
#define _CCPR1H_CCPR1H_POSN                                 0x0
#define _CCPR1H_CCPR1H_POSITION                             0x0
#define _CCPR1H_CCPR1H_SIZE                                 0x8
#define _CCPR1H_CCPR1H_LENGTH                               0x8
#define _CCPR1H_CCPR1H_MASK                                 0xFF

// Register: CCP1CON
#define CCP1CON CCP1CON
extern volatile unsigned char           CCP1CON             __at(0x293);
#ifndef _LIB_BUILD
asm("CCP1CON equ 0293h");
#endif
// aliases
extern volatile unsigned char           ECCP1CON            __at(0x293);
#ifndef _LIB_BUILD
asm("ECCP1CON equ 0293h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CCP1M                  :4;
        unsigned DC1B                   :2;
    };
    struct {
        unsigned CCP1M0                 :1;
        unsigned CCP1M1                 :1;
        unsigned CCP1M2                 :1;
        unsigned CCP1M3                 :1;
        unsigned DC1B0                  :1;
        unsigned DC1B1                  :1;
    };
    struct {
        unsigned                        :4;
        unsigned CCP1Y                  :1;
        unsigned CCP1X                  :1;
    };
} CCP1CONbits_t;
extern volatile CCP1CONbits_t CCP1CONbits __at(0x293);
// bitfield macros
#define _CCP1CON_CCP1M_POSN                                 0x0
#define _CCP1CON_CCP1M_POSITION                             0x0
#define _CCP1CON_CCP1M_SIZE                                 0x4
#define _CCP1CON_CCP1M_LENGTH                               0x4
#define _CCP1CON_CCP1M_MASK                                 0xF
#define _CCP1CON_DC1B_POSN                                  0x4
#define _CCP1CON_DC1B_POSITION                              0x4
#define _CCP1CON_DC1B_SIZE                                  0x2
#define _CCP1CON_DC1B_LENGTH                                0x2
#define _CCP1CON_DC1B_MASK                                  0x30
#define _CCP1CON_CCP1M0_POSN                                0x0
#define _CCP1CON_CCP1M0_POSITION                            0x0
#define _CCP1CON_CCP1M0_SIZE                                0x1
#define _CCP1CON_CCP1M0_LENGTH                              0x1
#define _CCP1CON_CCP1M0_MASK                                0x1
#define _CCP1CON_CCP1M1_POSN                                0x1
#define _CCP1CON_CCP1M1_POSITION                            0x1
#define _CCP1CON_CCP1M1_SIZE                                0x1
#define _CCP1CON_CCP1M1_LENGTH                              0x1
#define _CCP1CON_CCP1M1_MASK                                0x2
#define _CCP1CON_CCP1M2_POSN                                0x2
#define _CCP1CON_CCP1M2_POSITION                            0x2
#define _CCP1CON_CCP1M2_SIZE                                0x1
#define _CCP1CON_CCP1M2_LENGTH                              0x1
#define _CCP1CON_CCP1M2_MASK                                0x4
#define _CCP1CON_CCP1M3_POSN                                0x3
#define _CCP1CON_CCP1M3_POSITION                            0x3
#define _CCP1CON_CCP1M3_SIZE                                0x1
#define _CCP1CON_CCP1M3_LENGTH                              0x1
#define _CCP1CON_CCP1M3_MASK                                0x8
#define _CCP1CON_DC1B0_POSN                                 0x4
#define _CCP1CON_DC1B0_POSITION                             0x4
#define _CCP1CON_DC1B0_SIZE                                 0x1
#define _CCP1CON_DC1B0_LENGTH                               0x1
#define _CCP1CON_DC1B0_MASK                                 0x10
#define _CCP1CON_DC1B1_POSN                                 0x5
#define _CCP1CON_DC1B1_POSITION                             0x5
#define _CCP1CON_DC1B1_SIZE                                 0x1
#define _CCP1CON_DC1B1_LENGTH                               0x1
#define _CCP1CON_DC1B1_MASK                                 0x20
#define _CCP1CON_CCP1Y_POSN                                 0x4
#define _CCP1CON_CCP1Y_POSITION                             0x4
#define _CCP1CON_CCP1Y_SIZE                                 0x1
#define _CCP1CON_CCP1Y_LENGTH                               0x1
#define _CCP1CON_CCP1Y_MASK                                 0x10
#define _CCP1CON_CCP1X_POSN                                 0x5
#define _CCP1CON_CCP1X_POSITION                             0x5
#define _CCP1CON_CCP1X_SIZE                                 0x1
#define _CCP1CON_CCP1X_LENGTH                               0x1
#define _CCP1CON_CCP1X_MASK                                 0x20
// alias bitfield definitions
typedef union {
    struct {
        unsigned CCP1M                  :4;
        unsigned DC1B                   :2;
    };
    struct {
        unsigned CCP1M0                 :1;
        unsigned CCP1M1                 :1;
        unsigned CCP1M2                 :1;
        unsigned CCP1M3                 :1;
        unsigned DC1B0                  :1;
        unsigned DC1B1                  :1;
    };
    struct {
        unsigned                        :4;
        unsigned CCP1Y                  :1;
        unsigned CCP1X                  :1;
    };
} ECCP1CONbits_t;
extern volatile ECCP1CONbits_t ECCP1CONbits __at(0x293);
// bitfield macros
#define _ECCP1CON_CCP1M_POSN                                0x0
#define _ECCP1CON_CCP1M_POSITION                            0x0
#define _ECCP1CON_CCP1M_SIZE                                0x4
#define _ECCP1CON_CCP1M_LENGTH                              0x4
#define _ECCP1CON_CCP1M_MASK                                0xF
#define _ECCP1CON_DC1B_POSN                                 0x4
#define _ECCP1CON_DC1B_POSITION                             0x4
#define _ECCP1CON_DC1B_SIZE                                 0x2
#define _ECCP1CON_DC1B_LENGTH                               0x2
#define _ECCP1CON_DC1B_MASK                                 0x30
#define _ECCP1CON_CCP1M0_POSN                               0x0
#define _ECCP1CON_CCP1M0_POSITION                           0x0
#define _ECCP1CON_CCP1M0_SIZE                               0x1
#define _ECCP1CON_CCP1M0_LENGTH                             0x1
#define _ECCP1CON_CCP1M0_MASK                               0x1
#define _ECCP1CON_CCP1M1_POSN                               0x1
#define _ECCP1CON_CCP1M1_POSITION                           0x1
#define _ECCP1CON_CCP1M1_SIZE                               0x1
#define _ECCP1CON_CCP1M1_LENGTH                             0x1
#define _ECCP1CON_CCP1M1_MASK                               0x2
#define _ECCP1CON_CCP1M2_POSN                               0x2
#define _ECCP1CON_CCP1M2_POSITION                           0x2
#define _ECCP1CON_CCP1M2_SIZE                               0x1
#define _ECCP1CON_CCP1M2_LENGTH                             0x1
#define _ECCP1CON_CCP1M2_MASK                               0x4
#define _ECCP1CON_CCP1M3_POSN                               0x3
#define _ECCP1CON_CCP1M3_POSITION                           0x3
#define _ECCP1CON_CCP1M3_SIZE                               0x1
#define _ECCP1CON_CCP1M3_LENGTH                             0x1
#define _ECCP1CON_CCP1M3_MASK                               0x8
#define _ECCP1CON_DC1B0_POSN                                0x4
#define _ECCP1CON_DC1B0_POSITION                            0x4
#define _ECCP1CON_DC1B0_SIZE                                0x1
#define _ECCP1CON_DC1B0_LENGTH                              0x1
#define _ECCP1CON_DC1B0_MASK                                0x10
#define _ECCP1CON_DC1B1_POSN                                0x5
#define _ECCP1CON_DC1B1_POSITION                            0x5
#define _ECCP1CON_DC1B1_SIZE                                0x1
#define _ECCP1CON_DC1B1_LENGTH                              0x1
#define _ECCP1CON_DC1B1_MASK                                0x20
#define _ECCP1CON_CCP1Y_POSN                                0x4
#define _ECCP1CON_CCP1Y_POSITION                            0x4
#define _ECCP1CON_CCP1Y_SIZE                                0x1
#define _ECCP1CON_CCP1Y_LENGTH                              0x1
#define _ECCP1CON_CCP1Y_MASK                                0x10
#define _ECCP1CON_CCP1X_POSN                                0x5
#define _ECCP1CON_CCP1X_POSITION                            0x5
#define _ECCP1CON_CCP1X_SIZE                                0x1
#define _ECCP1CON_CCP1X_LENGTH                              0x1
#define _ECCP1CON_CCP1X_MASK                                0x20

// Register: CCPR2
#define CCPR2 CCPR2
extern volatile unsigned short          CCPR2               __at(0x298);
#ifndef _LIB_BUILD
asm("CCPR2 equ 0298h");
#endif

// Register: CCPR2L
#define CCPR2L CCPR2L
extern volatile unsigned char           CCPR2L              __at(0x298);
#ifndef _LIB_BUILD
asm("CCPR2L equ 0298h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CCPR2L                 :8;
    };
} CCPR2Lbits_t;
extern volatile CCPR2Lbits_t CCPR2Lbits __at(0x298);
// bitfield macros
#define _CCPR2L_CCPR2L_POSN                                 0x0
#define _CCPR2L_CCPR2L_POSITION                             0x0
#define _CCPR2L_CCPR2L_SIZE                                 0x8
#define _CCPR2L_CCPR2L_LENGTH                               0x8
#define _CCPR2L_CCPR2L_MASK                                 0xFF

// Register: CCPR2H
#define CCPR2H CCPR2H
extern volatile unsigned char           CCPR2H              __at(0x299);
#ifndef _LIB_BUILD
asm("CCPR2H equ 0299h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CCPR2H                 :8;
    };
} CCPR2Hbits_t;
extern volatile CCPR2Hbits_t CCPR2Hbits __at(0x299);
// bitfield macros
#define _CCPR2H_CCPR2H_POSN                                 0x0
#define _CCPR2H_CCPR2H_POSITION                             0x0
#define _CCPR2H_CCPR2H_SIZE                                 0x8
#define _CCPR2H_CCPR2H_LENGTH                               0x8
#define _CCPR2H_CCPR2H_MASK                                 0xFF

// Register: CCP2CON
#define CCP2CON CCP2CON
extern volatile unsigned char           CCP2CON             __at(0x29A);
#ifndef _LIB_BUILD
asm("CCP2CON equ 029Ah");
#endif
// aliases
extern volatile unsigned char           ECCP2CON            __at(0x29A);
#ifndef _LIB_BUILD
asm("ECCP2CON equ 029Ah");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CCP2M                  :4;
        unsigned DC2B                   :2;
    };
    struct {
        unsigned CCP2M0                 :1;
        unsigned CCP2M1                 :1;
        unsigned CCP2M2                 :1;
        unsigned CCP2M3                 :1;
        unsigned DC2B0                  :1;
        unsigned DC2B1                  :1;
    };
    struct {
        unsigned                        :4;
        unsigned CCP2Y                  :1;
        unsigned CCP2X                  :1;
    };
} CCP2CONbits_t;
extern volatile CCP2CONbits_t CCP2CONbits __at(0x29A);
// bitfield macros
#define _CCP2CON_CCP2M_POSN                                 0x0
#define _CCP2CON_CCP2M_POSITION                             0x0
#define _CCP2CON_CCP2M_SIZE                                 0x4
#define _CCP2CON_CCP2M_LENGTH                               0x4
#define _CCP2CON_CCP2M_MASK                                 0xF
#define _CCP2CON_DC2B_POSN                                  0x4
#define _CCP2CON_DC2B_POSITION                              0x4
#define _CCP2CON_DC2B_SIZE                                  0x2
#define _CCP2CON_DC2B_LENGTH                                0x2
#define _CCP2CON_DC2B_MASK                                  0x30
#define _CCP2CON_CCP2M0_POSN                                0x0
#define _CCP2CON_CCP2M0_POSITION                            0x0
#define _CCP2CON_CCP2M0_SIZE                                0x1
#define _CCP2CON_CCP2M0_LENGTH                              0x1
#define _CCP2CON_CCP2M0_MASK                                0x1
#define _CCP2CON_CCP2M1_POSN                                0x1
#define _CCP2CON_CCP2M1_POSITION                            0x1
#define _CCP2CON_CCP2M1_SIZE                                0x1
#define _CCP2CON_CCP2M1_LENGTH                              0x1
#define _CCP2CON_CCP2M1_MASK                                0x2
#define _CCP2CON_CCP2M2_POSN                                0x2
#define _CCP2CON_CCP2M2_POSITION                            0x2
#define _CCP2CON_CCP2M2_SIZE                                0x1
#define _CCP2CON_CCP2M2_LENGTH                              0x1
#define _CCP2CON_CCP2M2_MASK                                0x4
#define _CCP2CON_CCP2M3_POSN                                0x3
#define _CCP2CON_CCP2M3_POSITION                            0x3
#define _CCP2CON_CCP2M3_SIZE                                0x1
#define _CCP2CON_CCP2M3_LENGTH                              0x1
#define _CCP2CON_CCP2M3_MASK                                0x8
#define _CCP2CON_DC2B0_POSN                                 0x4
#define _CCP2CON_DC2B0_POSITION                             0x4
#define _CCP2CON_DC2B0_SIZE                                 0x1
#define _CCP2CON_DC2B0_LENGTH                               0x1
#define _CCP2CON_DC2B0_MASK                                 0x10
#define _CCP2CON_DC2B1_POSN                                 0x5
#define _CCP2CON_DC2B1_POSITION                             0x5
#define _CCP2CON_DC2B1_SIZE                                 0x1
#define _CCP2CON_DC2B1_LENGTH                               0x1
#define _CCP2CON_DC2B1_MASK                                 0x20
#define _CCP2CON_CCP2Y_POSN                                 0x4
#define _CCP2CON_CCP2Y_POSITION                             0x4
#define _CCP2CON_CCP2Y_SIZE                                 0x1
#define _CCP2CON_CCP2Y_LENGTH                               0x1
#define _CCP2CON_CCP2Y_MASK                                 0x10
#define _CCP2CON_CCP2X_POSN                                 0x5
#define _CCP2CON_CCP2X_POSITION                             0x5
#define _CCP2CON_CCP2X_SIZE                                 0x1
#define _CCP2CON_CCP2X_LENGTH                               0x1
#define _CCP2CON_CCP2X_MASK                                 0x20
// alias bitfield definitions
typedef union {
    struct {
        unsigned CCP2M                  :4;
        unsigned DC2B                   :2;
    };
    struct {
        unsigned CCP2M0                 :1;
        unsigned CCP2M1                 :1;
        unsigned CCP2M2                 :1;
        unsigned CCP2M3                 :1;
        unsigned DC2B0                  :1;
        unsigned DC2B1                  :1;
    };
    struct {
        unsigned                        :4;
        unsigned CCP2Y                  :1;
        unsigned CCP2X                  :1;
    };
} ECCP2CONbits_t;
extern volatile ECCP2CONbits_t ECCP2CONbits __at(0x29A);
// bitfield macros
#define _ECCP2CON_CCP2M_POSN                                0x0
#define _ECCP2CON_CCP2M_POSITION                            0x0
#define _ECCP2CON_CCP2M_SIZE                                0x4
#define _ECCP2CON_CCP2M_LENGTH                              0x4
#define _ECCP2CON_CCP2M_MASK                                0xF
#define _ECCP2CON_DC2B_POSN                                 0x4
#define _ECCP2CON_DC2B_POSITION                             0x4
#define _ECCP2CON_DC2B_SIZE                                 0x2
#define _ECCP2CON_DC2B_LENGTH                               0x2
#define _ECCP2CON_DC2B_MASK                                 0x30
#define _ECCP2CON_CCP2M0_POSN                               0x0
#define _ECCP2CON_CCP2M0_POSITION                           0x0
#define _ECCP2CON_CCP2M0_SIZE                               0x1
#define _ECCP2CON_CCP2M0_LENGTH                             0x1
#define _ECCP2CON_CCP2M0_MASK                               0x1
#define _ECCP2CON_CCP2M1_POSN                               0x1
#define _ECCP2CON_CCP2M1_POSITION                           0x1
#define _ECCP2CON_CCP2M1_SIZE                               0x1
#define _ECCP2CON_CCP2M1_LENGTH                             0x1
#define _ECCP2CON_CCP2M1_MASK                               0x2
#define _ECCP2CON_CCP2M2_POSN                               0x2
#define _ECCP2CON_CCP2M2_POSITION                           0x2
#define _ECCP2CON_CCP2M2_SIZE                               0x1
#define _ECCP2CON_CCP2M2_LENGTH                             0x1
#define _ECCP2CON_CCP2M2_MASK                               0x4
#define _ECCP2CON_CCP2M3_POSN                               0x3
#define _ECCP2CON_CCP2M3_POSITION                           0x3
#define _ECCP2CON_CCP2M3_SIZE                               0x1
#define _ECCP2CON_CCP2M3_LENGTH                             0x1
#define _ECCP2CON_CCP2M3_MASK                               0x8
#define _ECCP2CON_DC2B0_POSN                                0x4
#define _ECCP2CON_DC2B0_POSITION                            0x4
#define _ECCP2CON_DC2B0_SIZE                                0x1
#define _ECCP2CON_DC2B0_LENGTH                              0x1
#define _ECCP2CON_DC2B0_MASK                                0x10
#define _ECCP2CON_DC2B1_POSN                                0x5
#define _ECCP2CON_DC2B1_POSITION                            0x5
#define _ECCP2CON_DC2B1_SIZE                                0x1
#define _ECCP2CON_DC2B1_LENGTH                              0x1
#define _ECCP2CON_DC2B1_MASK                                0x20
#define _ECCP2CON_CCP2Y_POSN                                0x4
#define _ECCP2CON_CCP2Y_POSITION                            0x4
#define _ECCP2CON_CCP2Y_SIZE                                0x1
#define _ECCP2CON_CCP2Y_LENGTH                              0x1
#define _ECCP2CON_CCP2Y_MASK                                0x10
#define _ECCP2CON_CCP2X_POSN                                0x5
#define _ECCP2CON_CCP2X_POSITION                            0x5
#define _ECCP2CON_CCP2X_SIZE                                0x1
#define _ECCP2CON_CCP2X_LENGTH                              0x1
#define _ECCP2CON_CCP2X_MASK                                0x20

// Register: CCPTMRS
#define CCPTMRS CCPTMRS
extern volatile unsigned char           CCPTMRS             __at(0x29E);
#ifndef _LIB_BUILD
asm("CCPTMRS equ 029Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned C1TSEL                 :2;
        unsigned C2TSEL                 :2;
        unsigned P3TSEL                 :2;
        unsigned P4TSEL                 :2;
    };
    struct {
        unsigned C1TSEL0                :1;
        unsigned C1TSEL1                :1;
        unsigned C2TSEL0                :1;
        unsigned C2TSEL1                :1;
        unsigned P3TSEL0                :1;
        unsigned P3TSEL1                :1;
        unsigned P4TSEL0                :1;
        unsigned P4TSEL1                :1;
    };
} CCPTMRSbits_t;
extern volatile CCPTMRSbits_t CCPTMRSbits __at(0x29E);
// bitfield macros
#define _CCPTMRS_C1TSEL_POSN                                0x0
#define _CCPTMRS_C1TSEL_POSITION                            0x0
#define _CCPTMRS_C1TSEL_SIZE                                0x2
#define _CCPTMRS_C1TSEL_LENGTH                              0x2
#define _CCPTMRS_C1TSEL_MASK                                0x3
#define _CCPTMRS_C2TSEL_POSN                                0x2
#define _CCPTMRS_C2TSEL_POSITION                            0x2
#define _CCPTMRS_C2TSEL_SIZE                                0x2
#define _CCPTMRS_C2TSEL_LENGTH                              0x2
#define _CCPTMRS_C2TSEL_MASK                                0xC
#define _CCPTMRS_P3TSEL_POSN                                0x4
#define _CCPTMRS_P3TSEL_POSITION                            0x4
#define _CCPTMRS_P3TSEL_SIZE                                0x2
#define _CCPTMRS_P3TSEL_LENGTH                              0x2
#define _CCPTMRS_P3TSEL_MASK                                0x30
#define _CCPTMRS_P4TSEL_POSN                                0x6
#define _CCPTMRS_P4TSEL_POSITION                            0x6
#define _CCPTMRS_P4TSEL_SIZE                                0x2
#define _CCPTMRS_P4TSEL_LENGTH                              0x2
#define _CCPTMRS_P4TSEL_MASK                                0xC0
#define _CCPTMRS_C1TSEL0_POSN                               0x0
#define _CCPTMRS_C1TSEL0_POSITION                           0x0
#define _CCPTMRS_C1TSEL0_SIZE                               0x1
#define _CCPTMRS_C1TSEL0_LENGTH                             0x1
#define _CCPTMRS_C1TSEL0_MASK                               0x1
#define _CCPTMRS_C1TSEL1_POSN                               0x1
#define _CCPTMRS_C1TSEL1_POSITION                           0x1
#define _CCPTMRS_C1TSEL1_SIZE                               0x1
#define _CCPTMRS_C1TSEL1_LENGTH                             0x1
#define _CCPTMRS_C1TSEL1_MASK                               0x2
#define _CCPTMRS_C2TSEL0_POSN                               0x2
#define _CCPTMRS_C2TSEL0_POSITION                           0x2
#define _CCPTMRS_C2TSEL0_SIZE                               0x1
#define _CCPTMRS_C2TSEL0_LENGTH                             0x1
#define _CCPTMRS_C2TSEL0_MASK                               0x4
#define _CCPTMRS_C2TSEL1_POSN                               0x3
#define _CCPTMRS_C2TSEL1_POSITION                           0x3
#define _CCPTMRS_C2TSEL1_SIZE                               0x1
#define _CCPTMRS_C2TSEL1_LENGTH                             0x1
#define _CCPTMRS_C2TSEL1_MASK                               0x8
#define _CCPTMRS_P3TSEL0_POSN                               0x4
#define _CCPTMRS_P3TSEL0_POSITION                           0x4
#define _CCPTMRS_P3TSEL0_SIZE                               0x1
#define _CCPTMRS_P3TSEL0_LENGTH                             0x1
#define _CCPTMRS_P3TSEL0_MASK                               0x10
#define _CCPTMRS_P3TSEL1_POSN                               0x5
#define _CCPTMRS_P3TSEL1_POSITION                           0x5
#define _CCPTMRS_P3TSEL1_SIZE                               0x1
#define _CCPTMRS_P3TSEL1_LENGTH                             0x1
#define _CCPTMRS_P3TSEL1_MASK                               0x20
#define _CCPTMRS_P4TSEL0_POSN                               0x6
#define _CCPTMRS_P4TSEL0_POSITION                           0x6
#define _CCPTMRS_P4TSEL0_SIZE                               0x1
#define _CCPTMRS_P4TSEL0_LENGTH                             0x1
#define _CCPTMRS_P4TSEL0_MASK                               0x40
#define _CCPTMRS_P4TSEL1_POSN                               0x7
#define _CCPTMRS_P4TSEL1_POSITION                           0x7
#define _CCPTMRS_P4TSEL1_SIZE                               0x1
#define _CCPTMRS_P4TSEL1_LENGTH                             0x1
#define _CCPTMRS_P4TSEL1_MASK                               0x80

// Register: SLRCONA
#define SLRCONA SLRCONA
extern volatile unsigned char           SLRCONA             __at(0x30C);
#ifndef _LIB_BUILD
asm("SLRCONA equ 030Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SLRA0                  :1;
        unsigned SLRA1                  :1;
        unsigned SLRA2                  :1;
        unsigned                        :1;
        unsigned SLRA4                  :1;
        unsigned SLRA5                  :1;
    };
} SLRCONAbits_t;
extern volatile SLRCONAbits_t SLRCONAbits __at(0x30C);
// bitfield macros
#define _SLRCONA_SLRA0_POSN                                 0x0
#define _SLRCONA_SLRA0_POSITION                             0x0
#define _SLRCONA_SLRA0_SIZE                                 0x1
#define _SLRCONA_SLRA0_LENGTH                               0x1
#define _SLRCONA_SLRA0_MASK                                 0x1
#define _SLRCONA_SLRA1_POSN                                 0x1
#define _SLRCONA_SLRA1_POSITION                             0x1
#define _SLRCONA_SLRA1_SIZE                                 0x1
#define _SLRCONA_SLRA1_LENGTH                               0x1
#define _SLRCONA_SLRA1_MASK                                 0x2
#define _SLRCONA_SLRA2_POSN                                 0x2
#define _SLRCONA_SLRA2_POSITION                             0x2
#define _SLRCONA_SLRA2_SIZE                                 0x1
#define _SLRCONA_SLRA2_LENGTH                               0x1
#define _SLRCONA_SLRA2_MASK                                 0x4
#define _SLRCONA_SLRA4_POSN                                 0x4
#define _SLRCONA_SLRA4_POSITION                             0x4
#define _SLRCONA_SLRA4_SIZE                                 0x1
#define _SLRCONA_SLRA4_LENGTH                               0x1
#define _SLRCONA_SLRA4_MASK                                 0x10
#define _SLRCONA_SLRA5_POSN                                 0x5
#define _SLRCONA_SLRA5_POSITION                             0x5
#define _SLRCONA_SLRA5_SIZE                                 0x1
#define _SLRCONA_SLRA5_LENGTH                               0x1
#define _SLRCONA_SLRA5_MASK                                 0x20

// Register: SLRCONB
#define SLRCONB SLRCONB
extern volatile unsigned char           SLRCONB             __at(0x30D);
#ifndef _LIB_BUILD
asm("SLRCONB equ 030Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned SLRB4                  :1;
        unsigned SLRB5                  :1;
        unsigned SLRB6                  :1;
        unsigned SLRB7                  :1;
    };
} SLRCONBbits_t;
extern volatile SLRCONBbits_t SLRCONBbits __at(0x30D);
// bitfield macros
#define _SLRCONB_SLRB4_POSN                                 0x4
#define _SLRCONB_SLRB4_POSITION                             0x4
#define _SLRCONB_SLRB4_SIZE                                 0x1
#define _SLRCONB_SLRB4_LENGTH                               0x1
#define _SLRCONB_SLRB4_MASK                                 0x10
#define _SLRCONB_SLRB5_POSN                                 0x5
#define _SLRCONB_SLRB5_POSITION                             0x5
#define _SLRCONB_SLRB5_SIZE                                 0x1
#define _SLRCONB_SLRB5_LENGTH                               0x1
#define _SLRCONB_SLRB5_MASK                                 0x20
#define _SLRCONB_SLRB6_POSN                                 0x6
#define _SLRCONB_SLRB6_POSITION                             0x6
#define _SLRCONB_SLRB6_SIZE                                 0x1
#define _SLRCONB_SLRB6_LENGTH                               0x1
#define _SLRCONB_SLRB6_MASK                                 0x40
#define _SLRCONB_SLRB7_POSN                                 0x7
#define _SLRCONB_SLRB7_POSITION                             0x7
#define _SLRCONB_SLRB7_SIZE                                 0x1
#define _SLRCONB_SLRB7_LENGTH                               0x1
#define _SLRCONB_SLRB7_MASK                                 0x80

// Register: SLRCONC
#define SLRCONC SLRCONC
extern volatile unsigned char           SLRCONC             __at(0x30E);
#ifndef _LIB_BUILD
asm("SLRCONC equ 030Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SLRC0                  :1;
        unsigned SLRC1                  :1;
        unsigned SLRC2                  :1;
        unsigned SLRC3                  :1;
        unsigned SLRC4                  :1;
        unsigned SLRC5                  :1;
        unsigned SLRC6                  :1;
        unsigned SLRC7                  :1;
    };
} SLRCONCbits_t;
extern volatile SLRCONCbits_t SLRCONCbits __at(0x30E);
// bitfield macros
#define _SLRCONC_SLRC0_POSN                                 0x0
#define _SLRCONC_SLRC0_POSITION                             0x0
#define _SLRCONC_SLRC0_SIZE                                 0x1
#define _SLRCONC_SLRC0_LENGTH                               0x1
#define _SLRCONC_SLRC0_MASK                                 0x1
#define _SLRCONC_SLRC1_POSN                                 0x1
#define _SLRCONC_SLRC1_POSITION                             0x1
#define _SLRCONC_SLRC1_SIZE                                 0x1
#define _SLRCONC_SLRC1_LENGTH                               0x1
#define _SLRCONC_SLRC1_MASK                                 0x2
#define _SLRCONC_SLRC2_POSN                                 0x2
#define _SLRCONC_SLRC2_POSITION                             0x2
#define _SLRCONC_SLRC2_SIZE                                 0x1
#define _SLRCONC_SLRC2_LENGTH                               0x1
#define _SLRCONC_SLRC2_MASK                                 0x4
#define _SLRCONC_SLRC3_POSN                                 0x3
#define _SLRCONC_SLRC3_POSITION                             0x3
#define _SLRCONC_SLRC3_SIZE                                 0x1
#define _SLRCONC_SLRC3_LENGTH                               0x1
#define _SLRCONC_SLRC3_MASK                                 0x8
#define _SLRCONC_SLRC4_POSN                                 0x4
#define _SLRCONC_SLRC4_POSITION                             0x4
#define _SLRCONC_SLRC4_SIZE                                 0x1
#define _SLRCONC_SLRC4_LENGTH                               0x1
#define _SLRCONC_SLRC4_MASK                                 0x10
#define _SLRCONC_SLRC5_POSN                                 0x5
#define _SLRCONC_SLRC5_POSITION                             0x5
#define _SLRCONC_SLRC5_SIZE                                 0x1
#define _SLRCONC_SLRC5_LENGTH                               0x1
#define _SLRCONC_SLRC5_MASK                                 0x20
#define _SLRCONC_SLRC6_POSN                                 0x6
#define _SLRCONC_SLRC6_POSITION                             0x6
#define _SLRCONC_SLRC6_SIZE                                 0x1
#define _SLRCONC_SLRC6_LENGTH                               0x1
#define _SLRCONC_SLRC6_MASK                                 0x40
#define _SLRCONC_SLRC7_POSN                                 0x7
#define _SLRCONC_SLRC7_POSITION                             0x7
#define _SLRCONC_SLRC7_SIZE                                 0x1
#define _SLRCONC_SLRC7_LENGTH                               0x1
#define _SLRCONC_SLRC7_MASK                                 0x80

// Register: INLVLA
#define INLVLA INLVLA
extern volatile unsigned char           INLVLA              __at(0x38C);
#ifndef _LIB_BUILD
asm("INLVLA equ 038Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned INLVLA0                :1;
        unsigned INLVLA1                :1;
        unsigned INLVLA2                :1;
        unsigned INLVLA3                :1;
        unsigned INLVLA4                :1;
        unsigned INLVLA5                :1;
    };
} INLVLAbits_t;
extern volatile INLVLAbits_t INLVLAbits __at(0x38C);
// bitfield macros
#define _INLVLA_INLVLA0_POSN                                0x0
#define _INLVLA_INLVLA0_POSITION                            0x0
#define _INLVLA_INLVLA0_SIZE                                0x1
#define _INLVLA_INLVLA0_LENGTH                              0x1
#define _INLVLA_INLVLA0_MASK                                0x1
#define _INLVLA_INLVLA1_POSN                                0x1
#define _INLVLA_INLVLA1_POSITION                            0x1
#define _INLVLA_INLVLA1_SIZE                                0x1
#define _INLVLA_INLVLA1_LENGTH                              0x1
#define _INLVLA_INLVLA1_MASK                                0x2
#define _INLVLA_INLVLA2_POSN                                0x2
#define _INLVLA_INLVLA2_POSITION                            0x2
#define _INLVLA_INLVLA2_SIZE                                0x1
#define _INLVLA_INLVLA2_LENGTH                              0x1
#define _INLVLA_INLVLA2_MASK                                0x4
#define _INLVLA_INLVLA3_POSN                                0x3
#define _INLVLA_INLVLA3_POSITION                            0x3
#define _INLVLA_INLVLA3_SIZE                                0x1
#define _INLVLA_INLVLA3_LENGTH                              0x1
#define _INLVLA_INLVLA3_MASK                                0x8
#define _INLVLA_INLVLA4_POSN                                0x4
#define _INLVLA_INLVLA4_POSITION                            0x4
#define _INLVLA_INLVLA4_SIZE                                0x1
#define _INLVLA_INLVLA4_LENGTH                              0x1
#define _INLVLA_INLVLA4_MASK                                0x10
#define _INLVLA_INLVLA5_POSN                                0x5
#define _INLVLA_INLVLA5_POSITION                            0x5
#define _INLVLA_INLVLA5_SIZE                                0x1
#define _INLVLA_INLVLA5_LENGTH                              0x1
#define _INLVLA_INLVLA5_MASK                                0x20

// Register: INLVLB
#define INLVLB INLVLB
extern volatile unsigned char           INLVLB              __at(0x38D);
#ifndef _LIB_BUILD
asm("INLVLB equ 038Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned INLVLB4                :1;
        unsigned INLVLB5                :1;
        unsigned INLVLB6                :1;
        unsigned INLVLB7                :1;
    };
} INLVLBbits_t;
extern volatile INLVLBbits_t INLVLBbits __at(0x38D);
// bitfield macros
#define _INLVLB_INLVLB4_POSN                                0x4
#define _INLVLB_INLVLB4_POSITION                            0x4
#define _INLVLB_INLVLB4_SIZE                                0x1
#define _INLVLB_INLVLB4_LENGTH                              0x1
#define _INLVLB_INLVLB4_MASK                                0x10
#define _INLVLB_INLVLB5_POSN                                0x5
#define _INLVLB_INLVLB5_POSITION                            0x5
#define _INLVLB_INLVLB5_SIZE                                0x1
#define _INLVLB_INLVLB5_LENGTH                              0x1
#define _INLVLB_INLVLB5_MASK                                0x20
#define _INLVLB_INLVLB6_POSN                                0x6
#define _INLVLB_INLVLB6_POSITION                            0x6
#define _INLVLB_INLVLB6_SIZE                                0x1
#define _INLVLB_INLVLB6_LENGTH                              0x1
#define _INLVLB_INLVLB6_MASK                                0x40
#define _INLVLB_INLVLB7_POSN                                0x7
#define _INLVLB_INLVLB7_POSITION                            0x7
#define _INLVLB_INLVLB7_SIZE                                0x1
#define _INLVLB_INLVLB7_LENGTH                              0x1
#define _INLVLB_INLVLB7_MASK                                0x80

// Register: INLVLC
#define INLVLC INLVLC
extern volatile unsigned char           INLVLC              __at(0x38E);
#ifndef _LIB_BUILD
asm("INLVLC equ 038Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned INLVLC0                :1;
        unsigned INLVLC1                :1;
        unsigned INLVLC2                :1;
        unsigned INLVLC3                :1;
        unsigned INLVLC4                :1;
        unsigned INLVLC5                :1;
        unsigned INLVLC6                :1;
        unsigned INLVLC7                :1;
    };
} INLVLCbits_t;
extern volatile INLVLCbits_t INLVLCbits __at(0x38E);
// bitfield macros
#define _INLVLC_INLVLC0_POSN                                0x0
#define _INLVLC_INLVLC0_POSITION                            0x0
#define _INLVLC_INLVLC0_SIZE                                0x1
#define _INLVLC_INLVLC0_LENGTH                              0x1
#define _INLVLC_INLVLC0_MASK                                0x1
#define _INLVLC_INLVLC1_POSN                                0x1
#define _INLVLC_INLVLC1_POSITION                            0x1
#define _INLVLC_INLVLC1_SIZE                                0x1
#define _INLVLC_INLVLC1_LENGTH                              0x1
#define _INLVLC_INLVLC1_MASK                                0x2
#define _INLVLC_INLVLC2_POSN                                0x2
#define _INLVLC_INLVLC2_POSITION                            0x2
#define _INLVLC_INLVLC2_SIZE                                0x1
#define _INLVLC_INLVLC2_LENGTH                              0x1
#define _INLVLC_INLVLC2_MASK                                0x4
#define _INLVLC_INLVLC3_POSN                                0x3
#define _INLVLC_INLVLC3_POSITION                            0x3
#define _INLVLC_INLVLC3_SIZE                                0x1
#define _INLVLC_INLVLC3_LENGTH                              0x1
#define _INLVLC_INLVLC3_MASK                                0x8
#define _INLVLC_INLVLC4_POSN                                0x4
#define _INLVLC_INLVLC4_POSITION                            0x4
#define _INLVLC_INLVLC4_SIZE                                0x1
#define _INLVLC_INLVLC4_LENGTH                              0x1
#define _INLVLC_INLVLC4_MASK                                0x10
#define _INLVLC_INLVLC5_POSN                                0x5
#define _INLVLC_INLVLC5_POSITION                            0x5
#define _INLVLC_INLVLC5_SIZE                                0x1
#define _INLVLC_INLVLC5_LENGTH                              0x1
#define _INLVLC_INLVLC5_MASK                                0x20
#define _INLVLC_INLVLC6_POSN                                0x6
#define _INLVLC_INLVLC6_POSITION                            0x6
#define _INLVLC_INLVLC6_SIZE                                0x1
#define _INLVLC_INLVLC6_LENGTH                              0x1
#define _INLVLC_INLVLC6_MASK                                0x40
#define _INLVLC_INLVLC7_POSN                                0x7
#define _INLVLC_INLVLC7_POSITION                            0x7
#define _INLVLC_INLVLC7_SIZE                                0x1
#define _INLVLC_INLVLC7_LENGTH                              0x1
#define _INLVLC_INLVLC7_MASK                                0x80

// Register: IOCAP
#define IOCAP IOCAP
extern volatile unsigned char           IOCAP               __at(0x391);
#ifndef _LIB_BUILD
asm("IOCAP equ 0391h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned IOCAP0                 :1;
        unsigned IOCAP1                 :1;
        unsigned IOCAP2                 :1;
        unsigned IOCAP3                 :1;
        unsigned IOCAP4                 :1;
        unsigned IOCAP5                 :1;
    };
} IOCAPbits_t;
extern volatile IOCAPbits_t IOCAPbits __at(0x391);
// bitfield macros
#define _IOCAP_IOCAP0_POSN                                  0x0
#define _IOCAP_IOCAP0_POSITION                              0x0
#define _IOCAP_IOCAP0_SIZE                                  0x1
#define _IOCAP_IOCAP0_LENGTH                                0x1
#define _IOCAP_IOCAP0_MASK                                  0x1
#define _IOCAP_IOCAP1_POSN                                  0x1
#define _IOCAP_IOCAP1_POSITION                              0x1
#define _IOCAP_IOCAP1_SIZE                                  0x1
#define _IOCAP_IOCAP1_LENGTH                                0x1
#define _IOCAP_IOCAP1_MASK                                  0x2
#define _IOCAP_IOCAP2_POSN                                  0x2
#define _IOCAP_IOCAP2_POSITION                              0x2
#define _IOCAP_IOCAP2_SIZE                                  0x1
#define _IOCAP_IOCAP2_LENGTH                                0x1
#define _IOCAP_IOCAP2_MASK                                  0x4
#define _IOCAP_IOCAP3_POSN                                  0x3
#define _IOCAP_IOCAP3_POSITION                              0x3
#define _IOCAP_IOCAP3_SIZE                                  0x1
#define _IOCAP_IOCAP3_LENGTH                                0x1
#define _IOCAP_IOCAP3_MASK                                  0x8
#define _IOCAP_IOCAP4_POSN                                  0x4
#define _IOCAP_IOCAP4_POSITION                              0x4
#define _IOCAP_IOCAP4_SIZE                                  0x1
#define _IOCAP_IOCAP4_LENGTH                                0x1
#define _IOCAP_IOCAP4_MASK                                  0x10
#define _IOCAP_IOCAP5_POSN                                  0x5
#define _IOCAP_IOCAP5_POSITION                              0x5
#define _IOCAP_IOCAP5_SIZE                                  0x1
#define _IOCAP_IOCAP5_LENGTH                                0x1
#define _IOCAP_IOCAP5_MASK                                  0x20

// Register: IOCAN
#define IOCAN IOCAN
extern volatile unsigned char           IOCAN               __at(0x392);
#ifndef _LIB_BUILD
asm("IOCAN equ 0392h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned IOCAN0                 :1;
        unsigned IOCAN1                 :1;
        unsigned IOCAN2                 :1;
        unsigned IOCAN3                 :1;
        unsigned IOCAN4                 :1;
        unsigned IOCAN5                 :1;
    };
} IOCANbits_t;
extern volatile IOCANbits_t IOCANbits __at(0x392);
// bitfield macros
#define _IOCAN_IOCAN0_POSN                                  0x0
#define _IOCAN_IOCAN0_POSITION                              0x0
#define _IOCAN_IOCAN0_SIZE                                  0x1
#define _IOCAN_IOCAN0_LENGTH                                0x1
#define _IOCAN_IOCAN0_MASK                                  0x1
#define _IOCAN_IOCAN1_POSN                                  0x1
#define _IOCAN_IOCAN1_POSITION                              0x1
#define _IOCAN_IOCAN1_SIZE                                  0x1
#define _IOCAN_IOCAN1_LENGTH                                0x1
#define _IOCAN_IOCAN1_MASK                                  0x2
#define _IOCAN_IOCAN2_POSN                                  0x2
#define _IOCAN_IOCAN2_POSITION                              0x2
#define _IOCAN_IOCAN2_SIZE                                  0x1
#define _IOCAN_IOCAN2_LENGTH                                0x1
#define _IOCAN_IOCAN2_MASK                                  0x4
#define _IOCAN_IOCAN3_POSN                                  0x3
#define _IOCAN_IOCAN3_POSITION                              0x3
#define _IOCAN_IOCAN3_SIZE                                  0x1
#define _IOCAN_IOCAN3_LENGTH                                0x1
#define _IOCAN_IOCAN3_MASK                                  0x8
#define _IOCAN_IOCAN4_POSN                                  0x4
#define _IOCAN_IOCAN4_POSITION                              0x4
#define _IOCAN_IOCAN4_SIZE                                  0x1
#define _IOCAN_IOCAN4_LENGTH                                0x1
#define _IOCAN_IOCAN4_MASK                                  0x10
#define _IOCAN_IOCAN5_POSN                                  0x5
#define _IOCAN_IOCAN5_POSITION                              0x5
#define _IOCAN_IOCAN5_SIZE                                  0x1
#define _IOCAN_IOCAN5_LENGTH                                0x1
#define _IOCAN_IOCAN5_MASK                                  0x20

// Register: IOCAF
#define IOCAF IOCAF
extern volatile unsigned char           IOCAF               __at(0x393);
#ifndef _LIB_BUILD
asm("IOCAF equ 0393h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned IOCAF0                 :1;
        unsigned IOCAF1                 :1;
        unsigned IOCAF2                 :1;
        unsigned IOCAF3                 :1;
        unsigned IOCAF4                 :1;
        unsigned IOCAF5                 :1;
    };
} IOCAFbits_t;
extern volatile IOCAFbits_t IOCAFbits __at(0x393);
// bitfield macros
#define _IOCAF_IOCAF0_POSN                                  0x0
#define _IOCAF_IOCAF0_POSITION                              0x0
#define _IOCAF_IOCAF0_SIZE                                  0x1
#define _IOCAF_IOCAF0_LENGTH                                0x1
#define _IOCAF_IOCAF0_MASK                                  0x1
#define _IOCAF_IOCAF1_POSN                                  0x1
#define _IOCAF_IOCAF1_POSITION                              0x1
#define _IOCAF_IOCAF1_SIZE                                  0x1
#define _IOCAF_IOCAF1_LENGTH                                0x1
#define _IOCAF_IOCAF1_MASK                                  0x2
#define _IOCAF_IOCAF2_POSN                                  0x2
#define _IOCAF_IOCAF2_POSITION                              0x2
#define _IOCAF_IOCAF2_SIZE                                  0x1
#define _IOCAF_IOCAF2_LENGTH                                0x1
#define _IOCAF_IOCAF2_MASK                                  0x4
#define _IOCAF_IOCAF3_POSN                                  0x3
#define _IOCAF_IOCAF3_POSITION                              0x3
#define _IOCAF_IOCAF3_SIZE                                  0x1
#define _IOCAF_IOCAF3_LENGTH                                0x1
#define _IOCAF_IOCAF3_MASK                                  0x8
#define _IOCAF_IOCAF4_POSN                                  0x4
#define _IOCAF_IOCAF4_POSITION                              0x4
#define _IOCAF_IOCAF4_SIZE                                  0x1
#define _IOCAF_IOCAF4_LENGTH                                0x1
#define _IOCAF_IOCAF4_MASK                                  0x10
#define _IOCAF_IOCAF5_POSN                                  0x5
#define _IOCAF_IOCAF5_POSITION                              0x5
#define _IOCAF_IOCAF5_SIZE                                  0x1
#define _IOCAF_IOCAF5_LENGTH                                0x1
#define _IOCAF_IOCAF5_MASK                                  0x20

// Register: IOCBP
#define IOCBP IOCBP
extern volatile unsigned char           IOCBP               __at(0x394);
#ifndef _LIB_BUILD
asm("IOCBP equ 0394h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned IOCBP4                 :1;
        unsigned IOCBP5                 :1;
        unsigned IOCBP6                 :1;
        unsigned IOCBP7                 :1;
    };
} IOCBPbits_t;
extern volatile IOCBPbits_t IOCBPbits __at(0x394);
// bitfield macros
#define _IOCBP_IOCBP4_POSN                                  0x4
#define _IOCBP_IOCBP4_POSITION                              0x4
#define _IOCBP_IOCBP4_SIZE                                  0x1
#define _IOCBP_IOCBP4_LENGTH                                0x1
#define _IOCBP_IOCBP4_MASK                                  0x10
#define _IOCBP_IOCBP5_POSN                                  0x5
#define _IOCBP_IOCBP5_POSITION                              0x5
#define _IOCBP_IOCBP5_SIZE                                  0x1
#define _IOCBP_IOCBP5_LENGTH                                0x1
#define _IOCBP_IOCBP5_MASK                                  0x20
#define _IOCBP_IOCBP6_POSN                                  0x6
#define _IOCBP_IOCBP6_POSITION                              0x6
#define _IOCBP_IOCBP6_SIZE                                  0x1
#define _IOCBP_IOCBP6_LENGTH                                0x1
#define _IOCBP_IOCBP6_MASK                                  0x40
#define _IOCBP_IOCBP7_POSN                                  0x7
#define _IOCBP_IOCBP7_POSITION                              0x7
#define _IOCBP_IOCBP7_SIZE                                  0x1
#define _IOCBP_IOCBP7_LENGTH                                0x1
#define _IOCBP_IOCBP7_MASK                                  0x80

// Register: IOCBN
#define IOCBN IOCBN
extern volatile unsigned char           IOCBN               __at(0x395);
#ifndef _LIB_BUILD
asm("IOCBN equ 0395h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned IOCBN4                 :1;
        unsigned IOCBN5                 :1;
        unsigned IOCBN6                 :1;
        unsigned IOCBN7                 :1;
    };
} IOCBNbits_t;
extern volatile IOCBNbits_t IOCBNbits __at(0x395);
// bitfield macros
#define _IOCBN_IOCBN4_POSN                                  0x4
#define _IOCBN_IOCBN4_POSITION                              0x4
#define _IOCBN_IOCBN4_SIZE                                  0x1
#define _IOCBN_IOCBN4_LENGTH                                0x1
#define _IOCBN_IOCBN4_MASK                                  0x10
#define _IOCBN_IOCBN5_POSN                                  0x5
#define _IOCBN_IOCBN5_POSITION                              0x5
#define _IOCBN_IOCBN5_SIZE                                  0x1
#define _IOCBN_IOCBN5_LENGTH                                0x1
#define _IOCBN_IOCBN5_MASK                                  0x20
#define _IOCBN_IOCBN6_POSN                                  0x6
#define _IOCBN_IOCBN6_POSITION                              0x6
#define _IOCBN_IOCBN6_SIZE                                  0x1
#define _IOCBN_IOCBN6_LENGTH                                0x1
#define _IOCBN_IOCBN6_MASK                                  0x40
#define _IOCBN_IOCBN7_POSN                                  0x7
#define _IOCBN_IOCBN7_POSITION                              0x7
#define _IOCBN_IOCBN7_SIZE                                  0x1
#define _IOCBN_IOCBN7_LENGTH                                0x1
#define _IOCBN_IOCBN7_MASK                                  0x80

// Register: IOCBF
#define IOCBF IOCBF
extern volatile unsigned char           IOCBF               __at(0x396);
#ifndef _LIB_BUILD
asm("IOCBF equ 0396h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned IOCBF4                 :1;
        unsigned IOCBF5                 :1;
        unsigned IOCBF6                 :1;
        unsigned IOCBF7                 :1;
    };
} IOCBFbits_t;
extern volatile IOCBFbits_t IOCBFbits __at(0x396);
// bitfield macros
#define _IOCBF_IOCBF4_POSN                                  0x4
#define _IOCBF_IOCBF4_POSITION                              0x4
#define _IOCBF_IOCBF4_SIZE                                  0x1
#define _IOCBF_IOCBF4_LENGTH                                0x1
#define _IOCBF_IOCBF4_MASK                                  0x10
#define _IOCBF_IOCBF5_POSN                                  0x5
#define _IOCBF_IOCBF5_POSITION                              0x5
#define _IOCBF_IOCBF5_SIZE                                  0x1
#define _IOCBF_IOCBF5_LENGTH                                0x1
#define _IOCBF_IOCBF5_MASK                                  0x20
#define _IOCBF_IOCBF6_POSN                                  0x6
#define _IOCBF_IOCBF6_POSITION                              0x6
#define _IOCBF_IOCBF6_SIZE                                  0x1
#define _IOCBF_IOCBF6_LENGTH                                0x1
#define _IOCBF_IOCBF6_MASK                                  0x40
#define _IOCBF_IOCBF7_POSN                                  0x7
#define _IOCBF_IOCBF7_POSITION                              0x7
#define _IOCBF_IOCBF7_SIZE                                  0x1
#define _IOCBF_IOCBF7_LENGTH                                0x1
#define _IOCBF_IOCBF7_MASK                                  0x80

// Register: IOCCP
#define IOCCP IOCCP
extern volatile unsigned char           IOCCP               __at(0x397);
#ifndef _LIB_BUILD
asm("IOCCP equ 0397h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned IOCCP0                 :1;
        unsigned IOCCP1                 :1;
        unsigned IOCCP2                 :1;
        unsigned IOCCP3                 :1;
        unsigned IOCCP4                 :1;
        unsigned IOCCP5                 :1;
        unsigned IOCCP6                 :1;
        unsigned IOCCP7                 :1;
    };
} IOCCPbits_t;
extern volatile IOCCPbits_t IOCCPbits __at(0x397);
// bitfield macros
#define _IOCCP_IOCCP0_POSN                                  0x0
#define _IOCCP_IOCCP0_POSITION                              0x0
#define _IOCCP_IOCCP0_SIZE                                  0x1
#define _IOCCP_IOCCP0_LENGTH                                0x1
#define _IOCCP_IOCCP0_MASK                                  0x1
#define _IOCCP_IOCCP1_POSN                                  0x1
#define _IOCCP_IOCCP1_POSITION                              0x1
#define _IOCCP_IOCCP1_SIZE                                  0x1
#define _IOCCP_IOCCP1_LENGTH                                0x1
#define _IOCCP_IOCCP1_MASK                                  0x2
#define _IOCCP_IOCCP2_POSN                                  0x2
#define _IOCCP_IOCCP2_POSITION                              0x2
#define _IOCCP_IOCCP2_SIZE                                  0x1
#define _IOCCP_IOCCP2_LENGTH                                0x1
#define _IOCCP_IOCCP2_MASK                                  0x4
#define _IOCCP_IOCCP3_POSN                                  0x3
#define _IOCCP_IOCCP3_POSITION                              0x3
#define _IOCCP_IOCCP3_SIZE                                  0x1
#define _IOCCP_IOCCP3_LENGTH                                0x1
#define _IOCCP_IOCCP3_MASK                                  0x8
#define _IOCCP_IOCCP4_POSN                                  0x4
#define _IOCCP_IOCCP4_POSITION                              0x4
#define _IOCCP_IOCCP4_SIZE                                  0x1
#define _IOCCP_IOCCP4_LENGTH                                0x1
#define _IOCCP_IOCCP4_MASK                                  0x10
#define _IOCCP_IOCCP5_POSN                                  0x5
#define _IOCCP_IOCCP5_POSITION                              0x5
#define _IOCCP_IOCCP5_SIZE                                  0x1
#define _IOCCP_IOCCP5_LENGTH                                0x1
#define _IOCCP_IOCCP5_MASK                                  0x20
#define _IOCCP_IOCCP6_POSN                                  0x6
#define _IOCCP_IOCCP6_POSITION                              0x6
#define _IOCCP_IOCCP6_SIZE                                  0x1
#define _IOCCP_IOCCP6_LENGTH                                0x1
#define _IOCCP_IOCCP6_MASK                                  0x40
#define _IOCCP_IOCCP7_POSN                                  0x7
#define _IOCCP_IOCCP7_POSITION                              0x7
#define _IOCCP_IOCCP7_SIZE                                  0x1
#define _IOCCP_IOCCP7_LENGTH                                0x1
#define _IOCCP_IOCCP7_MASK                                  0x80

// Register: IOCCN
#define IOCCN IOCCN
extern volatile unsigned char           IOCCN               __at(0x398);
#ifndef _LIB_BUILD
asm("IOCCN equ 0398h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned IOCCN0                 :1;
        unsigned IOCCN1                 :1;
        unsigned IOCCN2                 :1;
        unsigned IOCCN3                 :1;
        unsigned IOCCN4                 :1;
        unsigned IOCCN5                 :1;
        unsigned IOCCN6                 :1;
        unsigned IOCCN7                 :1;
    };
} IOCCNbits_t;
extern volatile IOCCNbits_t IOCCNbits __at(0x398);
// bitfield macros
#define _IOCCN_IOCCN0_POSN                                  0x0
#define _IOCCN_IOCCN0_POSITION                              0x0
#define _IOCCN_IOCCN0_SIZE                                  0x1
#define _IOCCN_IOCCN0_LENGTH                                0x1
#define _IOCCN_IOCCN0_MASK                                  0x1
#define _IOCCN_IOCCN1_POSN                                  0x1
#define _IOCCN_IOCCN1_POSITION                              0x1
#define _IOCCN_IOCCN1_SIZE                                  0x1
#define _IOCCN_IOCCN1_LENGTH                                0x1
#define _IOCCN_IOCCN1_MASK                                  0x2
#define _IOCCN_IOCCN2_POSN                                  0x2
#define _IOCCN_IOCCN2_POSITION                              0x2
#define _IOCCN_IOCCN2_SIZE                                  0x1
#define _IOCCN_IOCCN2_LENGTH                                0x1
#define _IOCCN_IOCCN2_MASK                                  0x4
#define _IOCCN_IOCCN3_POSN                                  0x3
#define _IOCCN_IOCCN3_POSITION                              0x3
#define _IOCCN_IOCCN3_SIZE                                  0x1
#define _IOCCN_IOCCN3_LENGTH                                0x1
#define _IOCCN_IOCCN3_MASK                                  0x8
#define _IOCCN_IOCCN4_POSN                                  0x4
#define _IOCCN_IOCCN4_POSITION                              0x4
#define _IOCCN_IOCCN4_SIZE                                  0x1
#define _IOCCN_IOCCN4_LENGTH                                0x1
#define _IOCCN_IOCCN4_MASK                                  0x10
#define _IOCCN_IOCCN5_POSN                                  0x5
#define _IOCCN_IOCCN5_POSITION                              0x5
#define _IOCCN_IOCCN5_SIZE                                  0x1
#define _IOCCN_IOCCN5_LENGTH                                0x1
#define _IOCCN_IOCCN5_MASK                                  0x20
#define _IOCCN_IOCCN6_POSN                                  0x6
#define _IOCCN_IOCCN6_POSITION                              0x6
#define _IOCCN_IOCCN6_SIZE                                  0x1
#define _IOCCN_IOCCN6_LENGTH                                0x1
#define _IOCCN_IOCCN6_MASK                                  0x40
#define _IOCCN_IOCCN7_POSN                                  0x7
#define _IOCCN_IOCCN7_POSITION                              0x7
#define _IOCCN_IOCCN7_SIZE                                  0x1
#define _IOCCN_IOCCN7_LENGTH                                0x1
#define _IOCCN_IOCCN7_MASK                                  0x80

// Register: IOCCF
#define IOCCF IOCCF
extern volatile unsigned char           IOCCF               __at(0x399);
#ifndef _LIB_BUILD
asm("IOCCF equ 0399h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned IOCCF0                 :1;
        unsigned IOCCF1                 :1;
        unsigned IOCCF2                 :1;
        unsigned IOCCF3                 :1;
        unsigned IOCCF4                 :1;
        unsigned IOCCF5                 :1;
        unsigned IOCCF6                 :1;
        unsigned IOCCF7                 :1;
    };
} IOCCFbits_t;
extern volatile IOCCFbits_t IOCCFbits __at(0x399);
// bitfield macros
#define _IOCCF_IOCCF0_POSN                                  0x0
#define _IOCCF_IOCCF0_POSITION                              0x0
#define _IOCCF_IOCCF0_SIZE                                  0x1
#define _IOCCF_IOCCF0_LENGTH                                0x1
#define _IOCCF_IOCCF0_MASK                                  0x1
#define _IOCCF_IOCCF1_POSN                                  0x1
#define _IOCCF_IOCCF1_POSITION                              0x1
#define _IOCCF_IOCCF1_SIZE                                  0x1
#define _IOCCF_IOCCF1_LENGTH                                0x1
#define _IOCCF_IOCCF1_MASK                                  0x2
#define _IOCCF_IOCCF2_POSN                                  0x2
#define _IOCCF_IOCCF2_POSITION                              0x2
#define _IOCCF_IOCCF2_SIZE                                  0x1
#define _IOCCF_IOCCF2_LENGTH                                0x1
#define _IOCCF_IOCCF2_MASK                                  0x4
#define _IOCCF_IOCCF3_POSN                                  0x3
#define _IOCCF_IOCCF3_POSITION                              0x3
#define _IOCCF_IOCCF3_SIZE                                  0x1
#define _IOCCF_IOCCF3_LENGTH                                0x1
#define _IOCCF_IOCCF3_MASK                                  0x8
#define _IOCCF_IOCCF4_POSN                                  0x4
#define _IOCCF_IOCCF4_POSITION                              0x4
#define _IOCCF_IOCCF4_SIZE                                  0x1
#define _IOCCF_IOCCF4_LENGTH                                0x1
#define _IOCCF_IOCCF4_MASK                                  0x10
#define _IOCCF_IOCCF5_POSN                                  0x5
#define _IOCCF_IOCCF5_POSITION                              0x5
#define _IOCCF_IOCCF5_SIZE                                  0x1
#define _IOCCF_IOCCF5_LENGTH                                0x1
#define _IOCCF_IOCCF5_MASK                                  0x20
#define _IOCCF_IOCCF6_POSN                                  0x6
#define _IOCCF_IOCCF6_POSITION                              0x6
#define _IOCCF_IOCCF6_SIZE                                  0x1
#define _IOCCF_IOCCF6_LENGTH                                0x1
#define _IOCCF_IOCCF6_MASK                                  0x40
#define _IOCCF_IOCCF7_POSN                                  0x7
#define _IOCCF_IOCCF7_POSITION                              0x7
#define _IOCCF_IOCCF7_SIZE                                  0x1
#define _IOCCF_IOCCF7_LENGTH                                0x1
#define _IOCCF_IOCCF7_MASK                                  0x80

// Register: TMR4
#define TMR4 TMR4
extern volatile unsigned char           TMR4                __at(0x415);
#ifndef _LIB_BUILD
asm("TMR4 equ 0415h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TMR4                   :8;
    };
} TMR4bits_t;
extern volatile TMR4bits_t TMR4bits __at(0x415);
// bitfield macros
#define _TMR4_TMR4_POSN                                     0x0
#define _TMR4_TMR4_POSITION                                 0x0
#define _TMR4_TMR4_SIZE                                     0x8
#define _TMR4_TMR4_LENGTH                                   0x8
#define _TMR4_TMR4_MASK                                     0xFF

// Register: PR4
#define PR4 PR4
extern volatile unsigned char           PR4                 __at(0x416);
#ifndef _LIB_BUILD
asm("PR4 equ 0416h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PR4                    :8;
    };
} PR4bits_t;
extern volatile PR4bits_t PR4bits __at(0x416);
// bitfield macros
#define _PR4_PR4_POSN                                       0x0
#define _PR4_PR4_POSITION                                   0x0
#define _PR4_PR4_SIZE                                       0x8
#define _PR4_PR4_LENGTH                                     0x8
#define _PR4_PR4_MASK                                       0xFF

// Register: T4CON
#define T4CON T4CON
extern volatile unsigned char           T4CON               __at(0x417);
#ifndef _LIB_BUILD
asm("T4CON equ 0417h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned T4CKPS                 :2;
        unsigned TMR4ON                 :1;
        unsigned T4OUTPS                :4;
    };
    struct {
        unsigned T4CKPS0                :1;
        unsigned T4CKPS1                :1;
        unsigned                        :1;
        unsigned T4OUTPS0               :1;
        unsigned T4OUTPS1               :1;
        unsigned T4OUTPS2               :1;
        unsigned T4OUTPS3               :1;
    };
} T4CONbits_t;
extern volatile T4CONbits_t T4CONbits __at(0x417);
// bitfield macros
#define _T4CON_T4CKPS_POSN                                  0x0
#define _T4CON_T4CKPS_POSITION                              0x0
#define _T4CON_T4CKPS_SIZE                                  0x2
#define _T4CON_T4CKPS_LENGTH                                0x2
#define _T4CON_T4CKPS_MASK                                  0x3
#define _T4CON_TMR4ON_POSN                                  0x2
#define _T4CON_TMR4ON_POSITION                              0x2
#define _T4CON_TMR4ON_SIZE                                  0x1
#define _T4CON_TMR4ON_LENGTH                                0x1
#define _T4CON_TMR4ON_MASK                                  0x4
#define _T4CON_T4OUTPS_POSN                                 0x3
#define _T4CON_T4OUTPS_POSITION                             0x3
#define _T4CON_T4OUTPS_SIZE                                 0x4
#define _T4CON_T4OUTPS_LENGTH                               0x4
#define _T4CON_T4OUTPS_MASK                                 0x78
#define _T4CON_T4CKPS0_POSN                                 0x0
#define _T4CON_T4CKPS0_POSITION                             0x0
#define _T4CON_T4CKPS0_SIZE                                 0x1
#define _T4CON_T4CKPS0_LENGTH                               0x1
#define _T4CON_T4CKPS0_MASK                                 0x1
#define _T4CON_T4CKPS1_POSN                                 0x1
#define _T4CON_T4CKPS1_POSITION                             0x1
#define _T4CON_T4CKPS1_SIZE                                 0x1
#define _T4CON_T4CKPS1_LENGTH                               0x1
#define _T4CON_T4CKPS1_MASK                                 0x2
#define _T4CON_T4OUTPS0_POSN                                0x3
#define _T4CON_T4OUTPS0_POSITION                            0x3
#define _T4CON_T4OUTPS0_SIZE                                0x1
#define _T4CON_T4OUTPS0_LENGTH                              0x1
#define _T4CON_T4OUTPS0_MASK                                0x8
#define _T4CON_T4OUTPS1_POSN                                0x4
#define _T4CON_T4OUTPS1_POSITION                            0x4
#define _T4CON_T4OUTPS1_SIZE                                0x1
#define _T4CON_T4OUTPS1_LENGTH                              0x1
#define _T4CON_T4OUTPS1_MASK                                0x10
#define _T4CON_T4OUTPS2_POSN                                0x5
#define _T4CON_T4OUTPS2_POSITION                            0x5
#define _T4CON_T4OUTPS2_SIZE                                0x1
#define _T4CON_T4OUTPS2_LENGTH                              0x1
#define _T4CON_T4OUTPS2_MASK                                0x20
#define _T4CON_T4OUTPS3_POSN                                0x6
#define _T4CON_T4OUTPS3_POSITION                            0x6
#define _T4CON_T4OUTPS3_SIZE                                0x1
#define _T4CON_T4OUTPS3_LENGTH                              0x1
#define _T4CON_T4OUTPS3_MASK                                0x40

// Register: TMR6
#define TMR6 TMR6
extern volatile unsigned char           TMR6                __at(0x41C);
#ifndef _LIB_BUILD
asm("TMR6 equ 041Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TMR6                   :8;
    };
} TMR6bits_t;
extern volatile TMR6bits_t TMR6bits __at(0x41C);
// bitfield macros
#define _TMR6_TMR6_POSN                                     0x0
#define _TMR6_TMR6_POSITION                                 0x0
#define _TMR6_TMR6_SIZE                                     0x8
#define _TMR6_TMR6_LENGTH                                   0x8
#define _TMR6_TMR6_MASK                                     0xFF

// Register: PR6
#define PR6 PR6
extern volatile unsigned char           PR6                 __at(0x41D);
#ifndef _LIB_BUILD
asm("PR6 equ 041Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PR6                    :8;
    };
} PR6bits_t;
extern volatile PR6bits_t PR6bits __at(0x41D);
// bitfield macros
#define _PR6_PR6_POSN                                       0x0
#define _PR6_PR6_POSITION                                   0x0
#define _PR6_PR6_SIZE                                       0x8
#define _PR6_PR6_LENGTH                                     0x8
#define _PR6_PR6_MASK                                       0xFF

// Register: T6CON
#define T6CON T6CON
extern volatile unsigned char           T6CON               __at(0x41E);
#ifndef _LIB_BUILD
asm("T6CON equ 041Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned T6CKPS                 :2;
        unsigned TMR6ON                 :1;
        unsigned T6OUTPS                :4;
    };
    struct {
        unsigned T6CKPS0                :1;
        unsigned T6CKPS1                :1;
        unsigned                        :1;
        unsigned T6OUTPS0               :1;
        unsigned T6OUTPS1               :1;
        unsigned T6OUTPS2               :1;
        unsigned T6OUTPS3               :1;
    };
} T6CONbits_t;
extern volatile T6CONbits_t T6CONbits __at(0x41E);
// bitfield macros
#define _T6CON_T6CKPS_POSN                                  0x0
#define _T6CON_T6CKPS_POSITION                              0x0
#define _T6CON_T6CKPS_SIZE                                  0x2
#define _T6CON_T6CKPS_LENGTH                                0x2
#define _T6CON_T6CKPS_MASK                                  0x3
#define _T6CON_TMR6ON_POSN                                  0x2
#define _T6CON_TMR6ON_POSITION                              0x2
#define _T6CON_TMR6ON_SIZE                                  0x1
#define _T6CON_TMR6ON_LENGTH                                0x1
#define _T6CON_TMR6ON_MASK                                  0x4
#define _T6CON_T6OUTPS_POSN                                 0x3
#define _T6CON_T6OUTPS_POSITION                             0x3
#define _T6CON_T6OUTPS_SIZE                                 0x4
#define _T6CON_T6OUTPS_LENGTH                               0x4
#define _T6CON_T6OUTPS_MASK                                 0x78
#define _T6CON_T6CKPS0_POSN                                 0x0
#define _T6CON_T6CKPS0_POSITION                             0x0
#define _T6CON_T6CKPS0_SIZE                                 0x1
#define _T6CON_T6CKPS0_LENGTH                               0x1
#define _T6CON_T6CKPS0_MASK                                 0x1
#define _T6CON_T6CKPS1_POSN                                 0x1
#define _T6CON_T6CKPS1_POSITION                             0x1
#define _T6CON_T6CKPS1_SIZE                                 0x1
#define _T6CON_T6CKPS1_LENGTH                               0x1
#define _T6CON_T6CKPS1_MASK                                 0x2
#define _T6CON_T6OUTPS0_POSN                                0x3
#define _T6CON_T6OUTPS0_POSITION                            0x3
#define _T6CON_T6OUTPS0_SIZE                                0x1
#define _T6CON_T6OUTPS0_LENGTH                              0x1
#define _T6CON_T6OUTPS0_MASK                                0x8
#define _T6CON_T6OUTPS1_POSN                                0x4
#define _T6CON_T6OUTPS1_POSITION                            0x4
#define _T6CON_T6OUTPS1_SIZE                                0x1
#define _T6CON_T6OUTPS1_LENGTH                              0x1
#define _T6CON_T6OUTPS1_MASK                                0x10
#define _T6CON_T6OUTPS2_POSN                                0x5
#define _T6CON_T6OUTPS2_POSITION                            0x5
#define _T6CON_T6OUTPS2_SIZE                                0x1
#define _T6CON_T6OUTPS2_LENGTH                              0x1
#define _T6CON_T6OUTPS2_MASK                                0x20
#define _T6CON_T6OUTPS3_POSN                                0x6
#define _T6CON_T6OUTPS3_POSITION                            0x6
#define _T6CON_T6OUTPS3_SIZE                                0x1
#define _T6CON_T6OUTPS3_LENGTH                              0x1
#define _T6CON_T6OUTPS3_MASK                                0x40

// Register: OPA1CON
#define OPA1CON OPA1CON
extern volatile unsigned char           OPA1CON             __at(0x511);
#ifndef _LIB_BUILD
asm("OPA1CON equ 0511h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned OPA1PCH                :2;
        unsigned                        :2;
        unsigned OPA1UG                 :1;
        unsigned                        :1;
        unsigned OPA1SP                 :1;
        unsigned OPA1EN                 :1;
    };
    struct {
        unsigned OPA1PCH0               :1;
        unsigned OPA1PCH1               :1;
    };
} OPA1CONbits_t;
extern volatile OPA1CONbits_t OPA1CONbits __at(0x511);
// bitfield macros
#define _OPA1CON_OPA1PCH_POSN                               0x0
#define _OPA1CON_OPA1PCH_POSITION                           0x0
#define _OPA1CON_OPA1PCH_SIZE                               0x2
#define _OPA1CON_OPA1PCH_LENGTH                             0x2
#define _OPA1CON_OPA1PCH_MASK                               0x3
#define _OPA1CON_OPA1UG_POSN                                0x4
#define _OPA1CON_OPA1UG_POSITION                            0x4
#define _OPA1CON_OPA1UG_SIZE                                0x1
#define _OPA1CON_OPA1UG_LENGTH                              0x1
#define _OPA1CON_OPA1UG_MASK                                0x10
#define _OPA1CON_OPA1SP_POSN                                0x6
#define _OPA1CON_OPA1SP_POSITION                            0x6
#define _OPA1CON_OPA1SP_SIZE                                0x1
#define _OPA1CON_OPA1SP_LENGTH                              0x1
#define _OPA1CON_OPA1SP_MASK                                0x40
#define _OPA1CON_OPA1EN_POSN                                0x7
#define _OPA1CON_OPA1EN_POSITION                            0x7
#define _OPA1CON_OPA1EN_SIZE                                0x1
#define _OPA1CON_OPA1EN_LENGTH                              0x1
#define _OPA1CON_OPA1EN_MASK                                0x80
#define _OPA1CON_OPA1PCH0_POSN                              0x0
#define _OPA1CON_OPA1PCH0_POSITION                          0x0
#define _OPA1CON_OPA1PCH0_SIZE                              0x1
#define _OPA1CON_OPA1PCH0_LENGTH                            0x1
#define _OPA1CON_OPA1PCH0_MASK                              0x1
#define _OPA1CON_OPA1PCH1_POSN                              0x1
#define _OPA1CON_OPA1PCH1_POSITION                          0x1
#define _OPA1CON_OPA1PCH1_SIZE                              0x1
#define _OPA1CON_OPA1PCH1_LENGTH                            0x1
#define _OPA1CON_OPA1PCH1_MASK                              0x2

// Register: OPA2CON
#define OPA2CON OPA2CON
extern volatile unsigned char           OPA2CON             __at(0x515);
#ifndef _LIB_BUILD
asm("OPA2CON equ 0515h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned OPA2PCH                :2;
        unsigned                        :2;
        unsigned OPA2UG                 :1;
        unsigned                        :1;
        unsigned OPA2SP                 :1;
        unsigned OPA2EN                 :1;
    };
    struct {
        unsigned OPA2PCH0               :1;
        unsigned OPA2PCH1               :1;
    };
} OPA2CONbits_t;
extern volatile OPA2CONbits_t OPA2CONbits __at(0x515);
// bitfield macros
#define _OPA2CON_OPA2PCH_POSN                               0x0
#define _OPA2CON_OPA2PCH_POSITION                           0x0
#define _OPA2CON_OPA2PCH_SIZE                               0x2
#define _OPA2CON_OPA2PCH_LENGTH                             0x2
#define _OPA2CON_OPA2PCH_MASK                               0x3
#define _OPA2CON_OPA2UG_POSN                                0x4
#define _OPA2CON_OPA2UG_POSITION                            0x4
#define _OPA2CON_OPA2UG_SIZE                                0x1
#define _OPA2CON_OPA2UG_LENGTH                              0x1
#define _OPA2CON_OPA2UG_MASK                                0x10
#define _OPA2CON_OPA2SP_POSN                                0x6
#define _OPA2CON_OPA2SP_POSITION                            0x6
#define _OPA2CON_OPA2SP_SIZE                                0x1
#define _OPA2CON_OPA2SP_LENGTH                              0x1
#define _OPA2CON_OPA2SP_MASK                                0x40
#define _OPA2CON_OPA2EN_POSN                                0x7
#define _OPA2CON_OPA2EN_POSITION                            0x7
#define _OPA2CON_OPA2EN_SIZE                                0x1
#define _OPA2CON_OPA2EN_LENGTH                              0x1
#define _OPA2CON_OPA2EN_MASK                                0x80
#define _OPA2CON_OPA2PCH0_POSN                              0x0
#define _OPA2CON_OPA2PCH0_POSITION                          0x0
#define _OPA2CON_OPA2PCH0_SIZE                              0x1
#define _OPA2CON_OPA2PCH0_LENGTH                            0x1
#define _OPA2CON_OPA2PCH0_MASK                              0x1
#define _OPA2CON_OPA2PCH1_POSN                              0x1
#define _OPA2CON_OPA2PCH1_POSITION                          0x1
#define _OPA2CON_OPA2PCH1_SIZE                              0x1
#define _OPA2CON_OPA2PCH1_LENGTH                            0x1
#define _OPA2CON_OPA2PCH1_MASK                              0x2

// Register: PWM3DCL
#define PWM3DCL PWM3DCL
extern volatile unsigned char           PWM3DCL             __at(0x617);
#ifndef _LIB_BUILD
asm("PWM3DCL equ 0617h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :6;
        unsigned PWM3DCL                :2;
    };
    struct {
        unsigned                        :6;
        unsigned PWM3DCL0               :1;
        unsigned PWM3DCL1               :1;
    };
} PWM3DCLbits_t;
extern volatile PWM3DCLbits_t PWM3DCLbits __at(0x617);
// bitfield macros
#define _PWM3DCL_PWM3DCL_POSN                               0x6
#define _PWM3DCL_PWM3DCL_POSITION                           0x6
#define _PWM3DCL_PWM3DCL_SIZE                               0x2
#define _PWM3DCL_PWM3DCL_LENGTH                             0x2
#define _PWM3DCL_PWM3DCL_MASK                               0xC0
#define _PWM3DCL_PWM3DCL0_POSN                              0x6
#define _PWM3DCL_PWM3DCL0_POSITION                          0x6
#define _PWM3DCL_PWM3DCL0_SIZE                              0x1
#define _PWM3DCL_PWM3DCL0_LENGTH                            0x1
#define _PWM3DCL_PWM3DCL0_MASK                              0x40
#define _PWM3DCL_PWM3DCL1_POSN                              0x7
#define _PWM3DCL_PWM3DCL1_POSITION                          0x7
#define _PWM3DCL_PWM3DCL1_SIZE                              0x1
#define _PWM3DCL_PWM3DCL1_LENGTH                            0x1
#define _PWM3DCL_PWM3DCL1_MASK                              0x80

// Register: PWM3DCH
#define PWM3DCH PWM3DCH
extern volatile unsigned char           PWM3DCH             __at(0x618);
#ifndef _LIB_BUILD
asm("PWM3DCH equ 0618h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PWM3DCH                :8;
    };
    struct {
        unsigned PWM3DCH0               :1;
        unsigned PWM3DCH1               :1;
        unsigned PWM3DCH2               :1;
        unsigned PWM3DCH3               :1;
        unsigned PWM3DCH4               :1;
        unsigned PWM3DCH5               :1;
        unsigned PWM3DCH6               :1;
        unsigned PWM3DCH7               :1;
    };
} PWM3DCHbits_t;
extern volatile PWM3DCHbits_t PWM3DCHbits __at(0x618);
// bitfield macros
#define _PWM3DCH_PWM3DCH_POSN                               0x0
#define _PWM3DCH_PWM3DCH_POSITION                           0x0
#define _PWM3DCH_PWM3DCH_SIZE                               0x8
#define _PWM3DCH_PWM3DCH_LENGTH                             0x8
#define _PWM3DCH_PWM3DCH_MASK                               0xFF
#define _PWM3DCH_PWM3DCH0_POSN                              0x0
#define _PWM3DCH_PWM3DCH0_POSITION                          0x0
#define _PWM3DCH_PWM3DCH0_SIZE                              0x1
#define _PWM3DCH_PWM3DCH0_LENGTH                            0x1
#define _PWM3DCH_PWM3DCH0_MASK                              0x1
#define _PWM3DCH_PWM3DCH1_POSN                              0x1
#define _PWM3DCH_PWM3DCH1_POSITION                          0x1
#define _PWM3DCH_PWM3DCH1_SIZE                              0x1
#define _PWM3DCH_PWM3DCH1_LENGTH                            0x1
#define _PWM3DCH_PWM3DCH1_MASK                              0x2
#define _PWM3DCH_PWM3DCH2_POSN                              0x2
#define _PWM3DCH_PWM3DCH2_POSITION                          0x2
#define _PWM3DCH_PWM3DCH2_SIZE                              0x1
#define _PWM3DCH_PWM3DCH2_LENGTH                            0x1
#define _PWM3DCH_PWM3DCH2_MASK                              0x4
#define _PWM3DCH_PWM3DCH3_POSN                              0x3
#define _PWM3DCH_PWM3DCH3_POSITION                          0x3
#define _PWM3DCH_PWM3DCH3_SIZE                              0x1
#define _PWM3DCH_PWM3DCH3_LENGTH                            0x1
#define _PWM3DCH_PWM3DCH3_MASK                              0x8
#define _PWM3DCH_PWM3DCH4_POSN                              0x4
#define _PWM3DCH_PWM3DCH4_POSITION                          0x4
#define _PWM3DCH_PWM3DCH4_SIZE                              0x1
#define _PWM3DCH_PWM3DCH4_LENGTH                            0x1
#define _PWM3DCH_PWM3DCH4_MASK                              0x10
#define _PWM3DCH_PWM3DCH5_POSN                              0x5
#define _PWM3DCH_PWM3DCH5_POSITION                          0x5
#define _PWM3DCH_PWM3DCH5_SIZE                              0x1
#define _PWM3DCH_PWM3DCH5_LENGTH                            0x1
#define _PWM3DCH_PWM3DCH5_MASK                              0x20
#define _PWM3DCH_PWM3DCH6_POSN                              0x6
#define _PWM3DCH_PWM3DCH6_POSITION                          0x6
#define _PWM3DCH_PWM3DCH6_SIZE                              0x1
#define _PWM3DCH_PWM3DCH6_LENGTH                            0x1
#define _PWM3DCH_PWM3DCH6_MASK                              0x40
#define _PWM3DCH_PWM3DCH7_POSN                              0x7
#define _PWM3DCH_PWM3DCH7_POSITION                          0x7
#define _PWM3DCH_PWM3DCH7_SIZE                              0x1
#define _PWM3DCH_PWM3DCH7_LENGTH                            0x1
#define _PWM3DCH_PWM3DCH7_MASK                              0x80

// Register: PWM3CON
#define PWM3CON PWM3CON
extern volatile unsigned char           PWM3CON             __at(0x619);
#ifndef _LIB_BUILD
asm("PWM3CON equ 0619h");
#endif
// aliases
extern volatile unsigned char           PWM3CON0            __at(0x619);
#ifndef _LIB_BUILD
asm("PWM3CON0 equ 0619h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned PWM3POL                :1;
        unsigned PWM3OUT                :1;
        unsigned                        :1;
        unsigned PWM3EN                 :1;
    };
} PWM3CONbits_t;
extern volatile PWM3CONbits_t PWM3CONbits __at(0x619);
// bitfield macros
#define _PWM3CON_PWM3POL_POSN                               0x4
#define _PWM3CON_PWM3POL_POSITION                           0x4
#define _PWM3CON_PWM3POL_SIZE                               0x1
#define _PWM3CON_PWM3POL_LENGTH                             0x1
#define _PWM3CON_PWM3POL_MASK                               0x10
#define _PWM3CON_PWM3OUT_POSN                               0x5
#define _PWM3CON_PWM3OUT_POSITION                           0x5
#define _PWM3CON_PWM3OUT_SIZE                               0x1
#define _PWM3CON_PWM3OUT_LENGTH                             0x1
#define _PWM3CON_PWM3OUT_MASK                               0x20
#define _PWM3CON_PWM3EN_POSN                                0x7
#define _PWM3CON_PWM3EN_POSITION                            0x7
#define _PWM3CON_PWM3EN_SIZE                                0x1
#define _PWM3CON_PWM3EN_LENGTH                              0x1
#define _PWM3CON_PWM3EN_MASK                                0x80
// alias bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned PWM3POL                :1;
        unsigned PWM3OUT                :1;
        unsigned                        :1;
        unsigned PWM3EN                 :1;
    };
} PWM3CON0bits_t;
extern volatile PWM3CON0bits_t PWM3CON0bits __at(0x619);
// bitfield macros
#define _PWM3CON0_PWM3POL_POSN                              0x4
#define _PWM3CON0_PWM3POL_POSITION                          0x4
#define _PWM3CON0_PWM3POL_SIZE                              0x1
#define _PWM3CON0_PWM3POL_LENGTH                            0x1
#define _PWM3CON0_PWM3POL_MASK                              0x10
#define _PWM3CON0_PWM3OUT_POSN                              0x5
#define _PWM3CON0_PWM3OUT_POSITION                          0x5
#define _PWM3CON0_PWM3OUT_SIZE                              0x1
#define _PWM3CON0_PWM3OUT_LENGTH                            0x1
#define _PWM3CON0_PWM3OUT_MASK                              0x20
#define _PWM3CON0_PWM3EN_POSN                               0x7
#define _PWM3CON0_PWM3EN_POSITION                           0x7
#define _PWM3CON0_PWM3EN_SIZE                               0x1
#define _PWM3CON0_PWM3EN_LENGTH                             0x1
#define _PWM3CON0_PWM3EN_MASK                               0x80

// Register: PWM4DCL
#define PWM4DCL PWM4DCL
extern volatile unsigned char           PWM4DCL             __at(0x61A);
#ifndef _LIB_BUILD
asm("PWM4DCL equ 061Ah");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :6;
        unsigned PWM4DCL                :2;
    };
    struct {
        unsigned                        :6;
        unsigned PWM4DCL0               :1;
        unsigned PWM4DCL1               :1;
    };
} PWM4DCLbits_t;
extern volatile PWM4DCLbits_t PWM4DCLbits __at(0x61A);
// bitfield macros
#define _PWM4DCL_PWM4DCL_POSN                               0x6
#define _PWM4DCL_PWM4DCL_POSITION                           0x6
#define _PWM4DCL_PWM4DCL_SIZE                               0x2
#define _PWM4DCL_PWM4DCL_LENGTH                             0x2
#define _PWM4DCL_PWM4DCL_MASK                               0xC0
#define _PWM4DCL_PWM4DCL0_POSN                              0x6
#define _PWM4DCL_PWM4DCL0_POSITION                          0x6
#define _PWM4DCL_PWM4DCL0_SIZE                              0x1
#define _PWM4DCL_PWM4DCL0_LENGTH                            0x1
#define _PWM4DCL_PWM4DCL0_MASK                              0x40
#define _PWM4DCL_PWM4DCL1_POSN                              0x7
#define _PWM4DCL_PWM4DCL1_POSITION                          0x7
#define _PWM4DCL_PWM4DCL1_SIZE                              0x1
#define _PWM4DCL_PWM4DCL1_LENGTH                            0x1
#define _PWM4DCL_PWM4DCL1_MASK                              0x80

// Register: PWM4DCH
#define PWM4DCH PWM4DCH
extern volatile unsigned char           PWM4DCH             __at(0x61B);
#ifndef _LIB_BUILD
asm("PWM4DCH equ 061Bh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PWM4DCH                :8;
    };
    struct {
        unsigned PWM4DCH0               :1;
        unsigned PWM4DCH1               :1;
        unsigned PWM4DCH2               :1;
        unsigned PWM4DCH3               :1;
        unsigned PWM4DCH4               :1;
        unsigned PWM4DCH5               :1;
        unsigned PWM4DCH6               :1;
        unsigned PWM4DCH7               :1;
    };
} PWM4DCHbits_t;
extern volatile PWM4DCHbits_t PWM4DCHbits __at(0x61B);
// bitfield macros
#define _PWM4DCH_PWM4DCH_POSN                               0x0
#define _PWM4DCH_PWM4DCH_POSITION                           0x0
#define _PWM4DCH_PWM4DCH_SIZE                               0x8
#define _PWM4DCH_PWM4DCH_LENGTH                             0x8
#define _PWM4DCH_PWM4DCH_MASK                               0xFF
#define _PWM4DCH_PWM4DCH0_POSN                              0x0
#define _PWM4DCH_PWM4DCH0_POSITION                          0x0
#define _PWM4DCH_PWM4DCH0_SIZE                              0x1
#define _PWM4DCH_PWM4DCH0_LENGTH                            0x1
#define _PWM4DCH_PWM4DCH0_MASK                              0x1
#define _PWM4DCH_PWM4DCH1_POSN                              0x1
#define _PWM4DCH_PWM4DCH1_POSITION                          0x1
#define _PWM4DCH_PWM4DCH1_SIZE                              0x1
#define _PWM4DCH_PWM4DCH1_LENGTH                            0x1
#define _PWM4DCH_PWM4DCH1_MASK                              0x2
#define _PWM4DCH_PWM4DCH2_POSN                              0x2
#define _PWM4DCH_PWM4DCH2_POSITION                          0x2
#define _PWM4DCH_PWM4DCH2_SIZE                              0x1
#define _PWM4DCH_PWM4DCH2_LENGTH                            0x1
#define _PWM4DCH_PWM4DCH2_MASK                              0x4
#define _PWM4DCH_PWM4DCH3_POSN                              0x3
#define _PWM4DCH_PWM4DCH3_POSITION                          0x3
#define _PWM4DCH_PWM4DCH3_SIZE                              0x1
#define _PWM4DCH_PWM4DCH3_LENGTH                            0x1
#define _PWM4DCH_PWM4DCH3_MASK                              0x8
#define _PWM4DCH_PWM4DCH4_POSN                              0x4
#define _PWM4DCH_PWM4DCH4_POSITION                          0x4
#define _PWM4DCH_PWM4DCH4_SIZE                              0x1
#define _PWM4DCH_PWM4DCH4_LENGTH                            0x1
#define _PWM4DCH_PWM4DCH4_MASK                              0x10
#define _PWM4DCH_PWM4DCH5_POSN                              0x5
#define _PWM4DCH_PWM4DCH5_POSITION                          0x5
#define _PWM4DCH_PWM4DCH5_SIZE                              0x1
#define _PWM4DCH_PWM4DCH5_LENGTH                            0x1
#define _PWM4DCH_PWM4DCH5_MASK                              0x20
#define _PWM4DCH_PWM4DCH6_POSN                              0x6
#define _PWM4DCH_PWM4DCH6_POSITION                          0x6
#define _PWM4DCH_PWM4DCH6_SIZE                              0x1
#define _PWM4DCH_PWM4DCH6_LENGTH                            0x1
#define _PWM4DCH_PWM4DCH6_MASK                              0x40
#define _PWM4DCH_PWM4DCH7_POSN                              0x7
#define _PWM4DCH_PWM4DCH7_POSITION                          0x7
#define _PWM4DCH_PWM4DCH7_SIZE                              0x1
#define _PWM4DCH_PWM4DCH7_LENGTH                            0x1
#define _PWM4DCH_PWM4DCH7_MASK                              0x80

// Register: PWM4CON
#define PWM4CON PWM4CON
extern volatile unsigned char           PWM4CON             __at(0x61C);
#ifndef _LIB_BUILD
asm("PWM4CON equ 061Ch");
#endif
// aliases
extern volatile unsigned char           PWM4CON0            __at(0x61C);
#ifndef _LIB_BUILD
asm("PWM4CON0 equ 061Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned PWM4POL                :1;
        unsigned PWM4OUT                :1;
        unsigned                        :1;
        unsigned PWM4EN                 :1;
    };
} PWM4CONbits_t;
extern volatile PWM4CONbits_t PWM4CONbits __at(0x61C);
// bitfield macros
#define _PWM4CON_PWM4POL_POSN                               0x4
#define _PWM4CON_PWM4POL_POSITION                           0x4
#define _PWM4CON_PWM4POL_SIZE                               0x1
#define _PWM4CON_PWM4POL_LENGTH                             0x1
#define _PWM4CON_PWM4POL_MASK                               0x10
#define _PWM4CON_PWM4OUT_POSN                               0x5
#define _PWM4CON_PWM4OUT_POSITION                           0x5
#define _PWM4CON_PWM4OUT_SIZE                               0x1
#define _PWM4CON_PWM4OUT_LENGTH                             0x1
#define _PWM4CON_PWM4OUT_MASK                               0x20
#define _PWM4CON_PWM4EN_POSN                                0x7
#define _PWM4CON_PWM4EN_POSITION                            0x7
#define _PWM4CON_PWM4EN_SIZE                                0x1
#define _PWM4CON_PWM4EN_LENGTH                              0x1
#define _PWM4CON_PWM4EN_MASK                                0x80
// alias bitfield definitions
typedef union {
    struct {
        unsigned                        :4;
        unsigned PWM4POL                :1;
        unsigned PWM4OUT                :1;
        unsigned                        :1;
        unsigned PWM4EN                 :1;
    };
} PWM4CON0bits_t;
extern volatile PWM4CON0bits_t PWM4CON0bits __at(0x61C);
// bitfield macros
#define _PWM4CON0_PWM4POL_POSN                              0x4
#define _PWM4CON0_PWM4POL_POSITION                          0x4
#define _PWM4CON0_PWM4POL_SIZE                              0x1
#define _PWM4CON0_PWM4POL_LENGTH                            0x1
#define _PWM4CON0_PWM4POL_MASK                              0x10
#define _PWM4CON0_PWM4OUT_POSN                              0x5
#define _PWM4CON0_PWM4OUT_POSITION                          0x5
#define _PWM4CON0_PWM4OUT_SIZE                              0x1
#define _PWM4CON0_PWM4OUT_LENGTH                            0x1
#define _PWM4CON0_PWM4OUT_MASK                              0x20
#define _PWM4CON0_PWM4EN_POSN                               0x7
#define _PWM4CON0_PWM4EN_POSITION                           0x7
#define _PWM4CON0_PWM4EN_SIZE                               0x1
#define _PWM4CON0_PWM4EN_LENGTH                             0x1
#define _PWM4CON0_PWM4EN_MASK                               0x80

// Register: COG1PHR
#define COG1PHR COG1PHR
extern volatile unsigned char           COG1PHR             __at(0x691);
#ifndef _LIB_BUILD
asm("COG1PHR equ 0691h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1PHR                  :6;
    };
    struct {
        unsigned G1PHR0                 :1;
        unsigned G1PHR1                 :1;
        unsigned G1PHR2                 :1;
        unsigned G1PHR3                 :1;
        unsigned G1PHR4                 :1;
        unsigned G1PHR5                 :1;
    };
} COG1PHRbits_t;
extern volatile COG1PHRbits_t COG1PHRbits __at(0x691);
// bitfield macros
#define _COG1PHR_G1PHR_POSN                                 0x0
#define _COG1PHR_G1PHR_POSITION                             0x0
#define _COG1PHR_G1PHR_SIZE                                 0x6
#define _COG1PHR_G1PHR_LENGTH                               0x6
#define _COG1PHR_G1PHR_MASK                                 0x3F
#define _COG1PHR_G1PHR0_POSN                                0x0
#define _COG1PHR_G1PHR0_POSITION                            0x0
#define _COG1PHR_G1PHR0_SIZE                                0x1
#define _COG1PHR_G1PHR0_LENGTH                              0x1
#define _COG1PHR_G1PHR0_MASK                                0x1
#define _COG1PHR_G1PHR1_POSN                                0x1
#define _COG1PHR_G1PHR1_POSITION                            0x1
#define _COG1PHR_G1PHR1_SIZE                                0x1
#define _COG1PHR_G1PHR1_LENGTH                              0x1
#define _COG1PHR_G1PHR1_MASK                                0x2
#define _COG1PHR_G1PHR2_POSN                                0x2
#define _COG1PHR_G1PHR2_POSITION                            0x2
#define _COG1PHR_G1PHR2_SIZE                                0x1
#define _COG1PHR_G1PHR2_LENGTH                              0x1
#define _COG1PHR_G1PHR2_MASK                                0x4
#define _COG1PHR_G1PHR3_POSN                                0x3
#define _COG1PHR_G1PHR3_POSITION                            0x3
#define _COG1PHR_G1PHR3_SIZE                                0x1
#define _COG1PHR_G1PHR3_LENGTH                              0x1
#define _COG1PHR_G1PHR3_MASK                                0x8
#define _COG1PHR_G1PHR4_POSN                                0x4
#define _COG1PHR_G1PHR4_POSITION                            0x4
#define _COG1PHR_G1PHR4_SIZE                                0x1
#define _COG1PHR_G1PHR4_LENGTH                              0x1
#define _COG1PHR_G1PHR4_MASK                                0x10
#define _COG1PHR_G1PHR5_POSN                                0x5
#define _COG1PHR_G1PHR5_POSITION                            0x5
#define _COG1PHR_G1PHR5_SIZE                                0x1
#define _COG1PHR_G1PHR5_LENGTH                              0x1
#define _COG1PHR_G1PHR5_MASK                                0x20

// Register: COG1PHF
#define COG1PHF COG1PHF
extern volatile unsigned char           COG1PHF             __at(0x692);
#ifndef _LIB_BUILD
asm("COG1PHF equ 0692h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1PHF                  :6;
    };
    struct {
        unsigned G1PHF0                 :1;
        unsigned G1PHF1                 :1;
        unsigned G1PHF2                 :1;
        unsigned G1PHF3                 :1;
        unsigned G1PHF4                 :1;
        unsigned G1PHF5                 :1;
    };
} COG1PHFbits_t;
extern volatile COG1PHFbits_t COG1PHFbits __at(0x692);
// bitfield macros
#define _COG1PHF_G1PHF_POSN                                 0x0
#define _COG1PHF_G1PHF_POSITION                             0x0
#define _COG1PHF_G1PHF_SIZE                                 0x6
#define _COG1PHF_G1PHF_LENGTH                               0x6
#define _COG1PHF_G1PHF_MASK                                 0x3F
#define _COG1PHF_G1PHF0_POSN                                0x0
#define _COG1PHF_G1PHF0_POSITION                            0x0
#define _COG1PHF_G1PHF0_SIZE                                0x1
#define _COG1PHF_G1PHF0_LENGTH                              0x1
#define _COG1PHF_G1PHF0_MASK                                0x1
#define _COG1PHF_G1PHF1_POSN                                0x1
#define _COG1PHF_G1PHF1_POSITION                            0x1
#define _COG1PHF_G1PHF1_SIZE                                0x1
#define _COG1PHF_G1PHF1_LENGTH                              0x1
#define _COG1PHF_G1PHF1_MASK                                0x2
#define _COG1PHF_G1PHF2_POSN                                0x2
#define _COG1PHF_G1PHF2_POSITION                            0x2
#define _COG1PHF_G1PHF2_SIZE                                0x1
#define _COG1PHF_G1PHF2_LENGTH                              0x1
#define _COG1PHF_G1PHF2_MASK                                0x4
#define _COG1PHF_G1PHF3_POSN                                0x3
#define _COG1PHF_G1PHF3_POSITION                            0x3
#define _COG1PHF_G1PHF3_SIZE                                0x1
#define _COG1PHF_G1PHF3_LENGTH                              0x1
#define _COG1PHF_G1PHF3_MASK                                0x8
#define _COG1PHF_G1PHF4_POSN                                0x4
#define _COG1PHF_G1PHF4_POSITION                            0x4
#define _COG1PHF_G1PHF4_SIZE                                0x1
#define _COG1PHF_G1PHF4_LENGTH                              0x1
#define _COG1PHF_G1PHF4_MASK                                0x10
#define _COG1PHF_G1PHF5_POSN                                0x5
#define _COG1PHF_G1PHF5_POSITION                            0x5
#define _COG1PHF_G1PHF5_SIZE                                0x1
#define _COG1PHF_G1PHF5_LENGTH                              0x1
#define _COG1PHF_G1PHF5_MASK                                0x20

// Register: COG1BLKR
#define COG1BLKR COG1BLKR
extern volatile unsigned char           COG1BLKR            __at(0x693);
#ifndef _LIB_BUILD
asm("COG1BLKR equ 0693h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1BLKR                 :6;
    };
    struct {
        unsigned G1BLKR0                :1;
        unsigned G1BLKR1                :1;
        unsigned G1BLKR2                :1;
        unsigned G1BLKR3                :1;
        unsigned G1BLKR4                :1;
        unsigned G1BLKR5                :1;
    };
} COG1BLKRbits_t;
extern volatile COG1BLKRbits_t COG1BLKRbits __at(0x693);
// bitfield macros
#define _COG1BLKR_G1BLKR_POSN                               0x0
#define _COG1BLKR_G1BLKR_POSITION                           0x0
#define _COG1BLKR_G1BLKR_SIZE                               0x6
#define _COG1BLKR_G1BLKR_LENGTH                             0x6
#define _COG1BLKR_G1BLKR_MASK                               0x3F
#define _COG1BLKR_G1BLKR0_POSN                              0x0
#define _COG1BLKR_G1BLKR0_POSITION                          0x0
#define _COG1BLKR_G1BLKR0_SIZE                              0x1
#define _COG1BLKR_G1BLKR0_LENGTH                            0x1
#define _COG1BLKR_G1BLKR0_MASK                              0x1
#define _COG1BLKR_G1BLKR1_POSN                              0x1
#define _COG1BLKR_G1BLKR1_POSITION                          0x1
#define _COG1BLKR_G1BLKR1_SIZE                              0x1
#define _COG1BLKR_G1BLKR1_LENGTH                            0x1
#define _COG1BLKR_G1BLKR1_MASK                              0x2
#define _COG1BLKR_G1BLKR2_POSN                              0x2
#define _COG1BLKR_G1BLKR2_POSITION                          0x2
#define _COG1BLKR_G1BLKR2_SIZE                              0x1
#define _COG1BLKR_G1BLKR2_LENGTH                            0x1
#define _COG1BLKR_G1BLKR2_MASK                              0x4
#define _COG1BLKR_G1BLKR3_POSN                              0x3
#define _COG1BLKR_G1BLKR3_POSITION                          0x3
#define _COG1BLKR_G1BLKR3_SIZE                              0x1
#define _COG1BLKR_G1BLKR3_LENGTH                            0x1
#define _COG1BLKR_G1BLKR3_MASK                              0x8
#define _COG1BLKR_G1BLKR4_POSN                              0x4
#define _COG1BLKR_G1BLKR4_POSITION                          0x4
#define _COG1BLKR_G1BLKR4_SIZE                              0x1
#define _COG1BLKR_G1BLKR4_LENGTH                            0x1
#define _COG1BLKR_G1BLKR4_MASK                              0x10
#define _COG1BLKR_G1BLKR5_POSN                              0x5
#define _COG1BLKR_G1BLKR5_POSITION                          0x5
#define _COG1BLKR_G1BLKR5_SIZE                              0x1
#define _COG1BLKR_G1BLKR5_LENGTH                            0x1
#define _COG1BLKR_G1BLKR5_MASK                              0x20

// Register: COG1BLKF
#define COG1BLKF COG1BLKF
extern volatile unsigned char           COG1BLKF            __at(0x694);
#ifndef _LIB_BUILD
asm("COG1BLKF equ 0694h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1BLKF                 :6;
    };
    struct {
        unsigned G1BLKF0                :1;
        unsigned G1BLKF1                :1;
        unsigned G1BLKF2                :1;
        unsigned G1BLKF3                :1;
        unsigned G1BLKF4                :1;
        unsigned G1BLKF5                :1;
    };
} COG1BLKFbits_t;
extern volatile COG1BLKFbits_t COG1BLKFbits __at(0x694);
// bitfield macros
#define _COG1BLKF_G1BLKF_POSN                               0x0
#define _COG1BLKF_G1BLKF_POSITION                           0x0
#define _COG1BLKF_G1BLKF_SIZE                               0x6
#define _COG1BLKF_G1BLKF_LENGTH                             0x6
#define _COG1BLKF_G1BLKF_MASK                               0x3F
#define _COG1BLKF_G1BLKF0_POSN                              0x0
#define _COG1BLKF_G1BLKF0_POSITION                          0x0
#define _COG1BLKF_G1BLKF0_SIZE                              0x1
#define _COG1BLKF_G1BLKF0_LENGTH                            0x1
#define _COG1BLKF_G1BLKF0_MASK                              0x1
#define _COG1BLKF_G1BLKF1_POSN                              0x1
#define _COG1BLKF_G1BLKF1_POSITION                          0x1
#define _COG1BLKF_G1BLKF1_SIZE                              0x1
#define _COG1BLKF_G1BLKF1_LENGTH                            0x1
#define _COG1BLKF_G1BLKF1_MASK                              0x2
#define _COG1BLKF_G1BLKF2_POSN                              0x2
#define _COG1BLKF_G1BLKF2_POSITION                          0x2
#define _COG1BLKF_G1BLKF2_SIZE                              0x1
#define _COG1BLKF_G1BLKF2_LENGTH                            0x1
#define _COG1BLKF_G1BLKF2_MASK                              0x4
#define _COG1BLKF_G1BLKF3_POSN                              0x3
#define _COG1BLKF_G1BLKF3_POSITION                          0x3
#define _COG1BLKF_G1BLKF3_SIZE                              0x1
#define _COG1BLKF_G1BLKF3_LENGTH                            0x1
#define _COG1BLKF_G1BLKF3_MASK                              0x8
#define _COG1BLKF_G1BLKF4_POSN                              0x4
#define _COG1BLKF_G1BLKF4_POSITION                          0x4
#define _COG1BLKF_G1BLKF4_SIZE                              0x1
#define _COG1BLKF_G1BLKF4_LENGTH                            0x1
#define _COG1BLKF_G1BLKF4_MASK                              0x10
#define _COG1BLKF_G1BLKF5_POSN                              0x5
#define _COG1BLKF_G1BLKF5_POSITION                          0x5
#define _COG1BLKF_G1BLKF5_SIZE                              0x1
#define _COG1BLKF_G1BLKF5_LENGTH                            0x1
#define _COG1BLKF_G1BLKF5_MASK                              0x20

// Register: COG1DBR
#define COG1DBR COG1DBR
extern volatile unsigned char           COG1DBR             __at(0x695);
#ifndef _LIB_BUILD
asm("COG1DBR equ 0695h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1DBR                  :6;
    };
    struct {
        unsigned G1DBR0                 :1;
        unsigned G1DBR1                 :1;
        unsigned G1DBR2                 :1;
        unsigned G1DBR3                 :1;
        unsigned G1DBR4                 :1;
        unsigned G1DBR5                 :1;
    };
} COG1DBRbits_t;
extern volatile COG1DBRbits_t COG1DBRbits __at(0x695);
// bitfield macros
#define _COG1DBR_G1DBR_POSN                                 0x0
#define _COG1DBR_G1DBR_POSITION                             0x0
#define _COG1DBR_G1DBR_SIZE                                 0x6
#define _COG1DBR_G1DBR_LENGTH                               0x6
#define _COG1DBR_G1DBR_MASK                                 0x3F
#define _COG1DBR_G1DBR0_POSN                                0x0
#define _COG1DBR_G1DBR0_POSITION                            0x0
#define _COG1DBR_G1DBR0_SIZE                                0x1
#define _COG1DBR_G1DBR0_LENGTH                              0x1
#define _COG1DBR_G1DBR0_MASK                                0x1
#define _COG1DBR_G1DBR1_POSN                                0x1
#define _COG1DBR_G1DBR1_POSITION                            0x1
#define _COG1DBR_G1DBR1_SIZE                                0x1
#define _COG1DBR_G1DBR1_LENGTH                              0x1
#define _COG1DBR_G1DBR1_MASK                                0x2
#define _COG1DBR_G1DBR2_POSN                                0x2
#define _COG1DBR_G1DBR2_POSITION                            0x2
#define _COG1DBR_G1DBR2_SIZE                                0x1
#define _COG1DBR_G1DBR2_LENGTH                              0x1
#define _COG1DBR_G1DBR2_MASK                                0x4
#define _COG1DBR_G1DBR3_POSN                                0x3
#define _COG1DBR_G1DBR3_POSITION                            0x3
#define _COG1DBR_G1DBR3_SIZE                                0x1
#define _COG1DBR_G1DBR3_LENGTH                              0x1
#define _COG1DBR_G1DBR3_MASK                                0x8
#define _COG1DBR_G1DBR4_POSN                                0x4
#define _COG1DBR_G1DBR4_POSITION                            0x4
#define _COG1DBR_G1DBR4_SIZE                                0x1
#define _COG1DBR_G1DBR4_LENGTH                              0x1
#define _COG1DBR_G1DBR4_MASK                                0x10
#define _COG1DBR_G1DBR5_POSN                                0x5
#define _COG1DBR_G1DBR5_POSITION                            0x5
#define _COG1DBR_G1DBR5_SIZE                                0x1
#define _COG1DBR_G1DBR5_LENGTH                              0x1
#define _COG1DBR_G1DBR5_MASK                                0x20

// Register: COG1DBF
#define COG1DBF COG1DBF
extern volatile unsigned char           COG1DBF             __at(0x696);
#ifndef _LIB_BUILD
asm("COG1DBF equ 0696h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1DBF                  :6;
    };
    struct {
        unsigned G1DBF0                 :1;
        unsigned G1DBF1                 :1;
        unsigned G1DBF2                 :1;
        unsigned G1DBF3                 :1;
        unsigned G1DBF4                 :1;
        unsigned G1DBF5                 :1;
    };
} COG1DBFbits_t;
extern volatile COG1DBFbits_t COG1DBFbits __at(0x696);
// bitfield macros
#define _COG1DBF_G1DBF_POSN                                 0x0
#define _COG1DBF_G1DBF_POSITION                             0x0
#define _COG1DBF_G1DBF_SIZE                                 0x6
#define _COG1DBF_G1DBF_LENGTH                               0x6
#define _COG1DBF_G1DBF_MASK                                 0x3F
#define _COG1DBF_G1DBF0_POSN                                0x0
#define _COG1DBF_G1DBF0_POSITION                            0x0
#define _COG1DBF_G1DBF0_SIZE                                0x1
#define _COG1DBF_G1DBF0_LENGTH                              0x1
#define _COG1DBF_G1DBF0_MASK                                0x1
#define _COG1DBF_G1DBF1_POSN                                0x1
#define _COG1DBF_G1DBF1_POSITION                            0x1
#define _COG1DBF_G1DBF1_SIZE                                0x1
#define _COG1DBF_G1DBF1_LENGTH                              0x1
#define _COG1DBF_G1DBF1_MASK                                0x2
#define _COG1DBF_G1DBF2_POSN                                0x2
#define _COG1DBF_G1DBF2_POSITION                            0x2
#define _COG1DBF_G1DBF2_SIZE                                0x1
#define _COG1DBF_G1DBF2_LENGTH                              0x1
#define _COG1DBF_G1DBF2_MASK                                0x4
#define _COG1DBF_G1DBF3_POSN                                0x3
#define _COG1DBF_G1DBF3_POSITION                            0x3
#define _COG1DBF_G1DBF3_SIZE                                0x1
#define _COG1DBF_G1DBF3_LENGTH                              0x1
#define _COG1DBF_G1DBF3_MASK                                0x8
#define _COG1DBF_G1DBF4_POSN                                0x4
#define _COG1DBF_G1DBF4_POSITION                            0x4
#define _COG1DBF_G1DBF4_SIZE                                0x1
#define _COG1DBF_G1DBF4_LENGTH                              0x1
#define _COG1DBF_G1DBF4_MASK                                0x10
#define _COG1DBF_G1DBF5_POSN                                0x5
#define _COG1DBF_G1DBF5_POSITION                            0x5
#define _COG1DBF_G1DBF5_SIZE                                0x1
#define _COG1DBF_G1DBF5_LENGTH                              0x1
#define _COG1DBF_G1DBF5_MASK                                0x20

// Register: COG1CON0
#define COG1CON0 COG1CON0
extern volatile unsigned char           COG1CON0            __at(0x697);
#ifndef _LIB_BUILD
asm("COG1CON0 equ 0697h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1MD                   :3;
        unsigned G1CS                   :2;
        unsigned                        :1;
        unsigned G1LD                   :1;
        unsigned G1EN                   :1;
    };
    struct {
        unsigned G1MD0                  :1;
        unsigned G1MD1                  :1;
        unsigned G1MD2                  :1;
        unsigned G1CS0                  :1;
        unsigned G1CS1                  :1;
    };
} COG1CON0bits_t;
extern volatile COG1CON0bits_t COG1CON0bits __at(0x697);
// bitfield macros
#define _COG1CON0_G1MD_POSN                                 0x0
#define _COG1CON0_G1MD_POSITION                             0x0
#define _COG1CON0_G1MD_SIZE                                 0x3
#define _COG1CON0_G1MD_LENGTH                               0x3
#define _COG1CON0_G1MD_MASK                                 0x7
#define _COG1CON0_G1CS_POSN                                 0x3
#define _COG1CON0_G1CS_POSITION                             0x3
#define _COG1CON0_G1CS_SIZE                                 0x2
#define _COG1CON0_G1CS_LENGTH                               0x2
#define _COG1CON0_G1CS_MASK                                 0x18
#define _COG1CON0_G1LD_POSN                                 0x6
#define _COG1CON0_G1LD_POSITION                             0x6
#define _COG1CON0_G1LD_SIZE                                 0x1
#define _COG1CON0_G1LD_LENGTH                               0x1
#define _COG1CON0_G1LD_MASK                                 0x40
#define _COG1CON0_G1EN_POSN                                 0x7
#define _COG1CON0_G1EN_POSITION                             0x7
#define _COG1CON0_G1EN_SIZE                                 0x1
#define _COG1CON0_G1EN_LENGTH                               0x1
#define _COG1CON0_G1EN_MASK                                 0x80
#define _COG1CON0_G1MD0_POSN                                0x0
#define _COG1CON0_G1MD0_POSITION                            0x0
#define _COG1CON0_G1MD0_SIZE                                0x1
#define _COG1CON0_G1MD0_LENGTH                              0x1
#define _COG1CON0_G1MD0_MASK                                0x1
#define _COG1CON0_G1MD1_POSN                                0x1
#define _COG1CON0_G1MD1_POSITION                            0x1
#define _COG1CON0_G1MD1_SIZE                                0x1
#define _COG1CON0_G1MD1_LENGTH                              0x1
#define _COG1CON0_G1MD1_MASK                                0x2
#define _COG1CON0_G1MD2_POSN                                0x2
#define _COG1CON0_G1MD2_POSITION                            0x2
#define _COG1CON0_G1MD2_SIZE                                0x1
#define _COG1CON0_G1MD2_LENGTH                              0x1
#define _COG1CON0_G1MD2_MASK                                0x4
#define _COG1CON0_G1CS0_POSN                                0x3
#define _COG1CON0_G1CS0_POSITION                            0x3
#define _COG1CON0_G1CS0_SIZE                                0x1
#define _COG1CON0_G1CS0_LENGTH                              0x1
#define _COG1CON0_G1CS0_MASK                                0x8
#define _COG1CON0_G1CS1_POSN                                0x4
#define _COG1CON0_G1CS1_POSITION                            0x4
#define _COG1CON0_G1CS1_SIZE                                0x1
#define _COG1CON0_G1CS1_LENGTH                              0x1
#define _COG1CON0_G1CS1_MASK                                0x10

// Register: COG1CON1
#define COG1CON1 COG1CON1
extern volatile unsigned char           COG1CON1            __at(0x698);
#ifndef _LIB_BUILD
asm("COG1CON1 equ 0698h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1POLA                 :1;
        unsigned G1POLB                 :1;
        unsigned G1POLC                 :1;
        unsigned G1POLD                 :1;
        unsigned                        :2;
        unsigned G1FDBS                 :1;
        unsigned G1RDBS                 :1;
    };
} COG1CON1bits_t;
extern volatile COG1CON1bits_t COG1CON1bits __at(0x698);
// bitfield macros
#define _COG1CON1_G1POLA_POSN                               0x0
#define _COG1CON1_G1POLA_POSITION                           0x0
#define _COG1CON1_G1POLA_SIZE                               0x1
#define _COG1CON1_G1POLA_LENGTH                             0x1
#define _COG1CON1_G1POLA_MASK                               0x1
#define _COG1CON1_G1POLB_POSN                               0x1
#define _COG1CON1_G1POLB_POSITION                           0x1
#define _COG1CON1_G1POLB_SIZE                               0x1
#define _COG1CON1_G1POLB_LENGTH                             0x1
#define _COG1CON1_G1POLB_MASK                               0x2
#define _COG1CON1_G1POLC_POSN                               0x2
#define _COG1CON1_G1POLC_POSITION                           0x2
#define _COG1CON1_G1POLC_SIZE                               0x1
#define _COG1CON1_G1POLC_LENGTH                             0x1
#define _COG1CON1_G1POLC_MASK                               0x4
#define _COG1CON1_G1POLD_POSN                               0x3
#define _COG1CON1_G1POLD_POSITION                           0x3
#define _COG1CON1_G1POLD_SIZE                               0x1
#define _COG1CON1_G1POLD_LENGTH                             0x1
#define _COG1CON1_G1POLD_MASK                               0x8
#define _COG1CON1_G1FDBS_POSN                               0x6
#define _COG1CON1_G1FDBS_POSITION                           0x6
#define _COG1CON1_G1FDBS_SIZE                               0x1
#define _COG1CON1_G1FDBS_LENGTH                             0x1
#define _COG1CON1_G1FDBS_MASK                               0x40
#define _COG1CON1_G1RDBS_POSN                               0x7
#define _COG1CON1_G1RDBS_POSITION                           0x7
#define _COG1CON1_G1RDBS_SIZE                               0x1
#define _COG1CON1_G1RDBS_LENGTH                             0x1
#define _COG1CON1_G1RDBS_MASK                               0x80

// Register: COG1RIS
#define COG1RIS COG1RIS
extern volatile unsigned char           COG1RIS             __at(0x699);
#ifndef _LIB_BUILD
asm("COG1RIS equ 0699h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1RIS0                 :1;
        unsigned G1RIS1                 :1;
        unsigned G1RIS2                 :1;
        unsigned G1RIS3                 :1;
        unsigned G1RIS4                 :1;
        unsigned G1RIS5                 :1;
        unsigned G1RIS6                 :1;
    };
} COG1RISbits_t;
extern volatile COG1RISbits_t COG1RISbits __at(0x699);
// bitfield macros
#define _COG1RIS_G1RIS0_POSN                                0x0
#define _COG1RIS_G1RIS0_POSITION                            0x0
#define _COG1RIS_G1RIS0_SIZE                                0x1
#define _COG1RIS_G1RIS0_LENGTH                              0x1
#define _COG1RIS_G1RIS0_MASK                                0x1
#define _COG1RIS_G1RIS1_POSN                                0x1
#define _COG1RIS_G1RIS1_POSITION                            0x1
#define _COG1RIS_G1RIS1_SIZE                                0x1
#define _COG1RIS_G1RIS1_LENGTH                              0x1
#define _COG1RIS_G1RIS1_MASK                                0x2
#define _COG1RIS_G1RIS2_POSN                                0x2
#define _COG1RIS_G1RIS2_POSITION                            0x2
#define _COG1RIS_G1RIS2_SIZE                                0x1
#define _COG1RIS_G1RIS2_LENGTH                              0x1
#define _COG1RIS_G1RIS2_MASK                                0x4
#define _COG1RIS_G1RIS3_POSN                                0x3
#define _COG1RIS_G1RIS3_POSITION                            0x3
#define _COG1RIS_G1RIS3_SIZE                                0x1
#define _COG1RIS_G1RIS3_LENGTH                              0x1
#define _COG1RIS_G1RIS3_MASK                                0x8
#define _COG1RIS_G1RIS4_POSN                                0x4
#define _COG1RIS_G1RIS4_POSITION                            0x4
#define _COG1RIS_G1RIS4_SIZE                                0x1
#define _COG1RIS_G1RIS4_LENGTH                              0x1
#define _COG1RIS_G1RIS4_MASK                                0x10
#define _COG1RIS_G1RIS5_POSN                                0x5
#define _COG1RIS_G1RIS5_POSITION                            0x5
#define _COG1RIS_G1RIS5_SIZE                                0x1
#define _COG1RIS_G1RIS5_LENGTH                              0x1
#define _COG1RIS_G1RIS5_MASK                                0x20
#define _COG1RIS_G1RIS6_POSN                                0x6
#define _COG1RIS_G1RIS6_POSITION                            0x6
#define _COG1RIS_G1RIS6_SIZE                                0x1
#define _COG1RIS_G1RIS6_LENGTH                              0x1
#define _COG1RIS_G1RIS6_MASK                                0x40

// Register: COG1RSIM
#define COG1RSIM COG1RSIM
extern volatile unsigned char           COG1RSIM            __at(0x69A);
#ifndef _LIB_BUILD
asm("COG1RSIM equ 069Ah");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1RSIM0                :1;
        unsigned G1RSIM1                :1;
        unsigned G1RSIM2                :1;
        unsigned G1RSIM3                :1;
        unsigned G1RSIM4                :1;
        unsigned G1RSIM5                :1;
        unsigned G1RSIM6                :1;
    };
} COG1RSIMbits_t;
extern volatile COG1RSIMbits_t COG1RSIMbits __at(0x69A);
// bitfield macros
#define _COG1RSIM_G1RSIM0_POSN                              0x0
#define _COG1RSIM_G1RSIM0_POSITION                          0x0
#define _COG1RSIM_G1RSIM0_SIZE                              0x1
#define _COG1RSIM_G1RSIM0_LENGTH                            0x1
#define _COG1RSIM_G1RSIM0_MASK                              0x1
#define _COG1RSIM_G1RSIM1_POSN                              0x1
#define _COG1RSIM_G1RSIM1_POSITION                          0x1
#define _COG1RSIM_G1RSIM1_SIZE                              0x1
#define _COG1RSIM_G1RSIM1_LENGTH                            0x1
#define _COG1RSIM_G1RSIM1_MASK                              0x2
#define _COG1RSIM_G1RSIM2_POSN                              0x2
#define _COG1RSIM_G1RSIM2_POSITION                          0x2
#define _COG1RSIM_G1RSIM2_SIZE                              0x1
#define _COG1RSIM_G1RSIM2_LENGTH                            0x1
#define _COG1RSIM_G1RSIM2_MASK                              0x4
#define _COG1RSIM_G1RSIM3_POSN                              0x3
#define _COG1RSIM_G1RSIM3_POSITION                          0x3
#define _COG1RSIM_G1RSIM3_SIZE                              0x1
#define _COG1RSIM_G1RSIM3_LENGTH                            0x1
#define _COG1RSIM_G1RSIM3_MASK                              0x8
#define _COG1RSIM_G1RSIM4_POSN                              0x4
#define _COG1RSIM_G1RSIM4_POSITION                          0x4
#define _COG1RSIM_G1RSIM4_SIZE                              0x1
#define _COG1RSIM_G1RSIM4_LENGTH                            0x1
#define _COG1RSIM_G1RSIM4_MASK                              0x10
#define _COG1RSIM_G1RSIM5_POSN                              0x5
#define _COG1RSIM_G1RSIM5_POSITION                          0x5
#define _COG1RSIM_G1RSIM5_SIZE                              0x1
#define _COG1RSIM_G1RSIM5_LENGTH                            0x1
#define _COG1RSIM_G1RSIM5_MASK                              0x20
#define _COG1RSIM_G1RSIM6_POSN                              0x6
#define _COG1RSIM_G1RSIM6_POSITION                          0x6
#define _COG1RSIM_G1RSIM6_SIZE                              0x1
#define _COG1RSIM_G1RSIM6_LENGTH                            0x1
#define _COG1RSIM_G1RSIM6_MASK                              0x40

// Register: COG1FIS
#define COG1FIS COG1FIS
extern volatile unsigned char           COG1FIS             __at(0x69B);
#ifndef _LIB_BUILD
asm("COG1FIS equ 069Bh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1FIS0                 :1;
        unsigned G1FIS1                 :1;
        unsigned G1FIS2                 :1;
        unsigned G1FIS3                 :1;
        unsigned G1FIS4                 :1;
        unsigned G1FIS5                 :1;
        unsigned G1FIS6                 :1;
    };
} COG1FISbits_t;
extern volatile COG1FISbits_t COG1FISbits __at(0x69B);
// bitfield macros
#define _COG1FIS_G1FIS0_POSN                                0x0
#define _COG1FIS_G1FIS0_POSITION                            0x0
#define _COG1FIS_G1FIS0_SIZE                                0x1
#define _COG1FIS_G1FIS0_LENGTH                              0x1
#define _COG1FIS_G1FIS0_MASK                                0x1
#define _COG1FIS_G1FIS1_POSN                                0x1
#define _COG1FIS_G1FIS1_POSITION                            0x1
#define _COG1FIS_G1FIS1_SIZE                                0x1
#define _COG1FIS_G1FIS1_LENGTH                              0x1
#define _COG1FIS_G1FIS1_MASK                                0x2
#define _COG1FIS_G1FIS2_POSN                                0x2
#define _COG1FIS_G1FIS2_POSITION                            0x2
#define _COG1FIS_G1FIS2_SIZE                                0x1
#define _COG1FIS_G1FIS2_LENGTH                              0x1
#define _COG1FIS_G1FIS2_MASK                                0x4
#define _COG1FIS_G1FIS3_POSN                                0x3
#define _COG1FIS_G1FIS3_POSITION                            0x3
#define _COG1FIS_G1FIS3_SIZE                                0x1
#define _COG1FIS_G1FIS3_LENGTH                              0x1
#define _COG1FIS_G1FIS3_MASK                                0x8
#define _COG1FIS_G1FIS4_POSN                                0x4
#define _COG1FIS_G1FIS4_POSITION                            0x4
#define _COG1FIS_G1FIS4_SIZE                                0x1
#define _COG1FIS_G1FIS4_LENGTH                              0x1
#define _COG1FIS_G1FIS4_MASK                                0x10
#define _COG1FIS_G1FIS5_POSN                                0x5
#define _COG1FIS_G1FIS5_POSITION                            0x5
#define _COG1FIS_G1FIS5_SIZE                                0x1
#define _COG1FIS_G1FIS5_LENGTH                              0x1
#define _COG1FIS_G1FIS5_MASK                                0x20
#define _COG1FIS_G1FIS6_POSN                                0x6
#define _COG1FIS_G1FIS6_POSITION                            0x6
#define _COG1FIS_G1FIS6_SIZE                                0x1
#define _COG1FIS_G1FIS6_LENGTH                              0x1
#define _COG1FIS_G1FIS6_MASK                                0x40

// Register: COG1FSIM
#define COG1FSIM COG1FSIM
extern volatile unsigned char           COG1FSIM            __at(0x69C);
#ifndef _LIB_BUILD
asm("COG1FSIM equ 069Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1FSIM0                :1;
        unsigned G1FSIM1                :1;
        unsigned G1FSIM2                :1;
        unsigned G1FSIM3                :1;
        unsigned G1FSIM4                :1;
        unsigned G1FSIM5                :1;
        unsigned G1FSIM6                :1;
    };
} COG1FSIMbits_t;
extern volatile COG1FSIMbits_t COG1FSIMbits __at(0x69C);
// bitfield macros
#define _COG1FSIM_G1FSIM0_POSN                              0x0
#define _COG1FSIM_G1FSIM0_POSITION                          0x0
#define _COG1FSIM_G1FSIM0_SIZE                              0x1
#define _COG1FSIM_G1FSIM0_LENGTH                            0x1
#define _COG1FSIM_G1FSIM0_MASK                              0x1
#define _COG1FSIM_G1FSIM1_POSN                              0x1
#define _COG1FSIM_G1FSIM1_POSITION                          0x1
#define _COG1FSIM_G1FSIM1_SIZE                              0x1
#define _COG1FSIM_G1FSIM1_LENGTH                            0x1
#define _COG1FSIM_G1FSIM1_MASK                              0x2
#define _COG1FSIM_G1FSIM2_POSN                              0x2
#define _COG1FSIM_G1FSIM2_POSITION                          0x2
#define _COG1FSIM_G1FSIM2_SIZE                              0x1
#define _COG1FSIM_G1FSIM2_LENGTH                            0x1
#define _COG1FSIM_G1FSIM2_MASK                              0x4
#define _COG1FSIM_G1FSIM3_POSN                              0x3
#define _COG1FSIM_G1FSIM3_POSITION                          0x3
#define _COG1FSIM_G1FSIM3_SIZE                              0x1
#define _COG1FSIM_G1FSIM3_LENGTH                            0x1
#define _COG1FSIM_G1FSIM3_MASK                              0x8
#define _COG1FSIM_G1FSIM4_POSN                              0x4
#define _COG1FSIM_G1FSIM4_POSITION                          0x4
#define _COG1FSIM_G1FSIM4_SIZE                              0x1
#define _COG1FSIM_G1FSIM4_LENGTH                            0x1
#define _COG1FSIM_G1FSIM4_MASK                              0x10
#define _COG1FSIM_G1FSIM5_POSN                              0x5
#define _COG1FSIM_G1FSIM5_POSITION                          0x5
#define _COG1FSIM_G1FSIM5_SIZE                              0x1
#define _COG1FSIM_G1FSIM5_LENGTH                            0x1
#define _COG1FSIM_G1FSIM5_MASK                              0x20
#define _COG1FSIM_G1FSIM6_POSN                              0x6
#define _COG1FSIM_G1FSIM6_POSITION                          0x6
#define _COG1FSIM_G1FSIM6_SIZE                              0x1
#define _COG1FSIM_G1FSIM6_LENGTH                            0x1
#define _COG1FSIM_G1FSIM6_MASK                              0x40

// Register: COG1ASD0
#define COG1ASD0 COG1ASD0
extern volatile unsigned char           COG1ASD0            __at(0x69D);
#ifndef _LIB_BUILD
asm("COG1ASD0 equ 069Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned                        :2;
        unsigned G1ASDAC                :2;
        unsigned G1ASDBD                :2;
        unsigned G1ARSEN                :1;
        unsigned G1ASE                  :1;
    };
    struct {
        unsigned                        :2;
        unsigned G1ASDAC0               :1;
        unsigned G1ASDAC1               :1;
        unsigned G1ASDBD0               :1;
        unsigned G1ASDBD1               :1;
    };
} COG1ASD0bits_t;
extern volatile COG1ASD0bits_t COG1ASD0bits __at(0x69D);
// bitfield macros
#define _COG1ASD0_G1ASDAC_POSN                              0x2
#define _COG1ASD0_G1ASDAC_POSITION                          0x2
#define _COG1ASD0_G1ASDAC_SIZE                              0x2
#define _COG1ASD0_G1ASDAC_LENGTH                            0x2
#define _COG1ASD0_G1ASDAC_MASK                              0xC
#define _COG1ASD0_G1ASDBD_POSN                              0x4
#define _COG1ASD0_G1ASDBD_POSITION                          0x4
#define _COG1ASD0_G1ASDBD_SIZE                              0x2
#define _COG1ASD0_G1ASDBD_LENGTH                            0x2
#define _COG1ASD0_G1ASDBD_MASK                              0x30
#define _COG1ASD0_G1ARSEN_POSN                              0x6
#define _COG1ASD0_G1ARSEN_POSITION                          0x6
#define _COG1ASD0_G1ARSEN_SIZE                              0x1
#define _COG1ASD0_G1ARSEN_LENGTH                            0x1
#define _COG1ASD0_G1ARSEN_MASK                              0x40
#define _COG1ASD0_G1ASE_POSN                                0x7
#define _COG1ASD0_G1ASE_POSITION                            0x7
#define _COG1ASD0_G1ASE_SIZE                                0x1
#define _COG1ASD0_G1ASE_LENGTH                              0x1
#define _COG1ASD0_G1ASE_MASK                                0x80
#define _COG1ASD0_G1ASDAC0_POSN                             0x2
#define _COG1ASD0_G1ASDAC0_POSITION                         0x2
#define _COG1ASD0_G1ASDAC0_SIZE                             0x1
#define _COG1ASD0_G1ASDAC0_LENGTH                           0x1
#define _COG1ASD0_G1ASDAC0_MASK                             0x4
#define _COG1ASD0_G1ASDAC1_POSN                             0x3
#define _COG1ASD0_G1ASDAC1_POSITION                         0x3
#define _COG1ASD0_G1ASDAC1_SIZE                             0x1
#define _COG1ASD0_G1ASDAC1_LENGTH                           0x1
#define _COG1ASD0_G1ASDAC1_MASK                             0x8
#define _COG1ASD0_G1ASDBD0_POSN                             0x4
#define _COG1ASD0_G1ASDBD0_POSITION                         0x4
#define _COG1ASD0_G1ASDBD0_SIZE                             0x1
#define _COG1ASD0_G1ASDBD0_LENGTH                           0x1
#define _COG1ASD0_G1ASDBD0_MASK                             0x10
#define _COG1ASD0_G1ASDBD1_POSN                             0x5
#define _COG1ASD0_G1ASDBD1_POSITION                         0x5
#define _COG1ASD0_G1ASDBD1_SIZE                             0x1
#define _COG1ASD0_G1ASDBD1_LENGTH                           0x1
#define _COG1ASD0_G1ASDBD1_MASK                             0x20

// Register: COG1ASD1
#define COG1ASD1 COG1ASD1
extern volatile unsigned char           COG1ASD1            __at(0x69E);
#ifndef _LIB_BUILD
asm("COG1ASD1 equ 069Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1AS0E                 :1;
        unsigned G1AS1E                 :1;
        unsigned G1AS2E                 :1;
        unsigned G1AS3E                 :1;
    };
} COG1ASD1bits_t;
extern volatile COG1ASD1bits_t COG1ASD1bits __at(0x69E);
// bitfield macros
#define _COG1ASD1_G1AS0E_POSN                               0x0
#define _COG1ASD1_G1AS0E_POSITION                           0x0
#define _COG1ASD1_G1AS0E_SIZE                               0x1
#define _COG1ASD1_G1AS0E_LENGTH                             0x1
#define _COG1ASD1_G1AS0E_MASK                               0x1
#define _COG1ASD1_G1AS1E_POSN                               0x1
#define _COG1ASD1_G1AS1E_POSITION                           0x1
#define _COG1ASD1_G1AS1E_SIZE                               0x1
#define _COG1ASD1_G1AS1E_LENGTH                             0x1
#define _COG1ASD1_G1AS1E_MASK                               0x2
#define _COG1ASD1_G1AS2E_POSN                               0x2
#define _COG1ASD1_G1AS2E_POSITION                           0x2
#define _COG1ASD1_G1AS2E_SIZE                               0x1
#define _COG1ASD1_G1AS2E_LENGTH                             0x1
#define _COG1ASD1_G1AS2E_MASK                               0x4
#define _COG1ASD1_G1AS3E_POSN                               0x3
#define _COG1ASD1_G1AS3E_POSITION                           0x3
#define _COG1ASD1_G1AS3E_SIZE                               0x1
#define _COG1ASD1_G1AS3E_LENGTH                             0x1
#define _COG1ASD1_G1AS3E_MASK                               0x8

// Register: COG1STR
#define COG1STR COG1STR
extern volatile unsigned char           COG1STR             __at(0x69F);
#ifndef _LIB_BUILD
asm("COG1STR equ 069Fh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned G1STRA                 :1;
        unsigned G1STRB                 :1;
        unsigned G1STRC                 :1;
        unsigned G1STRD                 :1;
        unsigned G1SDATA                :1;
        unsigned G1SDATB                :1;
        unsigned G1SDATC                :1;
        unsigned G1SDATD                :1;
    };
} COG1STRbits_t;
extern volatile COG1STRbits_t COG1STRbits __at(0x69F);
// bitfield macros
#define _COG1STR_G1STRA_POSN                                0x0
#define _COG1STR_G1STRA_POSITION                            0x0
#define _COG1STR_G1STRA_SIZE                                0x1
#define _COG1STR_G1STRA_LENGTH                              0x1
#define _COG1STR_G1STRA_MASK                                0x1
#define _COG1STR_G1STRB_POSN                                0x1
#define _COG1STR_G1STRB_POSITION                            0x1
#define _COG1STR_G1STRB_SIZE                                0x1
#define _COG1STR_G1STRB_LENGTH                              0x1
#define _COG1STR_G1STRB_MASK                                0x2
#define _COG1STR_G1STRC_POSN                                0x2
#define _COG1STR_G1STRC_POSITION                            0x2
#define _COG1STR_G1STRC_SIZE                                0x1
#define _COG1STR_G1STRC_LENGTH                              0x1
#define _COG1STR_G1STRC_MASK                                0x4
#define _COG1STR_G1STRD_POSN                                0x3
#define _COG1STR_G1STRD_POSITION                            0x3
#define _COG1STR_G1STRD_SIZE                                0x1
#define _COG1STR_G1STRD_LENGTH                              0x1
#define _COG1STR_G1STRD_MASK                                0x8
#define _COG1STR_G1SDATA_POSN                               0x4
#define _COG1STR_G1SDATA_POSITION                           0x4
#define _COG1STR_G1SDATA_SIZE                               0x1
#define _COG1STR_G1SDATA_LENGTH                             0x1
#define _COG1STR_G1SDATA_MASK                               0x10
#define _COG1STR_G1SDATB_POSN                               0x5
#define _COG1STR_G1SDATB_POSITION                           0x5
#define _COG1STR_G1SDATB_SIZE                               0x1
#define _COG1STR_G1SDATB_LENGTH                             0x1
#define _COG1STR_G1SDATB_MASK                               0x20
#define _COG1STR_G1SDATC_POSN                               0x6
#define _COG1STR_G1SDATC_POSITION                           0x6
#define _COG1STR_G1SDATC_SIZE                               0x1
#define _COG1STR_G1SDATC_LENGTH                             0x1
#define _COG1STR_G1SDATC_MASK                               0x40
#define _COG1STR_G1SDATD_POSN                               0x7
#define _COG1STR_G1SDATD_POSITION                           0x7
#define _COG1STR_G1SDATD_SIZE                               0x1
#define _COG1STR_G1SDATD_LENGTH                             0x1
#define _COG1STR_G1SDATD_MASK                               0x80

// Register: PPSLOCK
#define PPSLOCK PPSLOCK
extern volatile unsigned char           PPSLOCK             __at(0xE0F);
#ifndef _LIB_BUILD
asm("PPSLOCK equ 0E0Fh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PPSLOCKED              :1;
    };
} PPSLOCKbits_t;
extern volatile PPSLOCKbits_t PPSLOCKbits __at(0xE0F);
// bitfield macros
#define _PPSLOCK_PPSLOCKED_POSN                             0x0
#define _PPSLOCK_PPSLOCKED_POSITION                         0x0
#define _PPSLOCK_PPSLOCKED_SIZE                             0x1
#define _PPSLOCK_PPSLOCKED_LENGTH                           0x1
#define _PPSLOCK_PPSLOCKED_MASK                             0x1

// Register: INTPPS
#define INTPPS INTPPS
extern volatile unsigned char           INTPPS              __at(0xE10);
#ifndef _LIB_BUILD
asm("INTPPS equ 0E10h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned INTPPS                 :5;
    };
} INTPPSbits_t;
extern volatile INTPPSbits_t INTPPSbits __at(0xE10);
// bitfield macros
#define _INTPPS_INTPPS_POSN                                 0x0
#define _INTPPS_INTPPS_POSITION                             0x0
#define _INTPPS_INTPPS_SIZE                                 0x5
#define _INTPPS_INTPPS_LENGTH                               0x5
#define _INTPPS_INTPPS_MASK                                 0x1F

// Register: T0CKIPPS
#define T0CKIPPS T0CKIPPS
extern volatile unsigned char           T0CKIPPS            __at(0xE11);
#ifndef _LIB_BUILD
asm("T0CKIPPS equ 0E11h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned T0CKIPPS               :5;
    };
} T0CKIPPSbits_t;
extern volatile T0CKIPPSbits_t T0CKIPPSbits __at(0xE11);
// bitfield macros
#define _T0CKIPPS_T0CKIPPS_POSN                             0x0
#define _T0CKIPPS_T0CKIPPS_POSITION                         0x0
#define _T0CKIPPS_T0CKIPPS_SIZE                             0x5
#define _T0CKIPPS_T0CKIPPS_LENGTH                           0x5
#define _T0CKIPPS_T0CKIPPS_MASK                             0x1F

// Register: T1CKIPPS
#define T1CKIPPS T1CKIPPS
extern volatile unsigned char           T1CKIPPS            __at(0xE12);
#ifndef _LIB_BUILD
asm("T1CKIPPS equ 0E12h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned T1CKIPPS               :5;
    };
} T1CKIPPSbits_t;
extern volatile T1CKIPPSbits_t T1CKIPPSbits __at(0xE12);
// bitfield macros
#define _T1CKIPPS_T1CKIPPS_POSN                             0x0
#define _T1CKIPPS_T1CKIPPS_POSITION                         0x0
#define _T1CKIPPS_T1CKIPPS_SIZE                             0x5
#define _T1CKIPPS_T1CKIPPS_LENGTH                           0x5
#define _T1CKIPPS_T1CKIPPS_MASK                             0x1F

// Register: T1GPPS
#define T1GPPS T1GPPS
extern volatile unsigned char           T1GPPS              __at(0xE13);
#ifndef _LIB_BUILD
asm("T1GPPS equ 0E13h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned T1GPPS                 :5;
    };
} T1GPPSbits_t;
extern volatile T1GPPSbits_t T1GPPSbits __at(0xE13);
// bitfield macros
#define _T1GPPS_T1GPPS_POSN                                 0x0
#define _T1GPPS_T1GPPS_POSITION                             0x0
#define _T1GPPS_T1GPPS_SIZE                                 0x5
#define _T1GPPS_T1GPPS_LENGTH                               0x5
#define _T1GPPS_T1GPPS_MASK                                 0x1F

// Register: CCP1PPS
#define CCP1PPS CCP1PPS
extern volatile unsigned char           CCP1PPS             __at(0xE14);
#ifndef _LIB_BUILD
asm("CCP1PPS equ 0E14h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CCP1PPS                :5;
    };
} CCP1PPSbits_t;
extern volatile CCP1PPSbits_t CCP1PPSbits __at(0xE14);
// bitfield macros
#define _CCP1PPS_CCP1PPS_POSN                               0x0
#define _CCP1PPS_CCP1PPS_POSITION                           0x0
#define _CCP1PPS_CCP1PPS_SIZE                               0x5
#define _CCP1PPS_CCP1PPS_LENGTH                             0x5
#define _CCP1PPS_CCP1PPS_MASK                               0x1F

// Register: CCP2PPS
#define CCP2PPS CCP2PPS
extern volatile unsigned char           CCP2PPS             __at(0xE15);
#ifndef _LIB_BUILD
asm("CCP2PPS equ 0E15h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CCP2PPS                :5;
    };
} CCP2PPSbits_t;
extern volatile CCP2PPSbits_t CCP2PPSbits __at(0xE15);
// bitfield macros
#define _CCP2PPS_CCP2PPS_POSN                               0x0
#define _CCP2PPS_CCP2PPS_POSITION                           0x0
#define _CCP2PPS_CCP2PPS_SIZE                               0x5
#define _CCP2PPS_CCP2PPS_LENGTH                             0x5
#define _CCP2PPS_CCP2PPS_MASK                               0x1F

// Register: COGINPPS
#define COGINPPS COGINPPS
extern volatile unsigned char           COGINPPS            __at(0xE17);
#ifndef _LIB_BUILD
asm("COGINPPS equ 0E17h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned COGINPPS               :5;
    };
} COGINPPSbits_t;
extern volatile COGINPPSbits_t COGINPPSbits __at(0xE17);
// bitfield macros
#define _COGINPPS_COGINPPS_POSN                             0x0
#define _COGINPPS_COGINPPS_POSITION                         0x0
#define _COGINPPS_COGINPPS_SIZE                             0x5
#define _COGINPPS_COGINPPS_LENGTH                           0x5
#define _COGINPPS_COGINPPS_MASK                             0x1F

// Register: SSPCLKPPS
#define SSPCLKPPS SSPCLKPPS
extern volatile unsigned char           SSPCLKPPS           __at(0xE20);
#ifndef _LIB_BUILD
asm("SSPCLKPPS equ 0E20h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SSPCLKPPS              :5;
    };
} SSPCLKPPSbits_t;
extern volatile SSPCLKPPSbits_t SSPCLKPPSbits __at(0xE20);
// bitfield macros
#define _SSPCLKPPS_SSPCLKPPS_POSN                           0x0
#define _SSPCLKPPS_SSPCLKPPS_POSITION                       0x0
#define _SSPCLKPPS_SSPCLKPPS_SIZE                           0x5
#define _SSPCLKPPS_SSPCLKPPS_LENGTH                         0x5
#define _SSPCLKPPS_SSPCLKPPS_MASK                           0x1F

// Register: SSPDATPPS
#define SSPDATPPS SSPDATPPS
extern volatile unsigned char           SSPDATPPS           __at(0xE21);
#ifndef _LIB_BUILD
asm("SSPDATPPS equ 0E21h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SSPDATPPS              :5;
    };
} SSPDATPPSbits_t;
extern volatile SSPDATPPSbits_t SSPDATPPSbits __at(0xE21);
// bitfield macros
#define _SSPDATPPS_SSPDATPPS_POSN                           0x0
#define _SSPDATPPS_SSPDATPPS_POSITION                       0x0
#define _SSPDATPPS_SSPDATPPS_SIZE                           0x5
#define _SSPDATPPS_SSPDATPPS_LENGTH                         0x5
#define _SSPDATPPS_SSPDATPPS_MASK                           0x1F

// Register: SSPSSPPS
#define SSPSSPPS SSPSSPPS
extern volatile unsigned char           SSPSSPPS            __at(0xE22);
#ifndef _LIB_BUILD
asm("SSPSSPPS equ 0E22h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned SSPSSPPS               :5;
    };
} SSPSSPPSbits_t;
extern volatile SSPSSPPSbits_t SSPSSPPSbits __at(0xE22);
// bitfield macros
#define _SSPSSPPS_SSPSSPPS_POSN                             0x0
#define _SSPSSPPS_SSPSSPPS_POSITION                         0x0
#define _SSPSSPPS_SSPSSPPS_SIZE                             0x5
#define _SSPSSPPS_SSPSSPPS_LENGTH                           0x5
#define _SSPSSPPS_SSPSSPPS_MASK                             0x1F

// Register: RXPPS
#define RXPPS RXPPS
extern volatile unsigned char           RXPPS               __at(0xE24);
#ifndef _LIB_BUILD
asm("RXPPS equ 0E24h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RXPPS                  :5;
    };
} RXPPSbits_t;
extern volatile RXPPSbits_t RXPPSbits __at(0xE24);
// bitfield macros
#define _RXPPS_RXPPS_POSN                                   0x0
#define _RXPPS_RXPPS_POSITION                               0x0
#define _RXPPS_RXPPS_SIZE                                   0x5
#define _RXPPS_RXPPS_LENGTH                                 0x5
#define _RXPPS_RXPPS_MASK                                   0x1F

// Register: CKPPS
#define CKPPS CKPPS
extern volatile unsigned char           CKPPS               __at(0xE25);
#ifndef _LIB_BUILD
asm("CKPPS equ 0E25h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CKPPS                  :5;
    };
} CKPPSbits_t;
extern volatile CKPPSbits_t CKPPSbits __at(0xE25);
// bitfield macros
#define _CKPPS_CKPPS_POSN                                   0x0
#define _CKPPS_CKPPS_POSITION                               0x0
#define _CKPPS_CKPPS_SIZE                                   0x5
#define _CKPPS_CKPPS_LENGTH                                 0x5
#define _CKPPS_CKPPS_MASK                                   0x1F

// Register: CLCIN0PPS
#define CLCIN0PPS CLCIN0PPS
extern volatile unsigned char           CLCIN0PPS           __at(0xE28);
#ifndef _LIB_BUILD
asm("CLCIN0PPS equ 0E28h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CLCIN0PPS              :5;
    };
} CLCIN0PPSbits_t;
extern volatile CLCIN0PPSbits_t CLCIN0PPSbits __at(0xE28);
// bitfield macros
#define _CLCIN0PPS_CLCIN0PPS_POSN                           0x0
#define _CLCIN0PPS_CLCIN0PPS_POSITION                       0x0
#define _CLCIN0PPS_CLCIN0PPS_SIZE                           0x5
#define _CLCIN0PPS_CLCIN0PPS_LENGTH                         0x5
#define _CLCIN0PPS_CLCIN0PPS_MASK                           0x1F

// Register: CLCIN1PPS
#define CLCIN1PPS CLCIN1PPS
extern volatile unsigned char           CLCIN1PPS           __at(0xE29);
#ifndef _LIB_BUILD
asm("CLCIN1PPS equ 0E29h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CLCIN1PPS              :5;
    };
} CLCIN1PPSbits_t;
extern volatile CLCIN1PPSbits_t CLCIN1PPSbits __at(0xE29);
// bitfield macros
#define _CLCIN1PPS_CLCIN1PPS_POSN                           0x0
#define _CLCIN1PPS_CLCIN1PPS_POSITION                       0x0
#define _CLCIN1PPS_CLCIN1PPS_SIZE                           0x5
#define _CLCIN1PPS_CLCIN1PPS_LENGTH                         0x5
#define _CLCIN1PPS_CLCIN1PPS_MASK                           0x1F

// Register: CLCIN2PPS
#define CLCIN2PPS CLCIN2PPS
extern volatile unsigned char           CLCIN2PPS           __at(0xE2A);
#ifndef _LIB_BUILD
asm("CLCIN2PPS equ 0E2Ah");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CLCIN2PPS              :5;
    };
} CLCIN2PPSbits_t;
extern volatile CLCIN2PPSbits_t CLCIN2PPSbits __at(0xE2A);
// bitfield macros
#define _CLCIN2PPS_CLCIN2PPS_POSN                           0x0
#define _CLCIN2PPS_CLCIN2PPS_POSITION                       0x0
#define _CLCIN2PPS_CLCIN2PPS_SIZE                           0x5
#define _CLCIN2PPS_CLCIN2PPS_LENGTH                         0x5
#define _CLCIN2PPS_CLCIN2PPS_MASK                           0x1F

// Register: CLCIN3PPS
#define CLCIN3PPS CLCIN3PPS
extern volatile unsigned char           CLCIN3PPS           __at(0xE2B);
#ifndef _LIB_BUILD
asm("CLCIN3PPS equ 0E2Bh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned CLCIN3PPS              :5;
    };
} CLCIN3PPSbits_t;
extern volatile CLCIN3PPSbits_t CLCIN3PPSbits __at(0xE2B);
// bitfield macros
#define _CLCIN3PPS_CLCIN3PPS_POSN                           0x0
#define _CLCIN3PPS_CLCIN3PPS_POSITION                       0x0
#define _CLCIN3PPS_CLCIN3PPS_SIZE                           0x5
#define _CLCIN3PPS_CLCIN3PPS_LENGTH                         0x5
#define _CLCIN3PPS_CLCIN3PPS_MASK                           0x1F

// Register: RA0PPS
#define RA0PPS RA0PPS
extern volatile unsigned char           RA0PPS              __at(0xE90);
#ifndef _LIB_BUILD
asm("RA0PPS equ 0E90h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RA0PPS                 :5;
    };
} RA0PPSbits_t;
extern volatile RA0PPSbits_t RA0PPSbits __at(0xE90);
// bitfield macros
#define _RA0PPS_RA0PPS_POSN                                 0x0
#define _RA0PPS_RA0PPS_POSITION                             0x0
#define _RA0PPS_RA0PPS_SIZE                                 0x5
#define _RA0PPS_RA0PPS_LENGTH                               0x5
#define _RA0PPS_RA0PPS_MASK                                 0x1F

// Register: RA1PPS
#define RA1PPS RA1PPS
extern volatile unsigned char           RA1PPS              __at(0xE91);
#ifndef _LIB_BUILD
asm("RA1PPS equ 0E91h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RA1PPS                 :5;
    };
} RA1PPSbits_t;
extern volatile RA1PPSbits_t RA1PPSbits __at(0xE91);
// bitfield macros
#define _RA1PPS_RA1PPS_POSN                                 0x0
#define _RA1PPS_RA1PPS_POSITION                             0x0
#define _RA1PPS_RA1PPS_SIZE                                 0x5
#define _RA1PPS_RA1PPS_LENGTH                               0x5
#define _RA1PPS_RA1PPS_MASK                                 0x1F

// Register: RA2PPS
#define RA2PPS RA2PPS
extern volatile unsigned char           RA2PPS              __at(0xE92);
#ifndef _LIB_BUILD
asm("RA2PPS equ 0E92h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RA2PPS                 :5;
    };
} RA2PPSbits_t;
extern volatile RA2PPSbits_t RA2PPSbits __at(0xE92);
// bitfield macros
#define _RA2PPS_RA2PPS_POSN                                 0x0
#define _RA2PPS_RA2PPS_POSITION                             0x0
#define _RA2PPS_RA2PPS_SIZE                                 0x5
#define _RA2PPS_RA2PPS_LENGTH                               0x5
#define _RA2PPS_RA2PPS_MASK                                 0x1F

// Register: RA4PPS
#define RA4PPS RA4PPS
extern volatile unsigned char           RA4PPS              __at(0xE94);
#ifndef _LIB_BUILD
asm("RA4PPS equ 0E94h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RA4PPS                 :5;
    };
} RA4PPSbits_t;
extern volatile RA4PPSbits_t RA4PPSbits __at(0xE94);
// bitfield macros
#define _RA4PPS_RA4PPS_POSN                                 0x0
#define _RA4PPS_RA4PPS_POSITION                             0x0
#define _RA4PPS_RA4PPS_SIZE                                 0x5
#define _RA4PPS_RA4PPS_LENGTH                               0x5
#define _RA4PPS_RA4PPS_MASK                                 0x1F

// Register: RA5PPS
#define RA5PPS RA5PPS
extern volatile unsigned char           RA5PPS              __at(0xE95);
#ifndef _LIB_BUILD
asm("RA5PPS equ 0E95h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RA5PPS                 :5;
    };
} RA5PPSbits_t;
extern volatile RA5PPSbits_t RA5PPSbits __at(0xE95);
// bitfield macros
#define _RA5PPS_RA5PPS_POSN                                 0x0
#define _RA5PPS_RA5PPS_POSITION                             0x0
#define _RA5PPS_RA5PPS_SIZE                                 0x5
#define _RA5PPS_RA5PPS_LENGTH                               0x5
#define _RA5PPS_RA5PPS_MASK                                 0x1F

// Register: RB4PPS
#define RB4PPS RB4PPS
extern volatile unsigned char           RB4PPS              __at(0xE9C);
#ifndef _LIB_BUILD
asm("RB4PPS equ 0E9Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RB4PPS                 :5;
    };
} RB4PPSbits_t;
extern volatile RB4PPSbits_t RB4PPSbits __at(0xE9C);
// bitfield macros
#define _RB4PPS_RB4PPS_POSN                                 0x0
#define _RB4PPS_RB4PPS_POSITION                             0x0
#define _RB4PPS_RB4PPS_SIZE                                 0x5
#define _RB4PPS_RB4PPS_LENGTH                               0x5
#define _RB4PPS_RB4PPS_MASK                                 0x1F

// Register: RB5PPS
#define RB5PPS RB5PPS
extern volatile unsigned char           RB5PPS              __at(0xE9D);
#ifndef _LIB_BUILD
asm("RB5PPS equ 0E9Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RB5PPS                 :5;
    };
} RB5PPSbits_t;
extern volatile RB5PPSbits_t RB5PPSbits __at(0xE9D);
// bitfield macros
#define _RB5PPS_RB5PPS_POSN                                 0x0
#define _RB5PPS_RB5PPS_POSITION                             0x0
#define _RB5PPS_RB5PPS_SIZE                                 0x5
#define _RB5PPS_RB5PPS_LENGTH                               0x5
#define _RB5PPS_RB5PPS_MASK                                 0x1F

// Register: RB6PPS
#define RB6PPS RB6PPS
extern volatile unsigned char           RB6PPS              __at(0xE9E);
#ifndef _LIB_BUILD
asm("RB6PPS equ 0E9Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RB6PPS                 :5;
    };
} RB6PPSbits_t;
extern volatile RB6PPSbits_t RB6PPSbits __at(0xE9E);
// bitfield macros
#define _RB6PPS_RB6PPS_POSN                                 0x0
#define _RB6PPS_RB6PPS_POSITION                             0x0
#define _RB6PPS_RB6PPS_SIZE                                 0x5
#define _RB6PPS_RB6PPS_LENGTH                               0x5
#define _RB6PPS_RB6PPS_MASK                                 0x1F

// Register: RB7PPS
#define RB7PPS RB7PPS
extern volatile unsigned char           RB7PPS              __at(0xE9F);
#ifndef _LIB_BUILD
asm("RB7PPS equ 0E9Fh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RB7PPS                 :5;
    };
} RB7PPSbits_t;
extern volatile RB7PPSbits_t RB7PPSbits __at(0xE9F);
// bitfield macros
#define _RB7PPS_RB7PPS_POSN                                 0x0
#define _RB7PPS_RB7PPS_POSITION                             0x0
#define _RB7PPS_RB7PPS_SIZE                                 0x5
#define _RB7PPS_RB7PPS_LENGTH                               0x5
#define _RB7PPS_RB7PPS_MASK                                 0x1F

// Register: RC0PPS
#define RC0PPS RC0PPS
extern volatile unsigned char           RC0PPS              __at(0xEA0);
#ifndef _LIB_BUILD
asm("RC0PPS equ 0EA0h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RC0PPS                 :5;
    };
} RC0PPSbits_t;
extern volatile RC0PPSbits_t RC0PPSbits __at(0xEA0);
// bitfield macros
#define _RC0PPS_RC0PPS_POSN                                 0x0
#define _RC0PPS_RC0PPS_POSITION                             0x0
#define _RC0PPS_RC0PPS_SIZE                                 0x5
#define _RC0PPS_RC0PPS_LENGTH                               0x5
#define _RC0PPS_RC0PPS_MASK                                 0x1F

// Register: RC1PPS
#define RC1PPS RC1PPS
extern volatile unsigned char           RC1PPS              __at(0xEA1);
#ifndef _LIB_BUILD
asm("RC1PPS equ 0EA1h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RC1PPS                 :5;
    };
} RC1PPSbits_t;
extern volatile RC1PPSbits_t RC1PPSbits __at(0xEA1);
// bitfield macros
#define _RC1PPS_RC1PPS_POSN                                 0x0
#define _RC1PPS_RC1PPS_POSITION                             0x0
#define _RC1PPS_RC1PPS_SIZE                                 0x5
#define _RC1PPS_RC1PPS_LENGTH                               0x5
#define _RC1PPS_RC1PPS_MASK                                 0x1F

// Register: RC2PPS
#define RC2PPS RC2PPS
extern volatile unsigned char           RC2PPS              __at(0xEA2);
#ifndef _LIB_BUILD
asm("RC2PPS equ 0EA2h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RC2PPS                 :5;
    };
} RC2PPSbits_t;
extern volatile RC2PPSbits_t RC2PPSbits __at(0xEA2);
// bitfield macros
#define _RC2PPS_RC2PPS_POSN                                 0x0
#define _RC2PPS_RC2PPS_POSITION                             0x0
#define _RC2PPS_RC2PPS_SIZE                                 0x5
#define _RC2PPS_RC2PPS_LENGTH                               0x5
#define _RC2PPS_RC2PPS_MASK                                 0x1F

// Register: RC3PPS
#define RC3PPS RC3PPS
extern volatile unsigned char           RC3PPS              __at(0xEA3);
#ifndef _LIB_BUILD
asm("RC3PPS equ 0EA3h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RC3PPS                 :5;
    };
} RC3PPSbits_t;
extern volatile RC3PPSbits_t RC3PPSbits __at(0xEA3);
// bitfield macros
#define _RC3PPS_RC3PPS_POSN                                 0x0
#define _RC3PPS_RC3PPS_POSITION                             0x0
#define _RC3PPS_RC3PPS_SIZE                                 0x5
#define _RC3PPS_RC3PPS_LENGTH                               0x5
#define _RC3PPS_RC3PPS_MASK                                 0x1F

// Register: RC4PPS
#define RC4PPS RC4PPS
extern volatile unsigned char           RC4PPS              __at(0xEA4);
#ifndef _LIB_BUILD
asm("RC4PPS equ 0EA4h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RC4PPS                 :5;
    };
} RC4PPSbits_t;
extern volatile RC4PPSbits_t RC4PPSbits __at(0xEA4);
// bitfield macros
#define _RC4PPS_RC4PPS_POSN                                 0x0
#define _RC4PPS_RC4PPS_POSITION                             0x0
#define _RC4PPS_RC4PPS_SIZE                                 0x5
#define _RC4PPS_RC4PPS_LENGTH                               0x5
#define _RC4PPS_RC4PPS_MASK                                 0x1F

// Register: RC5PPS
#define RC5PPS RC5PPS
extern volatile unsigned char           RC5PPS              __at(0xEA5);
#ifndef _LIB_BUILD
asm("RC5PPS equ 0EA5h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RC5PPS                 :5;
    };
} RC5PPSbits_t;
extern volatile RC5PPSbits_t RC5PPSbits __at(0xEA5);
// bitfield macros
#define _RC5PPS_RC5PPS_POSN                                 0x0
#define _RC5PPS_RC5PPS_POSITION                             0x0
#define _RC5PPS_RC5PPS_SIZE                                 0x5
#define _RC5PPS_RC5PPS_LENGTH                               0x5
#define _RC5PPS_RC5PPS_MASK                                 0x1F

// Register: RC6PPS
#define RC6PPS RC6PPS
extern volatile unsigned char           RC6PPS              __at(0xEA6);
#ifndef _LIB_BUILD
asm("RC6PPS equ 0EA6h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RC6PPS                 :5;
    };
} RC6PPSbits_t;
extern volatile RC6PPSbits_t RC6PPSbits __at(0xEA6);
// bitfield macros
#define _RC6PPS_RC6PPS_POSN                                 0x0
#define _RC6PPS_RC6PPS_POSITION                             0x0
#define _RC6PPS_RC6PPS_SIZE                                 0x5
#define _RC6PPS_RC6PPS_LENGTH                               0x5
#define _RC6PPS_RC6PPS_MASK                                 0x1F

// Register: RC7PPS
#define RC7PPS RC7PPS
extern volatile unsigned char           RC7PPS              __at(0xEA7);
#ifndef _LIB_BUILD
asm("RC7PPS equ 0EA7h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned RC7PPS                 :5;
    };
} RC7PPSbits_t;
extern volatile RC7PPSbits_t RC7PPSbits __at(0xEA7);
// bitfield macros
#define _RC7PPS_RC7PPS_POSN                                 0x0
#define _RC7PPS_RC7PPS_POSITION                             0x0
#define _RC7PPS_RC7PPS_SIZE                                 0x5
#define _RC7PPS_RC7PPS_LENGTH                               0x5
#define _RC7PPS_RC7PPS_MASK                                 0x1F

// Register: CLCDATA
#define CLCDATA CLCDATA
extern volatile unsigned char           CLCDATA             __at(0xF0F);
#ifndef _LIB_BUILD
asm("CLCDATA equ 0F0Fh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned MCLC1OUT               :1;
        unsigned MCLC2OUT               :1;
        unsigned MCLC3OUT               :1;
    };
} CLCDATAbits_t;
extern volatile CLCDATAbits_t CLCDATAbits __at(0xF0F);
// bitfield macros
#define _CLCDATA_MCLC1OUT_POSN                              0x0
#define _CLCDATA_MCLC1OUT_POSITION                          0x0
#define _CLCDATA_MCLC1OUT_SIZE                              0x1
#define _CLCDATA_MCLC1OUT_LENGTH                            0x1
#define _CLCDATA_MCLC1OUT_MASK                              0x1
#define _CLCDATA_MCLC2OUT_POSN                              0x1
#define _CLCDATA_MCLC2OUT_POSITION                          0x1
#define _CLCDATA_MCLC2OUT_SIZE                              0x1
#define _CLCDATA_MCLC2OUT_LENGTH                            0x1
#define _CLCDATA_MCLC2OUT_MASK                              0x2
#define _CLCDATA_MCLC3OUT_POSN                              0x2
#define _CLCDATA_MCLC3OUT_POSITION                          0x2
#define _CLCDATA_MCLC3OUT_SIZE                              0x1
#define _CLCDATA_MCLC3OUT_LENGTH                            0x1
#define _CLCDATA_MCLC3OUT_MASK                              0x4

// Register: CLC1CON
#define CLC1CON CLC1CON
extern volatile unsigned char           CLC1CON             __at(0xF10);
#ifndef _LIB_BUILD
asm("CLC1CON equ 0F10h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC1MODE                :3;
        unsigned LC1INTN                :1;
        unsigned LC1INTP                :1;
        unsigned LC1OUT                 :1;
        unsigned                        :1;
        unsigned LC1EN                  :1;
    };
    struct {
        unsigned LC1MODE0               :1;
        unsigned LC1MODE1               :1;
        unsigned LC1MODE2               :1;
    };
    struct {
        unsigned MODE                   :3;
        unsigned INTN                   :1;
        unsigned INTP                   :1;
        unsigned OUT                    :1;
        unsigned                        :1;
        unsigned EN                     :1;
    };
    struct {
        unsigned MODE0                  :1;
        unsigned MODE1                  :1;
        unsigned MODE2                  :1;
    };
} CLC1CONbits_t;
extern volatile CLC1CONbits_t CLC1CONbits __at(0xF10);
// bitfield macros
#define _CLC1CON_LC1MODE_POSN                               0x0
#define _CLC1CON_LC1MODE_POSITION                           0x0
#define _CLC1CON_LC1MODE_SIZE                               0x3
#define _CLC1CON_LC1MODE_LENGTH                             0x3
#define _CLC1CON_LC1MODE_MASK                               0x7
#define _CLC1CON_LC1INTN_POSN                               0x3
#define _CLC1CON_LC1INTN_POSITION                           0x3
#define _CLC1CON_LC1INTN_SIZE                               0x1
#define _CLC1CON_LC1INTN_LENGTH                             0x1
#define _CLC1CON_LC1INTN_MASK                               0x8
#define _CLC1CON_LC1INTP_POSN                               0x4
#define _CLC1CON_LC1INTP_POSITION                           0x4
#define _CLC1CON_LC1INTP_SIZE                               0x1
#define _CLC1CON_LC1INTP_LENGTH                             0x1
#define _CLC1CON_LC1INTP_MASK                               0x10
#define _CLC1CON_LC1OUT_POSN                                0x5
#define _CLC1CON_LC1OUT_POSITION                            0x5
#define _CLC1CON_LC1OUT_SIZE                                0x1
#define _CLC1CON_LC1OUT_LENGTH                              0x1
#define _CLC1CON_LC1OUT_MASK                                0x20
#define _CLC1CON_LC1EN_POSN                                 0x7
#define _CLC1CON_LC1EN_POSITION                             0x7
#define _CLC1CON_LC1EN_SIZE                                 0x1
#define _CLC1CON_LC1EN_LENGTH                               0x1
#define _CLC1CON_LC1EN_MASK                                 0x80
#define _CLC1CON_LC1MODE0_POSN                              0x0
#define _CLC1CON_LC1MODE0_POSITION                          0x0
#define _CLC1CON_LC1MODE0_SIZE                              0x1
#define _CLC1CON_LC1MODE0_LENGTH                            0x1
#define _CLC1CON_LC1MODE0_MASK                              0x1
#define _CLC1CON_LC1MODE1_POSN                              0x1
#define _CLC1CON_LC1MODE1_POSITION                          0x1
#define _CLC1CON_LC1MODE1_SIZE                              0x1
#define _CLC1CON_LC1MODE1_LENGTH                            0x1
#define _CLC1CON_LC1MODE1_MASK                              0x2
#define _CLC1CON_LC1MODE2_POSN                              0x2
#define _CLC1CON_LC1MODE2_POSITION                          0x2
#define _CLC1CON_LC1MODE2_SIZE                              0x1
#define _CLC1CON_LC1MODE2_LENGTH                            0x1
#define _CLC1CON_LC1MODE2_MASK                              0x4
#define _CLC1CON_MODE_POSN                                  0x0
#define _CLC1CON_MODE_POSITION                              0x0
#define _CLC1CON_MODE_SIZE                                  0x3
#define _CLC1CON_MODE_LENGTH                                0x3
#define _CLC1CON_MODE_MASK                                  0x7
#define _CLC1CON_INTN_POSN                                  0x3
#define _CLC1CON_INTN_POSITION                              0x3
#define _CLC1CON_INTN_SIZE                                  0x1
#define _CLC1CON_INTN_LENGTH                                0x1
#define _CLC1CON_INTN_MASK                                  0x8
#define _CLC1CON_INTP_POSN                                  0x4
#define _CLC1CON_INTP_POSITION                              0x4
#define _CLC1CON_INTP_SIZE                                  0x1
#define _CLC1CON_INTP_LENGTH                                0x1
#define _CLC1CON_INTP_MASK                                  0x10
#define _CLC1CON_OUT_POSN                                   0x5
#define _CLC1CON_OUT_POSITION                               0x5
#define _CLC1CON_OUT_SIZE                                   0x1
#define _CLC1CON_OUT_LENGTH                                 0x1
#define _CLC1CON_OUT_MASK                                   0x20
#define _CLC1CON_EN_POSN                                    0x7
#define _CLC1CON_EN_POSITION                                0x7
#define _CLC1CON_EN_SIZE                                    0x1
#define _CLC1CON_EN_LENGTH                                  0x1
#define _CLC1CON_EN_MASK                                    0x80
#define _CLC1CON_MODE0_POSN                                 0x0
#define _CLC1CON_MODE0_POSITION                             0x0
#define _CLC1CON_MODE0_SIZE                                 0x1
#define _CLC1CON_MODE0_LENGTH                               0x1
#define _CLC1CON_MODE0_MASK                                 0x1
#define _CLC1CON_MODE1_POSN                                 0x1
#define _CLC1CON_MODE1_POSITION                             0x1
#define _CLC1CON_MODE1_SIZE                                 0x1
#define _CLC1CON_MODE1_LENGTH                               0x1
#define _CLC1CON_MODE1_MASK                                 0x2
#define _CLC1CON_MODE2_POSN                                 0x2
#define _CLC1CON_MODE2_POSITION                             0x2
#define _CLC1CON_MODE2_SIZE                                 0x1
#define _CLC1CON_MODE2_LENGTH                               0x1
#define _CLC1CON_MODE2_MASK                                 0x4

// Register: CLC1POL
#define CLC1POL CLC1POL
extern volatile unsigned char           CLC1POL             __at(0xF11);
#ifndef _LIB_BUILD
asm("CLC1POL equ 0F11h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC1G1POL               :1;
        unsigned LC1G2POL               :1;
        unsigned LC1G3POL               :1;
        unsigned LC1G4POL               :1;
        unsigned                        :3;
        unsigned LC1POL                 :1;
    };
    struct {
        unsigned G1POL                  :1;
        unsigned G2POL                  :1;
        unsigned G3POL                  :1;
        unsigned G4POL                  :1;
        unsigned                        :3;
        unsigned POL                    :1;
    };
} CLC1POLbits_t;
extern volatile CLC1POLbits_t CLC1POLbits __at(0xF11);
// bitfield macros
#define _CLC1POL_LC1G1POL_POSN                              0x0
#define _CLC1POL_LC1G1POL_POSITION                          0x0
#define _CLC1POL_LC1G1POL_SIZE                              0x1
#define _CLC1POL_LC1G1POL_LENGTH                            0x1
#define _CLC1POL_LC1G1POL_MASK                              0x1
#define _CLC1POL_LC1G2POL_POSN                              0x1
#define _CLC1POL_LC1G2POL_POSITION                          0x1
#define _CLC1POL_LC1G2POL_SIZE                              0x1
#define _CLC1POL_LC1G2POL_LENGTH                            0x1
#define _CLC1POL_LC1G2POL_MASK                              0x2
#define _CLC1POL_LC1G3POL_POSN                              0x2
#define _CLC1POL_LC1G3POL_POSITION                          0x2
#define _CLC1POL_LC1G3POL_SIZE                              0x1
#define _CLC1POL_LC1G3POL_LENGTH                            0x1
#define _CLC1POL_LC1G3POL_MASK                              0x4
#define _CLC1POL_LC1G4POL_POSN                              0x3
#define _CLC1POL_LC1G4POL_POSITION                          0x3
#define _CLC1POL_LC1G4POL_SIZE                              0x1
#define _CLC1POL_LC1G4POL_LENGTH                            0x1
#define _CLC1POL_LC1G4POL_MASK                              0x8
#define _CLC1POL_LC1POL_POSN                                0x7
#define _CLC1POL_LC1POL_POSITION                            0x7
#define _CLC1POL_LC1POL_SIZE                                0x1
#define _CLC1POL_LC1POL_LENGTH                              0x1
#define _CLC1POL_LC1POL_MASK                                0x80
#define _CLC1POL_G1POL_POSN                                 0x0
#define _CLC1POL_G1POL_POSITION                             0x0
#define _CLC1POL_G1POL_SIZE                                 0x1
#define _CLC1POL_G1POL_LENGTH                               0x1
#define _CLC1POL_G1POL_MASK                                 0x1
#define _CLC1POL_G2POL_POSN                                 0x1
#define _CLC1POL_G2POL_POSITION                             0x1
#define _CLC1POL_G2POL_SIZE                                 0x1
#define _CLC1POL_G2POL_LENGTH                               0x1
#define _CLC1POL_G2POL_MASK                                 0x2
#define _CLC1POL_G3POL_POSN                                 0x2
#define _CLC1POL_G3POL_POSITION                             0x2
#define _CLC1POL_G3POL_SIZE                                 0x1
#define _CLC1POL_G3POL_LENGTH                               0x1
#define _CLC1POL_G3POL_MASK                                 0x4
#define _CLC1POL_G4POL_POSN                                 0x3
#define _CLC1POL_G4POL_POSITION                             0x3
#define _CLC1POL_G4POL_SIZE                                 0x1
#define _CLC1POL_G4POL_LENGTH                               0x1
#define _CLC1POL_G4POL_MASK                                 0x8
#define _CLC1POL_POL_POSN                                   0x7
#define _CLC1POL_POL_POSITION                               0x7
#define _CLC1POL_POL_SIZE                                   0x1
#define _CLC1POL_POL_LENGTH                                 0x1
#define _CLC1POL_POL_MASK                                   0x80

// Register: CLC1SEL0
#define CLC1SEL0 CLC1SEL0
extern volatile unsigned char           CLC1SEL0            __at(0xF12);
#ifndef _LIB_BUILD
asm("CLC1SEL0 equ 0F12h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC1D1S0                :1;
        unsigned LC1D1S1                :1;
        unsigned LC1D1S2                :1;
        unsigned LC1D1S3                :1;
        unsigned LC1D1S4                :1;
    };
    struct {
        unsigned LC1D1S                 :8;
    };
    struct {
        unsigned D1S                    :8;
    };
    struct {
        unsigned D1S0                   :1;
        unsigned D1S1                   :1;
        unsigned D1S2                   :1;
        unsigned D1S3                   :1;
        unsigned D1S4                   :1;
    };
} CLC1SEL0bits_t;
extern volatile CLC1SEL0bits_t CLC1SEL0bits __at(0xF12);
// bitfield macros
#define _CLC1SEL0_LC1D1S0_POSN                              0x0
#define _CLC1SEL0_LC1D1S0_POSITION                          0x0
#define _CLC1SEL0_LC1D1S0_SIZE                              0x1
#define _CLC1SEL0_LC1D1S0_LENGTH                            0x1
#define _CLC1SEL0_LC1D1S0_MASK                              0x1
#define _CLC1SEL0_LC1D1S1_POSN                              0x1
#define _CLC1SEL0_LC1D1S1_POSITION                          0x1
#define _CLC1SEL0_LC1D1S1_SIZE                              0x1
#define _CLC1SEL0_LC1D1S1_LENGTH                            0x1
#define _CLC1SEL0_LC1D1S1_MASK                              0x2
#define _CLC1SEL0_LC1D1S2_POSN                              0x2
#define _CLC1SEL0_LC1D1S2_POSITION                          0x2
#define _CLC1SEL0_LC1D1S2_SIZE                              0x1
#define _CLC1SEL0_LC1D1S2_LENGTH                            0x1
#define _CLC1SEL0_LC1D1S2_MASK                              0x4
#define _CLC1SEL0_LC1D1S3_POSN                              0x3
#define _CLC1SEL0_LC1D1S3_POSITION                          0x3
#define _CLC1SEL0_LC1D1S3_SIZE                              0x1
#define _CLC1SEL0_LC1D1S3_LENGTH                            0x1
#define _CLC1SEL0_LC1D1S3_MASK                              0x8
#define _CLC1SEL0_LC1D1S4_POSN                              0x4
#define _CLC1SEL0_LC1D1S4_POSITION                          0x4
#define _CLC1SEL0_LC1D1S4_SIZE                              0x1
#define _CLC1SEL0_LC1D1S4_LENGTH                            0x1
#define _CLC1SEL0_LC1D1S4_MASK                              0x10
#define _CLC1SEL0_LC1D1S_POSN                               0x0
#define _CLC1SEL0_LC1D1S_POSITION                           0x0
#define _CLC1SEL0_LC1D1S_SIZE                               0x8
#define _CLC1SEL0_LC1D1S_LENGTH                             0x8
#define _CLC1SEL0_LC1D1S_MASK                               0xFF
#define _CLC1SEL0_D1S_POSN                                  0x0
#define _CLC1SEL0_D1S_POSITION                              0x0
#define _CLC1SEL0_D1S_SIZE                                  0x8
#define _CLC1SEL0_D1S_LENGTH                                0x8
#define _CLC1SEL0_D1S_MASK                                  0xFF
#define _CLC1SEL0_D1S0_POSN                                 0x0
#define _CLC1SEL0_D1S0_POSITION                             0x0
#define _CLC1SEL0_D1S0_SIZE                                 0x1
#define _CLC1SEL0_D1S0_LENGTH                               0x1
#define _CLC1SEL0_D1S0_MASK                                 0x1
#define _CLC1SEL0_D1S1_POSN                                 0x1
#define _CLC1SEL0_D1S1_POSITION                             0x1
#define _CLC1SEL0_D1S1_SIZE                                 0x1
#define _CLC1SEL0_D1S1_LENGTH                               0x1
#define _CLC1SEL0_D1S1_MASK                                 0x2
#define _CLC1SEL0_D1S2_POSN                                 0x2
#define _CLC1SEL0_D1S2_POSITION                             0x2
#define _CLC1SEL0_D1S2_SIZE                                 0x1
#define _CLC1SEL0_D1S2_LENGTH                               0x1
#define _CLC1SEL0_D1S2_MASK                                 0x4
#define _CLC1SEL0_D1S3_POSN                                 0x3
#define _CLC1SEL0_D1S3_POSITION                             0x3
#define _CLC1SEL0_D1S3_SIZE                                 0x1
#define _CLC1SEL0_D1S3_LENGTH                               0x1
#define _CLC1SEL0_D1S3_MASK                                 0x8
#define _CLC1SEL0_D1S4_POSN                                 0x4
#define _CLC1SEL0_D1S4_POSITION                             0x4
#define _CLC1SEL0_D1S4_SIZE                                 0x1
#define _CLC1SEL0_D1S4_LENGTH                               0x1
#define _CLC1SEL0_D1S4_MASK                                 0x10

// Register: CLC1SEL1
#define CLC1SEL1 CLC1SEL1
extern volatile unsigned char           CLC1SEL1            __at(0xF13);
#ifndef _LIB_BUILD
asm("CLC1SEL1 equ 0F13h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC1D2S0                :1;
        unsigned LC1D2S1                :1;
        unsigned LC1D2S2                :1;
        unsigned LC1D2S3                :1;
        unsigned LC1D2S4                :1;
    };
    struct {
        unsigned LC1D2S                 :8;
    };
    struct {
        unsigned D2S                    :8;
    };
    struct {
        unsigned D2S0                   :1;
        unsigned D2S1                   :1;
        unsigned D2S2                   :1;
        unsigned D2S3                   :1;
        unsigned D2S4                   :1;
    };
} CLC1SEL1bits_t;
extern volatile CLC1SEL1bits_t CLC1SEL1bits __at(0xF13);
// bitfield macros
#define _CLC1SEL1_LC1D2S0_POSN                              0x0
#define _CLC1SEL1_LC1D2S0_POSITION                          0x0
#define _CLC1SEL1_LC1D2S0_SIZE                              0x1
#define _CLC1SEL1_LC1D2S0_LENGTH                            0x1
#define _CLC1SEL1_LC1D2S0_MASK                              0x1
#define _CLC1SEL1_LC1D2S1_POSN                              0x1
#define _CLC1SEL1_LC1D2S1_POSITION                          0x1
#define _CLC1SEL1_LC1D2S1_SIZE                              0x1
#define _CLC1SEL1_LC1D2S1_LENGTH                            0x1
#define _CLC1SEL1_LC1D2S1_MASK                              0x2
#define _CLC1SEL1_LC1D2S2_POSN                              0x2
#define _CLC1SEL1_LC1D2S2_POSITION                          0x2
#define _CLC1SEL1_LC1D2S2_SIZE                              0x1
#define _CLC1SEL1_LC1D2S2_LENGTH                            0x1
#define _CLC1SEL1_LC1D2S2_MASK                              0x4
#define _CLC1SEL1_LC1D2S3_POSN                              0x3
#define _CLC1SEL1_LC1D2S3_POSITION                          0x3
#define _CLC1SEL1_LC1D2S3_SIZE                              0x1
#define _CLC1SEL1_LC1D2S3_LENGTH                            0x1
#define _CLC1SEL1_LC1D2S3_MASK                              0x8
#define _CLC1SEL1_LC1D2S4_POSN                              0x4
#define _CLC1SEL1_LC1D2S4_POSITION                          0x4
#define _CLC1SEL1_LC1D2S4_SIZE                              0x1
#define _CLC1SEL1_LC1D2S4_LENGTH                            0x1
#define _CLC1SEL1_LC1D2S4_MASK                              0x10
#define _CLC1SEL1_LC1D2S_POSN                               0x0
#define _CLC1SEL1_LC1D2S_POSITION                           0x0
#define _CLC1SEL1_LC1D2S_SIZE                               0x8
#define _CLC1SEL1_LC1D2S_LENGTH                             0x8
#define _CLC1SEL1_LC1D2S_MASK                               0xFF
#define _CLC1SEL1_D2S_POSN                                  0x0
#define _CLC1SEL1_D2S_POSITION                              0x0
#define _CLC1SEL1_D2S_SIZE                                  0x8
#define _CLC1SEL1_D2S_LENGTH                                0x8
#define _CLC1SEL1_D2S_MASK                                  0xFF
#define _CLC1SEL1_D2S0_POSN                                 0x0
#define _CLC1SEL1_D2S0_POSITION                             0x0
#define _CLC1SEL1_D2S0_SIZE                                 0x1
#define _CLC1SEL1_D2S0_LENGTH                               0x1
#define _CLC1SEL1_D2S0_MASK                                 0x1
#define _CLC1SEL1_D2S1_POSN                                 0x1
#define _CLC1SEL1_D2S1_POSITION                             0x1
#define _CLC1SEL1_D2S1_SIZE                                 0x1
#define _CLC1SEL1_D2S1_LENGTH                               0x1
#define _CLC1SEL1_D2S1_MASK                                 0x2
#define _CLC1SEL1_D2S2_POSN                                 0x2
#define _CLC1SEL1_D2S2_POSITION                             0x2
#define _CLC1SEL1_D2S2_SIZE                                 0x1
#define _CLC1SEL1_D2S2_LENGTH                               0x1
#define _CLC1SEL1_D2S2_MASK                                 0x4
#define _CLC1SEL1_D2S3_POSN                                 0x3
#define _CLC1SEL1_D2S3_POSITION                             0x3
#define _CLC1SEL1_D2S3_SIZE                                 0x1
#define _CLC1SEL1_D2S3_LENGTH                               0x1
#define _CLC1SEL1_D2S3_MASK                                 0x8
#define _CLC1SEL1_D2S4_POSN                                 0x4
#define _CLC1SEL1_D2S4_POSITION                             0x4
#define _CLC1SEL1_D2S4_SIZE                                 0x1
#define _CLC1SEL1_D2S4_LENGTH                               0x1
#define _CLC1SEL1_D2S4_MASK                                 0x10

// Register: CLC1SEL2
#define CLC1SEL2 CLC1SEL2
extern volatile unsigned char           CLC1SEL2            __at(0xF14);
#ifndef _LIB_BUILD
asm("CLC1SEL2 equ 0F14h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC1D3S0                :1;
        unsigned LC1D3S1                :1;
        unsigned LC1D3S2                :1;
        unsigned LC1D3S3                :1;
        unsigned LC1D3S4                :1;
    };
    struct {
        unsigned LC1D3S                 :8;
    };
    struct {
        unsigned D3S                    :8;
    };
    struct {
        unsigned D3S0                   :1;
        unsigned D3S1                   :1;
        unsigned D3S2                   :1;
        unsigned D3S3                   :1;
        unsigned D3S4                   :1;
    };
} CLC1SEL2bits_t;
extern volatile CLC1SEL2bits_t CLC1SEL2bits __at(0xF14);
// bitfield macros
#define _CLC1SEL2_LC1D3S0_POSN                              0x0
#define _CLC1SEL2_LC1D3S0_POSITION                          0x0
#define _CLC1SEL2_LC1D3S0_SIZE                              0x1
#define _CLC1SEL2_LC1D3S0_LENGTH                            0x1
#define _CLC1SEL2_LC1D3S0_MASK                              0x1
#define _CLC1SEL2_LC1D3S1_POSN                              0x1
#define _CLC1SEL2_LC1D3S1_POSITION                          0x1
#define _CLC1SEL2_LC1D3S1_SIZE                              0x1
#define _CLC1SEL2_LC1D3S1_LENGTH                            0x1
#define _CLC1SEL2_LC1D3S1_MASK                              0x2
#define _CLC1SEL2_LC1D3S2_POSN                              0x2
#define _CLC1SEL2_LC1D3S2_POSITION                          0x2
#define _CLC1SEL2_LC1D3S2_SIZE                              0x1
#define _CLC1SEL2_LC1D3S2_LENGTH                            0x1
#define _CLC1SEL2_LC1D3S2_MASK                              0x4
#define _CLC1SEL2_LC1D3S3_POSN                              0x3
#define _CLC1SEL2_LC1D3S3_POSITION                          0x3
#define _CLC1SEL2_LC1D3S3_SIZE                              0x1
#define _CLC1SEL2_LC1D3S3_LENGTH                            0x1
#define _CLC1SEL2_LC1D3S3_MASK                              0x8
#define _CLC1SEL2_LC1D3S4_POSN                              0x4
#define _CLC1SEL2_LC1D3S4_POSITION                          0x4
#define _CLC1SEL2_LC1D3S4_SIZE                              0x1
#define _CLC1SEL2_LC1D3S4_LENGTH                            0x1
#define _CLC1SEL2_LC1D3S4_MASK                              0x10
#define _CLC1SEL2_LC1D3S_POSN                               0x0
#define _CLC1SEL2_LC1D3S_POSITION                           0x0
#define _CLC1SEL2_LC1D3S_SIZE                               0x8
#define _CLC1SEL2_LC1D3S_LENGTH                             0x8
#define _CLC1SEL2_LC1D3S_MASK                               0xFF
#define _CLC1SEL2_D3S_POSN                                  0x0
#define _CLC1SEL2_D3S_POSITION                              0x0
#define _CLC1SEL2_D3S_SIZE                                  0x8
#define _CLC1SEL2_D3S_LENGTH                                0x8
#define _CLC1SEL2_D3S_MASK                                  0xFF
#define _CLC1SEL2_D3S0_POSN                                 0x0
#define _CLC1SEL2_D3S0_POSITION                             0x0
#define _CLC1SEL2_D3S0_SIZE                                 0x1
#define _CLC1SEL2_D3S0_LENGTH                               0x1
#define _CLC1SEL2_D3S0_MASK                                 0x1
#define _CLC1SEL2_D3S1_POSN                                 0x1
#define _CLC1SEL2_D3S1_POSITION                             0x1
#define _CLC1SEL2_D3S1_SIZE                                 0x1
#define _CLC1SEL2_D3S1_LENGTH                               0x1
#define _CLC1SEL2_D3S1_MASK                                 0x2
#define _CLC1SEL2_D3S2_POSN                                 0x2
#define _CLC1SEL2_D3S2_POSITION                             0x2
#define _CLC1SEL2_D3S2_SIZE                                 0x1
#define _CLC1SEL2_D3S2_LENGTH                               0x1
#define _CLC1SEL2_D3S2_MASK                                 0x4
#define _CLC1SEL2_D3S3_POSN                                 0x3
#define _CLC1SEL2_D3S3_POSITION                             0x3
#define _CLC1SEL2_D3S3_SIZE                                 0x1
#define _CLC1SEL2_D3S3_LENGTH                               0x1
#define _CLC1SEL2_D3S3_MASK                                 0x8
#define _CLC1SEL2_D3S4_POSN                                 0x4
#define _CLC1SEL2_D3S4_POSITION                             0x4
#define _CLC1SEL2_D3S4_SIZE                                 0x1
#define _CLC1SEL2_D3S4_LENGTH                               0x1
#define _CLC1SEL2_D3S4_MASK                                 0x10

// Register: CLC1SEL3
#define CLC1SEL3 CLC1SEL3
extern volatile unsigned char           CLC1SEL3            __at(0xF15);
#ifndef _LIB_BUILD
asm("CLC1SEL3 equ 0F15h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC1D4S0                :1;
        unsigned LC1D4S1                :1;
        unsigned LC1D4S2                :1;
        unsigned LC1D4S3                :1;
        unsigned LC1D4S4                :1;
    };
    struct {
        unsigned LC1D4S                 :8;
    };
    struct {
        unsigned D4S                    :8;
    };
    struct {
        unsigned D4S0                   :1;
        unsigned D4S1                   :1;
        unsigned D4S2                   :1;
        unsigned D4S3                   :1;
        unsigned D4S4                   :1;
    };
} CLC1SEL3bits_t;
extern volatile CLC1SEL3bits_t CLC1SEL3bits __at(0xF15);
// bitfield macros
#define _CLC1SEL3_LC1D4S0_POSN                              0x0
#define _CLC1SEL3_LC1D4S0_POSITION                          0x0
#define _CLC1SEL3_LC1D4S0_SIZE                              0x1
#define _CLC1SEL3_LC1D4S0_LENGTH                            0x1
#define _CLC1SEL3_LC1D4S0_MASK                              0x1
#define _CLC1SEL3_LC1D4S1_POSN                              0x1
#define _CLC1SEL3_LC1D4S1_POSITION                          0x1
#define _CLC1SEL3_LC1D4S1_SIZE                              0x1
#define _CLC1SEL3_LC1D4S1_LENGTH                            0x1
#define _CLC1SEL3_LC1D4S1_MASK                              0x2
#define _CLC1SEL3_LC1D4S2_POSN                              0x2
#define _CLC1SEL3_LC1D4S2_POSITION                          0x2
#define _CLC1SEL3_LC1D4S2_SIZE                              0x1
#define _CLC1SEL3_LC1D4S2_LENGTH                            0x1
#define _CLC1SEL3_LC1D4S2_MASK                              0x4
#define _CLC1SEL3_LC1D4S3_POSN                              0x3
#define _CLC1SEL3_LC1D4S3_POSITION                          0x3
#define _CLC1SEL3_LC1D4S3_SIZE                              0x1
#define _CLC1SEL3_LC1D4S3_LENGTH                            0x1
#define _CLC1SEL3_LC1D4S3_MASK                              0x8
#define _CLC1SEL3_LC1D4S4_POSN                              0x4
#define _CLC1SEL3_LC1D4S4_POSITION                          0x4
#define _CLC1SEL3_LC1D4S4_SIZE                              0x1
#define _CLC1SEL3_LC1D4S4_LENGTH                            0x1
#define _CLC1SEL3_LC1D4S4_MASK                              0x10
#define _CLC1SEL3_LC1D4S_POSN                               0x0
#define _CLC1SEL3_LC1D4S_POSITION                           0x0
#define _CLC1SEL3_LC1D4S_SIZE                               0x8
#define _CLC1SEL3_LC1D4S_LENGTH                             0x8
#define _CLC1SEL3_LC1D4S_MASK                               0xFF
#define _CLC1SEL3_D4S_POSN                                  0x0
#define _CLC1SEL3_D4S_POSITION                              0x0
#define _CLC1SEL3_D4S_SIZE                                  0x8
#define _CLC1SEL3_D4S_LENGTH                                0x8
#define _CLC1SEL3_D4S_MASK                                  0xFF
#define _CLC1SEL3_D4S0_POSN                                 0x0
#define _CLC1SEL3_D4S0_POSITION                             0x0
#define _CLC1SEL3_D4S0_SIZE                                 0x1
#define _CLC1SEL3_D4S0_LENGTH                               0x1
#define _CLC1SEL3_D4S0_MASK                                 0x1
#define _CLC1SEL3_D4S1_POSN                                 0x1
#define _CLC1SEL3_D4S1_POSITION                             0x1
#define _CLC1SEL3_D4S1_SIZE                                 0x1
#define _CLC1SEL3_D4S1_LENGTH                               0x1
#define _CLC1SEL3_D4S1_MASK                                 0x2
#define _CLC1SEL3_D4S2_POSN                                 0x2
#define _CLC1SEL3_D4S2_POSITION                             0x2
#define _CLC1SEL3_D4S2_SIZE                                 0x1
#define _CLC1SEL3_D4S2_LENGTH                               0x1
#define _CLC1SEL3_D4S2_MASK                                 0x4
#define _CLC1SEL3_D4S3_POSN                                 0x3
#define _CLC1SEL3_D4S3_POSITION                             0x3
#define _CLC1SEL3_D4S3_SIZE                                 0x1
#define _CLC1SEL3_D4S3_LENGTH                               0x1
#define _CLC1SEL3_D4S3_MASK                                 0x8
#define _CLC1SEL3_D4S4_POSN                                 0x4
#define _CLC1SEL3_D4S4_POSITION                             0x4
#define _CLC1SEL3_D4S4_SIZE                                 0x1
#define _CLC1SEL3_D4S4_LENGTH                               0x1
#define _CLC1SEL3_D4S4_MASK                                 0x10

// Register: CLC1GLS0
#define CLC1GLS0 CLC1GLS0
extern volatile unsigned char           CLC1GLS0            __at(0xF16);
#ifndef _LIB_BUILD
asm("CLC1GLS0 equ 0F16h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC1G1D1N               :1;
        unsigned LC1G1D1T               :1;
        unsigned LC1G1D2N               :1;
        unsigned LC1G1D2T               :1;
        unsigned LC1G1D3N               :1;
        unsigned LC1G1D3T               :1;
        unsigned LC1G1D4N               :1;
        unsigned LC1G1D4T               :1;
    };
    struct {
        unsigned D1N                    :1;
        unsigned D1T                    :1;
        unsigned D2N                    :1;
        unsigned D2T                    :1;
        unsigned D3N                    :1;
        unsigned D3T                    :1;
        unsigned D4N                    :1;
        unsigned D4T                    :1;
    };
} CLC1GLS0bits_t;
extern volatile CLC1GLS0bits_t CLC1GLS0bits __at(0xF16);
// bitfield macros
#define _CLC1GLS0_LC1G1D1N_POSN                             0x0
#define _CLC1GLS0_LC1G1D1N_POSITION                         0x0
#define _CLC1GLS0_LC1G1D1N_SIZE                             0x1
#define _CLC1GLS0_LC1G1D1N_LENGTH                           0x1
#define _CLC1GLS0_LC1G1D1N_MASK                             0x1
#define _CLC1GLS0_LC1G1D1T_POSN                             0x1
#define _CLC1GLS0_LC1G1D1T_POSITION                         0x1
#define _CLC1GLS0_LC1G1D1T_SIZE                             0x1
#define _CLC1GLS0_LC1G1D1T_LENGTH                           0x1
#define _CLC1GLS0_LC1G1D1T_MASK                             0x2
#define _CLC1GLS0_LC1G1D2N_POSN                             0x2
#define _CLC1GLS0_LC1G1D2N_POSITION                         0x2
#define _CLC1GLS0_LC1G1D2N_SIZE                             0x1
#define _CLC1GLS0_LC1G1D2N_LENGTH                           0x1
#define _CLC1GLS0_LC1G1D2N_MASK                             0x4
#define _CLC1GLS0_LC1G1D2T_POSN                             0x3
#define _CLC1GLS0_LC1G1D2T_POSITION                         0x3
#define _CLC1GLS0_LC1G1D2T_SIZE                             0x1
#define _CLC1GLS0_LC1G1D2T_LENGTH                           0x1
#define _CLC1GLS0_LC1G1D2T_MASK                             0x8
#define _CLC1GLS0_LC1G1D3N_POSN                             0x4
#define _CLC1GLS0_LC1G1D3N_POSITION                         0x4
#define _CLC1GLS0_LC1G1D3N_SIZE                             0x1
#define _CLC1GLS0_LC1G1D3N_LENGTH                           0x1
#define _CLC1GLS0_LC1G1D3N_MASK                             0x10
#define _CLC1GLS0_LC1G1D3T_POSN                             0x5
#define _CLC1GLS0_LC1G1D3T_POSITION                         0x5
#define _CLC1GLS0_LC1G1D3T_SIZE                             0x1
#define _CLC1GLS0_LC1G1D3T_LENGTH                           0x1
#define _CLC1GLS0_LC1G1D3T_MASK                             0x20
#define _CLC1GLS0_LC1G1D4N_POSN                             0x6
#define _CLC1GLS0_LC1G1D4N_POSITION                         0x6
#define _CLC1GLS0_LC1G1D4N_SIZE                             0x1
#define _CLC1GLS0_LC1G1D4N_LENGTH                           0x1
#define _CLC1GLS0_LC1G1D4N_MASK                             0x40
#define _CLC1GLS0_LC1G1D4T_POSN                             0x7
#define _CLC1GLS0_LC1G1D4T_POSITION                         0x7
#define _CLC1GLS0_LC1G1D4T_SIZE                             0x1
#define _CLC1GLS0_LC1G1D4T_LENGTH                           0x1
#define _CLC1GLS0_LC1G1D4T_MASK                             0x80
#define _CLC1GLS0_D1N_POSN                                  0x0
#define _CLC1GLS0_D1N_POSITION                              0x0
#define _CLC1GLS0_D1N_SIZE                                  0x1
#define _CLC1GLS0_D1N_LENGTH                                0x1
#define _CLC1GLS0_D1N_MASK                                  0x1
#define _CLC1GLS0_D1T_POSN                                  0x1
#define _CLC1GLS0_D1T_POSITION                              0x1
#define _CLC1GLS0_D1T_SIZE                                  0x1
#define _CLC1GLS0_D1T_LENGTH                                0x1
#define _CLC1GLS0_D1T_MASK                                  0x2
#define _CLC1GLS0_D2N_POSN                                  0x2
#define _CLC1GLS0_D2N_POSITION                              0x2
#define _CLC1GLS0_D2N_SIZE                                  0x1
#define _CLC1GLS0_D2N_LENGTH                                0x1
#define _CLC1GLS0_D2N_MASK                                  0x4
#define _CLC1GLS0_D2T_POSN                                  0x3
#define _CLC1GLS0_D2T_POSITION                              0x3
#define _CLC1GLS0_D2T_SIZE                                  0x1
#define _CLC1GLS0_D2T_LENGTH                                0x1
#define _CLC1GLS0_D2T_MASK                                  0x8
#define _CLC1GLS0_D3N_POSN                                  0x4
#define _CLC1GLS0_D3N_POSITION                              0x4
#define _CLC1GLS0_D3N_SIZE                                  0x1
#define _CLC1GLS0_D3N_LENGTH                                0x1
#define _CLC1GLS0_D3N_MASK                                  0x10
#define _CLC1GLS0_D3T_POSN                                  0x5
#define _CLC1GLS0_D3T_POSITION                              0x5
#define _CLC1GLS0_D3T_SIZE                                  0x1
#define _CLC1GLS0_D3T_LENGTH                                0x1
#define _CLC1GLS0_D3T_MASK                                  0x20
#define _CLC1GLS0_D4N_POSN                                  0x6
#define _CLC1GLS0_D4N_POSITION                              0x6
#define _CLC1GLS0_D4N_SIZE                                  0x1
#define _CLC1GLS0_D4N_LENGTH                                0x1
#define _CLC1GLS0_D4N_MASK                                  0x40
#define _CLC1GLS0_D4T_POSN                                  0x7
#define _CLC1GLS0_D4T_POSITION                              0x7
#define _CLC1GLS0_D4T_SIZE                                  0x1
#define _CLC1GLS0_D4T_LENGTH                                0x1
#define _CLC1GLS0_D4T_MASK                                  0x80

// Register: CLC1GLS1
#define CLC1GLS1 CLC1GLS1
extern volatile unsigned char           CLC1GLS1            __at(0xF17);
#ifndef _LIB_BUILD
asm("CLC1GLS1 equ 0F17h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC1G2D1N               :1;
        unsigned LC1G2D1T               :1;
        unsigned LC1G2D2N               :1;
        unsigned LC1G2D2T               :1;
        unsigned LC1G2D3N               :1;
        unsigned LC1G2D3T               :1;
        unsigned LC1G2D4N               :1;
        unsigned LC1G2D4T               :1;
    };
    struct {
        unsigned D1N                    :1;
        unsigned D1T                    :1;
        unsigned D2N                    :1;
        unsigned D2T                    :1;
        unsigned D3N                    :1;
        unsigned D3T                    :1;
        unsigned D4N                    :1;
        unsigned D4T                    :1;
    };
} CLC1GLS1bits_t;
extern volatile CLC1GLS1bits_t CLC1GLS1bits __at(0xF17);
// bitfield macros
#define _CLC1GLS1_LC1G2D1N_POSN                             0x0
#define _CLC1GLS1_LC1G2D1N_POSITION                         0x0
#define _CLC1GLS1_LC1G2D1N_SIZE                             0x1
#define _CLC1GLS1_LC1G2D1N_LENGTH                           0x1
#define _CLC1GLS1_LC1G2D1N_MASK                             0x1
#define _CLC1GLS1_LC1G2D1T_POSN                             0x1
#define _CLC1GLS1_LC1G2D1T_POSITION                         0x1
#define _CLC1GLS1_LC1G2D1T_SIZE                             0x1
#define _CLC1GLS1_LC1G2D1T_LENGTH                           0x1
#define _CLC1GLS1_LC1G2D1T_MASK                             0x2
#define _CLC1GLS1_LC1G2D2N_POSN                             0x2
#define _CLC1GLS1_LC1G2D2N_POSITION                         0x2
#define _CLC1GLS1_LC1G2D2N_SIZE                             0x1
#define _CLC1GLS1_LC1G2D2N_LENGTH                           0x1
#define _CLC1GLS1_LC1G2D2N_MASK                             0x4
#define _CLC1GLS1_LC1G2D2T_POSN                             0x3
#define _CLC1GLS1_LC1G2D2T_POSITION                         0x3
#define _CLC1GLS1_LC1G2D2T_SIZE                             0x1
#define _CLC1GLS1_LC1G2D2T_LENGTH                           0x1
#define _CLC1GLS1_LC1G2D2T_MASK                             0x8
#define _CLC1GLS1_LC1G2D3N_POSN                             0x4
#define _CLC1GLS1_LC1G2D3N_POSITION                         0x4
#define _CLC1GLS1_LC1G2D3N_SIZE                             0x1
#define _CLC1GLS1_LC1G2D3N_LENGTH                           0x1
#define _CLC1GLS1_LC1G2D3N_MASK                             0x10
#define _CLC1GLS1_LC1G2D3T_POSN                             0x5
#define _CLC1GLS1_LC1G2D3T_POSITION                         0x5
#define _CLC1GLS1_LC1G2D3T_SIZE                             0x1
#define _CLC1GLS1_LC1G2D3T_LENGTH                           0x1
#define _CLC1GLS1_LC1G2D3T_MASK                             0x20
#define _CLC1GLS1_LC1G2D4N_POSN                             0x6
#define _CLC1GLS1_LC1G2D4N_POSITION                         0x6
#define _CLC1GLS1_LC1G2D4N_SIZE                             0x1
#define _CLC1GLS1_LC1G2D4N_LENGTH                           0x1
#define _CLC1GLS1_LC1G2D4N_MASK                             0x40
#define _CLC1GLS1_LC1G2D4T_POSN                             0x7
#define _CLC1GLS1_LC1G2D4T_POSITION                         0x7
#define _CLC1GLS1_LC1G2D4T_SIZE                             0x1
#define _CLC1GLS1_LC1G2D4T_LENGTH                           0x1
#define _CLC1GLS1_LC1G2D4T_MASK                             0x80
#define _CLC1GLS1_D1N_POSN                                  0x0
#define _CLC1GLS1_D1N_POSITION                              0x0
#define _CLC1GLS1_D1N_SIZE                                  0x1
#define _CLC1GLS1_D1N_LENGTH                                0x1
#define _CLC1GLS1_D1N_MASK                                  0x1
#define _CLC1GLS1_D1T_POSN                                  0x1
#define _CLC1GLS1_D1T_POSITION                              0x1
#define _CLC1GLS1_D1T_SIZE                                  0x1
#define _CLC1GLS1_D1T_LENGTH                                0x1
#define _CLC1GLS1_D1T_MASK                                  0x2
#define _CLC1GLS1_D2N_POSN                                  0x2
#define _CLC1GLS1_D2N_POSITION                              0x2
#define _CLC1GLS1_D2N_SIZE                                  0x1
#define _CLC1GLS1_D2N_LENGTH                                0x1
#define _CLC1GLS1_D2N_MASK                                  0x4
#define _CLC1GLS1_D2T_POSN                                  0x3
#define _CLC1GLS1_D2T_POSITION                              0x3
#define _CLC1GLS1_D2T_SIZE                                  0x1
#define _CLC1GLS1_D2T_LENGTH                                0x1
#define _CLC1GLS1_D2T_MASK                                  0x8
#define _CLC1GLS1_D3N_POSN                                  0x4
#define _CLC1GLS1_D3N_POSITION                              0x4
#define _CLC1GLS1_D3N_SIZE                                  0x1
#define _CLC1GLS1_D3N_LENGTH                                0x1
#define _CLC1GLS1_D3N_MASK                                  0x10
#define _CLC1GLS1_D3T_POSN                                  0x5
#define _CLC1GLS1_D3T_POSITION                              0x5
#define _CLC1GLS1_D3T_SIZE                                  0x1
#define _CLC1GLS1_D3T_LENGTH                                0x1
#define _CLC1GLS1_D3T_MASK                                  0x20
#define _CLC1GLS1_D4N_POSN                                  0x6
#define _CLC1GLS1_D4N_POSITION                              0x6
#define _CLC1GLS1_D4N_SIZE                                  0x1
#define _CLC1GLS1_D4N_LENGTH                                0x1
#define _CLC1GLS1_D4N_MASK                                  0x40
#define _CLC1GLS1_D4T_POSN                                  0x7
#define _CLC1GLS1_D4T_POSITION                              0x7
#define _CLC1GLS1_D4T_SIZE                                  0x1
#define _CLC1GLS1_D4T_LENGTH                                0x1
#define _CLC1GLS1_D4T_MASK                                  0x80

// Register: CLC1GLS2
#define CLC1GLS2 CLC1GLS2
extern volatile unsigned char           CLC1GLS2            __at(0xF18);
#ifndef _LIB_BUILD
asm("CLC1GLS2 equ 0F18h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC1G3D1N               :1;
        unsigned LC1G3D1T               :1;
        unsigned LC1G3D2N               :1;
        unsigned LC1G3D2T               :1;
        unsigned LC1G3D3N               :1;
        unsigned LC1G3D3T               :1;
        unsigned LC1G3D4N               :1;
        unsigned LC1G3D4T               :1;
    };
    struct {
        unsigned D1N                    :1;
        unsigned D1T                    :1;
        unsigned D2N                    :1;
        unsigned D2T                    :1;
        unsigned D3N                    :1;
        unsigned D3T                    :1;
        unsigned D4N                    :1;
        unsigned D4T                    :1;
    };
} CLC1GLS2bits_t;
extern volatile CLC1GLS2bits_t CLC1GLS2bits __at(0xF18);
// bitfield macros
#define _CLC1GLS2_LC1G3D1N_POSN                             0x0
#define _CLC1GLS2_LC1G3D1N_POSITION                         0x0
#define _CLC1GLS2_LC1G3D1N_SIZE                             0x1
#define _CLC1GLS2_LC1G3D1N_LENGTH                           0x1
#define _CLC1GLS2_LC1G3D1N_MASK                             0x1
#define _CLC1GLS2_LC1G3D1T_POSN                             0x1
#define _CLC1GLS2_LC1G3D1T_POSITION                         0x1
#define _CLC1GLS2_LC1G3D1T_SIZE                             0x1
#define _CLC1GLS2_LC1G3D1T_LENGTH                           0x1
#define _CLC1GLS2_LC1G3D1T_MASK                             0x2
#define _CLC1GLS2_LC1G3D2N_POSN                             0x2
#define _CLC1GLS2_LC1G3D2N_POSITION                         0x2
#define _CLC1GLS2_LC1G3D2N_SIZE                             0x1
#define _CLC1GLS2_LC1G3D2N_LENGTH                           0x1
#define _CLC1GLS2_LC1G3D2N_MASK                             0x4
#define _CLC1GLS2_LC1G3D2T_POSN                             0x3
#define _CLC1GLS2_LC1G3D2T_POSITION                         0x3
#define _CLC1GLS2_LC1G3D2T_SIZE                             0x1
#define _CLC1GLS2_LC1G3D2T_LENGTH                           0x1
#define _CLC1GLS2_LC1G3D2T_MASK                             0x8
#define _CLC1GLS2_LC1G3D3N_POSN                             0x4
#define _CLC1GLS2_LC1G3D3N_POSITION                         0x4
#define _CLC1GLS2_LC1G3D3N_SIZE                             0x1
#define _CLC1GLS2_LC1G3D3N_LENGTH                           0x1
#define _CLC1GLS2_LC1G3D3N_MASK                             0x10
#define _CLC1GLS2_LC1G3D3T_POSN                             0x5
#define _CLC1GLS2_LC1G3D3T_POSITION                         0x5
#define _CLC1GLS2_LC1G3D3T_SIZE                             0x1
#define _CLC1GLS2_LC1G3D3T_LENGTH                           0x1
#define _CLC1GLS2_LC1G3D3T_MASK                             0x20
#define _CLC1GLS2_LC1G3D4N_POSN                             0x6
#define _CLC1GLS2_LC1G3D4N_POSITION                         0x6
#define _CLC1GLS2_LC1G3D4N_SIZE                             0x1
#define _CLC1GLS2_LC1G3D4N_LENGTH                           0x1
#define _CLC1GLS2_LC1G3D4N_MASK                             0x40
#define _CLC1GLS2_LC1G3D4T_POSN                             0x7
#define _CLC1GLS2_LC1G3D4T_POSITION                         0x7
#define _CLC1GLS2_LC1G3D4T_SIZE                             0x1
#define _CLC1GLS2_LC1G3D4T_LENGTH                           0x1
#define _CLC1GLS2_LC1G3D4T_MASK                             0x80
#define _CLC1GLS2_D1N_POSN                                  0x0
#define _CLC1GLS2_D1N_POSITION                              0x0
#define _CLC1GLS2_D1N_SIZE                                  0x1
#define _CLC1GLS2_D1N_LENGTH                                0x1
#define _CLC1GLS2_D1N_MASK                                  0x1
#define _CLC1GLS2_D1T_POSN                                  0x1
#define _CLC1GLS2_D1T_POSITION                              0x1
#define _CLC1GLS2_D1T_SIZE                                  0x1
#define _CLC1GLS2_D1T_LENGTH                                0x1
#define _CLC1GLS2_D1T_MASK                                  0x2
#define _CLC1GLS2_D2N_POSN                                  0x2
#define _CLC1GLS2_D2N_POSITION                              0x2
#define _CLC1GLS2_D2N_SIZE                                  0x1
#define _CLC1GLS2_D2N_LENGTH                                0x1
#define _CLC1GLS2_D2N_MASK                                  0x4
#define _CLC1GLS2_D2T_POSN                                  0x3
#define _CLC1GLS2_D2T_POSITION                              0x3
#define _CLC1GLS2_D2T_SIZE                                  0x1
#define _CLC1GLS2_D2T_LENGTH                                0x1
#define _CLC1GLS2_D2T_MASK                                  0x8
#define _CLC1GLS2_D3N_POSN                                  0x4
#define _CLC1GLS2_D3N_POSITION                              0x4
#define _CLC1GLS2_D3N_SIZE                                  0x1
#define _CLC1GLS2_D3N_LENGTH                                0x1
#define _CLC1GLS2_D3N_MASK                                  0x10
#define _CLC1GLS2_D3T_POSN                                  0x5
#define _CLC1GLS2_D3T_POSITION                              0x5
#define _CLC1GLS2_D3T_SIZE                                  0x1
#define _CLC1GLS2_D3T_LENGTH                                0x1
#define _CLC1GLS2_D3T_MASK                                  0x20
#define _CLC1GLS2_D4N_POSN                                  0x6
#define _CLC1GLS2_D4N_POSITION                              0x6
#define _CLC1GLS2_D4N_SIZE                                  0x1
#define _CLC1GLS2_D4N_LENGTH                                0x1
#define _CLC1GLS2_D4N_MASK                                  0x40
#define _CLC1GLS2_D4T_POSN                                  0x7
#define _CLC1GLS2_D4T_POSITION                              0x7
#define _CLC1GLS2_D4T_SIZE                                  0x1
#define _CLC1GLS2_D4T_LENGTH                                0x1
#define _CLC1GLS2_D4T_MASK                                  0x80

// Register: CLC1GLS3
#define CLC1GLS3 CLC1GLS3
extern volatile unsigned char           CLC1GLS3            __at(0xF19);
#ifndef _LIB_BUILD
asm("CLC1GLS3 equ 0F19h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC1G4D1N               :1;
        unsigned LC1G4D1T               :1;
        unsigned LC1G4D2N               :1;
        unsigned LC1G4D2T               :1;
        unsigned LC1G4D3N               :1;
        unsigned LC1G4D3T               :1;
        unsigned LC1G4D4N               :1;
        unsigned LC1G4D4T               :1;
    };
    struct {
        unsigned G4D1N                  :1;
        unsigned G4D1T                  :1;
        unsigned G4D2N                  :1;
        unsigned G4D2T                  :1;
        unsigned G4D3N                  :1;
        unsigned G4D3T                  :1;
        unsigned G4D4N                  :1;
        unsigned G4D4T                  :1;
    };
} CLC1GLS3bits_t;
extern volatile CLC1GLS3bits_t CLC1GLS3bits __at(0xF19);
// bitfield macros
#define _CLC1GLS3_LC1G4D1N_POSN                             0x0
#define _CLC1GLS3_LC1G4D1N_POSITION                         0x0
#define _CLC1GLS3_LC1G4D1N_SIZE                             0x1
#define _CLC1GLS3_LC1G4D1N_LENGTH                           0x1
#define _CLC1GLS3_LC1G4D1N_MASK                             0x1
#define _CLC1GLS3_LC1G4D1T_POSN                             0x1
#define _CLC1GLS3_LC1G4D1T_POSITION                         0x1
#define _CLC1GLS3_LC1G4D1T_SIZE                             0x1
#define _CLC1GLS3_LC1G4D1T_LENGTH                           0x1
#define _CLC1GLS3_LC1G4D1T_MASK                             0x2
#define _CLC1GLS3_LC1G4D2N_POSN                             0x2
#define _CLC1GLS3_LC1G4D2N_POSITION                         0x2
#define _CLC1GLS3_LC1G4D2N_SIZE                             0x1
#define _CLC1GLS3_LC1G4D2N_LENGTH                           0x1
#define _CLC1GLS3_LC1G4D2N_MASK                             0x4
#define _CLC1GLS3_LC1G4D2T_POSN                             0x3
#define _CLC1GLS3_LC1G4D2T_POSITION                         0x3
#define _CLC1GLS3_LC1G4D2T_SIZE                             0x1
#define _CLC1GLS3_LC1G4D2T_LENGTH                           0x1
#define _CLC1GLS3_LC1G4D2T_MASK                             0x8
#define _CLC1GLS3_LC1G4D3N_POSN                             0x4
#define _CLC1GLS3_LC1G4D3N_POSITION                         0x4
#define _CLC1GLS3_LC1G4D3N_SIZE                             0x1
#define _CLC1GLS3_LC1G4D3N_LENGTH                           0x1
#define _CLC1GLS3_LC1G4D3N_MASK                             0x10
#define _CLC1GLS3_LC1G4D3T_POSN                             0x5
#define _CLC1GLS3_LC1G4D3T_POSITION                         0x5
#define _CLC1GLS3_LC1G4D3T_SIZE                             0x1
#define _CLC1GLS3_LC1G4D3T_LENGTH                           0x1
#define _CLC1GLS3_LC1G4D3T_MASK                             0x20
#define _CLC1GLS3_LC1G4D4N_POSN                             0x6
#define _CLC1GLS3_LC1G4D4N_POSITION                         0x6
#define _CLC1GLS3_LC1G4D4N_SIZE                             0x1
#define _CLC1GLS3_LC1G4D4N_LENGTH                           0x1
#define _CLC1GLS3_LC1G4D4N_MASK                             0x40
#define _CLC1GLS3_LC1G4D4T_POSN                             0x7
#define _CLC1GLS3_LC1G4D4T_POSITION                         0x7
#define _CLC1GLS3_LC1G4D4T_SIZE                             0x1
#define _CLC1GLS3_LC1G4D4T_LENGTH                           0x1
#define _CLC1GLS3_LC1G4D4T_MASK                             0x80
#define _CLC1GLS3_G4D1N_POSN                                0x0
#define _CLC1GLS3_G4D1N_POSITION                            0x0
#define _CLC1GLS3_G4D1N_SIZE                                0x1
#define _CLC1GLS3_G4D1N_LENGTH                              0x1
#define _CLC1GLS3_G4D1N_MASK                                0x1
#define _CLC1GLS3_G4D1T_POSN                                0x1
#define _CLC1GLS3_G4D1T_POSITION                            0x1
#define _CLC1GLS3_G4D1T_SIZE                                0x1
#define _CLC1GLS3_G4D1T_LENGTH                              0x1
#define _CLC1GLS3_G4D1T_MASK                                0x2
#define _CLC1GLS3_G4D2N_POSN                                0x2
#define _CLC1GLS3_G4D2N_POSITION                            0x2
#define _CLC1GLS3_G4D2N_SIZE                                0x1
#define _CLC1GLS3_G4D2N_LENGTH                              0x1
#define _CLC1GLS3_G4D2N_MASK                                0x4
#define _CLC1GLS3_G4D2T_POSN                                0x3
#define _CLC1GLS3_G4D2T_POSITION                            0x3
#define _CLC1GLS3_G4D2T_SIZE                                0x1
#define _CLC1GLS3_G4D2T_LENGTH                              0x1
#define _CLC1GLS3_G4D2T_MASK                                0x8
#define _CLC1GLS3_G4D3N_POSN                                0x4
#define _CLC1GLS3_G4D3N_POSITION                            0x4
#define _CLC1GLS3_G4D3N_SIZE                                0x1
#define _CLC1GLS3_G4D3N_LENGTH                              0x1
#define _CLC1GLS3_G4D3N_MASK                                0x10
#define _CLC1GLS3_G4D3T_POSN                                0x5
#define _CLC1GLS3_G4D3T_POSITION                            0x5
#define _CLC1GLS3_G4D3T_SIZE                                0x1
#define _CLC1GLS3_G4D3T_LENGTH                              0x1
#define _CLC1GLS3_G4D3T_MASK                                0x20
#define _CLC1GLS3_G4D4N_POSN                                0x6
#define _CLC1GLS3_G4D4N_POSITION                            0x6
#define _CLC1GLS3_G4D4N_SIZE                                0x1
#define _CLC1GLS3_G4D4N_LENGTH                              0x1
#define _CLC1GLS3_G4D4N_MASK                                0x40
#define _CLC1GLS3_G4D4T_POSN                                0x7
#define _CLC1GLS3_G4D4T_POSITION                            0x7
#define _CLC1GLS3_G4D4T_SIZE                                0x1
#define _CLC1GLS3_G4D4T_LENGTH                              0x1
#define _CLC1GLS3_G4D4T_MASK                                0x80

// Register: CLC2CON
#define CLC2CON CLC2CON
extern volatile unsigned char           CLC2CON             __at(0xF1A);
#ifndef _LIB_BUILD
asm("CLC2CON equ 0F1Ah");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC2MODE                :3;
        unsigned LC2INTN                :1;
        unsigned LC2INTP                :1;
        unsigned LC2OUT                 :1;
        unsigned                        :1;
        unsigned LC2EN                  :1;
    };
    struct {
        unsigned LC2MODE0               :1;
        unsigned LC2MODE1               :1;
        unsigned LC2MODE2               :1;
    };
    struct {
        unsigned MODE                   :3;
        unsigned INTN                   :1;
        unsigned INTP                   :1;
        unsigned OUT                    :1;
        unsigned                        :1;
        unsigned EN                     :1;
    };
    struct {
        unsigned MODE0                  :1;
        unsigned MODE1                  :1;
        unsigned MODE2                  :1;
    };
} CLC2CONbits_t;
extern volatile CLC2CONbits_t CLC2CONbits __at(0xF1A);
// bitfield macros
#define _CLC2CON_LC2MODE_POSN                               0x0
#define _CLC2CON_LC2MODE_POSITION                           0x0
#define _CLC2CON_LC2MODE_SIZE                               0x3
#define _CLC2CON_LC2MODE_LENGTH                             0x3
#define _CLC2CON_LC2MODE_MASK                               0x7
#define _CLC2CON_LC2INTN_POSN                               0x3
#define _CLC2CON_LC2INTN_POSITION                           0x3
#define _CLC2CON_LC2INTN_SIZE                               0x1
#define _CLC2CON_LC2INTN_LENGTH                             0x1
#define _CLC2CON_LC2INTN_MASK                               0x8
#define _CLC2CON_LC2INTP_POSN                               0x4
#define _CLC2CON_LC2INTP_POSITION                           0x4
#define _CLC2CON_LC2INTP_SIZE                               0x1
#define _CLC2CON_LC2INTP_LENGTH                             0x1
#define _CLC2CON_LC2INTP_MASK                               0x10
#define _CLC2CON_LC2OUT_POSN                                0x5
#define _CLC2CON_LC2OUT_POSITION                            0x5
#define _CLC2CON_LC2OUT_SIZE                                0x1
#define _CLC2CON_LC2OUT_LENGTH                              0x1
#define _CLC2CON_LC2OUT_MASK                                0x20
#define _CLC2CON_LC2EN_POSN                                 0x7
#define _CLC2CON_LC2EN_POSITION                             0x7
#define _CLC2CON_LC2EN_SIZE                                 0x1
#define _CLC2CON_LC2EN_LENGTH                               0x1
#define _CLC2CON_LC2EN_MASK                                 0x80
#define _CLC2CON_LC2MODE0_POSN                              0x0
#define _CLC2CON_LC2MODE0_POSITION                          0x0
#define _CLC2CON_LC2MODE0_SIZE                              0x1
#define _CLC2CON_LC2MODE0_LENGTH                            0x1
#define _CLC2CON_LC2MODE0_MASK                              0x1
#define _CLC2CON_LC2MODE1_POSN                              0x1
#define _CLC2CON_LC2MODE1_POSITION                          0x1
#define _CLC2CON_LC2MODE1_SIZE                              0x1
#define _CLC2CON_LC2MODE1_LENGTH                            0x1
#define _CLC2CON_LC2MODE1_MASK                              0x2
#define _CLC2CON_LC2MODE2_POSN                              0x2
#define _CLC2CON_LC2MODE2_POSITION                          0x2
#define _CLC2CON_LC2MODE2_SIZE                              0x1
#define _CLC2CON_LC2MODE2_LENGTH                            0x1
#define _CLC2CON_LC2MODE2_MASK                              0x4
#define _CLC2CON_MODE_POSN                                  0x0
#define _CLC2CON_MODE_POSITION                              0x0
#define _CLC2CON_MODE_SIZE                                  0x3
#define _CLC2CON_MODE_LENGTH                                0x3
#define _CLC2CON_MODE_MASK                                  0x7
#define _CLC2CON_INTN_POSN                                  0x3
#define _CLC2CON_INTN_POSITION                              0x3
#define _CLC2CON_INTN_SIZE                                  0x1
#define _CLC2CON_INTN_LENGTH                                0x1
#define _CLC2CON_INTN_MASK                                  0x8
#define _CLC2CON_INTP_POSN                                  0x4
#define _CLC2CON_INTP_POSITION                              0x4
#define _CLC2CON_INTP_SIZE                                  0x1
#define _CLC2CON_INTP_LENGTH                                0x1
#define _CLC2CON_INTP_MASK                                  0x10
#define _CLC2CON_OUT_POSN                                   0x5
#define _CLC2CON_OUT_POSITION                               0x5
#define _CLC2CON_OUT_SIZE                                   0x1
#define _CLC2CON_OUT_LENGTH                                 0x1
#define _CLC2CON_OUT_MASK                                   0x20
#define _CLC2CON_EN_POSN                                    0x7
#define _CLC2CON_EN_POSITION                                0x7
#define _CLC2CON_EN_SIZE                                    0x1
#define _CLC2CON_EN_LENGTH                                  0x1
#define _CLC2CON_EN_MASK                                    0x80
#define _CLC2CON_MODE0_POSN                                 0x0
#define _CLC2CON_MODE0_POSITION                             0x0
#define _CLC2CON_MODE0_SIZE                                 0x1
#define _CLC2CON_MODE0_LENGTH                               0x1
#define _CLC2CON_MODE0_MASK                                 0x1
#define _CLC2CON_MODE1_POSN                                 0x1
#define _CLC2CON_MODE1_POSITION                             0x1
#define _CLC2CON_MODE1_SIZE                                 0x1
#define _CLC2CON_MODE1_LENGTH                               0x1
#define _CLC2CON_MODE1_MASK                                 0x2
#define _CLC2CON_MODE2_POSN                                 0x2
#define _CLC2CON_MODE2_POSITION                             0x2
#define _CLC2CON_MODE2_SIZE                                 0x1
#define _CLC2CON_MODE2_LENGTH                               0x1
#define _CLC2CON_MODE2_MASK                                 0x4

// Register: CLC2POL
#define CLC2POL CLC2POL
extern volatile unsigned char           CLC2POL             __at(0xF1B);
#ifndef _LIB_BUILD
asm("CLC2POL equ 0F1Bh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC2G1POL               :1;
        unsigned LC2G2POL               :1;
        unsigned LC2G3POL               :1;
        unsigned LC2G4POL               :1;
        unsigned                        :3;
        unsigned LC2POL                 :1;
    };
    struct {
        unsigned G1POL                  :1;
        unsigned G2POL                  :1;
        unsigned G3POL                  :1;
        unsigned G4POL                  :1;
        unsigned                        :3;
        unsigned POL                    :1;
    };
} CLC2POLbits_t;
extern volatile CLC2POLbits_t CLC2POLbits __at(0xF1B);
// bitfield macros
#define _CLC2POL_LC2G1POL_POSN                              0x0
#define _CLC2POL_LC2G1POL_POSITION                          0x0
#define _CLC2POL_LC2G1POL_SIZE                              0x1
#define _CLC2POL_LC2G1POL_LENGTH                            0x1
#define _CLC2POL_LC2G1POL_MASK                              0x1
#define _CLC2POL_LC2G2POL_POSN                              0x1
#define _CLC2POL_LC2G2POL_POSITION                          0x1
#define _CLC2POL_LC2G2POL_SIZE                              0x1
#define _CLC2POL_LC2G2POL_LENGTH                            0x1
#define _CLC2POL_LC2G2POL_MASK                              0x2
#define _CLC2POL_LC2G3POL_POSN                              0x2
#define _CLC2POL_LC2G3POL_POSITION                          0x2
#define _CLC2POL_LC2G3POL_SIZE                              0x1
#define _CLC2POL_LC2G3POL_LENGTH                            0x1
#define _CLC2POL_LC2G3POL_MASK                              0x4
#define _CLC2POL_LC2G4POL_POSN                              0x3
#define _CLC2POL_LC2G4POL_POSITION                          0x3
#define _CLC2POL_LC2G4POL_SIZE                              0x1
#define _CLC2POL_LC2G4POL_LENGTH                            0x1
#define _CLC2POL_LC2G4POL_MASK                              0x8
#define _CLC2POL_LC2POL_POSN                                0x7
#define _CLC2POL_LC2POL_POSITION                            0x7
#define _CLC2POL_LC2POL_SIZE                                0x1
#define _CLC2POL_LC2POL_LENGTH                              0x1
#define _CLC2POL_LC2POL_MASK                                0x80
#define _CLC2POL_G1POL_POSN                                 0x0
#define _CLC2POL_G1POL_POSITION                             0x0
#define _CLC2POL_G1POL_SIZE                                 0x1
#define _CLC2POL_G1POL_LENGTH                               0x1
#define _CLC2POL_G1POL_MASK                                 0x1
#define _CLC2POL_G2POL_POSN                                 0x1
#define _CLC2POL_G2POL_POSITION                             0x1
#define _CLC2POL_G2POL_SIZE                                 0x1
#define _CLC2POL_G2POL_LENGTH                               0x1
#define _CLC2POL_G2POL_MASK                                 0x2
#define _CLC2POL_G3POL_POSN                                 0x2
#define _CLC2POL_G3POL_POSITION                             0x2
#define _CLC2POL_G3POL_SIZE                                 0x1
#define _CLC2POL_G3POL_LENGTH                               0x1
#define _CLC2POL_G3POL_MASK                                 0x4
#define _CLC2POL_G4POL_POSN                                 0x3
#define _CLC2POL_G4POL_POSITION                             0x3
#define _CLC2POL_G4POL_SIZE                                 0x1
#define _CLC2POL_G4POL_LENGTH                               0x1
#define _CLC2POL_G4POL_MASK                                 0x8
#define _CLC2POL_POL_POSN                                   0x7
#define _CLC2POL_POL_POSITION                               0x7
#define _CLC2POL_POL_SIZE                                   0x1
#define _CLC2POL_POL_LENGTH                                 0x1
#define _CLC2POL_POL_MASK                                   0x80

// Register: CLC2SEL0
#define CLC2SEL0 CLC2SEL0
extern volatile unsigned char           CLC2SEL0            __at(0xF1C);
#ifndef _LIB_BUILD
asm("CLC2SEL0 equ 0F1Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC2D1S0                :1;
        unsigned LC2D1S1                :1;
        unsigned LC2D1S2                :1;
        unsigned LC2D1S3                :1;
        unsigned LC2D1S4                :1;
    };
    struct {
        unsigned LC2D1S                 :8;
    };
    struct {
        unsigned D1S                    :8;
    };
    struct {
        unsigned D1S0                   :1;
        unsigned D1S1                   :1;
        unsigned D1S2                   :1;
        unsigned D1S3                   :1;
        unsigned D1S4                   :1;
    };
} CLC2SEL0bits_t;
extern volatile CLC2SEL0bits_t CLC2SEL0bits __at(0xF1C);
// bitfield macros
#define _CLC2SEL0_LC2D1S0_POSN                              0x0
#define _CLC2SEL0_LC2D1S0_POSITION                          0x0
#define _CLC2SEL0_LC2D1S0_SIZE                              0x1
#define _CLC2SEL0_LC2D1S0_LENGTH                            0x1
#define _CLC2SEL0_LC2D1S0_MASK                              0x1
#define _CLC2SEL0_LC2D1S1_POSN                              0x1
#define _CLC2SEL0_LC2D1S1_POSITION                          0x1
#define _CLC2SEL0_LC2D1S1_SIZE                              0x1
#define _CLC2SEL0_LC2D1S1_LENGTH                            0x1
#define _CLC2SEL0_LC2D1S1_MASK                              0x2
#define _CLC2SEL0_LC2D1S2_POSN                              0x2
#define _CLC2SEL0_LC2D1S2_POSITION                          0x2
#define _CLC2SEL0_LC2D1S2_SIZE                              0x1
#define _CLC2SEL0_LC2D1S2_LENGTH                            0x1
#define _CLC2SEL0_LC2D1S2_MASK                              0x4
#define _CLC2SEL0_LC2D1S3_POSN                              0x3
#define _CLC2SEL0_LC2D1S3_POSITION                          0x3
#define _CLC2SEL0_LC2D1S3_SIZE                              0x1
#define _CLC2SEL0_LC2D1S3_LENGTH                            0x1
#define _CLC2SEL0_LC2D1S3_MASK                              0x8
#define _CLC2SEL0_LC2D1S4_POSN                              0x4
#define _CLC2SEL0_LC2D1S4_POSITION                          0x4
#define _CLC2SEL0_LC2D1S4_SIZE                              0x1
#define _CLC2SEL0_LC2D1S4_LENGTH                            0x1
#define _CLC2SEL0_LC2D1S4_MASK                              0x10
#define _CLC2SEL0_LC2D1S_POSN                               0x0
#define _CLC2SEL0_LC2D1S_POSITION                           0x0
#define _CLC2SEL0_LC2D1S_SIZE                               0x8
#define _CLC2SEL0_LC2D1S_LENGTH                             0x8
#define _CLC2SEL0_LC2D1S_MASK                               0xFF
#define _CLC2SEL0_D1S_POSN                                  0x0
#define _CLC2SEL0_D1S_POSITION                              0x0
#define _CLC2SEL0_D1S_SIZE                                  0x8
#define _CLC2SEL0_D1S_LENGTH                                0x8
#define _CLC2SEL0_D1S_MASK                                  0xFF
#define _CLC2SEL0_D1S0_POSN                                 0x0
#define _CLC2SEL0_D1S0_POSITION                             0x0
#define _CLC2SEL0_D1S0_SIZE                                 0x1
#define _CLC2SEL0_D1S0_LENGTH                               0x1
#define _CLC2SEL0_D1S0_MASK                                 0x1
#define _CLC2SEL0_D1S1_POSN                                 0x1
#define _CLC2SEL0_D1S1_POSITION                             0x1
#define _CLC2SEL0_D1S1_SIZE                                 0x1
#define _CLC2SEL0_D1S1_LENGTH                               0x1
#define _CLC2SEL0_D1S1_MASK                                 0x2
#define _CLC2SEL0_D1S2_POSN                                 0x2
#define _CLC2SEL0_D1S2_POSITION                             0x2
#define _CLC2SEL0_D1S2_SIZE                                 0x1
#define _CLC2SEL0_D1S2_LENGTH                               0x1
#define _CLC2SEL0_D1S2_MASK                                 0x4
#define _CLC2SEL0_D1S3_POSN                                 0x3
#define _CLC2SEL0_D1S3_POSITION                             0x3
#define _CLC2SEL0_D1S3_SIZE                                 0x1
#define _CLC2SEL0_D1S3_LENGTH                               0x1
#define _CLC2SEL0_D1S3_MASK                                 0x8
#define _CLC2SEL0_D1S4_POSN                                 0x4
#define _CLC2SEL0_D1S4_POSITION                             0x4
#define _CLC2SEL0_D1S4_SIZE                                 0x1
#define _CLC2SEL0_D1S4_LENGTH                               0x1
#define _CLC2SEL0_D1S4_MASK                                 0x10

// Register: CLC2SEL1
#define CLC2SEL1 CLC2SEL1
extern volatile unsigned char           CLC2SEL1            __at(0xF1D);
#ifndef _LIB_BUILD
asm("CLC2SEL1 equ 0F1Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC2D2S0                :1;
        unsigned LC2D2S1                :1;
        unsigned LC2D2S2                :1;
        unsigned LC2D2S3                :1;
        unsigned LC2D2S4                :1;
    };
    struct {
        unsigned LC2D2S                 :8;
    };
    struct {
        unsigned D2S                    :8;
    };
    struct {
        unsigned D2S0                   :1;
        unsigned D2S1                   :1;
        unsigned D2S2                   :1;
        unsigned D2S3                   :1;
        unsigned D2S4                   :1;
    };
} CLC2SEL1bits_t;
extern volatile CLC2SEL1bits_t CLC2SEL1bits __at(0xF1D);
// bitfield macros
#define _CLC2SEL1_LC2D2S0_POSN                              0x0
#define _CLC2SEL1_LC2D2S0_POSITION                          0x0
#define _CLC2SEL1_LC2D2S0_SIZE                              0x1
#define _CLC2SEL1_LC2D2S0_LENGTH                            0x1
#define _CLC2SEL1_LC2D2S0_MASK                              0x1
#define _CLC2SEL1_LC2D2S1_POSN                              0x1
#define _CLC2SEL1_LC2D2S1_POSITION                          0x1
#define _CLC2SEL1_LC2D2S1_SIZE                              0x1
#define _CLC2SEL1_LC2D2S1_LENGTH                            0x1
#define _CLC2SEL1_LC2D2S1_MASK                              0x2
#define _CLC2SEL1_LC2D2S2_POSN                              0x2
#define _CLC2SEL1_LC2D2S2_POSITION                          0x2
#define _CLC2SEL1_LC2D2S2_SIZE                              0x1
#define _CLC2SEL1_LC2D2S2_LENGTH                            0x1
#define _CLC2SEL1_LC2D2S2_MASK                              0x4
#define _CLC2SEL1_LC2D2S3_POSN                              0x3
#define _CLC2SEL1_LC2D2S3_POSITION                          0x3
#define _CLC2SEL1_LC2D2S3_SIZE                              0x1
#define _CLC2SEL1_LC2D2S3_LENGTH                            0x1
#define _CLC2SEL1_LC2D2S3_MASK                              0x8
#define _CLC2SEL1_LC2D2S4_POSN                              0x4
#define _CLC2SEL1_LC2D2S4_POSITION                          0x4
#define _CLC2SEL1_LC2D2S4_SIZE                              0x1
#define _CLC2SEL1_LC2D2S4_LENGTH                            0x1
#define _CLC2SEL1_LC2D2S4_MASK                              0x10
#define _CLC2SEL1_LC2D2S_POSN                               0x0
#define _CLC2SEL1_LC2D2S_POSITION                           0x0
#define _CLC2SEL1_LC2D2S_SIZE                               0x8
#define _CLC2SEL1_LC2D2S_LENGTH                             0x8
#define _CLC2SEL1_LC2D2S_MASK                               0xFF
#define _CLC2SEL1_D2S_POSN                                  0x0
#define _CLC2SEL1_D2S_POSITION                              0x0
#define _CLC2SEL1_D2S_SIZE                                  0x8
#define _CLC2SEL1_D2S_LENGTH                                0x8
#define _CLC2SEL1_D2S_MASK                                  0xFF
#define _CLC2SEL1_D2S0_POSN                                 0x0
#define _CLC2SEL1_D2S0_POSITION                             0x0
#define _CLC2SEL1_D2S0_SIZE                                 0x1
#define _CLC2SEL1_D2S0_LENGTH                               0x1
#define _CLC2SEL1_D2S0_MASK                                 0x1
#define _CLC2SEL1_D2S1_POSN                                 0x1
#define _CLC2SEL1_D2S1_POSITION                             0x1
#define _CLC2SEL1_D2S1_SIZE                                 0x1
#define _CLC2SEL1_D2S1_LENGTH                               0x1
#define _CLC2SEL1_D2S1_MASK                                 0x2
#define _CLC2SEL1_D2S2_POSN                                 0x2
#define _CLC2SEL1_D2S2_POSITION                             0x2
#define _CLC2SEL1_D2S2_SIZE                                 0x1
#define _CLC2SEL1_D2S2_LENGTH                               0x1
#define _CLC2SEL1_D2S2_MASK                                 0x4
#define _CLC2SEL1_D2S3_POSN                                 0x3
#define _CLC2SEL1_D2S3_POSITION                             0x3
#define _CLC2SEL1_D2S3_SIZE                                 0x1
#define _CLC2SEL1_D2S3_LENGTH                               0x1
#define _CLC2SEL1_D2S3_MASK                                 0x8
#define _CLC2SEL1_D2S4_POSN                                 0x4
#define _CLC2SEL1_D2S4_POSITION                             0x4
#define _CLC2SEL1_D2S4_SIZE                                 0x1
#define _CLC2SEL1_D2S4_LENGTH                               0x1
#define _CLC2SEL1_D2S4_MASK                                 0x10

// Register: CLC2SEL2
#define CLC2SEL2 CLC2SEL2
extern volatile unsigned char           CLC2SEL2            __at(0xF1E);
#ifndef _LIB_BUILD
asm("CLC2SEL2 equ 0F1Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC2D3S0                :1;
        unsigned LC2D3S1                :1;
        unsigned LC2D3S2                :1;
        unsigned LC2D3S3                :1;
        unsigned LC2D3S4                :1;
    };
    struct {
        unsigned LC2D3S                 :8;
    };
    struct {
        unsigned D3S                    :8;
    };
    struct {
        unsigned D3S0                   :1;
        unsigned D3S1                   :1;
        unsigned D3S2                   :1;
        unsigned D3S3                   :1;
        unsigned D3S4                   :1;
    };
} CLC2SEL2bits_t;
extern volatile CLC2SEL2bits_t CLC2SEL2bits __at(0xF1E);
// bitfield macros
#define _CLC2SEL2_LC2D3S0_POSN                              0x0
#define _CLC2SEL2_LC2D3S0_POSITION                          0x0
#define _CLC2SEL2_LC2D3S0_SIZE                              0x1
#define _CLC2SEL2_LC2D3S0_LENGTH                            0x1
#define _CLC2SEL2_LC2D3S0_MASK                              0x1
#define _CLC2SEL2_LC2D3S1_POSN                              0x1
#define _CLC2SEL2_LC2D3S1_POSITION                          0x1
#define _CLC2SEL2_LC2D3S1_SIZE                              0x1
#define _CLC2SEL2_LC2D3S1_LENGTH                            0x1
#define _CLC2SEL2_LC2D3S1_MASK                              0x2
#define _CLC2SEL2_LC2D3S2_POSN                              0x2
#define _CLC2SEL2_LC2D3S2_POSITION                          0x2
#define _CLC2SEL2_LC2D3S2_SIZE                              0x1
#define _CLC2SEL2_LC2D3S2_LENGTH                            0x1
#define _CLC2SEL2_LC2D3S2_MASK                              0x4
#define _CLC2SEL2_LC2D3S3_POSN                              0x3
#define _CLC2SEL2_LC2D3S3_POSITION                          0x3
#define _CLC2SEL2_LC2D3S3_SIZE                              0x1
#define _CLC2SEL2_LC2D3S3_LENGTH                            0x1
#define _CLC2SEL2_LC2D3S3_MASK                              0x8
#define _CLC2SEL2_LC2D3S4_POSN                              0x4
#define _CLC2SEL2_LC2D3S4_POSITION                          0x4
#define _CLC2SEL2_LC2D3S4_SIZE                              0x1
#define _CLC2SEL2_LC2D3S4_LENGTH                            0x1
#define _CLC2SEL2_LC2D3S4_MASK                              0x10
#define _CLC2SEL2_LC2D3S_POSN                               0x0
#define _CLC2SEL2_LC2D3S_POSITION                           0x0
#define _CLC2SEL2_LC2D3S_SIZE                               0x8
#define _CLC2SEL2_LC2D3S_LENGTH                             0x8
#define _CLC2SEL2_LC2D3S_MASK                               0xFF
#define _CLC2SEL2_D3S_POSN                                  0x0
#define _CLC2SEL2_D3S_POSITION                              0x0
#define _CLC2SEL2_D3S_SIZE                                  0x8
#define _CLC2SEL2_D3S_LENGTH                                0x8
#define _CLC2SEL2_D3S_MASK                                  0xFF
#define _CLC2SEL2_D3S0_POSN                                 0x0
#define _CLC2SEL2_D3S0_POSITION                             0x0
#define _CLC2SEL2_D3S0_SIZE                                 0x1
#define _CLC2SEL2_D3S0_LENGTH                               0x1
#define _CLC2SEL2_D3S0_MASK                                 0x1
#define _CLC2SEL2_D3S1_POSN                                 0x1
#define _CLC2SEL2_D3S1_POSITION                             0x1
#define _CLC2SEL2_D3S1_SIZE                                 0x1
#define _CLC2SEL2_D3S1_LENGTH                               0x1
#define _CLC2SEL2_D3S1_MASK                                 0x2
#define _CLC2SEL2_D3S2_POSN                                 0x2
#define _CLC2SEL2_D3S2_POSITION                             0x2
#define _CLC2SEL2_D3S2_SIZE                                 0x1
#define _CLC2SEL2_D3S2_LENGTH                               0x1
#define _CLC2SEL2_D3S2_MASK                                 0x4
#define _CLC2SEL2_D3S3_POSN                                 0x3
#define _CLC2SEL2_D3S3_POSITION                             0x3
#define _CLC2SEL2_D3S3_SIZE                                 0x1
#define _CLC2SEL2_D3S3_LENGTH                               0x1
#define _CLC2SEL2_D3S3_MASK                                 0x8
#define _CLC2SEL2_D3S4_POSN                                 0x4
#define _CLC2SEL2_D3S4_POSITION                             0x4
#define _CLC2SEL2_D3S4_SIZE                                 0x1
#define _CLC2SEL2_D3S4_LENGTH                               0x1
#define _CLC2SEL2_D3S4_MASK                                 0x10

// Register: CLC2SEL3
#define CLC2SEL3 CLC2SEL3
extern volatile unsigned char           CLC2SEL3            __at(0xF1F);
#ifndef _LIB_BUILD
asm("CLC2SEL3 equ 0F1Fh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC2D4S0                :1;
        unsigned LC2D4S1                :1;
        unsigned LC2D4S2                :1;
        unsigned LC2D4S3                :1;
        unsigned LC2D4S4                :1;
    };
    struct {
        unsigned LC2D4S                 :8;
    };
    struct {
        unsigned D4S                    :8;
    };
    struct {
        unsigned D4S0                   :1;
        unsigned D4S1                   :1;
        unsigned D4S2                   :1;
        unsigned D4S3                   :1;
        unsigned D4S4                   :1;
    };
} CLC2SEL3bits_t;
extern volatile CLC2SEL3bits_t CLC2SEL3bits __at(0xF1F);
// bitfield macros
#define _CLC2SEL3_LC2D4S0_POSN                              0x0
#define _CLC2SEL3_LC2D4S0_POSITION                          0x0
#define _CLC2SEL3_LC2D4S0_SIZE                              0x1
#define _CLC2SEL3_LC2D4S0_LENGTH                            0x1
#define _CLC2SEL3_LC2D4S0_MASK                              0x1
#define _CLC2SEL3_LC2D4S1_POSN                              0x1
#define _CLC2SEL3_LC2D4S1_POSITION                          0x1
#define _CLC2SEL3_LC2D4S1_SIZE                              0x1
#define _CLC2SEL3_LC2D4S1_LENGTH                            0x1
#define _CLC2SEL3_LC2D4S1_MASK                              0x2
#define _CLC2SEL3_LC2D4S2_POSN                              0x2
#define _CLC2SEL3_LC2D4S2_POSITION                          0x2
#define _CLC2SEL3_LC2D4S2_SIZE                              0x1
#define _CLC2SEL3_LC2D4S2_LENGTH                            0x1
#define _CLC2SEL3_LC2D4S2_MASK                              0x4
#define _CLC2SEL3_LC2D4S3_POSN                              0x3
#define _CLC2SEL3_LC2D4S3_POSITION                          0x3
#define _CLC2SEL3_LC2D4S3_SIZE                              0x1
#define _CLC2SEL3_LC2D4S3_LENGTH                            0x1
#define _CLC2SEL3_LC2D4S3_MASK                              0x8
#define _CLC2SEL3_LC2D4S4_POSN                              0x4
#define _CLC2SEL3_LC2D4S4_POSITION                          0x4
#define _CLC2SEL3_LC2D4S4_SIZE                              0x1
#define _CLC2SEL3_LC2D4S4_LENGTH                            0x1
#define _CLC2SEL3_LC2D4S4_MASK                              0x10
#define _CLC2SEL3_LC2D4S_POSN                               0x0
#define _CLC2SEL3_LC2D4S_POSITION                           0x0
#define _CLC2SEL3_LC2D4S_SIZE                               0x8
#define _CLC2SEL3_LC2D4S_LENGTH                             0x8
#define _CLC2SEL3_LC2D4S_MASK                               0xFF
#define _CLC2SEL3_D4S_POSN                                  0x0
#define _CLC2SEL3_D4S_POSITION                              0x0
#define _CLC2SEL3_D4S_SIZE                                  0x8
#define _CLC2SEL3_D4S_LENGTH                                0x8
#define _CLC2SEL3_D4S_MASK                                  0xFF
#define _CLC2SEL3_D4S0_POSN                                 0x0
#define _CLC2SEL3_D4S0_POSITION                             0x0
#define _CLC2SEL3_D4S0_SIZE                                 0x1
#define _CLC2SEL3_D4S0_LENGTH                               0x1
#define _CLC2SEL3_D4S0_MASK                                 0x1
#define _CLC2SEL3_D4S1_POSN                                 0x1
#define _CLC2SEL3_D4S1_POSITION                             0x1
#define _CLC2SEL3_D4S1_SIZE                                 0x1
#define _CLC2SEL3_D4S1_LENGTH                               0x1
#define _CLC2SEL3_D4S1_MASK                                 0x2
#define _CLC2SEL3_D4S2_POSN                                 0x2
#define _CLC2SEL3_D4S2_POSITION                             0x2
#define _CLC2SEL3_D4S2_SIZE                                 0x1
#define _CLC2SEL3_D4S2_LENGTH                               0x1
#define _CLC2SEL3_D4S2_MASK                                 0x4
#define _CLC2SEL3_D4S3_POSN                                 0x3
#define _CLC2SEL3_D4S3_POSITION                             0x3
#define _CLC2SEL3_D4S3_SIZE                                 0x1
#define _CLC2SEL3_D4S3_LENGTH                               0x1
#define _CLC2SEL3_D4S3_MASK                                 0x8
#define _CLC2SEL3_D4S4_POSN                                 0x4
#define _CLC2SEL3_D4S4_POSITION                             0x4
#define _CLC2SEL3_D4S4_SIZE                                 0x1
#define _CLC2SEL3_D4S4_LENGTH                               0x1
#define _CLC2SEL3_D4S4_MASK                                 0x10

// Register: CLC2GLS0
#define CLC2GLS0 CLC2GLS0
extern volatile unsigned char           CLC2GLS0            __at(0xF20);
#ifndef _LIB_BUILD
asm("CLC2GLS0 equ 0F20h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC2G1D1N               :1;
        unsigned LC2G1D1T               :1;
        unsigned LC2G1D2N               :1;
        unsigned LC2G1D2T               :1;
        unsigned LC2G1D3N               :1;
        unsigned LC2G1D3T               :1;
        unsigned LC2G1D4N               :1;
        unsigned LC2G1D4T               :1;
    };
    struct {
        unsigned D1N                    :1;
        unsigned D1T                    :1;
        unsigned D2N                    :1;
        unsigned D2T                    :1;
        unsigned D3N                    :1;
        unsigned D3T                    :1;
        unsigned D4N                    :1;
        unsigned D4T                    :1;
    };
} CLC2GLS0bits_t;
extern volatile CLC2GLS0bits_t CLC2GLS0bits __at(0xF20);
// bitfield macros
#define _CLC2GLS0_LC2G1D1N_POSN                             0x0
#define _CLC2GLS0_LC2G1D1N_POSITION                         0x0
#define _CLC2GLS0_LC2G1D1N_SIZE                             0x1
#define _CLC2GLS0_LC2G1D1N_LENGTH                           0x1
#define _CLC2GLS0_LC2G1D1N_MASK                             0x1
#define _CLC2GLS0_LC2G1D1T_POSN                             0x1
#define _CLC2GLS0_LC2G1D1T_POSITION                         0x1
#define _CLC2GLS0_LC2G1D1T_SIZE                             0x1
#define _CLC2GLS0_LC2G1D1T_LENGTH                           0x1
#define _CLC2GLS0_LC2G1D1T_MASK                             0x2
#define _CLC2GLS0_LC2G1D2N_POSN                             0x2
#define _CLC2GLS0_LC2G1D2N_POSITION                         0x2
#define _CLC2GLS0_LC2G1D2N_SIZE                             0x1
#define _CLC2GLS0_LC2G1D2N_LENGTH                           0x1
#define _CLC2GLS0_LC2G1D2N_MASK                             0x4
#define _CLC2GLS0_LC2G1D2T_POSN                             0x3
#define _CLC2GLS0_LC2G1D2T_POSITION                         0x3
#define _CLC2GLS0_LC2G1D2T_SIZE                             0x1
#define _CLC2GLS0_LC2G1D2T_LENGTH                           0x1
#define _CLC2GLS0_LC2G1D2T_MASK                             0x8
#define _CLC2GLS0_LC2G1D3N_POSN                             0x4
#define _CLC2GLS0_LC2G1D3N_POSITION                         0x4
#define _CLC2GLS0_LC2G1D3N_SIZE                             0x1
#define _CLC2GLS0_LC2G1D3N_LENGTH                           0x1
#define _CLC2GLS0_LC2G1D3N_MASK                             0x10
#define _CLC2GLS0_LC2G1D3T_POSN                             0x5
#define _CLC2GLS0_LC2G1D3T_POSITION                         0x5
#define _CLC2GLS0_LC2G1D3T_SIZE                             0x1
#define _CLC2GLS0_LC2G1D3T_LENGTH                           0x1
#define _CLC2GLS0_LC2G1D3T_MASK                             0x20
#define _CLC2GLS0_LC2G1D4N_POSN                             0x6
#define _CLC2GLS0_LC2G1D4N_POSITION                         0x6
#define _CLC2GLS0_LC2G1D4N_SIZE                             0x1
#define _CLC2GLS0_LC2G1D4N_LENGTH                           0x1
#define _CLC2GLS0_LC2G1D4N_MASK                             0x40
#define _CLC2GLS0_LC2G1D4T_POSN                             0x7
#define _CLC2GLS0_LC2G1D4T_POSITION                         0x7
#define _CLC2GLS0_LC2G1D4T_SIZE                             0x1
#define _CLC2GLS0_LC2G1D4T_LENGTH                           0x1
#define _CLC2GLS0_LC2G1D4T_MASK                             0x80
#define _CLC2GLS0_D1N_POSN                                  0x0
#define _CLC2GLS0_D1N_POSITION                              0x0
#define _CLC2GLS0_D1N_SIZE                                  0x1
#define _CLC2GLS0_D1N_LENGTH                                0x1
#define _CLC2GLS0_D1N_MASK                                  0x1
#define _CLC2GLS0_D1T_POSN                                  0x1
#define _CLC2GLS0_D1T_POSITION                              0x1
#define _CLC2GLS0_D1T_SIZE                                  0x1
#define _CLC2GLS0_D1T_LENGTH                                0x1
#define _CLC2GLS0_D1T_MASK                                  0x2
#define _CLC2GLS0_D2N_POSN                                  0x2
#define _CLC2GLS0_D2N_POSITION                              0x2
#define _CLC2GLS0_D2N_SIZE                                  0x1
#define _CLC2GLS0_D2N_LENGTH                                0x1
#define _CLC2GLS0_D2N_MASK                                  0x4
#define _CLC2GLS0_D2T_POSN                                  0x3
#define _CLC2GLS0_D2T_POSITION                              0x3
#define _CLC2GLS0_D2T_SIZE                                  0x1
#define _CLC2GLS0_D2T_LENGTH                                0x1
#define _CLC2GLS0_D2T_MASK                                  0x8
#define _CLC2GLS0_D3N_POSN                                  0x4
#define _CLC2GLS0_D3N_POSITION                              0x4
#define _CLC2GLS0_D3N_SIZE                                  0x1
#define _CLC2GLS0_D3N_LENGTH                                0x1
#define _CLC2GLS0_D3N_MASK                                  0x10
#define _CLC2GLS0_D3T_POSN                                  0x5
#define _CLC2GLS0_D3T_POSITION                              0x5
#define _CLC2GLS0_D3T_SIZE                                  0x1
#define _CLC2GLS0_D3T_LENGTH                                0x1
#define _CLC2GLS0_D3T_MASK                                  0x20
#define _CLC2GLS0_D4N_POSN                                  0x6
#define _CLC2GLS0_D4N_POSITION                              0x6
#define _CLC2GLS0_D4N_SIZE                                  0x1
#define _CLC2GLS0_D4N_LENGTH                                0x1
#define _CLC2GLS0_D4N_MASK                                  0x40
#define _CLC2GLS0_D4T_POSN                                  0x7
#define _CLC2GLS0_D4T_POSITION                              0x7
#define _CLC2GLS0_D4T_SIZE                                  0x1
#define _CLC2GLS0_D4T_LENGTH                                0x1
#define _CLC2GLS0_D4T_MASK                                  0x80

// Register: CLC2GLS1
#define CLC2GLS1 CLC2GLS1
extern volatile unsigned char           CLC2GLS1            __at(0xF21);
#ifndef _LIB_BUILD
asm("CLC2GLS1 equ 0F21h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC2G2D1N               :1;
        unsigned LC2G2D1T               :1;
        unsigned LC2G2D2N               :1;
        unsigned LC2G2D2T               :1;
        unsigned LC2G2D3N               :1;
        unsigned LC2G2D3T               :1;
        unsigned LC2G2D4N               :1;
        unsigned LC2G2D4T               :1;
    };
    struct {
        unsigned D1N                    :1;
        unsigned D1T                    :1;
        unsigned D2N                    :1;
        unsigned D2T                    :1;
        unsigned D3N                    :1;
        unsigned D3T                    :1;
        unsigned D4N                    :1;
        unsigned D4T                    :1;
    };
} CLC2GLS1bits_t;
extern volatile CLC2GLS1bits_t CLC2GLS1bits __at(0xF21);
// bitfield macros
#define _CLC2GLS1_LC2G2D1N_POSN                             0x0
#define _CLC2GLS1_LC2G2D1N_POSITION                         0x0
#define _CLC2GLS1_LC2G2D1N_SIZE                             0x1
#define _CLC2GLS1_LC2G2D1N_LENGTH                           0x1
#define _CLC2GLS1_LC2G2D1N_MASK                             0x1
#define _CLC2GLS1_LC2G2D1T_POSN                             0x1
#define _CLC2GLS1_LC2G2D1T_POSITION                         0x1
#define _CLC2GLS1_LC2G2D1T_SIZE                             0x1
#define _CLC2GLS1_LC2G2D1T_LENGTH                           0x1
#define _CLC2GLS1_LC2G2D1T_MASK                             0x2
#define _CLC2GLS1_LC2G2D2N_POSN                             0x2
#define _CLC2GLS1_LC2G2D2N_POSITION                         0x2
#define _CLC2GLS1_LC2G2D2N_SIZE                             0x1
#define _CLC2GLS1_LC2G2D2N_LENGTH                           0x1
#define _CLC2GLS1_LC2G2D2N_MASK                             0x4
#define _CLC2GLS1_LC2G2D2T_POSN                             0x3
#define _CLC2GLS1_LC2G2D2T_POSITION                         0x3
#define _CLC2GLS1_LC2G2D2T_SIZE                             0x1
#define _CLC2GLS1_LC2G2D2T_LENGTH                           0x1
#define _CLC2GLS1_LC2G2D2T_MASK                             0x8
#define _CLC2GLS1_LC2G2D3N_POSN                             0x4
#define _CLC2GLS1_LC2G2D3N_POSITION                         0x4
#define _CLC2GLS1_LC2G2D3N_SIZE                             0x1
#define _CLC2GLS1_LC2G2D3N_LENGTH                           0x1
#define _CLC2GLS1_LC2G2D3N_MASK                             0x10
#define _CLC2GLS1_LC2G2D3T_POSN                             0x5
#define _CLC2GLS1_LC2G2D3T_POSITION                         0x5
#define _CLC2GLS1_LC2G2D3T_SIZE                             0x1
#define _CLC2GLS1_LC2G2D3T_LENGTH                           0x1
#define _CLC2GLS1_LC2G2D3T_MASK                             0x20
#define _CLC2GLS1_LC2G2D4N_POSN                             0x6
#define _CLC2GLS1_LC2G2D4N_POSITION                         0x6
#define _CLC2GLS1_LC2G2D4N_SIZE                             0x1
#define _CLC2GLS1_LC2G2D4N_LENGTH                           0x1
#define _CLC2GLS1_LC2G2D4N_MASK                             0x40
#define _CLC2GLS1_LC2G2D4T_POSN                             0x7
#define _CLC2GLS1_LC2G2D4T_POSITION                         0x7
#define _CLC2GLS1_LC2G2D4T_SIZE                             0x1
#define _CLC2GLS1_LC2G2D4T_LENGTH                           0x1
#define _CLC2GLS1_LC2G2D4T_MASK                             0x80
#define _CLC2GLS1_D1N_POSN                                  0x0
#define _CLC2GLS1_D1N_POSITION                              0x0
#define _CLC2GLS1_D1N_SIZE                                  0x1
#define _CLC2GLS1_D1N_LENGTH                                0x1
#define _CLC2GLS1_D1N_MASK                                  0x1
#define _CLC2GLS1_D1T_POSN                                  0x1
#define _CLC2GLS1_D1T_POSITION                              0x1
#define _CLC2GLS1_D1T_SIZE                                  0x1
#define _CLC2GLS1_D1T_LENGTH                                0x1
#define _CLC2GLS1_D1T_MASK                                  0x2
#define _CLC2GLS1_D2N_POSN                                  0x2
#define _CLC2GLS1_D2N_POSITION                              0x2
#define _CLC2GLS1_D2N_SIZE                                  0x1
#define _CLC2GLS1_D2N_LENGTH                                0x1
#define _CLC2GLS1_D2N_MASK                                  0x4
#define _CLC2GLS1_D2T_POSN                                  0x3
#define _CLC2GLS1_D2T_POSITION                              0x3
#define _CLC2GLS1_D2T_SIZE                                  0x1
#define _CLC2GLS1_D2T_LENGTH                                0x1
#define _CLC2GLS1_D2T_MASK                                  0x8
#define _CLC2GLS1_D3N_POSN                                  0x4
#define _CLC2GLS1_D3N_POSITION                              0x4
#define _CLC2GLS1_D3N_SIZE                                  0x1
#define _CLC2GLS1_D3N_LENGTH                                0x1
#define _CLC2GLS1_D3N_MASK                                  0x10
#define _CLC2GLS1_D3T_POSN                                  0x5
#define _CLC2GLS1_D3T_POSITION                              0x5
#define _CLC2GLS1_D3T_SIZE                                  0x1
#define _CLC2GLS1_D3T_LENGTH                                0x1
#define _CLC2GLS1_D3T_MASK                                  0x20
#define _CLC2GLS1_D4N_POSN                                  0x6
#define _CLC2GLS1_D4N_POSITION                              0x6
#define _CLC2GLS1_D4N_SIZE                                  0x1
#define _CLC2GLS1_D4N_LENGTH                                0x1
#define _CLC2GLS1_D4N_MASK                                  0x40
#define _CLC2GLS1_D4T_POSN                                  0x7
#define _CLC2GLS1_D4T_POSITION                              0x7
#define _CLC2GLS1_D4T_SIZE                                  0x1
#define _CLC2GLS1_D4T_LENGTH                                0x1
#define _CLC2GLS1_D4T_MASK                                  0x80

// Register: CLC2GLS2
#define CLC2GLS2 CLC2GLS2
extern volatile unsigned char           CLC2GLS2            __at(0xF22);
#ifndef _LIB_BUILD
asm("CLC2GLS2 equ 0F22h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC2G3D1N               :1;
        unsigned LC2G3D1T               :1;
        unsigned LC2G3D2N               :1;
        unsigned LC2G3D2T               :1;
        unsigned LC2G3D3N               :1;
        unsigned LC2G3D3T               :1;
        unsigned LC2G3D4N               :1;
        unsigned LC2G3D4T               :1;
    };
    struct {
        unsigned D1N                    :1;
        unsigned D1T                    :1;
        unsigned D2N                    :1;
        unsigned D2T                    :1;
        unsigned D3N                    :1;
        unsigned D3T                    :1;
        unsigned D4N                    :1;
        unsigned D4T                    :1;
    };
} CLC2GLS2bits_t;
extern volatile CLC2GLS2bits_t CLC2GLS2bits __at(0xF22);
// bitfield macros
#define _CLC2GLS2_LC2G3D1N_POSN                             0x0
#define _CLC2GLS2_LC2G3D1N_POSITION                         0x0
#define _CLC2GLS2_LC2G3D1N_SIZE                             0x1
#define _CLC2GLS2_LC2G3D1N_LENGTH                           0x1
#define _CLC2GLS2_LC2G3D1N_MASK                             0x1
#define _CLC2GLS2_LC2G3D1T_POSN                             0x1
#define _CLC2GLS2_LC2G3D1T_POSITION                         0x1
#define _CLC2GLS2_LC2G3D1T_SIZE                             0x1
#define _CLC2GLS2_LC2G3D1T_LENGTH                           0x1
#define _CLC2GLS2_LC2G3D1T_MASK                             0x2
#define _CLC2GLS2_LC2G3D2N_POSN                             0x2
#define _CLC2GLS2_LC2G3D2N_POSITION                         0x2
#define _CLC2GLS2_LC2G3D2N_SIZE                             0x1
#define _CLC2GLS2_LC2G3D2N_LENGTH                           0x1
#define _CLC2GLS2_LC2G3D2N_MASK                             0x4
#define _CLC2GLS2_LC2G3D2T_POSN                             0x3
#define _CLC2GLS2_LC2G3D2T_POSITION                         0x3
#define _CLC2GLS2_LC2G3D2T_SIZE                             0x1
#define _CLC2GLS2_LC2G3D2T_LENGTH                           0x1
#define _CLC2GLS2_LC2G3D2T_MASK                             0x8
#define _CLC2GLS2_LC2G3D3N_POSN                             0x4
#define _CLC2GLS2_LC2G3D3N_POSITION                         0x4
#define _CLC2GLS2_LC2G3D3N_SIZE                             0x1
#define _CLC2GLS2_LC2G3D3N_LENGTH                           0x1
#define _CLC2GLS2_LC2G3D3N_MASK                             0x10
#define _CLC2GLS2_LC2G3D3T_POSN                             0x5
#define _CLC2GLS2_LC2G3D3T_POSITION                         0x5
#define _CLC2GLS2_LC2G3D3T_SIZE                             0x1
#define _CLC2GLS2_LC2G3D3T_LENGTH                           0x1
#define _CLC2GLS2_LC2G3D3T_MASK                             0x20
#define _CLC2GLS2_LC2G3D4N_POSN                             0x6
#define _CLC2GLS2_LC2G3D4N_POSITION                         0x6
#define _CLC2GLS2_LC2G3D4N_SIZE                             0x1
#define _CLC2GLS2_LC2G3D4N_LENGTH                           0x1
#define _CLC2GLS2_LC2G3D4N_MASK                             0x40
#define _CLC2GLS2_LC2G3D4T_POSN                             0x7
#define _CLC2GLS2_LC2G3D4T_POSITION                         0x7
#define _CLC2GLS2_LC2G3D4T_SIZE                             0x1
#define _CLC2GLS2_LC2G3D4T_LENGTH                           0x1
#define _CLC2GLS2_LC2G3D4T_MASK                             0x80
#define _CLC2GLS2_D1N_POSN                                  0x0
#define _CLC2GLS2_D1N_POSITION                              0x0
#define _CLC2GLS2_D1N_SIZE                                  0x1
#define _CLC2GLS2_D1N_LENGTH                                0x1
#define _CLC2GLS2_D1N_MASK                                  0x1
#define _CLC2GLS2_D1T_POSN                                  0x1
#define _CLC2GLS2_D1T_POSITION                              0x1
#define _CLC2GLS2_D1T_SIZE                                  0x1
#define _CLC2GLS2_D1T_LENGTH                                0x1
#define _CLC2GLS2_D1T_MASK                                  0x2
#define _CLC2GLS2_D2N_POSN                                  0x2
#define _CLC2GLS2_D2N_POSITION                              0x2
#define _CLC2GLS2_D2N_SIZE                                  0x1
#define _CLC2GLS2_D2N_LENGTH                                0x1
#define _CLC2GLS2_D2N_MASK                                  0x4
#define _CLC2GLS2_D2T_POSN                                  0x3
#define _CLC2GLS2_D2T_POSITION                              0x3
#define _CLC2GLS2_D2T_SIZE                                  0x1
#define _CLC2GLS2_D2T_LENGTH                                0x1
#define _CLC2GLS2_D2T_MASK                                  0x8
#define _CLC2GLS2_D3N_POSN                                  0x4
#define _CLC2GLS2_D3N_POSITION                              0x4
#define _CLC2GLS2_D3N_SIZE                                  0x1
#define _CLC2GLS2_D3N_LENGTH                                0x1
#define _CLC2GLS2_D3N_MASK                                  0x10
#define _CLC2GLS2_D3T_POSN                                  0x5
#define _CLC2GLS2_D3T_POSITION                              0x5
#define _CLC2GLS2_D3T_SIZE                                  0x1
#define _CLC2GLS2_D3T_LENGTH                                0x1
#define _CLC2GLS2_D3T_MASK                                  0x20
#define _CLC2GLS2_D4N_POSN                                  0x6
#define _CLC2GLS2_D4N_POSITION                              0x6
#define _CLC2GLS2_D4N_SIZE                                  0x1
#define _CLC2GLS2_D4N_LENGTH                                0x1
#define _CLC2GLS2_D4N_MASK                                  0x40
#define _CLC2GLS2_D4T_POSN                                  0x7
#define _CLC2GLS2_D4T_POSITION                              0x7
#define _CLC2GLS2_D4T_SIZE                                  0x1
#define _CLC2GLS2_D4T_LENGTH                                0x1
#define _CLC2GLS2_D4T_MASK                                  0x80

// Register: CLC2GLS3
#define CLC2GLS3 CLC2GLS3
extern volatile unsigned char           CLC2GLS3            __at(0xF23);
#ifndef _LIB_BUILD
asm("CLC2GLS3 equ 0F23h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC2G4D1N               :1;
        unsigned LC2G4D1T               :1;
        unsigned LC2G4D2N               :1;
        unsigned LC2G4D2T               :1;
        unsigned LC2G4D3N               :1;
        unsigned LC2G4D3T               :1;
        unsigned LC2G4D4N               :1;
        unsigned LC2G4D4T               :1;
    };
    struct {
        unsigned G4D1N                  :1;
        unsigned G4D1T                  :1;
        unsigned G4D2N                  :1;
        unsigned G4D2T                  :1;
        unsigned G4D3N                  :1;
        unsigned G4D3T                  :1;
        unsigned G4D4N                  :1;
        unsigned G4D4T                  :1;
    };
} CLC2GLS3bits_t;
extern volatile CLC2GLS3bits_t CLC2GLS3bits __at(0xF23);
// bitfield macros
#define _CLC2GLS3_LC2G4D1N_POSN                             0x0
#define _CLC2GLS3_LC2G4D1N_POSITION                         0x0
#define _CLC2GLS3_LC2G4D1N_SIZE                             0x1
#define _CLC2GLS3_LC2G4D1N_LENGTH                           0x1
#define _CLC2GLS3_LC2G4D1N_MASK                             0x1
#define _CLC2GLS3_LC2G4D1T_POSN                             0x1
#define _CLC2GLS3_LC2G4D1T_POSITION                         0x1
#define _CLC2GLS3_LC2G4D1T_SIZE                             0x1
#define _CLC2GLS3_LC2G4D1T_LENGTH                           0x1
#define _CLC2GLS3_LC2G4D1T_MASK                             0x2
#define _CLC2GLS3_LC2G4D2N_POSN                             0x2
#define _CLC2GLS3_LC2G4D2N_POSITION                         0x2
#define _CLC2GLS3_LC2G4D2N_SIZE                             0x1
#define _CLC2GLS3_LC2G4D2N_LENGTH                           0x1
#define _CLC2GLS3_LC2G4D2N_MASK                             0x4
#define _CLC2GLS3_LC2G4D2T_POSN                             0x3
#define _CLC2GLS3_LC2G4D2T_POSITION                         0x3
#define _CLC2GLS3_LC2G4D2T_SIZE                             0x1
#define _CLC2GLS3_LC2G4D2T_LENGTH                           0x1
#define _CLC2GLS3_LC2G4D2T_MASK                             0x8
#define _CLC2GLS3_LC2G4D3N_POSN                             0x4
#define _CLC2GLS3_LC2G4D3N_POSITION                         0x4
#define _CLC2GLS3_LC2G4D3N_SIZE                             0x1
#define _CLC2GLS3_LC2G4D3N_LENGTH                           0x1
#define _CLC2GLS3_LC2G4D3N_MASK                             0x10
#define _CLC2GLS3_LC2G4D3T_POSN                             0x5
#define _CLC2GLS3_LC2G4D3T_POSITION                         0x5
#define _CLC2GLS3_LC2G4D3T_SIZE                             0x1
#define _CLC2GLS3_LC2G4D3T_LENGTH                           0x1
#define _CLC2GLS3_LC2G4D3T_MASK                             0x20
#define _CLC2GLS3_LC2G4D4N_POSN                             0x6
#define _CLC2GLS3_LC2G4D4N_POSITION                         0x6
#define _CLC2GLS3_LC2G4D4N_SIZE                             0x1
#define _CLC2GLS3_LC2G4D4N_LENGTH                           0x1
#define _CLC2GLS3_LC2G4D4N_MASK                             0x40
#define _CLC2GLS3_LC2G4D4T_POSN                             0x7
#define _CLC2GLS3_LC2G4D4T_POSITION                         0x7
#define _CLC2GLS3_LC2G4D4T_SIZE                             0x1
#define _CLC2GLS3_LC2G4D4T_LENGTH                           0x1
#define _CLC2GLS3_LC2G4D4T_MASK                             0x80
#define _CLC2GLS3_G4D1N_POSN                                0x0
#define _CLC2GLS3_G4D1N_POSITION                            0x0
#define _CLC2GLS3_G4D1N_SIZE                                0x1
#define _CLC2GLS3_G4D1N_LENGTH                              0x1
#define _CLC2GLS3_G4D1N_MASK                                0x1
#define _CLC2GLS3_G4D1T_POSN                                0x1
#define _CLC2GLS3_G4D1T_POSITION                            0x1
#define _CLC2GLS3_G4D1T_SIZE                                0x1
#define _CLC2GLS3_G4D1T_LENGTH                              0x1
#define _CLC2GLS3_G4D1T_MASK                                0x2
#define _CLC2GLS3_G4D2N_POSN                                0x2
#define _CLC2GLS3_G4D2N_POSITION                            0x2
#define _CLC2GLS3_G4D2N_SIZE                                0x1
#define _CLC2GLS3_G4D2N_LENGTH                              0x1
#define _CLC2GLS3_G4D2N_MASK                                0x4
#define _CLC2GLS3_G4D2T_POSN                                0x3
#define _CLC2GLS3_G4D2T_POSITION                            0x3
#define _CLC2GLS3_G4D2T_SIZE                                0x1
#define _CLC2GLS3_G4D2T_LENGTH                              0x1
#define _CLC2GLS3_G4D2T_MASK                                0x8
#define _CLC2GLS3_G4D3N_POSN                                0x4
#define _CLC2GLS3_G4D3N_POSITION                            0x4
#define _CLC2GLS3_G4D3N_SIZE                                0x1
#define _CLC2GLS3_G4D3N_LENGTH                              0x1
#define _CLC2GLS3_G4D3N_MASK                                0x10
#define _CLC2GLS3_G4D3T_POSN                                0x5
#define _CLC2GLS3_G4D3T_POSITION                            0x5
#define _CLC2GLS3_G4D3T_SIZE                                0x1
#define _CLC2GLS3_G4D3T_LENGTH                              0x1
#define _CLC2GLS3_G4D3T_MASK                                0x20
#define _CLC2GLS3_G4D4N_POSN                                0x6
#define _CLC2GLS3_G4D4N_POSITION                            0x6
#define _CLC2GLS3_G4D4N_SIZE                                0x1
#define _CLC2GLS3_G4D4N_LENGTH                              0x1
#define _CLC2GLS3_G4D4N_MASK                                0x40
#define _CLC2GLS3_G4D4T_POSN                                0x7
#define _CLC2GLS3_G4D4T_POSITION                            0x7
#define _CLC2GLS3_G4D4T_SIZE                                0x1
#define _CLC2GLS3_G4D4T_LENGTH                              0x1
#define _CLC2GLS3_G4D4T_MASK                                0x80

// Register: CLC3CON
#define CLC3CON CLC3CON
extern volatile unsigned char           CLC3CON             __at(0xF24);
#ifndef _LIB_BUILD
asm("CLC3CON equ 0F24h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC3MODE                :3;
        unsigned LC3INTN                :1;
        unsigned LC3INTP                :1;
        unsigned LC3OUT                 :1;
        unsigned                        :1;
        unsigned LC3EN                  :1;
    };
    struct {
        unsigned LC3MODE0               :1;
        unsigned LC3MODE1               :1;
        unsigned LC3MODE2               :1;
    };
    struct {
        unsigned MODE                   :3;
        unsigned INTN                   :1;
        unsigned INTP                   :1;
        unsigned OUT                    :1;
        unsigned                        :1;
        unsigned EN                     :1;
    };
    struct {
        unsigned MODE0                  :1;
        unsigned MODE1                  :1;
        unsigned MODE2                  :1;
    };
} CLC3CONbits_t;
extern volatile CLC3CONbits_t CLC3CONbits __at(0xF24);
// bitfield macros
#define _CLC3CON_LC3MODE_POSN                               0x0
#define _CLC3CON_LC3MODE_POSITION                           0x0
#define _CLC3CON_LC3MODE_SIZE                               0x3
#define _CLC3CON_LC3MODE_LENGTH                             0x3
#define _CLC3CON_LC3MODE_MASK                               0x7
#define _CLC3CON_LC3INTN_POSN                               0x3
#define _CLC3CON_LC3INTN_POSITION                           0x3
#define _CLC3CON_LC3INTN_SIZE                               0x1
#define _CLC3CON_LC3INTN_LENGTH                             0x1
#define _CLC3CON_LC3INTN_MASK                               0x8
#define _CLC3CON_LC3INTP_POSN                               0x4
#define _CLC3CON_LC3INTP_POSITION                           0x4
#define _CLC3CON_LC3INTP_SIZE                               0x1
#define _CLC3CON_LC3INTP_LENGTH                             0x1
#define _CLC3CON_LC3INTP_MASK                               0x10
#define _CLC3CON_LC3OUT_POSN                                0x5
#define _CLC3CON_LC3OUT_POSITION                            0x5
#define _CLC3CON_LC3OUT_SIZE                                0x1
#define _CLC3CON_LC3OUT_LENGTH                              0x1
#define _CLC3CON_LC3OUT_MASK                                0x20
#define _CLC3CON_LC3EN_POSN                                 0x7
#define _CLC3CON_LC3EN_POSITION                             0x7
#define _CLC3CON_LC3EN_SIZE                                 0x1
#define _CLC3CON_LC3EN_LENGTH                               0x1
#define _CLC3CON_LC3EN_MASK                                 0x80
#define _CLC3CON_LC3MODE0_POSN                              0x0
#define _CLC3CON_LC3MODE0_POSITION                          0x0
#define _CLC3CON_LC3MODE0_SIZE                              0x1
#define _CLC3CON_LC3MODE0_LENGTH                            0x1
#define _CLC3CON_LC3MODE0_MASK                              0x1
#define _CLC3CON_LC3MODE1_POSN                              0x1
#define _CLC3CON_LC3MODE1_POSITION                          0x1
#define _CLC3CON_LC3MODE1_SIZE                              0x1
#define _CLC3CON_LC3MODE1_LENGTH                            0x1
#define _CLC3CON_LC3MODE1_MASK                              0x2
#define _CLC3CON_LC3MODE2_POSN                              0x2
#define _CLC3CON_LC3MODE2_POSITION                          0x2
#define _CLC3CON_LC3MODE2_SIZE                              0x1
#define _CLC3CON_LC3MODE2_LENGTH                            0x1
#define _CLC3CON_LC3MODE2_MASK                              0x4
#define _CLC3CON_MODE_POSN                                  0x0
#define _CLC3CON_MODE_POSITION                              0x0
#define _CLC3CON_MODE_SIZE                                  0x3
#define _CLC3CON_MODE_LENGTH                                0x3
#define _CLC3CON_MODE_MASK                                  0x7
#define _CLC3CON_INTN_POSN                                  0x3
#define _CLC3CON_INTN_POSITION                              0x3
#define _CLC3CON_INTN_SIZE                                  0x1
#define _CLC3CON_INTN_LENGTH                                0x1
#define _CLC3CON_INTN_MASK                                  0x8
#define _CLC3CON_INTP_POSN                                  0x4
#define _CLC3CON_INTP_POSITION                              0x4
#define _CLC3CON_INTP_SIZE                                  0x1
#define _CLC3CON_INTP_LENGTH                                0x1
#define _CLC3CON_INTP_MASK                                  0x10
#define _CLC3CON_OUT_POSN                                   0x5
#define _CLC3CON_OUT_POSITION                               0x5
#define _CLC3CON_OUT_SIZE                                   0x1
#define _CLC3CON_OUT_LENGTH                                 0x1
#define _CLC3CON_OUT_MASK                                   0x20
#define _CLC3CON_EN_POSN                                    0x7
#define _CLC3CON_EN_POSITION                                0x7
#define _CLC3CON_EN_SIZE                                    0x1
#define _CLC3CON_EN_LENGTH                                  0x1
#define _CLC3CON_EN_MASK                                    0x80
#define _CLC3CON_MODE0_POSN                                 0x0
#define _CLC3CON_MODE0_POSITION                             0x0
#define _CLC3CON_MODE0_SIZE                                 0x1
#define _CLC3CON_MODE0_LENGTH                               0x1
#define _CLC3CON_MODE0_MASK                                 0x1
#define _CLC3CON_MODE1_POSN                                 0x1
#define _CLC3CON_MODE1_POSITION                             0x1
#define _CLC3CON_MODE1_SIZE                                 0x1
#define _CLC3CON_MODE1_LENGTH                               0x1
#define _CLC3CON_MODE1_MASK                                 0x2
#define _CLC3CON_MODE2_POSN                                 0x2
#define _CLC3CON_MODE2_POSITION                             0x2
#define _CLC3CON_MODE2_SIZE                                 0x1
#define _CLC3CON_MODE2_LENGTH                               0x1
#define _CLC3CON_MODE2_MASK                                 0x4

// Register: CLC3POL
#define CLC3POL CLC3POL
extern volatile unsigned char           CLC3POL             __at(0xF25);
#ifndef _LIB_BUILD
asm("CLC3POL equ 0F25h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC3G1POL               :1;
        unsigned LC3G2POL               :1;
        unsigned LC3G3POL               :1;
        unsigned LC3G4POL               :1;
        unsigned                        :3;
        unsigned LC3POL                 :1;
    };
    struct {
        unsigned G1POL                  :1;
        unsigned G2POL                  :1;
        unsigned G3POL                  :1;
        unsigned G4POL                  :1;
        unsigned                        :3;
        unsigned POL                    :1;
    };
} CLC3POLbits_t;
extern volatile CLC3POLbits_t CLC3POLbits __at(0xF25);
// bitfield macros
#define _CLC3POL_LC3G1POL_POSN                              0x0
#define _CLC3POL_LC3G1POL_POSITION                          0x0
#define _CLC3POL_LC3G1POL_SIZE                              0x1
#define _CLC3POL_LC3G1POL_LENGTH                            0x1
#define _CLC3POL_LC3G1POL_MASK                              0x1
#define _CLC3POL_LC3G2POL_POSN                              0x1
#define _CLC3POL_LC3G2POL_POSITION                          0x1
#define _CLC3POL_LC3G2POL_SIZE                              0x1
#define _CLC3POL_LC3G2POL_LENGTH                            0x1
#define _CLC3POL_LC3G2POL_MASK                              0x2
#define _CLC3POL_LC3G3POL_POSN                              0x2
#define _CLC3POL_LC3G3POL_POSITION                          0x2
#define _CLC3POL_LC3G3POL_SIZE                              0x1
#define _CLC3POL_LC3G3POL_LENGTH                            0x1
#define _CLC3POL_LC3G3POL_MASK                              0x4
#define _CLC3POL_LC3G4POL_POSN                              0x3
#define _CLC3POL_LC3G4POL_POSITION                          0x3
#define _CLC3POL_LC3G4POL_SIZE                              0x1
#define _CLC3POL_LC3G4POL_LENGTH                            0x1
#define _CLC3POL_LC3G4POL_MASK                              0x8
#define _CLC3POL_LC3POL_POSN                                0x7
#define _CLC3POL_LC3POL_POSITION                            0x7
#define _CLC3POL_LC3POL_SIZE                                0x1
#define _CLC3POL_LC3POL_LENGTH                              0x1
#define _CLC3POL_LC3POL_MASK                                0x80
#define _CLC3POL_G1POL_POSN                                 0x0
#define _CLC3POL_G1POL_POSITION                             0x0
#define _CLC3POL_G1POL_SIZE                                 0x1
#define _CLC3POL_G1POL_LENGTH                               0x1
#define _CLC3POL_G1POL_MASK                                 0x1
#define _CLC3POL_G2POL_POSN                                 0x1
#define _CLC3POL_G2POL_POSITION                             0x1
#define _CLC3POL_G2POL_SIZE                                 0x1
#define _CLC3POL_G2POL_LENGTH                               0x1
#define _CLC3POL_G2POL_MASK                                 0x2
#define _CLC3POL_G3POL_POSN                                 0x2
#define _CLC3POL_G3POL_POSITION                             0x2
#define _CLC3POL_G3POL_SIZE                                 0x1
#define _CLC3POL_G3POL_LENGTH                               0x1
#define _CLC3POL_G3POL_MASK                                 0x4
#define _CLC3POL_G4POL_POSN                                 0x3
#define _CLC3POL_G4POL_POSITION                             0x3
#define _CLC3POL_G4POL_SIZE                                 0x1
#define _CLC3POL_G4POL_LENGTH                               0x1
#define _CLC3POL_G4POL_MASK                                 0x8
#define _CLC3POL_POL_POSN                                   0x7
#define _CLC3POL_POL_POSITION                               0x7
#define _CLC3POL_POL_SIZE                                   0x1
#define _CLC3POL_POL_LENGTH                                 0x1
#define _CLC3POL_POL_MASK                                   0x80

// Register: CLC3SEL0
#define CLC3SEL0 CLC3SEL0
extern volatile unsigned char           CLC3SEL0            __at(0xF26);
#ifndef _LIB_BUILD
asm("CLC3SEL0 equ 0F26h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC3D1S0                :1;
        unsigned LC3D1S1                :1;
        unsigned LC3D1S2                :1;
        unsigned LC3D1S3                :1;
        unsigned LC3D1S4                :1;
    };
    struct {
        unsigned LC3D1S                 :8;
    };
    struct {
        unsigned D1S                    :8;
    };
    struct {
        unsigned D1S0                   :1;
        unsigned D1S1                   :1;
        unsigned D1S2                   :1;
        unsigned D1S3                   :1;
        unsigned D1S4                   :1;
    };
} CLC3SEL0bits_t;
extern volatile CLC3SEL0bits_t CLC3SEL0bits __at(0xF26);
// bitfield macros
#define _CLC3SEL0_LC3D1S0_POSN                              0x0
#define _CLC3SEL0_LC3D1S0_POSITION                          0x0
#define _CLC3SEL0_LC3D1S0_SIZE                              0x1
#define _CLC3SEL0_LC3D1S0_LENGTH                            0x1
#define _CLC3SEL0_LC3D1S0_MASK                              0x1
#define _CLC3SEL0_LC3D1S1_POSN                              0x1
#define _CLC3SEL0_LC3D1S1_POSITION                          0x1
#define _CLC3SEL0_LC3D1S1_SIZE                              0x1
#define _CLC3SEL0_LC3D1S1_LENGTH                            0x1
#define _CLC3SEL0_LC3D1S1_MASK                              0x2
#define _CLC3SEL0_LC3D1S2_POSN                              0x2
#define _CLC3SEL0_LC3D1S2_POSITION                          0x2
#define _CLC3SEL0_LC3D1S2_SIZE                              0x1
#define _CLC3SEL0_LC3D1S2_LENGTH                            0x1
#define _CLC3SEL0_LC3D1S2_MASK                              0x4
#define _CLC3SEL0_LC3D1S3_POSN                              0x3
#define _CLC3SEL0_LC3D1S3_POSITION                          0x3
#define _CLC3SEL0_LC3D1S3_SIZE                              0x1
#define _CLC3SEL0_LC3D1S3_LENGTH                            0x1
#define _CLC3SEL0_LC3D1S3_MASK                              0x8
#define _CLC3SEL0_LC3D1S4_POSN                              0x4
#define _CLC3SEL0_LC3D1S4_POSITION                          0x4
#define _CLC3SEL0_LC3D1S4_SIZE                              0x1
#define _CLC3SEL0_LC3D1S4_LENGTH                            0x1
#define _CLC3SEL0_LC3D1S4_MASK                              0x10
#define _CLC3SEL0_LC3D1S_POSN                               0x0
#define _CLC3SEL0_LC3D1S_POSITION                           0x0
#define _CLC3SEL0_LC3D1S_SIZE                               0x8
#define _CLC3SEL0_LC3D1S_LENGTH                             0x8
#define _CLC3SEL0_LC3D1S_MASK                               0xFF
#define _CLC3SEL0_D1S_POSN                                  0x0
#define _CLC3SEL0_D1S_POSITION                              0x0
#define _CLC3SEL0_D1S_SIZE                                  0x8
#define _CLC3SEL0_D1S_LENGTH                                0x8
#define _CLC3SEL0_D1S_MASK                                  0xFF
#define _CLC3SEL0_D1S0_POSN                                 0x0
#define _CLC3SEL0_D1S0_POSITION                             0x0
#define _CLC3SEL0_D1S0_SIZE                                 0x1
#define _CLC3SEL0_D1S0_LENGTH                               0x1
#define _CLC3SEL0_D1S0_MASK                                 0x1
#define _CLC3SEL0_D1S1_POSN                                 0x1
#define _CLC3SEL0_D1S1_POSITION                             0x1
#define _CLC3SEL0_D1S1_SIZE                                 0x1
#define _CLC3SEL0_D1S1_LENGTH                               0x1
#define _CLC3SEL0_D1S1_MASK                                 0x2
#define _CLC3SEL0_D1S2_POSN                                 0x2
#define _CLC3SEL0_D1S2_POSITION                             0x2
#define _CLC3SEL0_D1S2_SIZE                                 0x1
#define _CLC3SEL0_D1S2_LENGTH                               0x1
#define _CLC3SEL0_D1S2_MASK                                 0x4
#define _CLC3SEL0_D1S3_POSN                                 0x3
#define _CLC3SEL0_D1S3_POSITION                             0x3
#define _CLC3SEL0_D1S3_SIZE                                 0x1
#define _CLC3SEL0_D1S3_LENGTH                               0x1
#define _CLC3SEL0_D1S3_MASK                                 0x8
#define _CLC3SEL0_D1S4_POSN                                 0x4
#define _CLC3SEL0_D1S4_POSITION                             0x4
#define _CLC3SEL0_D1S4_SIZE                                 0x1
#define _CLC3SEL0_D1S4_LENGTH                               0x1
#define _CLC3SEL0_D1S4_MASK                                 0x10

// Register: CLC3SEL1
#define CLC3SEL1 CLC3SEL1
extern volatile unsigned char           CLC3SEL1            __at(0xF27);
#ifndef _LIB_BUILD
asm("CLC3SEL1 equ 0F27h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC3D2S0                :1;
        unsigned LC3D2S1                :1;
        unsigned LC3D2S2                :1;
        unsigned LC3D2S3                :1;
        unsigned LC3D2S4                :1;
    };
    struct {
        unsigned LC3D2S                 :8;
    };
    struct {
        unsigned D2S                    :8;
    };
    struct {
        unsigned D2S0                   :1;
        unsigned D2S1                   :1;
        unsigned D2S2                   :1;
        unsigned D2S3                   :1;
        unsigned D2S4                   :1;
    };
} CLC3SEL1bits_t;
extern volatile CLC3SEL1bits_t CLC3SEL1bits __at(0xF27);
// bitfield macros
#define _CLC3SEL1_LC3D2S0_POSN                              0x0
#define _CLC3SEL1_LC3D2S0_POSITION                          0x0
#define _CLC3SEL1_LC3D2S0_SIZE                              0x1
#define _CLC3SEL1_LC3D2S0_LENGTH                            0x1
#define _CLC3SEL1_LC3D2S0_MASK                              0x1
#define _CLC3SEL1_LC3D2S1_POSN                              0x1
#define _CLC3SEL1_LC3D2S1_POSITION                          0x1
#define _CLC3SEL1_LC3D2S1_SIZE                              0x1
#define _CLC3SEL1_LC3D2S1_LENGTH                            0x1
#define _CLC3SEL1_LC3D2S1_MASK                              0x2
#define _CLC3SEL1_LC3D2S2_POSN                              0x2
#define _CLC3SEL1_LC3D2S2_POSITION                          0x2
#define _CLC3SEL1_LC3D2S2_SIZE                              0x1
#define _CLC3SEL1_LC3D2S2_LENGTH                            0x1
#define _CLC3SEL1_LC3D2S2_MASK                              0x4
#define _CLC3SEL1_LC3D2S3_POSN                              0x3
#define _CLC3SEL1_LC3D2S3_POSITION                          0x3
#define _CLC3SEL1_LC3D2S3_SIZE                              0x1
#define _CLC3SEL1_LC3D2S3_LENGTH                            0x1
#define _CLC3SEL1_LC3D2S3_MASK                              0x8
#define _CLC3SEL1_LC3D2S4_POSN                              0x4
#define _CLC3SEL1_LC3D2S4_POSITION                          0x4
#define _CLC3SEL1_LC3D2S4_SIZE                              0x1
#define _CLC3SEL1_LC3D2S4_LENGTH                            0x1
#define _CLC3SEL1_LC3D2S4_MASK                              0x10
#define _CLC3SEL1_LC3D2S_POSN                               0x0
#define _CLC3SEL1_LC3D2S_POSITION                           0x0
#define _CLC3SEL1_LC3D2S_SIZE                               0x8
#define _CLC3SEL1_LC3D2S_LENGTH                             0x8
#define _CLC3SEL1_LC3D2S_MASK                               0xFF
#define _CLC3SEL1_D2S_POSN                                  0x0
#define _CLC3SEL1_D2S_POSITION                              0x0
#define _CLC3SEL1_D2S_SIZE                                  0x8
#define _CLC3SEL1_D2S_LENGTH                                0x8
#define _CLC3SEL1_D2S_MASK                                  0xFF
#define _CLC3SEL1_D2S0_POSN                                 0x0
#define _CLC3SEL1_D2S0_POSITION                             0x0
#define _CLC3SEL1_D2S0_SIZE                                 0x1
#define _CLC3SEL1_D2S0_LENGTH                               0x1
#define _CLC3SEL1_D2S0_MASK                                 0x1
#define _CLC3SEL1_D2S1_POSN                                 0x1
#define _CLC3SEL1_D2S1_POSITION                             0x1
#define _CLC3SEL1_D2S1_SIZE                                 0x1
#define _CLC3SEL1_D2S1_LENGTH                               0x1
#define _CLC3SEL1_D2S1_MASK                                 0x2
#define _CLC3SEL1_D2S2_POSN                                 0x2
#define _CLC3SEL1_D2S2_POSITION                             0x2
#define _CLC3SEL1_D2S2_SIZE                                 0x1
#define _CLC3SEL1_D2S2_LENGTH                               0x1
#define _CLC3SEL1_D2S2_MASK                                 0x4
#define _CLC3SEL1_D2S3_POSN                                 0x3
#define _CLC3SEL1_D2S3_POSITION                             0x3
#define _CLC3SEL1_D2S3_SIZE                                 0x1
#define _CLC3SEL1_D2S3_LENGTH                               0x1
#define _CLC3SEL1_D2S3_MASK                                 0x8
#define _CLC3SEL1_D2S4_POSN                                 0x4
#define _CLC3SEL1_D2S4_POSITION                             0x4
#define _CLC3SEL1_D2S4_SIZE                                 0x1
#define _CLC3SEL1_D2S4_LENGTH                               0x1
#define _CLC3SEL1_D2S4_MASK                                 0x10

// Register: CLC3SEL2
#define CLC3SEL2 CLC3SEL2
extern volatile unsigned char           CLC3SEL2            __at(0xF28);
#ifndef _LIB_BUILD
asm("CLC3SEL2 equ 0F28h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC3D3S0                :1;
        unsigned LC3D3S1                :1;
        unsigned LC3D3S2                :1;
        unsigned LC3D3S3                :1;
        unsigned LC3D3S4                :1;
    };
    struct {
        unsigned LC3D3S                 :8;
    };
    struct {
        unsigned D3S                    :8;
    };
    struct {
        unsigned D3S0                   :1;
        unsigned D3S1                   :1;
        unsigned D3S2                   :1;
        unsigned D3S3                   :1;
        unsigned D3S4                   :1;
    };
} CLC3SEL2bits_t;
extern volatile CLC3SEL2bits_t CLC3SEL2bits __at(0xF28);
// bitfield macros
#define _CLC3SEL2_LC3D3S0_POSN                              0x0
#define _CLC3SEL2_LC3D3S0_POSITION                          0x0
#define _CLC3SEL2_LC3D3S0_SIZE                              0x1
#define _CLC3SEL2_LC3D3S0_LENGTH                            0x1
#define _CLC3SEL2_LC3D3S0_MASK                              0x1
#define _CLC3SEL2_LC3D3S1_POSN                              0x1
#define _CLC3SEL2_LC3D3S1_POSITION                          0x1
#define _CLC3SEL2_LC3D3S1_SIZE                              0x1
#define _CLC3SEL2_LC3D3S1_LENGTH                            0x1
#define _CLC3SEL2_LC3D3S1_MASK                              0x2
#define _CLC3SEL2_LC3D3S2_POSN                              0x2
#define _CLC3SEL2_LC3D3S2_POSITION                          0x2
#define _CLC3SEL2_LC3D3S2_SIZE                              0x1
#define _CLC3SEL2_LC3D3S2_LENGTH                            0x1
#define _CLC3SEL2_LC3D3S2_MASK                              0x4
#define _CLC3SEL2_LC3D3S3_POSN                              0x3
#define _CLC3SEL2_LC3D3S3_POSITION                          0x3
#define _CLC3SEL2_LC3D3S3_SIZE                              0x1
#define _CLC3SEL2_LC3D3S3_LENGTH                            0x1
#define _CLC3SEL2_LC3D3S3_MASK                              0x8
#define _CLC3SEL2_LC3D3S4_POSN                              0x4
#define _CLC3SEL2_LC3D3S4_POSITION                          0x4
#define _CLC3SEL2_LC3D3S4_SIZE                              0x1
#define _CLC3SEL2_LC3D3S4_LENGTH                            0x1
#define _CLC3SEL2_LC3D3S4_MASK                              0x10
#define _CLC3SEL2_LC3D3S_POSN                               0x0
#define _CLC3SEL2_LC3D3S_POSITION                           0x0
#define _CLC3SEL2_LC3D3S_SIZE                               0x8
#define _CLC3SEL2_LC3D3S_LENGTH                             0x8
#define _CLC3SEL2_LC3D3S_MASK                               0xFF
#define _CLC3SEL2_D3S_POSN                                  0x0
#define _CLC3SEL2_D3S_POSITION                              0x0
#define _CLC3SEL2_D3S_SIZE                                  0x8
#define _CLC3SEL2_D3S_LENGTH                                0x8
#define _CLC3SEL2_D3S_MASK                                  0xFF
#define _CLC3SEL2_D3S0_POSN                                 0x0
#define _CLC3SEL2_D3S0_POSITION                             0x0
#define _CLC3SEL2_D3S0_SIZE                                 0x1
#define _CLC3SEL2_D3S0_LENGTH                               0x1
#define _CLC3SEL2_D3S0_MASK                                 0x1
#define _CLC3SEL2_D3S1_POSN                                 0x1
#define _CLC3SEL2_D3S1_POSITION                             0x1
#define _CLC3SEL2_D3S1_SIZE                                 0x1
#define _CLC3SEL2_D3S1_LENGTH                               0x1
#define _CLC3SEL2_D3S1_MASK                                 0x2
#define _CLC3SEL2_D3S2_POSN                                 0x2
#define _CLC3SEL2_D3S2_POSITION                             0x2
#define _CLC3SEL2_D3S2_SIZE                                 0x1
#define _CLC3SEL2_D3S2_LENGTH                               0x1
#define _CLC3SEL2_D3S2_MASK                                 0x4
#define _CLC3SEL2_D3S3_POSN                                 0x3
#define _CLC3SEL2_D3S3_POSITION                             0x3
#define _CLC3SEL2_D3S3_SIZE                                 0x1
#define _CLC3SEL2_D3S3_LENGTH                               0x1
#define _CLC3SEL2_D3S3_MASK                                 0x8
#define _CLC3SEL2_D3S4_POSN                                 0x4
#define _CLC3SEL2_D3S4_POSITION                             0x4
#define _CLC3SEL2_D3S4_SIZE                                 0x1
#define _CLC3SEL2_D3S4_LENGTH                               0x1
#define _CLC3SEL2_D3S4_MASK                                 0x10

// Register: CLC3SEL3
#define CLC3SEL3 CLC3SEL3
extern volatile unsigned char           CLC3SEL3            __at(0xF29);
#ifndef _LIB_BUILD
asm("CLC3SEL3 equ 0F29h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC3D4S0                :1;
        unsigned LC3D4S1                :1;
        unsigned LC3D4S2                :1;
        unsigned LC3D4S3                :1;
        unsigned LC3D4S4                :1;
    };
    struct {
        unsigned LC3D4S                 :8;
    };
    struct {
        unsigned D4S                    :8;
    };
    struct {
        unsigned D4S0                   :1;
        unsigned D4S1                   :1;
        unsigned D4S2                   :1;
        unsigned D4S3                   :1;
        unsigned D4S4                   :1;
    };
} CLC3SEL3bits_t;
extern volatile CLC3SEL3bits_t CLC3SEL3bits __at(0xF29);
// bitfield macros
#define _CLC3SEL3_LC3D4S0_POSN                              0x0
#define _CLC3SEL3_LC3D4S0_POSITION                          0x0
#define _CLC3SEL3_LC3D4S0_SIZE                              0x1
#define _CLC3SEL3_LC3D4S0_LENGTH                            0x1
#define _CLC3SEL3_LC3D4S0_MASK                              0x1
#define _CLC3SEL3_LC3D4S1_POSN                              0x1
#define _CLC3SEL3_LC3D4S1_POSITION                          0x1
#define _CLC3SEL3_LC3D4S1_SIZE                              0x1
#define _CLC3SEL3_LC3D4S1_LENGTH                            0x1
#define _CLC3SEL3_LC3D4S1_MASK                              0x2
#define _CLC3SEL3_LC3D4S2_POSN                              0x2
#define _CLC3SEL3_LC3D4S2_POSITION                          0x2
#define _CLC3SEL3_LC3D4S2_SIZE                              0x1
#define _CLC3SEL3_LC3D4S2_LENGTH                            0x1
#define _CLC3SEL3_LC3D4S2_MASK                              0x4
#define _CLC3SEL3_LC3D4S3_POSN                              0x3
#define _CLC3SEL3_LC3D4S3_POSITION                          0x3
#define _CLC3SEL3_LC3D4S3_SIZE                              0x1
#define _CLC3SEL3_LC3D4S3_LENGTH                            0x1
#define _CLC3SEL3_LC3D4S3_MASK                              0x8
#define _CLC3SEL3_LC3D4S4_POSN                              0x4
#define _CLC3SEL3_LC3D4S4_POSITION                          0x4
#define _CLC3SEL3_LC3D4S4_SIZE                              0x1
#define _CLC3SEL3_LC3D4S4_LENGTH                            0x1
#define _CLC3SEL3_LC3D4S4_MASK                              0x10
#define _CLC3SEL3_LC3D4S_POSN                               0x0
#define _CLC3SEL3_LC3D4S_POSITION                           0x0
#define _CLC3SEL3_LC3D4S_SIZE                               0x8
#define _CLC3SEL3_LC3D4S_LENGTH                             0x8
#define _CLC3SEL3_LC3D4S_MASK                               0xFF
#define _CLC3SEL3_D4S_POSN                                  0x0
#define _CLC3SEL3_D4S_POSITION                              0x0
#define _CLC3SEL3_D4S_SIZE                                  0x8
#define _CLC3SEL3_D4S_LENGTH                                0x8
#define _CLC3SEL3_D4S_MASK                                  0xFF
#define _CLC3SEL3_D4S0_POSN                                 0x0
#define _CLC3SEL3_D4S0_POSITION                             0x0
#define _CLC3SEL3_D4S0_SIZE                                 0x1
#define _CLC3SEL3_D4S0_LENGTH                               0x1
#define _CLC3SEL3_D4S0_MASK                                 0x1
#define _CLC3SEL3_D4S1_POSN                                 0x1
#define _CLC3SEL3_D4S1_POSITION                             0x1
#define _CLC3SEL3_D4S1_SIZE                                 0x1
#define _CLC3SEL3_D4S1_LENGTH                               0x1
#define _CLC3SEL3_D4S1_MASK                                 0x2
#define _CLC3SEL3_D4S2_POSN                                 0x2
#define _CLC3SEL3_D4S2_POSITION                             0x2
#define _CLC3SEL3_D4S2_SIZE                                 0x1
#define _CLC3SEL3_D4S2_LENGTH                               0x1
#define _CLC3SEL3_D4S2_MASK                                 0x4
#define _CLC3SEL3_D4S3_POSN                                 0x3
#define _CLC3SEL3_D4S3_POSITION                             0x3
#define _CLC3SEL3_D4S3_SIZE                                 0x1
#define _CLC3SEL3_D4S3_LENGTH                               0x1
#define _CLC3SEL3_D4S3_MASK                                 0x8
#define _CLC3SEL3_D4S4_POSN                                 0x4
#define _CLC3SEL3_D4S4_POSITION                             0x4
#define _CLC3SEL3_D4S4_SIZE                                 0x1
#define _CLC3SEL3_D4S4_LENGTH                               0x1
#define _CLC3SEL3_D4S4_MASK                                 0x10

// Register: CLC3GLS0
#define CLC3GLS0 CLC3GLS0
extern volatile unsigned char           CLC3GLS0            __at(0xF2A);
#ifndef _LIB_BUILD
asm("CLC3GLS0 equ 0F2Ah");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC3G1D1N               :1;
        unsigned LC3G1D1T               :1;
        unsigned LC3G1D2N               :1;
        unsigned LC3G1D2T               :1;
        unsigned LC3G1D3N               :1;
        unsigned LC3G1D3T               :1;
        unsigned LC3G1D4N               :1;
        unsigned LC3G1D4T               :1;
    };
    struct {
        unsigned D1N                    :1;
        unsigned D1T                    :1;
        unsigned D2N                    :1;
        unsigned D2T                    :1;
        unsigned D3N                    :1;
        unsigned D3T                    :1;
        unsigned D4N                    :1;
        unsigned D4T                    :1;
    };
} CLC3GLS0bits_t;
extern volatile CLC3GLS0bits_t CLC3GLS0bits __at(0xF2A);
// bitfield macros
#define _CLC3GLS0_LC3G1D1N_POSN                             0x0
#define _CLC3GLS0_LC3G1D1N_POSITION                         0x0
#define _CLC3GLS0_LC3G1D1N_SIZE                             0x1
#define _CLC3GLS0_LC3G1D1N_LENGTH                           0x1
#define _CLC3GLS0_LC3G1D1N_MASK                             0x1
#define _CLC3GLS0_LC3G1D1T_POSN                             0x1
#define _CLC3GLS0_LC3G1D1T_POSITION                         0x1
#define _CLC3GLS0_LC3G1D1T_SIZE                             0x1
#define _CLC3GLS0_LC3G1D1T_LENGTH                           0x1
#define _CLC3GLS0_LC3G1D1T_MASK                             0x2
#define _CLC3GLS0_LC3G1D2N_POSN                             0x2
#define _CLC3GLS0_LC3G1D2N_POSITION                         0x2
#define _CLC3GLS0_LC3G1D2N_SIZE                             0x1
#define _CLC3GLS0_LC3G1D2N_LENGTH                           0x1
#define _CLC3GLS0_LC3G1D2N_MASK                             0x4
#define _CLC3GLS0_LC3G1D2T_POSN                             0x3
#define _CLC3GLS0_LC3G1D2T_POSITION                         0x3
#define _CLC3GLS0_LC3G1D2T_SIZE                             0x1
#define _CLC3GLS0_LC3G1D2T_LENGTH                           0x1
#define _CLC3GLS0_LC3G1D2T_MASK                             0x8
#define _CLC3GLS0_LC3G1D3N_POSN                             0x4
#define _CLC3GLS0_LC3G1D3N_POSITION                         0x4
#define _CLC3GLS0_LC3G1D3N_SIZE                             0x1
#define _CLC3GLS0_LC3G1D3N_LENGTH                           0x1
#define _CLC3GLS0_LC3G1D3N_MASK                             0x10
#define _CLC3GLS0_LC3G1D3T_POSN                             0x5
#define _CLC3GLS0_LC3G1D3T_POSITION                         0x5
#define _CLC3GLS0_LC3G1D3T_SIZE                             0x1
#define _CLC3GLS0_LC3G1D3T_LENGTH                           0x1
#define _CLC3GLS0_LC3G1D3T_MASK                             0x20
#define _CLC3GLS0_LC3G1D4N_POSN                             0x6
#define _CLC3GLS0_LC3G1D4N_POSITION                         0x6
#define _CLC3GLS0_LC3G1D4N_SIZE                             0x1
#define _CLC3GLS0_LC3G1D4N_LENGTH                           0x1
#define _CLC3GLS0_LC3G1D4N_MASK                             0x40
#define _CLC3GLS0_LC3G1D4T_POSN                             0x7
#define _CLC3GLS0_LC3G1D4T_POSITION                         0x7
#define _CLC3GLS0_LC3G1D4T_SIZE                             0x1
#define _CLC3GLS0_LC3G1D4T_LENGTH                           0x1
#define _CLC3GLS0_LC3G1D4T_MASK                             0x80
#define _CLC3GLS0_D1N_POSN                                  0x0
#define _CLC3GLS0_D1N_POSITION                              0x0
#define _CLC3GLS0_D1N_SIZE                                  0x1
#define _CLC3GLS0_D1N_LENGTH                                0x1
#define _CLC3GLS0_D1N_MASK                                  0x1
#define _CLC3GLS0_D1T_POSN                                  0x1
#define _CLC3GLS0_D1T_POSITION                              0x1
#define _CLC3GLS0_D1T_SIZE                                  0x1
#define _CLC3GLS0_D1T_LENGTH                                0x1
#define _CLC3GLS0_D1T_MASK                                  0x2
#define _CLC3GLS0_D2N_POSN                                  0x2
#define _CLC3GLS0_D2N_POSITION                              0x2
#define _CLC3GLS0_D2N_SIZE                                  0x1
#define _CLC3GLS0_D2N_LENGTH                                0x1
#define _CLC3GLS0_D2N_MASK                                  0x4
#define _CLC3GLS0_D2T_POSN                                  0x3
#define _CLC3GLS0_D2T_POSITION                              0x3
#define _CLC3GLS0_D2T_SIZE                                  0x1
#define _CLC3GLS0_D2T_LENGTH                                0x1
#define _CLC3GLS0_D2T_MASK                                  0x8
#define _CLC3GLS0_D3N_POSN                                  0x4
#define _CLC3GLS0_D3N_POSITION                              0x4
#define _CLC3GLS0_D3N_SIZE                                  0x1
#define _CLC3GLS0_D3N_LENGTH                                0x1
#define _CLC3GLS0_D3N_MASK                                  0x10
#define _CLC3GLS0_D3T_POSN                                  0x5
#define _CLC3GLS0_D3T_POSITION                              0x5
#define _CLC3GLS0_D3T_SIZE                                  0x1
#define _CLC3GLS0_D3T_LENGTH                                0x1
#define _CLC3GLS0_D3T_MASK                                  0x20
#define _CLC3GLS0_D4N_POSN                                  0x6
#define _CLC3GLS0_D4N_POSITION                              0x6
#define _CLC3GLS0_D4N_SIZE                                  0x1
#define _CLC3GLS0_D4N_LENGTH                                0x1
#define _CLC3GLS0_D4N_MASK                                  0x40
#define _CLC3GLS0_D4T_POSN                                  0x7
#define _CLC3GLS0_D4T_POSITION                              0x7
#define _CLC3GLS0_D4T_SIZE                                  0x1
#define _CLC3GLS0_D4T_LENGTH                                0x1
#define _CLC3GLS0_D4T_MASK                                  0x80

// Register: CLC3GLS1
#define CLC3GLS1 CLC3GLS1
extern volatile unsigned char           CLC3GLS1            __at(0xF2B);
#ifndef _LIB_BUILD
asm("CLC3GLS1 equ 0F2Bh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC3G2D1N               :1;
        unsigned LC3G2D1T               :1;
        unsigned LC3G2D2N               :1;
        unsigned LC3G2D2T               :1;
        unsigned LC3G2D3N               :1;
        unsigned LC3G2D3T               :1;
        unsigned LC3G2D4N               :1;
        unsigned LC3G2D4T               :1;
    };
    struct {
        unsigned D1N                    :1;
        unsigned D1T                    :1;
        unsigned D2N                    :1;
        unsigned D2T                    :1;
        unsigned D3N                    :1;
        unsigned D3T                    :1;
        unsigned D4N                    :1;
        unsigned D4T                    :1;
    };
} CLC3GLS1bits_t;
extern volatile CLC3GLS1bits_t CLC3GLS1bits __at(0xF2B);
// bitfield macros
#define _CLC3GLS1_LC3G2D1N_POSN                             0x0
#define _CLC3GLS1_LC3G2D1N_POSITION                         0x0
#define _CLC3GLS1_LC3G2D1N_SIZE                             0x1
#define _CLC3GLS1_LC3G2D1N_LENGTH                           0x1
#define _CLC3GLS1_LC3G2D1N_MASK                             0x1
#define _CLC3GLS1_LC3G2D1T_POSN                             0x1
#define _CLC3GLS1_LC3G2D1T_POSITION                         0x1
#define _CLC3GLS1_LC3G2D1T_SIZE                             0x1
#define _CLC3GLS1_LC3G2D1T_LENGTH                           0x1
#define _CLC3GLS1_LC3G2D1T_MASK                             0x2
#define _CLC3GLS1_LC3G2D2N_POSN                             0x2
#define _CLC3GLS1_LC3G2D2N_POSITION                         0x2
#define _CLC3GLS1_LC3G2D2N_SIZE                             0x1
#define _CLC3GLS1_LC3G2D2N_LENGTH                           0x1
#define _CLC3GLS1_LC3G2D2N_MASK                             0x4
#define _CLC3GLS1_LC3G2D2T_POSN                             0x3
#define _CLC3GLS1_LC3G2D2T_POSITION                         0x3
#define _CLC3GLS1_LC3G2D2T_SIZE                             0x1
#define _CLC3GLS1_LC3G2D2T_LENGTH                           0x1
#define _CLC3GLS1_LC3G2D2T_MASK                             0x8
#define _CLC3GLS1_LC3G2D3N_POSN                             0x4
#define _CLC3GLS1_LC3G2D3N_POSITION                         0x4
#define _CLC3GLS1_LC3G2D3N_SIZE                             0x1
#define _CLC3GLS1_LC3G2D3N_LENGTH                           0x1
#define _CLC3GLS1_LC3G2D3N_MASK                             0x10
#define _CLC3GLS1_LC3G2D3T_POSN                             0x5
#define _CLC3GLS1_LC3G2D3T_POSITION                         0x5
#define _CLC3GLS1_LC3G2D3T_SIZE                             0x1
#define _CLC3GLS1_LC3G2D3T_LENGTH                           0x1
#define _CLC3GLS1_LC3G2D3T_MASK                             0x20
#define _CLC3GLS1_LC3G2D4N_POSN                             0x6
#define _CLC3GLS1_LC3G2D4N_POSITION                         0x6
#define _CLC3GLS1_LC3G2D4N_SIZE                             0x1
#define _CLC3GLS1_LC3G2D4N_LENGTH                           0x1
#define _CLC3GLS1_LC3G2D4N_MASK                             0x40
#define _CLC3GLS1_LC3G2D4T_POSN                             0x7
#define _CLC3GLS1_LC3G2D4T_POSITION                         0x7
#define _CLC3GLS1_LC3G2D4T_SIZE                             0x1
#define _CLC3GLS1_LC3G2D4T_LENGTH                           0x1
#define _CLC3GLS1_LC3G2D4T_MASK                             0x80
#define _CLC3GLS1_D1N_POSN                                  0x0
#define _CLC3GLS1_D1N_POSITION                              0x0
#define _CLC3GLS1_D1N_SIZE                                  0x1
#define _CLC3GLS1_D1N_LENGTH                                0x1
#define _CLC3GLS1_D1N_MASK                                  0x1
#define _CLC3GLS1_D1T_POSN                                  0x1
#define _CLC3GLS1_D1T_POSITION                              0x1
#define _CLC3GLS1_D1T_SIZE                                  0x1
#define _CLC3GLS1_D1T_LENGTH                                0x1
#define _CLC3GLS1_D1T_MASK                                  0x2
#define _CLC3GLS1_D2N_POSN                                  0x2
#define _CLC3GLS1_D2N_POSITION                              0x2
#define _CLC3GLS1_D2N_SIZE                                  0x1
#define _CLC3GLS1_D2N_LENGTH                                0x1
#define _CLC3GLS1_D2N_MASK                                  0x4
#define _CLC3GLS1_D2T_POSN                                  0x3
#define _CLC3GLS1_D2T_POSITION                              0x3
#define _CLC3GLS1_D2T_SIZE                                  0x1
#define _CLC3GLS1_D2T_LENGTH                                0x1
#define _CLC3GLS1_D2T_MASK                                  0x8
#define _CLC3GLS1_D3N_POSN                                  0x4
#define _CLC3GLS1_D3N_POSITION                              0x4
#define _CLC3GLS1_D3N_SIZE                                  0x1
#define _CLC3GLS1_D3N_LENGTH                                0x1
#define _CLC3GLS1_D3N_MASK                                  0x10
#define _CLC3GLS1_D3T_POSN                                  0x5
#define _CLC3GLS1_D3T_POSITION                              0x5
#define _CLC3GLS1_D3T_SIZE                                  0x1
#define _CLC3GLS1_D3T_LENGTH                                0x1
#define _CLC3GLS1_D3T_MASK                                  0x20
#define _CLC3GLS1_D4N_POSN                                  0x6
#define _CLC3GLS1_D4N_POSITION                              0x6
#define _CLC3GLS1_D4N_SIZE                                  0x1
#define _CLC3GLS1_D4N_LENGTH                                0x1
#define _CLC3GLS1_D4N_MASK                                  0x40
#define _CLC3GLS1_D4T_POSN                                  0x7
#define _CLC3GLS1_D4T_POSITION                              0x7
#define _CLC3GLS1_D4T_SIZE                                  0x1
#define _CLC3GLS1_D4T_LENGTH                                0x1
#define _CLC3GLS1_D4T_MASK                                  0x80

// Register: CLC3GLS2
#define CLC3GLS2 CLC3GLS2
extern volatile unsigned char           CLC3GLS2            __at(0xF2C);
#ifndef _LIB_BUILD
asm("CLC3GLS2 equ 0F2Ch");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC3G3D1N               :1;
        unsigned LC3G3D1T               :1;
        unsigned LC3G3D2N               :1;
        unsigned LC3G3D2T               :1;
        unsigned LC3G3D3N               :1;
        unsigned LC3G3D3T               :1;
        unsigned LC3G3D4N               :1;
        unsigned LC3G3D4T               :1;
    };
    struct {
        unsigned D1N                    :1;
        unsigned D1T                    :1;
        unsigned D2N                    :1;
        unsigned D2T                    :1;
        unsigned D3N                    :1;
        unsigned D3T                    :1;
        unsigned D4N                    :1;
        unsigned D4T                    :1;
    };
} CLC3GLS2bits_t;
extern volatile CLC3GLS2bits_t CLC3GLS2bits __at(0xF2C);
// bitfield macros
#define _CLC3GLS2_LC3G3D1N_POSN                             0x0
#define _CLC3GLS2_LC3G3D1N_POSITION                         0x0
#define _CLC3GLS2_LC3G3D1N_SIZE                             0x1
#define _CLC3GLS2_LC3G3D1N_LENGTH                           0x1
#define _CLC3GLS2_LC3G3D1N_MASK                             0x1
#define _CLC3GLS2_LC3G3D1T_POSN                             0x1
#define _CLC3GLS2_LC3G3D1T_POSITION                         0x1
#define _CLC3GLS2_LC3G3D1T_SIZE                             0x1
#define _CLC3GLS2_LC3G3D1T_LENGTH                           0x1
#define _CLC3GLS2_LC3G3D1T_MASK                             0x2
#define _CLC3GLS2_LC3G3D2N_POSN                             0x2
#define _CLC3GLS2_LC3G3D2N_POSITION                         0x2
#define _CLC3GLS2_LC3G3D2N_SIZE                             0x1
#define _CLC3GLS2_LC3G3D2N_LENGTH                           0x1
#define _CLC3GLS2_LC3G3D2N_MASK                             0x4
#define _CLC3GLS2_LC3G3D2T_POSN                             0x3
#define _CLC3GLS2_LC3G3D2T_POSITION                         0x3
#define _CLC3GLS2_LC3G3D2T_SIZE                             0x1
#define _CLC3GLS2_LC3G3D2T_LENGTH                           0x1
#define _CLC3GLS2_LC3G3D2T_MASK                             0x8
#define _CLC3GLS2_LC3G3D3N_POSN                             0x4
#define _CLC3GLS2_LC3G3D3N_POSITION                         0x4
#define _CLC3GLS2_LC3G3D3N_SIZE                             0x1
#define _CLC3GLS2_LC3G3D3N_LENGTH                           0x1
#define _CLC3GLS2_LC3G3D3N_MASK                             0x10
#define _CLC3GLS2_LC3G3D3T_POSN                             0x5
#define _CLC3GLS2_LC3G3D3T_POSITION                         0x5
#define _CLC3GLS2_LC3G3D3T_SIZE                             0x1
#define _CLC3GLS2_LC3G3D3T_LENGTH                           0x1
#define _CLC3GLS2_LC3G3D3T_MASK                             0x20
#define _CLC3GLS2_LC3G3D4N_POSN                             0x6
#define _CLC3GLS2_LC3G3D4N_POSITION                         0x6
#define _CLC3GLS2_LC3G3D4N_SIZE                             0x1
#define _CLC3GLS2_LC3G3D4N_LENGTH                           0x1
#define _CLC3GLS2_LC3G3D4N_MASK                             0x40
#define _CLC3GLS2_LC3G3D4T_POSN                             0x7
#define _CLC3GLS2_LC3G3D4T_POSITION                         0x7
#define _CLC3GLS2_LC3G3D4T_SIZE                             0x1
#define _CLC3GLS2_LC3G3D4T_LENGTH                           0x1
#define _CLC3GLS2_LC3G3D4T_MASK                             0x80
#define _CLC3GLS2_D1N_POSN                                  0x0
#define _CLC3GLS2_D1N_POSITION                              0x0
#define _CLC3GLS2_D1N_SIZE                                  0x1
#define _CLC3GLS2_D1N_LENGTH                                0x1
#define _CLC3GLS2_D1N_MASK                                  0x1
#define _CLC3GLS2_D1T_POSN                                  0x1
#define _CLC3GLS2_D1T_POSITION                              0x1
#define _CLC3GLS2_D1T_SIZE                                  0x1
#define _CLC3GLS2_D1T_LENGTH                                0x1
#define _CLC3GLS2_D1T_MASK                                  0x2
#define _CLC3GLS2_D2N_POSN                                  0x2
#define _CLC3GLS2_D2N_POSITION                              0x2
#define _CLC3GLS2_D2N_SIZE                                  0x1
#define _CLC3GLS2_D2N_LENGTH                                0x1
#define _CLC3GLS2_D2N_MASK                                  0x4
#define _CLC3GLS2_D2T_POSN                                  0x3
#define _CLC3GLS2_D2T_POSITION                              0x3
#define _CLC3GLS2_D2T_SIZE                                  0x1
#define _CLC3GLS2_D2T_LENGTH                                0x1
#define _CLC3GLS2_D2T_MASK                                  0x8
#define _CLC3GLS2_D3N_POSN                                  0x4
#define _CLC3GLS2_D3N_POSITION                              0x4
#define _CLC3GLS2_D3N_SIZE                                  0x1
#define _CLC3GLS2_D3N_LENGTH                                0x1
#define _CLC3GLS2_D3N_MASK                                  0x10
#define _CLC3GLS2_D3T_POSN                                  0x5
#define _CLC3GLS2_D3T_POSITION                              0x5
#define _CLC3GLS2_D3T_SIZE                                  0x1
#define _CLC3GLS2_D3T_LENGTH                                0x1
#define _CLC3GLS2_D3T_MASK                                  0x20
#define _CLC3GLS2_D4N_POSN                                  0x6
#define _CLC3GLS2_D4N_POSITION                              0x6
#define _CLC3GLS2_D4N_SIZE                                  0x1
#define _CLC3GLS2_D4N_LENGTH                                0x1
#define _CLC3GLS2_D4N_MASK                                  0x40
#define _CLC3GLS2_D4T_POSN                                  0x7
#define _CLC3GLS2_D4T_POSITION                              0x7
#define _CLC3GLS2_D4T_SIZE                                  0x1
#define _CLC3GLS2_D4T_LENGTH                                0x1
#define _CLC3GLS2_D4T_MASK                                  0x80

// Register: CLC3GLS3
#define CLC3GLS3 CLC3GLS3
extern volatile unsigned char           CLC3GLS3            __at(0xF2D);
#ifndef _LIB_BUILD
asm("CLC3GLS3 equ 0F2Dh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned LC3G4D1N               :1;
        unsigned LC3G4D1T               :1;
        unsigned LC3G4D2N               :1;
        unsigned LC3G4D2T               :1;
        unsigned LC3G4D3N               :1;
        unsigned LC3G4D3T               :1;
        unsigned LC3G4D4N               :1;
        unsigned LC3G4D4T               :1;
    };
    struct {
        unsigned G4D1N                  :1;
        unsigned G4D1T                  :1;
        unsigned G4D2N                  :1;
        unsigned G4D2T                  :1;
        unsigned G4D3N                  :1;
        unsigned G4D3T                  :1;
        unsigned G4D4N                  :1;
        unsigned G4D4T                  :1;
    };
} CLC3GLS3bits_t;
extern volatile CLC3GLS3bits_t CLC3GLS3bits __at(0xF2D);
// bitfield macros
#define _CLC3GLS3_LC3G4D1N_POSN                             0x0
#define _CLC3GLS3_LC3G4D1N_POSITION                         0x0
#define _CLC3GLS3_LC3G4D1N_SIZE                             0x1
#define _CLC3GLS3_LC3G4D1N_LENGTH                           0x1
#define _CLC3GLS3_LC3G4D1N_MASK                             0x1
#define _CLC3GLS3_LC3G4D1T_POSN                             0x1
#define _CLC3GLS3_LC3G4D1T_POSITION                         0x1
#define _CLC3GLS3_LC3G4D1T_SIZE                             0x1
#define _CLC3GLS3_LC3G4D1T_LENGTH                           0x1
#define _CLC3GLS3_LC3G4D1T_MASK                             0x2
#define _CLC3GLS3_LC3G4D2N_POSN                             0x2
#define _CLC3GLS3_LC3G4D2N_POSITION                         0x2
#define _CLC3GLS3_LC3G4D2N_SIZE                             0x1
#define _CLC3GLS3_LC3G4D2N_LENGTH                           0x1
#define _CLC3GLS3_LC3G4D2N_MASK                             0x4
#define _CLC3GLS3_LC3G4D2T_POSN                             0x3
#define _CLC3GLS3_LC3G4D2T_POSITION                         0x3
#define _CLC3GLS3_LC3G4D2T_SIZE                             0x1
#define _CLC3GLS3_LC3G4D2T_LENGTH                           0x1
#define _CLC3GLS3_LC3G4D2T_MASK                             0x8
#define _CLC3GLS3_LC3G4D3N_POSN                             0x4
#define _CLC3GLS3_LC3G4D3N_POSITION                         0x4
#define _CLC3GLS3_LC3G4D3N_SIZE                             0x1
#define _CLC3GLS3_LC3G4D3N_LENGTH                           0x1
#define _CLC3GLS3_LC3G4D3N_MASK                             0x10
#define _CLC3GLS3_LC3G4D3T_POSN                             0x5
#define _CLC3GLS3_LC3G4D3T_POSITION                         0x5
#define _CLC3GLS3_LC3G4D3T_SIZE                             0x1
#define _CLC3GLS3_LC3G4D3T_LENGTH                           0x1
#define _CLC3GLS3_LC3G4D3T_MASK                             0x20
#define _CLC3GLS3_LC3G4D4N_POSN                             0x6
#define _CLC3GLS3_LC3G4D4N_POSITION                         0x6
#define _CLC3GLS3_LC3G4D4N_SIZE                             0x1
#define _CLC3GLS3_LC3G4D4N_LENGTH                           0x1
#define _CLC3GLS3_LC3G4D4N_MASK                             0x40
#define _CLC3GLS3_LC3G4D4T_POSN                             0x7
#define _CLC3GLS3_LC3G4D4T_POSITION                         0x7
#define _CLC3GLS3_LC3G4D4T_SIZE                             0x1
#define _CLC3GLS3_LC3G4D4T_LENGTH                           0x1
#define _CLC3GLS3_LC3G4D4T_MASK                             0x80
#define _CLC3GLS3_G4D1N_POSN                                0x0
#define _CLC3GLS3_G4D1N_POSITION                            0x0
#define _CLC3GLS3_G4D1N_SIZE                                0x1
#define _CLC3GLS3_G4D1N_LENGTH                              0x1
#define _CLC3GLS3_G4D1N_MASK                                0x1
#define _CLC3GLS3_G4D1T_POSN                                0x1
#define _CLC3GLS3_G4D1T_POSITION                            0x1
#define _CLC3GLS3_G4D1T_SIZE                                0x1
#define _CLC3GLS3_G4D1T_LENGTH                              0x1
#define _CLC3GLS3_G4D1T_MASK                                0x2
#define _CLC3GLS3_G4D2N_POSN                                0x2
#define _CLC3GLS3_G4D2N_POSITION                            0x2
#define _CLC3GLS3_G4D2N_SIZE                                0x1
#define _CLC3GLS3_G4D2N_LENGTH                              0x1
#define _CLC3GLS3_G4D2N_MASK                                0x4
#define _CLC3GLS3_G4D2T_POSN                                0x3
#define _CLC3GLS3_G4D2T_POSITION                            0x3
#define _CLC3GLS3_G4D2T_SIZE                                0x1
#define _CLC3GLS3_G4D2T_LENGTH                              0x1
#define _CLC3GLS3_G4D2T_MASK                                0x8
#define _CLC3GLS3_G4D3N_POSN                                0x4
#define _CLC3GLS3_G4D3N_POSITION                            0x4
#define _CLC3GLS3_G4D3N_SIZE                                0x1
#define _CLC3GLS3_G4D3N_LENGTH                              0x1
#define _CLC3GLS3_G4D3N_MASK                                0x10
#define _CLC3GLS3_G4D3T_POSN                                0x5
#define _CLC3GLS3_G4D3T_POSITION                            0x5
#define _CLC3GLS3_G4D3T_SIZE                                0x1
#define _CLC3GLS3_G4D3T_LENGTH                              0x1
#define _CLC3GLS3_G4D3T_MASK                                0x20
#define _CLC3GLS3_G4D4N_POSN                                0x6
#define _CLC3GLS3_G4D4N_POSITION                            0x6
#define _CLC3GLS3_G4D4N_SIZE                                0x1
#define _CLC3GLS3_G4D4N_LENGTH                              0x1
#define _CLC3GLS3_G4D4N_MASK                                0x40
#define _CLC3GLS3_G4D4T_POSN                                0x7
#define _CLC3GLS3_G4D4T_POSITION                            0x7
#define _CLC3GLS3_G4D4T_SIZE                                0x1
#define _CLC3GLS3_G4D4T_LENGTH                              0x1
#define _CLC3GLS3_G4D4T_MASK                                0x80

// Register: ICDBK0H
#define ICDBK0H ICDBK0H
extern volatile unsigned char           ICDBK0H             __at(0xF9E);
#ifndef _LIB_BUILD
asm("ICDBK0H equ 0F9Eh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned BKA8                   :1;
        unsigned BKA9                   :1;
        unsigned BKA10                  :1;
        unsigned BKA11                  :1;
        unsigned BKA12                  :1;
        unsigned BKA13                  :1;
        unsigned BKA14                  :1;
    };
} ICDBK0Hbits_t;
extern volatile ICDBK0Hbits_t ICDBK0Hbits __at(0xF9E);
// bitfield macros
#define _ICDBK0H_BKA8_POSN                                  0x0
#define _ICDBK0H_BKA8_POSITION                              0x0
#define _ICDBK0H_BKA8_SIZE                                  0x1
#define _ICDBK0H_BKA8_LENGTH                                0x1
#define _ICDBK0H_BKA8_MASK                                  0x1
#define _ICDBK0H_BKA9_POSN                                  0x1
#define _ICDBK0H_BKA9_POSITION                              0x1
#define _ICDBK0H_BKA9_SIZE                                  0x1
#define _ICDBK0H_BKA9_LENGTH                                0x1
#define _ICDBK0H_BKA9_MASK                                  0x2
#define _ICDBK0H_BKA10_POSN                                 0x2
#define _ICDBK0H_BKA10_POSITION                             0x2
#define _ICDBK0H_BKA10_SIZE                                 0x1
#define _ICDBK0H_BKA10_LENGTH                               0x1
#define _ICDBK0H_BKA10_MASK                                 0x4
#define _ICDBK0H_BKA11_POSN                                 0x3
#define _ICDBK0H_BKA11_POSITION                             0x3
#define _ICDBK0H_BKA11_SIZE                                 0x1
#define _ICDBK0H_BKA11_LENGTH                               0x1
#define _ICDBK0H_BKA11_MASK                                 0x8
#define _ICDBK0H_BKA12_POSN                                 0x4
#define _ICDBK0H_BKA12_POSITION                             0x4
#define _ICDBK0H_BKA12_SIZE                                 0x1
#define _ICDBK0H_BKA12_LENGTH                               0x1
#define _ICDBK0H_BKA12_MASK                                 0x10
#define _ICDBK0H_BKA13_POSN                                 0x5
#define _ICDBK0H_BKA13_POSITION                             0x5
#define _ICDBK0H_BKA13_SIZE                                 0x1
#define _ICDBK0H_BKA13_LENGTH                               0x1
#define _ICDBK0H_BKA13_MASK                                 0x20
#define _ICDBK0H_BKA14_POSN                                 0x6
#define _ICDBK0H_BKA14_POSITION                             0x6
#define _ICDBK0H_BKA14_SIZE                                 0x1
#define _ICDBK0H_BKA14_LENGTH                               0x1
#define _ICDBK0H_BKA14_MASK                                 0x40

// Register: STATUS_SHAD
#define STATUS_SHAD STATUS_SHAD
extern volatile unsigned char           STATUS_SHAD         __at(0xFE4);
#ifndef _LIB_BUILD
asm("STATUS_SHAD equ 0FE4h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned C_SHAD                 :1;
        unsigned DC_SHAD                :1;
        unsigned Z_SHAD                 :1;
    };
} STATUS_SHADbits_t;
extern volatile STATUS_SHADbits_t STATUS_SHADbits __at(0xFE4);
// bitfield macros
#define _STATUS_SHAD_C_SHAD_POSN                            0x0
#define _STATUS_SHAD_C_SHAD_POSITION                        0x0
#define _STATUS_SHAD_C_SHAD_SIZE                            0x1
#define _STATUS_SHAD_C_SHAD_LENGTH                          0x1
#define _STATUS_SHAD_C_SHAD_MASK                            0x1
#define _STATUS_SHAD_DC_SHAD_POSN                           0x1
#define _STATUS_SHAD_DC_SHAD_POSITION                       0x1
#define _STATUS_SHAD_DC_SHAD_SIZE                           0x1
#define _STATUS_SHAD_DC_SHAD_LENGTH                         0x1
#define _STATUS_SHAD_DC_SHAD_MASK                           0x2
#define _STATUS_SHAD_Z_SHAD_POSN                            0x2
#define _STATUS_SHAD_Z_SHAD_POSITION                        0x2
#define _STATUS_SHAD_Z_SHAD_SIZE                            0x1
#define _STATUS_SHAD_Z_SHAD_LENGTH                          0x1
#define _STATUS_SHAD_Z_SHAD_MASK                            0x4

// Register: WREG_SHAD
#define WREG_SHAD WREG_SHAD
extern volatile unsigned char           WREG_SHAD           __at(0xFE5);
#ifndef _LIB_BUILD
asm("WREG_SHAD equ 0FE5h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned WREG_SHAD              :8;
    };
} WREG_SHADbits_t;
extern volatile WREG_SHADbits_t WREG_SHADbits __at(0xFE5);
// bitfield macros
#define _WREG_SHAD_WREG_SHAD_POSN                           0x0
#define _WREG_SHAD_WREG_SHAD_POSITION                       0x0
#define _WREG_SHAD_WREG_SHAD_SIZE                           0x8
#define _WREG_SHAD_WREG_SHAD_LENGTH                         0x8
#define _WREG_SHAD_WREG_SHAD_MASK                           0xFF

// Register: BSR_SHAD
#define BSR_SHAD BSR_SHAD
extern volatile unsigned char           BSR_SHAD            __at(0xFE6);
#ifndef _LIB_BUILD
asm("BSR_SHAD equ 0FE6h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned BSR_SHAD               :5;
    };
} BSR_SHADbits_t;
extern volatile BSR_SHADbits_t BSR_SHADbits __at(0xFE6);
// bitfield macros
#define _BSR_SHAD_BSR_SHAD_POSN                             0x0
#define _BSR_SHAD_BSR_SHAD_POSITION                         0x0
#define _BSR_SHAD_BSR_SHAD_SIZE                             0x5
#define _BSR_SHAD_BSR_SHAD_LENGTH                           0x5
#define _BSR_SHAD_BSR_SHAD_MASK                             0x1F

// Register: PCLATH_SHAD
#define PCLATH_SHAD PCLATH_SHAD
extern volatile unsigned char           PCLATH_SHAD         __at(0xFE7);
#ifndef _LIB_BUILD
asm("PCLATH_SHAD equ 0FE7h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PCLATH_SHAD            :7;
    };
} PCLATH_SHADbits_t;
extern volatile PCLATH_SHADbits_t PCLATH_SHADbits __at(0xFE7);
// bitfield macros
#define _PCLATH_SHAD_PCLATH_SHAD_POSN                       0x0
#define _PCLATH_SHAD_PCLATH_SHAD_POSITION                   0x0
#define _PCLATH_SHAD_PCLATH_SHAD_SIZE                       0x7
#define _PCLATH_SHAD_PCLATH_SHAD_LENGTH                     0x7
#define _PCLATH_SHAD_PCLATH_SHAD_MASK                       0x7F

// Register: FSR0L_SHAD
#define FSR0L_SHAD FSR0L_SHAD
extern volatile unsigned char           FSR0L_SHAD          __at(0xFE8);
#ifndef _LIB_BUILD
asm("FSR0L_SHAD equ 0FE8h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned FSR0L_SHAD             :8;
    };
} FSR0L_SHADbits_t;
extern volatile FSR0L_SHADbits_t FSR0L_SHADbits __at(0xFE8);
// bitfield macros
#define _FSR0L_SHAD_FSR0L_SHAD_POSN                         0x0
#define _FSR0L_SHAD_FSR0L_SHAD_POSITION                     0x0
#define _FSR0L_SHAD_FSR0L_SHAD_SIZE                         0x8
#define _FSR0L_SHAD_FSR0L_SHAD_LENGTH                       0x8
#define _FSR0L_SHAD_FSR0L_SHAD_MASK                         0xFF

// Register: FSR0H_SHAD
#define FSR0H_SHAD FSR0H_SHAD
extern volatile unsigned char           FSR0H_SHAD          __at(0xFE9);
#ifndef _LIB_BUILD
asm("FSR0H_SHAD equ 0FE9h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned FSR0H_SHAD             :8;
    };
} FSR0H_SHADbits_t;
extern volatile FSR0H_SHADbits_t FSR0H_SHADbits __at(0xFE9);
// bitfield macros
#define _FSR0H_SHAD_FSR0H_SHAD_POSN                         0x0
#define _FSR0H_SHAD_FSR0H_SHAD_POSITION                     0x0
#define _FSR0H_SHAD_FSR0H_SHAD_SIZE                         0x8
#define _FSR0H_SHAD_FSR0H_SHAD_LENGTH                       0x8
#define _FSR0H_SHAD_FSR0H_SHAD_MASK                         0xFF

// Register: FSR1L_SHAD
#define FSR1L_SHAD FSR1L_SHAD
extern volatile unsigned char           FSR1L_SHAD          __at(0xFEA);
#ifndef _LIB_BUILD
asm("FSR1L_SHAD equ 0FEAh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned FSR1L_SHAD             :8;
    };
} FSR1L_SHADbits_t;
extern volatile FSR1L_SHADbits_t FSR1L_SHADbits __at(0xFEA);
// bitfield macros
#define _FSR1L_SHAD_FSR1L_SHAD_POSN                         0x0
#define _FSR1L_SHAD_FSR1L_SHAD_POSITION                     0x0
#define _FSR1L_SHAD_FSR1L_SHAD_SIZE                         0x8
#define _FSR1L_SHAD_FSR1L_SHAD_LENGTH                       0x8
#define _FSR1L_SHAD_FSR1L_SHAD_MASK                         0xFF

// Register: FSR1H_SHAD
#define FSR1H_SHAD FSR1H_SHAD
extern volatile unsigned char           FSR1H_SHAD          __at(0xFEB);
#ifndef _LIB_BUILD
asm("FSR1H_SHAD equ 0FEBh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned FSR1H_SHAD             :8;
    };
} FSR1H_SHADbits_t;
extern volatile FSR1H_SHADbits_t FSR1H_SHADbits __at(0xFEB);
// bitfield macros
#define _FSR1H_SHAD_FSR1H_SHAD_POSN                         0x0
#define _FSR1H_SHAD_FSR1H_SHAD_POSITION                     0x0
#define _FSR1H_SHAD_FSR1H_SHAD_SIZE                         0x8
#define _FSR1H_SHAD_FSR1H_SHAD_LENGTH                       0x8
#define _FSR1H_SHAD_FSR1H_SHAD_MASK                         0xFF

// Register: STKPTR
#define STKPTR STKPTR
extern volatile unsigned char           STKPTR              __at(0xFED);
#ifndef _LIB_BUILD
asm("STKPTR equ 0FEDh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned STKPTR                 :5;
    };
} STKPTRbits_t;
extern volatile STKPTRbits_t STKPTRbits __at(0xFED);
// bitfield macros
#define _STKPTR_STKPTR_POSN                                 0x0
#define _STKPTR_STKPTR_POSITION                             0x0
#define _STKPTR_STKPTR_SIZE                                 0x5
#define _STKPTR_STKPTR_LENGTH                               0x5
#define _STKPTR_STKPTR_MASK                                 0x1F

// Register: TOSL
#define TOSL TOSL
extern volatile unsigned char           TOSL                __at(0xFEE);
#ifndef _LIB_BUILD
asm("TOSL equ 0FEEh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TOSL                   :8;
    };
} TOSLbits_t;
extern volatile TOSLbits_t TOSLbits __at(0xFEE);
// bitfield macros
#define _TOSL_TOSL_POSN                                     0x0
#define _TOSL_TOSL_POSITION                                 0x0
#define _TOSL_TOSL_SIZE                                     0x8
#define _TOSL_TOSL_LENGTH                                   0x8
#define _TOSL_TOSL_MASK                                     0xFF

// Register: TOSH
#define TOSH TOSH
extern volatile unsigned char           TOSH                __at(0xFEF);
#ifndef _LIB_BUILD
asm("TOSH equ 0FEFh");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned TOSH                   :7;
    };
} TOSHbits_t;
extern volatile TOSHbits_t TOSHbits __at(0xFEF);
// bitfield macros
#define _TOSH_TOSH_POSN                                     0x0
#define _TOSH_TOSH_POSITION                                 0x0
#define _TOSH_TOSH_SIZE                                     0x7
#define _TOSH_TOSH_LENGTH                                   0x7
#define _TOSH_TOSH_MASK                                     0x7F

/*
 * Bit Definitions
 */
#define _DEPRECATED __attribute__((__deprecated__))
#ifndef BANKMASK
#define BANKMASK(addr) ((addr)&07Fh)
#endif
// BAUD1CON<ABDEN>
extern volatile __bit                   ABDEN               __at(0xCF8);	// @ (0x19F * 8 + 0)
#define                                 ABDEN_bit           BANKMASK(BAUD1CON), 0
// BAUD1CON<ABDOVF>
extern volatile __bit                   ABDOVF              __at(0xCFF);	// @ (0x19F * 8 + 7)
#define                                 ABDOVF_bit          BANKMASK(BAUD1CON), 7
// SSP1CON2<ACKDT>
extern volatile __bit                   ACKDT               __at(0x10B5);	// @ (0x216 * 8 + 5)
#define                                 ACKDT_bit           BANKMASK(SSP1CON2), 5
// SSP1CON2<ACKEN>
extern volatile __bit                   ACKEN               __at(0x10B4);	// @ (0x216 * 8 + 4)
#define                                 ACKEN_bit           BANKMASK(SSP1CON2), 4
// SSP1CON2<ACKSTAT>
extern volatile __bit                   ACKSTAT             __at(0x10B6);	// @ (0x216 * 8 + 6)
#define                                 ACKSTAT_bit         BANKMASK(SSP1CON2), 6
// SSP1CON3<ACKTIM>
extern volatile __bit                   ACKTIM              __at(0x10BF);	// @ (0x217 * 8 + 7)
#define                                 ACKTIM_bit          BANKMASK(SSP1CON3), 7
// SSP1ADD<ADD0>
extern volatile __bit                   ADD0                __at(0x1090);	// @ (0x212 * 8 + 0)
#define                                 ADD0_bit            BANKMASK(SSP1ADD), 0
// SSP1ADD<ADD1>
extern volatile __bit                   ADD1                __at(0x1091);	// @ (0x212 * 8 + 1)
#define                                 ADD1_bit            BANKMASK(SSP1ADD), 1
// SSP1ADD<ADD2>
extern volatile __bit                   ADD2                __at(0x1092);	// @ (0x212 * 8 + 2)
#define                                 ADD2_bit            BANKMASK(SSP1ADD), 2
// SSP1ADD<ADD3>
extern volatile __bit                   ADD3                __at(0x1093);	// @ (0x212 * 8 + 3)
#define                                 ADD3_bit            BANKMASK(SSP1ADD), 3
// SSP1ADD<ADD4>
extern volatile __bit                   ADD4                __at(0x1094);	// @ (0x212 * 8 + 4)
#define                                 ADD4_bit            BANKMASK(SSP1ADD), 4
// SSP1ADD<ADD5>
extern volatile __bit                   ADD5                __at(0x1095);	// @ (0x212 * 8 + 5)
#define                                 ADD5_bit            BANKMASK(SSP1ADD), 5
// SSP1ADD<ADD6>
extern volatile __bit                   ADD6                __at(0x1096);	// @ (0x212 * 8 + 6)
#define                                 ADD6_bit            BANKMASK(SSP1ADD), 6
// SSP1ADD<ADD7>
extern volatile __bit                   ADD7                __at(0x1097);	// @ (0x212 * 8 + 7)
#define                                 ADD7_bit            BANKMASK(SSP1ADD), 7
// RC1STA<ADDEN>
extern volatile __bit                   ADDEN               __at(0xCEB);	// @ (0x19D * 8 + 3)
#define                                 ADDEN_bit           BANKMASK(RC1STA), 3
// ADCON1<ADFM>
extern volatile __bit                   ADFM                __at(0x4F7);	// @ (0x9E * 8 + 7)
#define                                 ADFM_bit            BANKMASK(ADCON1), 7
// FVRCON<ADFVR0>
extern volatile __bit                   ADFVR0              __at(0x8B8);	// @ (0x117 * 8 + 0)
#define                                 ADFVR0_bit          BANKMASK(FVRCON), 0
// FVRCON<ADFVR1>
extern volatile __bit                   ADFVR1              __at(0x8B9);	// @ (0x117 * 8 + 1)
#define                                 ADFVR1_bit          BANKMASK(FVRCON), 1
// ADCON0<ADGO>
extern volatile __bit                   ADGO                __at(0x4E9);	// @ (0x9D * 8 + 1)
#define                                 ADGO_bit            BANKMASK(ADCON0), 1
// PIE1<ADIE>
extern volatile __bit                   ADIE                __at(0x48E);	// @ (0x91 * 8 + 6)
#define                                 ADIE_bit            BANKMASK(PIE1), 6
// PIR1<ADIF>
extern volatile __bit                   ADIF                __at(0x8E);	// @ (0x11 * 8 + 6)
#define                                 ADIF_bit            BANKMASK(PIR1), 6
// ADCON1<ADNREF>
extern volatile __bit                   ADNREF              __at(0x4F2);	// @ (0x9E * 8 + 2)
#define                                 ADNREF_bit          BANKMASK(ADCON1), 2
// ADCON0<ADON>
extern volatile __bit                   ADON                __at(0x4E8);	// @ (0x9D * 8 + 0)
#define                                 ADON_bit            BANKMASK(ADCON0), 0
// ADCON1<ADPREF0>
extern volatile __bit                   ADPREF0             __at(0x4F0);	// @ (0x9E * 8 + 0)
#define                                 ADPREF0_bit         BANKMASK(ADCON1), 0
// ADCON1<ADPREF1>
extern volatile __bit                   ADPREF1             __at(0x4F1);	// @ (0x9E * 8 + 1)
#define                                 ADPREF1_bit         BANKMASK(ADCON1), 1
// SSP1CON3<AHEN>
extern volatile __bit                   AHEN                __at(0x10B9);	// @ (0x217 * 8 + 1)
#define                                 AHEN_bit            BANKMASK(SSP1CON3), 1
// ANSELA<ANS5>
extern volatile __bit                   ANS5                __at(0xC65);	// @ (0x18C * 8 + 5)
#define                                 ANS5_bit            BANKMASK(ANSELA), 5
// ANSELA<ANSA0>
extern volatile __bit                   ANSA0               __at(0xC60);	// @ (0x18C * 8 + 0)
#define                                 ANSA0_bit           BANKMASK(ANSELA), 0
// ANSELA<ANSA1>
extern volatile __bit                   ANSA1               __at(0xC61);	// @ (0x18C * 8 + 1)
#define                                 ANSA1_bit           BANKMASK(ANSELA), 1
// ANSELA<ANSA2>
extern volatile __bit                   ANSA2               __at(0xC62);	// @ (0x18C * 8 + 2)
#define                                 ANSA2_bit           BANKMASK(ANSELA), 2
// ANSELA<ANSA4>
extern volatile __bit                   ANSA4               __at(0xC64);	// @ (0x18C * 8 + 4)
#define                                 ANSA4_bit           BANKMASK(ANSELA), 4
// ANSELB<ANSB4>
extern volatile __bit                   ANSB4               __at(0xC6C);	// @ (0x18D * 8 + 4)
#define                                 ANSB4_bit           BANKMASK(ANSELB), 4
// ANSELB<ANSB5>
extern volatile __bit                   ANSB5               __at(0xC6D);	// @ (0x18D * 8 + 5)
#define                                 ANSB5_bit           BANKMASK(ANSELB), 5
// ANSELC<ANSC0>
extern volatile __bit                   ANSC0               __at(0xC70);	// @ (0x18E * 8 + 0)
#define                                 ANSC0_bit           BANKMASK(ANSELC), 0
// ANSELC<ANSC1>
extern volatile __bit                   ANSC1               __at(0xC71);	// @ (0x18E * 8 + 1)
#define                                 ANSC1_bit           BANKMASK(ANSELC), 1
// ANSELC<ANSC2>
extern volatile __bit                   ANSC2               __at(0xC72);	// @ (0x18E * 8 + 2)
#define                                 ANSC2_bit           BANKMASK(ANSELC), 2
// ANSELC<ANSC3>
extern volatile __bit                   ANSC3               __at(0xC73);	// @ (0x18E * 8 + 3)
#define                                 ANSC3_bit           BANKMASK(ANSELC), 3
// ANSELC<ANSC6>
extern volatile __bit                   ANSC6               __at(0xC76);	// @ (0x18E * 8 + 6)
#define                                 ANSC6_bit           BANKMASK(ANSELC), 6
// ANSELC<ANSC7>
extern volatile __bit                   ANSC7               __at(0xC77);	// @ (0x18E * 8 + 7)
#define                                 ANSC7_bit           BANKMASK(ANSELC), 7
// PIE2<BCL1IE>
extern volatile __bit                   BCL1IE              __at(0x493);	// @ (0x92 * 8 + 3)
#define                                 BCL1IE_bit          BANKMASK(PIE2), 3
// PIR2<BCL1IF>
extern volatile __bit                   BCL1IF              __at(0x93);	// @ (0x12 * 8 + 3)
#define                                 BCL1IF_bit          BANKMASK(PIR2), 3
// SSP1STAT<BF>
extern volatile __bit                   BF                  __at(0x10A0);	// @ (0x214 * 8 + 0)
#define                                 BF_bit              BANKMASK(SSP1STAT), 0
// ICDBK0H<BKA10>
extern volatile __bit                   BKA10               __at(0x7CF2);	// @ (0xF9E * 8 + 2)
#define                                 BKA10_bit           BANKMASK(ICDBK0H), 2
// ICDBK0H<BKA11>
extern volatile __bit                   BKA11               __at(0x7CF3);	// @ (0xF9E * 8 + 3)
#define                                 BKA11_bit           BANKMASK(ICDBK0H), 3
// ICDBK0H<BKA12>
extern volatile __bit                   BKA12               __at(0x7CF4);	// @ (0xF9E * 8 + 4)
#define                                 BKA12_bit           BANKMASK(ICDBK0H), 4
// ICDBK0H<BKA13>
extern volatile __bit                   BKA13               __at(0x7CF5);	// @ (0xF9E * 8 + 5)
#define                                 BKA13_bit           BANKMASK(ICDBK0H), 5
// ICDBK0H<BKA14>
extern volatile __bit                   BKA14               __at(0x7CF6);	// @ (0xF9E * 8 + 6)
#define                                 BKA14_bit           BANKMASK(ICDBK0H), 6
// ICDBK0H<BKA8>
extern volatile __bit                   BKA8                __at(0x7CF0);	// @ (0xF9E * 8 + 0)
#define                                 BKA8_bit            BANKMASK(ICDBK0H), 0
// ICDBK0H<BKA9>
extern volatile __bit                   BKA9                __at(0x7CF1);	// @ (0xF9E * 8 + 1)
#define                                 BKA9_bit            BANKMASK(ICDBK0H), 1
// SSP1CON3<BOEN>
extern volatile __bit                   BOEN                __at(0x10BC);	// @ (0x217 * 8 + 4)
#define                                 BOEN_bit            BANKMASK(SSP1CON3), 4
// BORCON<BORFS>
extern volatile __bit                   BORFS               __at(0x8B6);	// @ (0x116 * 8 + 6)
#define                                 BORFS_bit           BANKMASK(BORCON), 6
// BORCON<BORRDY>
extern volatile __bit                   BORRDY              __at(0x8B0);	// @ (0x116 * 8 + 0)
#define                                 BORRDY_bit          BANKMASK(BORCON), 0
// BAUD1CON<BRG16>
extern volatile __bit                   BRG16               __at(0xCFB);	// @ (0x19F * 8 + 3)
#define                                 BRG16_bit           BANKMASK(BAUD1CON), 3
// TX1STA<BRGH>
extern volatile __bit                   BRGH                __at(0xCF2);	// @ (0x19E * 8 + 2)
#define                                 BRGH_bit            BANKMASK(TX1STA), 2
// BSR<BSR0>
extern volatile __bit                   BSR0                __at(0x40);	// @ (0x8 * 8 + 0)
#define                                 BSR0_bit            BANKMASK(BSR), 0
// BSR<BSR1>
extern volatile __bit                   BSR1                __at(0x41);	// @ (0x8 * 8 + 1)
#define                                 BSR1_bit            BANKMASK(BSR), 1
// BSR<BSR2>
extern volatile __bit                   BSR2                __at(0x42);	// @ (0x8 * 8 + 2)
#define                                 BSR2_bit            BANKMASK(BSR), 2
// BSR<BSR3>
extern volatile __bit                   BSR3                __at(0x43);	// @ (0x8 * 8 + 3)
#define                                 BSR3_bit            BANKMASK(BSR), 3
// BSR<BSR4>
extern volatile __bit                   BSR4                __at(0x44);	// @ (0x8 * 8 + 4)
#define                                 BSR4_bit            BANKMASK(BSR), 4
// SSP1BUF<BUF0>
extern volatile __bit                   BUF0                __at(0x1088);	// @ (0x211 * 8 + 0)
#define                                 BUF0_bit            BANKMASK(SSP1BUF), 0
// SSP1BUF<BUF1>
extern volatile __bit                   BUF1                __at(0x1089);	// @ (0x211 * 8 + 1)
#define                                 BUF1_bit            BANKMASK(SSP1BUF), 1
// SSP1BUF<BUF2>
extern volatile __bit                   BUF2                __at(0x108A);	// @ (0x211 * 8 + 2)
#define                                 BUF2_bit            BANKMASK(SSP1BUF), 2
// SSP1BUF<BUF3>
extern volatile __bit                   BUF3                __at(0x108B);	// @ (0x211 * 8 + 3)
#define                                 BUF3_bit            BANKMASK(SSP1BUF), 3
// SSP1BUF<BUF4>
extern volatile __bit                   BUF4                __at(0x108C);	// @ (0x211 * 8 + 4)
#define                                 BUF4_bit            BANKMASK(SSP1BUF), 4
// SSP1BUF<BUF5>
extern volatile __bit                   BUF5                __at(0x108D);	// @ (0x211 * 8 + 5)
#define                                 BUF5_bit            BANKMASK(SSP1BUF), 5
// SSP1BUF<BUF6>
extern volatile __bit                   BUF6                __at(0x108E);	// @ (0x211 * 8 + 6)
#define                                 BUF6_bit            BANKMASK(SSP1BUF), 6
// SSP1BUF<BUF7>
extern volatile __bit                   BUF7                __at(0x108F);	// @ (0x211 * 8 + 7)
#define                                 BUF7_bit            BANKMASK(SSP1BUF), 7
// CM1CON0<C1HYS>
extern volatile __bit                   C1HYS               __at(0x889);	// @ (0x111 * 8 + 1)
#define                                 C1HYS_bit           BANKMASK(CM1CON0), 1
// PIE2<C1IE>
extern volatile __bit                   C1IE                __at(0x495);	// @ (0x92 * 8 + 5)
#define                                 C1IE_bit            BANKMASK(PIE2), 5
// PIR2<C1IF>
extern volatile __bit                   C1IF                __at(0x95);	// @ (0x12 * 8 + 5)
#define                                 C1IF_bit            BANKMASK(PIR2), 5
// CM1CON1<C1INTN>
extern volatile __bit                   C1INTN              __at(0x896);	// @ (0x112 * 8 + 6)
#define                                 C1INTN_bit          BANKMASK(CM1CON1), 6
// CM1CON1<C1INTP>
extern volatile __bit                   C1INTP              __at(0x897);	// @ (0x112 * 8 + 7)
#define                                 C1INTP_bit          BANKMASK(CM1CON1), 7
// CM1CON1<C1NCH0>
extern volatile __bit                   C1NCH0              __at(0x890);	// @ (0x112 * 8 + 0)
#define                                 C1NCH0_bit          BANKMASK(CM1CON1), 0
// CM1CON1<C1NCH1>
extern volatile __bit                   C1NCH1              __at(0x891);	// @ (0x112 * 8 + 1)
#define                                 C1NCH1_bit          BANKMASK(CM1CON1), 1
// CM1CON1<C1NCH2>
extern volatile __bit                   C1NCH2              __at(0x892);	// @ (0x112 * 8 + 2)
#define                                 C1NCH2_bit          BANKMASK(CM1CON1), 2
// CM1CON0<C1ON>
extern volatile __bit                   C1ON                __at(0x88F);	// @ (0x111 * 8 + 7)
#define                                 C1ON_bit            BANKMASK(CM1CON0), 7
// CM1CON0<C1OUT>
extern volatile __bit                   C1OUT               __at(0x88E);	// @ (0x111 * 8 + 6)
#define                                 C1OUT_bit           BANKMASK(CM1CON0), 6
// CM1CON1<C1PCH0>
extern volatile __bit                   C1PCH0              __at(0x893);	// @ (0x112 * 8 + 3)
#define                                 C1PCH0_bit          BANKMASK(CM1CON1), 3
// CM1CON1<C1PCH1>
extern volatile __bit                   C1PCH1              __at(0x894);	// @ (0x112 * 8 + 4)
#define                                 C1PCH1_bit          BANKMASK(CM1CON1), 4
// CM1CON1<C1PCH2>
extern volatile __bit                   C1PCH2              __at(0x895);	// @ (0x112 * 8 + 5)
#define                                 C1PCH2_bit          BANKMASK(CM1CON1), 5
// CM1CON0<C1POL>
extern volatile __bit                   C1POL               __at(0x88C);	// @ (0x111 * 8 + 4)
#define                                 C1POL_bit           BANKMASK(CM1CON0), 4
// CM1CON0<C1SP>
extern volatile __bit                   C1SP                __at(0x88A);	// @ (0x111 * 8 + 2)
#define                                 C1SP_bit            BANKMASK(CM1CON0), 2
// CM1CON0<C1SYNC>
extern volatile __bit                   C1SYNC              __at(0x888);	// @ (0x111 * 8 + 0)
#define                                 C1SYNC_bit          BANKMASK(CM1CON0), 0
// CCPTMRS<C1TSEL0>
extern volatile __bit                   C1TSEL0             __at(0x14F0);	// @ (0x29E * 8 + 0)
#define                                 C1TSEL0_bit         BANKMASK(CCPTMRS), 0
// CCPTMRS<C1TSEL1>
extern volatile __bit                   C1TSEL1             __at(0x14F1);	// @ (0x29E * 8 + 1)
#define                                 C1TSEL1_bit         BANKMASK(CCPTMRS), 1
// CM1CON0<C1ZLF>
extern volatile __bit                   C1ZLF               __at(0x88B);	// @ (0x111 * 8 + 3)
#define                                 C1ZLF_bit           BANKMASK(CM1CON0), 3
// CM2CON0<C2HYS>
extern volatile __bit                   C2HYS               __at(0x899);	// @ (0x113 * 8 + 1)
#define                                 C2HYS_bit           BANKMASK(CM2CON0), 1
// PIE2<C2IE>
extern volatile __bit                   C2IE                __at(0x496);	// @ (0x92 * 8 + 6)
#define                                 C2IE_bit            BANKMASK(PIE2), 6
// PIR2<C2IF>
extern volatile __bit                   C2IF                __at(0x96);	// @ (0x12 * 8 + 6)
#define                                 C2IF_bit            BANKMASK(PIR2), 6
// CM2CON1<C2INTN>
extern volatile __bit                   C2INTN              __at(0x8A6);	// @ (0x114 * 8 + 6)
#define                                 C2INTN_bit          BANKMASK(CM2CON1), 6
// CM2CON1<C2INTP>
extern volatile __bit                   C2INTP              __at(0x8A7);	// @ (0x114 * 8 + 7)
#define                                 C2INTP_bit          BANKMASK(CM2CON1), 7
// CM2CON1<C2NCH0>
extern volatile __bit                   C2NCH0              __at(0x8A0);	// @ (0x114 * 8 + 0)
#define                                 C2NCH0_bit          BANKMASK(CM2CON1), 0
// CM2CON1<C2NCH1>
extern volatile __bit                   C2NCH1              __at(0x8A1);	// @ (0x114 * 8 + 1)
#define                                 C2NCH1_bit          BANKMASK(CM2CON1), 1
// CM2CON1<C2NCH2>
extern volatile __bit                   C2NCH2              __at(0x8A2);	// @ (0x114 * 8 + 2)
#define                                 C2NCH2_bit          BANKMASK(CM2CON1), 2
// CM2CON0<C2ON>
extern volatile __bit                   C2ON                __at(0x89F);	// @ (0x113 * 8 + 7)
#define                                 C2ON_bit            BANKMASK(CM2CON0), 7
// CM2CON0<C2OUT>
extern volatile __bit                   C2OUT               __at(0x89E);	// @ (0x113 * 8 + 6)
#define                                 C2OUT_bit           BANKMASK(CM2CON0), 6
// CM2CON1<C2PCH0>
extern volatile __bit                   C2PCH0              __at(0x8A3);	// @ (0x114 * 8 + 3)
#define                                 C2PCH0_bit          BANKMASK(CM2CON1), 3
// CM2CON1<C2PCH1>
extern volatile __bit                   C2PCH1              __at(0x8A4);	// @ (0x114 * 8 + 4)
#define                                 C2PCH1_bit          BANKMASK(CM2CON1), 4
// CM2CON1<C2PCH2>
extern volatile __bit                   C2PCH2              __at(0x8A5);	// @ (0x114 * 8 + 5)
#define                                 C2PCH2_bit          BANKMASK(CM2CON1), 5
// CM2CON0<C2POL>
extern volatile __bit                   C2POL               __at(0x89C);	// @ (0x113 * 8 + 4)
#define                                 C2POL_bit           BANKMASK(CM2CON0), 4
// CM2CON0<C2SP>
extern volatile __bit                   C2SP                __at(0x89A);	// @ (0x113 * 8 + 2)
#define                                 C2SP_bit            BANKMASK(CM2CON0), 2
// CM2CON0<C2SYNC>
extern volatile __bit                   C2SYNC              __at(0x898);	// @ (0x113 * 8 + 0)
#define                                 C2SYNC_bit          BANKMASK(CM2CON0), 0
// CCPTMRS<C2TSEL0>
extern volatile __bit                   C2TSEL0             __at(0x14F2);	// @ (0x29E * 8 + 2)
#define                                 C2TSEL0_bit         BANKMASK(CCPTMRS), 2
// CCPTMRS<C2TSEL1>
extern volatile __bit                   C2TSEL1             __at(0x14F3);	// @ (0x29E * 8 + 3)
#define                                 C2TSEL1_bit         BANKMASK(CCPTMRS), 3
// CM2CON0<C2ZLF>
extern volatile __bit                   C2ZLF               __at(0x89B);	// @ (0x113 * 8 + 3)
#define                                 C2ZLF_bit           BANKMASK(CM2CON0), 3
// STATUS<CARRY>
extern volatile __bit                   CARRY               __at(0x18);	// @ (0x3 * 8 + 0)
#define                                 CARRY_bit           BANKMASK(STATUS), 0
// PIE1<CCP1IE>
extern volatile __bit                   CCP1IE              __at(0x48A);	// @ (0x91 * 8 + 2)
#define                                 CCP1IE_bit          BANKMASK(PIE1), 2
// PIR1<CCP1IF>
extern volatile __bit                   CCP1IF              __at(0x8A);	// @ (0x11 * 8 + 2)
#define                                 CCP1IF_bit          BANKMASK(PIR1), 2
// CCP1CON<CCP1M0>
extern volatile __bit                   CCP1M0              __at(0x1498);	// @ (0x293 * 8 + 0)
#define                                 CCP1M0_bit          BANKMASK(CCP1CON), 0
// CCP1CON<CCP1M1>
extern volatile __bit                   CCP1M1              __at(0x1499);	// @ (0x293 * 8 + 1)
#define                                 CCP1M1_bit          BANKMASK(CCP1CON), 1
// CCP1CON<CCP1M2>
extern volatile __bit                   CCP1M2              __at(0x149A);	// @ (0x293 * 8 + 2)
#define                                 CCP1M2_bit          BANKMASK(CCP1CON), 2
// CCP1CON<CCP1M3>
extern volatile __bit                   CCP1M3              __at(0x149B);	// @ (0x293 * 8 + 3)
#define                                 CCP1M3_bit          BANKMASK(CCP1CON), 3
// CCP1CON<CCP1X>
extern volatile __bit                   CCP1X               __at(0x149D);	// @ (0x293 * 8 + 5)
#define                                 CCP1X_bit           BANKMASK(CCP1CON), 5
// CCP1CON<CCP1Y>
extern volatile __bit                   CCP1Y               __at(0x149C);	// @ (0x293 * 8 + 4)
#define                                 CCP1Y_bit           BANKMASK(CCP1CON), 4
// PIE2<CCP2IE>
extern volatile __bit                   CCP2IE              __at(0x490);	// @ (0x92 * 8 + 0)
#define                                 CCP2IE_bit          BANKMASK(PIE2), 0
// PIR2<CCP2IF>
extern volatile __bit                   CCP2IF              __at(0x90);	// @ (0x12 * 8 + 0)
#define                                 CCP2IF_bit          BANKMASK(PIR2), 0
// CCP2CON<CCP2M0>
extern volatile __bit                   CCP2M0              __at(0x14D0);	// @ (0x29A * 8 + 0)
#define                                 CCP2M0_bit          BANKMASK(CCP2CON), 0
// CCP2CON<CCP2M1>
extern volatile __bit                   CCP2M1              __at(0x14D1);	// @ (0x29A * 8 + 1)
#define                                 CCP2M1_bit          BANKMASK(CCP2CON), 1
// CCP2CON<CCP2M2>
extern volatile __bit                   CCP2M2              __at(0x14D2);	// @ (0x29A * 8 + 2)
#define                                 CCP2M2_bit          BANKMASK(CCP2CON), 2
// CCP2CON<CCP2M3>
extern volatile __bit                   CCP2M3              __at(0x14D3);	// @ (0x29A * 8 + 3)
#define                                 CCP2M3_bit          BANKMASK(CCP2CON), 3
// CCP2CON<CCP2X>
extern volatile __bit                   CCP2X               __at(0x14D5);	// @ (0x29A * 8 + 5)
#define                                 CCP2X_bit           BANKMASK(CCP2CON), 5
// CCP2CON<CCP2Y>
extern volatile __bit                   CCP2Y               __at(0x14D4);	// @ (0x29A * 8 + 4)
#define                                 CCP2Y_bit           BANKMASK(CCP2CON), 4
// PIE1<CCPIE>
extern volatile __bit                   CCPIE               __at(0x48A);	// @ (0x91 * 8 + 2)
#define                                 CCPIE_bit           BANKMASK(PIE1), 2
// PIR1<CCPIF>
extern volatile __bit                   CCPIF               __at(0x8A);	// @ (0x11 * 8 + 2)
#define                                 CCPIF_bit           BANKMASK(PIR1), 2
// FVRCON<CDAFVR0>
extern volatile __bit                   CDAFVR0             __at(0x8BA);	// @ (0x117 * 8 + 2)
#define                                 CDAFVR0_bit         BANKMASK(FVRCON), 2
// FVRCON<CDAFVR1>
extern volatile __bit                   CDAFVR1             __at(0x8BB);	// @ (0x117 * 8 + 3)
#define                                 CDAFVR1_bit         BANKMASK(FVRCON), 3
// PMCON1<CFGS>
extern volatile __bit                   CFGS                __at(0xCAE);	// @ (0x195 * 8 + 6)
#define                                 CFGS_bit            BANKMASK(PMCON1), 6
// ADCON0<CHS0>
extern volatile __bit                   CHS0                __at(0x4EA);	// @ (0x9D * 8 + 2)
#define                                 CHS0_bit            BANKMASK(ADCON0), 2
// ADCON0<CHS1>
extern volatile __bit                   CHS1                __at(0x4EB);	// @ (0x9D * 8 + 3)
#define                                 CHS1_bit            BANKMASK(ADCON0), 3
// ADCON0<CHS2>
extern volatile __bit                   CHS2                __at(0x4EC);	// @ (0x9D * 8 + 4)
#define                                 CHS2_bit            BANKMASK(ADCON0), 4
// ADCON0<CHS3>
extern volatile __bit                   CHS3                __at(0x4ED);	// @ (0x9D * 8 + 5)
#define                                 CHS3_bit            BANKMASK(ADCON0), 5
// ADCON0<CHS4>
extern volatile __bit                   CHS4                __at(0x4EE);	// @ (0x9D * 8 + 6)
#define                                 CHS4_bit            BANKMASK(ADCON0), 6
// SSP1STAT<CKE>
extern volatile __bit                   CKE                 __at(0x10A6);	// @ (0x214 * 8 + 6)
#define                                 CKE_bit             BANKMASK(SSP1STAT), 6
// SSP1CON1<CKP>
extern volatile __bit                   CKP                 __at(0x10AC);	// @ (0x215 * 8 + 4)
#define                                 CKP_bit             BANKMASK(SSP1CON1), 4
// PIE3<CLC1IE>
extern volatile __bit                   CLC1IE              __at(0x498);	// @ (0x93 * 8 + 0)
#define                                 CLC1IE_bit          BANKMASK(PIE3), 0
// PIR3<CLC1IF>
extern volatile __bit                   CLC1IF              __at(0x98);	// @ (0x13 * 8 + 0)
#define                                 CLC1IF_bit          BANKMASK(PIR3), 0
// PIE3<CLC2IE>
extern volatile __bit                   CLC2IE              __at(0x499);	// @ (0x93 * 8 + 1)
#define                                 CLC2IE_bit          BANKMASK(PIE3), 1
// PIR3<CLC2IF>
extern volatile __bit                   CLC2IF              __at(0x99);	// @ (0x13 * 8 + 1)
#define                                 CLC2IF_bit          BANKMASK(PIR3), 1
// PIE3<CLC3IE>
extern volatile __bit                   CLC3IE              __at(0x49A);	// @ (0x93 * 8 + 2)
#define                                 CLC3IE_bit          BANKMASK(PIE3), 2
// PIR3<CLC3IF>
extern volatile __bit                   CLC3IF              __at(0x9A);	// @ (0x13 * 8 + 2)
#define                                 CLC3IF_bit          BANKMASK(PIR3), 2
// PIE3<COGIE>
extern volatile __bit                   COGIE               __at(0x49D);	// @ (0x93 * 8 + 5)
#define                                 COGIE_bit           BANKMASK(PIE3), 5
// PIR3<COGIF>
extern volatile __bit                   COGIF               __at(0x9D);	// @ (0x13 * 8 + 5)
#define                                 COGIF_bit           BANKMASK(PIR3), 5
// RC1STA<CREN>
extern volatile __bit                   CREN                __at(0xCEC);	// @ (0x19D * 8 + 4)
#define                                 CREN_bit            BANKMASK(RC1STA), 4
// TX1STA<CSRC>
extern volatile __bit                   CSRC                __at(0xCF7);	// @ (0x19E * 8 + 7)
#define                                 CSRC_bit            BANKMASK(TX1STA), 7
// STATUS_SHAD<C_SHAD>
extern volatile __bit                   C_SHAD              __at(0x7F20);	// @ (0xFE4 * 8 + 0)
#define                                 C_SHAD_bit          BANKMASK(STATUS_SHAD), 0
// DAC1CON0<DAC1EN>
extern volatile __bit                   DAC1EN              __at(0x8C7);	// @ (0x118 * 8 + 7)
#define                                 DAC1EN_bit          BANKMASK(DAC1CON0), 7
// DAC1CON0<DAC1NSS>
extern volatile __bit                   DAC1NSS             __at(0x8C0);	// @ (0x118 * 8 + 0)
#define                                 DAC1NSS_bit         BANKMASK(DAC1CON0), 0
// DAC1CON0<DAC1OE1>
extern volatile __bit                   DAC1OE1             __at(0x8C5);	// @ (0x118 * 8 + 5)
#define                                 DAC1OE1_bit         BANKMASK(DAC1CON0), 5
// DAC1CON0<DAC1OE2>
extern volatile __bit                   DAC1OE2             __at(0x8C4);	// @ (0x118 * 8 + 4)
#define                                 DAC1OE2_bit         BANKMASK(DAC1CON0), 4
// DAC1CON0<DAC1PSS0>
extern volatile __bit                   DAC1PSS0            __at(0x8C2);	// @ (0x118 * 8 + 2)
#define                                 DAC1PSS0_bit        BANKMASK(DAC1CON0), 2
// DAC1CON0<DAC1PSS1>
extern volatile __bit                   DAC1PSS1            __at(0x8C3);	// @ (0x118 * 8 + 3)
#define                                 DAC1PSS1_bit        BANKMASK(DAC1CON0), 3
// DAC1CON1<DAC1R0>
extern volatile __bit                   DAC1R0              __at(0x8C8);	// @ (0x119 * 8 + 0)
#define                                 DAC1R0_bit          BANKMASK(DAC1CON1), 0
// DAC1CON1<DAC1R1>
extern volatile __bit                   DAC1R1              __at(0x8C9);	// @ (0x119 * 8 + 1)
#define                                 DAC1R1_bit          BANKMASK(DAC1CON1), 1
// DAC1CON1<DAC1R2>
extern volatile __bit                   DAC1R2              __at(0x8CA);	// @ (0x119 * 8 + 2)
#define                                 DAC1R2_bit          BANKMASK(DAC1CON1), 2
// DAC1CON1<DAC1R3>
extern volatile __bit                   DAC1R3              __at(0x8CB);	// @ (0x119 * 8 + 3)
#define                                 DAC1R3_bit          BANKMASK(DAC1CON1), 3
// DAC1CON1<DAC1R4>
extern volatile __bit                   DAC1R4              __at(0x8CC);	// @ (0x119 * 8 + 4)
#define                                 DAC1R4_bit          BANKMASK(DAC1CON1), 4
// DAC1CON1<DAC1R5>
extern volatile __bit                   DAC1R5              __at(0x8CD);	// @ (0x119 * 8 + 5)
#define                                 DAC1R5_bit          BANKMASK(DAC1CON1), 5
// DAC1CON1<DAC1R6>
extern volatile __bit                   DAC1R6              __at(0x8CE);	// @ (0x119 * 8 + 6)
#define                                 DAC1R6_bit          BANKMASK(DAC1CON1), 6
// DAC1CON1<DAC1R7>
extern volatile __bit                   DAC1R7              __at(0x8CF);	// @ (0x119 * 8 + 7)
#define                                 DAC1R7_bit          BANKMASK(DAC1CON1), 7
// DAC1CON0<DACEN>
extern volatile __bit                   DACEN               __at(0x8C7);	// @ (0x118 * 8 + 7)
#define                                 DACEN_bit           BANKMASK(DAC1CON0), 7
// DAC1CON0<DACNSS>
extern volatile __bit                   DACNSS              __at(0x8C0);	// @ (0x118 * 8 + 0)
#define                                 DACNSS_bit          BANKMASK(DAC1CON0), 0
// DAC1CON0<DACOE0>
extern volatile __bit                   DACOE0              __at(0x8C4);	// @ (0x118 * 8 + 4)
#define                                 DACOE0_bit          BANKMASK(DAC1CON0), 4
// DAC1CON0<DACOE1>
extern volatile __bit                   DACOE1              __at(0x8C5);	// @ (0x118 * 8 + 5)
#define                                 DACOE1_bit          BANKMASK(DAC1CON0), 5
// DAC1CON0<DACPSS0>
extern volatile __bit                   DACPSS0             __at(0x8C2);	// @ (0x118 * 8 + 2)
#define                                 DACPSS0_bit         BANKMASK(DAC1CON0), 2
// DAC1CON0<DACPSS1>
extern volatile __bit                   DACPSS1             __at(0x8C3);	// @ (0x118 * 8 + 3)
#define                                 DACPSS1_bit         BANKMASK(DAC1CON0), 3
// DAC1CON1<DACR0>
extern volatile __bit                   DACR0               __at(0x8C8);	// @ (0x119 * 8 + 0)
#define                                 DACR0_bit           BANKMASK(DAC1CON1), 0
// DAC1CON1<DACR1>
extern volatile __bit                   DACR1               __at(0x8C9);	// @ (0x119 * 8 + 1)
#define                                 DACR1_bit           BANKMASK(DAC1CON1), 1
// DAC1CON1<DACR2>
extern volatile __bit                   DACR2               __at(0x8CA);	// @ (0x119 * 8 + 2)
#define                                 DACR2_bit           BANKMASK(DAC1CON1), 2
// DAC1CON1<DACR3>
extern volatile __bit                   DACR3               __at(0x8CB);	// @ (0x119 * 8 + 3)
#define                                 DACR3_bit           BANKMASK(DAC1CON1), 3
// DAC1CON1<DACR4>
extern volatile __bit                   DACR4               __at(0x8CC);	// @ (0x119 * 8 + 4)
#define                                 DACR4_bit           BANKMASK(DAC1CON1), 4
// DAC1CON1<DACR5>
extern volatile __bit                   DACR5               __at(0x8CD);	// @ (0x119 * 8 + 5)
#define                                 DACR5_bit           BANKMASK(DAC1CON1), 5
// DAC1CON1<DACR6>
extern volatile __bit                   DACR6               __at(0x8CE);	// @ (0x119 * 8 + 6)
#define                                 DACR6_bit           BANKMASK(DAC1CON1), 6
// DAC1CON1<DACR7>
extern volatile __bit                   DACR7               __at(0x8CF);	// @ (0x119 * 8 + 7)
#define                                 DACR7_bit           BANKMASK(DAC1CON1), 7
// STATUS<DC>
extern volatile __bit                   DC                  __at(0x19);	// @ (0x3 * 8 + 1)
#define                                 DC_bit              BANKMASK(STATUS), 1
// CCP1CON<DC1B0>
extern volatile __bit                   DC1B0               __at(0x149C);	// @ (0x293 * 8 + 4)
#define                                 DC1B0_bit           BANKMASK(CCP1CON), 4
// CCP1CON<DC1B1>
extern volatile __bit                   DC1B1               __at(0x149D);	// @ (0x293 * 8 + 5)
#define                                 DC1B1_bit           BANKMASK(CCP1CON), 5
// CCP2CON<DC2B0>
extern volatile __bit                   DC2B0               __at(0x14D4);	// @ (0x29A * 8 + 4)
#define                                 DC2B0_bit           BANKMASK(CCP2CON), 4
// CCP2CON<DC2B1>
extern volatile __bit                   DC2B1               __at(0x14D5);	// @ (0x29A * 8 + 5)
#define                                 DC2B1_bit           BANKMASK(CCP2CON), 5
// STATUS_SHAD<DC_SHAD>
extern volatile __bit                   DC_SHAD             __at(0x7F21);	// @ (0xFE4 * 8 + 1)
#define                                 DC_SHAD_bit         BANKMASK(STATUS_SHAD), 1
// SSP1CON3<DHEN>
extern volatile __bit                   DHEN                __at(0x10B8);	// @ (0x217 * 8 + 0)
#define                                 DHEN_bit            BANKMASK(SSP1CON3), 0
// SSP1STAT<D_nA>
extern volatile __bit                   D_nA                __at(0x10A5);	// @ (0x214 * 8 + 5)
#define                                 D_nA_bit            BANKMASK(SSP1STAT), 5
// RC1STA<FERR>
extern volatile __bit                   FERR                __at(0xCEA);	// @ (0x19D * 8 + 2)
#define                                 FERR_bit            BANKMASK(RC1STA), 2
// PMCON1<FREE>
extern volatile __bit                   FREE                __at(0xCAC);	// @ (0x195 * 8 + 4)
#define                                 FREE_bit            BANKMASK(PMCON1), 4
// FVRCON<FVREN>
extern volatile __bit                   FVREN               __at(0x8BF);	// @ (0x117 * 8 + 7)
#define                                 FVREN_bit           BANKMASK(FVRCON), 7
// FVRCON<FVRRDY>
extern volatile __bit                   FVRRDY              __at(0x8BE);	// @ (0x117 * 8 + 6)
#define                                 FVRRDY_bit          BANKMASK(FVRCON), 6
// COG1ASD0<G1ARSEN>
extern volatile __bit                   G1ARSEN             __at(0x34EE);	// @ (0x69D * 8 + 6)
#define                                 G1ARSEN_bit         BANKMASK(COG1ASD0), 6
// COG1ASD1<G1AS0E>
extern volatile __bit                   G1AS0E              __at(0x34F0);	// @ (0x69E * 8 + 0)
#define                                 G1AS0E_bit          BANKMASK(COG1ASD1), 0
// COG1ASD1<G1AS1E>
extern volatile __bit                   G1AS1E              __at(0x34F1);	// @ (0x69E * 8 + 1)
#define                                 G1AS1E_bit          BANKMASK(COG1ASD1), 1
// COG1ASD1<G1AS2E>
extern volatile __bit                   G1AS2E              __at(0x34F2);	// @ (0x69E * 8 + 2)
#define                                 G1AS2E_bit          BANKMASK(COG1ASD1), 2
// COG1ASD1<G1AS3E>
extern volatile __bit                   G1AS3E              __at(0x34F3);	// @ (0x69E * 8 + 3)
#define                                 G1AS3E_bit          BANKMASK(COG1ASD1), 3
// COG1ASD0<G1ASDAC0>
extern volatile __bit                   G1ASDAC0            __at(0x34EA);	// @ (0x69D * 8 + 2)
#define                                 G1ASDAC0_bit        BANKMASK(COG1ASD0), 2
// COG1ASD0<G1ASDAC1>
extern volatile __bit                   G1ASDAC1            __at(0x34EB);	// @ (0x69D * 8 + 3)
#define                                 G1ASDAC1_bit        BANKMASK(COG1ASD0), 3
// COG1ASD0<G1ASDBD0>
extern volatile __bit                   G1ASDBD0            __at(0x34EC);	// @ (0x69D * 8 + 4)
#define                                 G1ASDBD0_bit        BANKMASK(COG1ASD0), 4
// COG1ASD0<G1ASDBD1>
extern volatile __bit                   G1ASDBD1            __at(0x34ED);	// @ (0x69D * 8 + 5)
#define                                 G1ASDBD1_bit        BANKMASK(COG1ASD0), 5
// COG1ASD0<G1ASE>
extern volatile __bit                   G1ASE               __at(0x34EF);	// @ (0x69D * 8 + 7)
#define                                 G1ASE_bit           BANKMASK(COG1ASD0), 7
// COG1BLKF<G1BLKF0>
extern volatile __bit                   G1BLKF0             __at(0x34A0);	// @ (0x694 * 8 + 0)
#define                                 G1BLKF0_bit         BANKMASK(COG1BLKF), 0
// COG1BLKF<G1BLKF1>
extern volatile __bit                   G1BLKF1             __at(0x34A1);	// @ (0x694 * 8 + 1)
#define                                 G1BLKF1_bit         BANKMASK(COG1BLKF), 1
// COG1BLKF<G1BLKF2>
extern volatile __bit                   G1BLKF2             __at(0x34A2);	// @ (0x694 * 8 + 2)
#define                                 G1BLKF2_bit         BANKMASK(COG1BLKF), 2
// COG1BLKF<G1BLKF3>
extern volatile __bit                   G1BLKF3             __at(0x34A3);	// @ (0x694 * 8 + 3)
#define                                 G1BLKF3_bit         BANKMASK(COG1BLKF), 3
// COG1BLKF<G1BLKF4>
extern volatile __bit                   G1BLKF4             __at(0x34A4);	// @ (0x694 * 8 + 4)
#define                                 G1BLKF4_bit         BANKMASK(COG1BLKF), 4
// COG1BLKF<G1BLKF5>
extern volatile __bit                   G1BLKF5             __at(0x34A5);	// @ (0x694 * 8 + 5)
#define                                 G1BLKF5_bit         BANKMASK(COG1BLKF), 5
// COG1BLKR<G1BLKR0>
extern volatile __bit                   G1BLKR0             __at(0x3498);	// @ (0x693 * 8 + 0)
#define                                 G1BLKR0_bit         BANKMASK(COG1BLKR), 0
// COG1BLKR<G1BLKR1>
extern volatile __bit                   G1BLKR1             __at(0x3499);	// @ (0x693 * 8 + 1)
#define                                 G1BLKR1_bit         BANKMASK(COG1BLKR), 1
// COG1BLKR<G1BLKR2>
extern volatile __bit                   G1BLKR2             __at(0x349A);	// @ (0x693 * 8 + 2)
#define                                 G1BLKR2_bit         BANKMASK(COG1BLKR), 2
// COG1BLKR<G1BLKR3>
extern volatile __bit                   G1BLKR3             __at(0x349B);	// @ (0x693 * 8 + 3)
#define                                 G1BLKR3_bit         BANKMASK(COG1BLKR), 3
// COG1BLKR<G1BLKR4>
extern volatile __bit                   G1BLKR4             __at(0x349C);	// @ (0x693 * 8 + 4)
#define                                 G1BLKR4_bit         BANKMASK(COG1BLKR), 4
// COG1BLKR<G1BLKR5>
extern volatile __bit                   G1BLKR5             __at(0x349D);	// @ (0x693 * 8 + 5)
#define                                 G1BLKR5_bit         BANKMASK(COG1BLKR), 5
// COG1CON0<G1CS0>
extern volatile __bit                   G1CS0               __at(0x34BB);	// @ (0x697 * 8 + 3)
#define                                 G1CS0_bit           BANKMASK(COG1CON0), 3
// COG1CON0<G1CS1>
extern volatile __bit                   G1CS1               __at(0x34BC);	// @ (0x697 * 8 + 4)
#define                                 G1CS1_bit           BANKMASK(COG1CON0), 4
// COG1DBF<G1DBF0>
extern volatile __bit                   G1DBF0              __at(0x34B0);	// @ (0x696 * 8 + 0)
#define                                 G1DBF0_bit          BANKMASK(COG1DBF), 0
// COG1DBF<G1DBF1>
extern volatile __bit                   G1DBF1              __at(0x34B1);	// @ (0x696 * 8 + 1)
#define                                 G1DBF1_bit          BANKMASK(COG1DBF), 1
// COG1DBF<G1DBF2>
extern volatile __bit                   G1DBF2              __at(0x34B2);	// @ (0x696 * 8 + 2)
#define                                 G1DBF2_bit          BANKMASK(COG1DBF), 2
// COG1DBF<G1DBF3>
extern volatile __bit                   G1DBF3              __at(0x34B3);	// @ (0x696 * 8 + 3)
#define                                 G1DBF3_bit          BANKMASK(COG1DBF), 3
// COG1DBF<G1DBF4>
extern volatile __bit                   G1DBF4              __at(0x34B4);	// @ (0x696 * 8 + 4)
#define                                 G1DBF4_bit          BANKMASK(COG1DBF), 4
// COG1DBF<G1DBF5>
extern volatile __bit                   G1DBF5              __at(0x34B5);	// @ (0x696 * 8 + 5)
#define                                 G1DBF5_bit          BANKMASK(COG1DBF), 5
// COG1DBR<G1DBR0>
extern volatile __bit                   G1DBR0              __at(0x34A8);	// @ (0x695 * 8 + 0)
#define                                 G1DBR0_bit          BANKMASK(COG1DBR), 0
// COG1DBR<G1DBR1>
extern volatile __bit                   G1DBR1              __at(0x34A9);	// @ (0x695 * 8 + 1)
#define                                 G1DBR1_bit          BANKMASK(COG1DBR), 1
// COG1DBR<G1DBR2>
extern volatile __bit                   G1DBR2              __at(0x34AA);	// @ (0x695 * 8 + 2)
#define                                 G1DBR2_bit          BANKMASK(COG1DBR), 2
// COG1DBR<G1DBR3>
extern volatile __bit                   G1DBR3              __at(0x34AB);	// @ (0x695 * 8 + 3)
#define                                 G1DBR3_bit          BANKMASK(COG1DBR), 3
// COG1DBR<G1DBR4>
extern volatile __bit                   G1DBR4              __at(0x34AC);	// @ (0x695 * 8 + 4)
#define                                 G1DBR4_bit          BANKMASK(COG1DBR), 4
// COG1DBR<G1DBR5>
extern volatile __bit                   G1DBR5              __at(0x34AD);	// @ (0x695 * 8 + 5)
#define                                 G1DBR5_bit          BANKMASK(COG1DBR), 5
// COG1CON0<G1EN>
extern volatile __bit                   G1EN                __at(0x34BF);	// @ (0x697 * 8 + 7)
#define                                 G1EN_bit            BANKMASK(COG1CON0), 7
// COG1CON1<G1FDBS>
extern volatile __bit                   G1FDBS              __at(0x34C6);	// @ (0x698 * 8 + 6)
#define                                 G1FDBS_bit          BANKMASK(COG1CON1), 6
// COG1FIS<G1FIS0>
extern volatile __bit                   G1FIS0              __at(0x34D8);	// @ (0x69B * 8 + 0)
#define                                 G1FIS0_bit          BANKMASK(COG1FIS), 0
// COG1FIS<G1FIS1>
extern volatile __bit                   G1FIS1              __at(0x34D9);	// @ (0x69B * 8 + 1)
#define                                 G1FIS1_bit          BANKMASK(COG1FIS), 1
// COG1FIS<G1FIS2>
extern volatile __bit                   G1FIS2              __at(0x34DA);	// @ (0x69B * 8 + 2)
#define                                 G1FIS2_bit          BANKMASK(COG1FIS), 2
// COG1FIS<G1FIS3>
extern volatile __bit                   G1FIS3              __at(0x34DB);	// @ (0x69B * 8 + 3)
#define                                 G1FIS3_bit          BANKMASK(COG1FIS), 3
// COG1FIS<G1FIS4>
extern volatile __bit                   G1FIS4              __at(0x34DC);	// @ (0x69B * 8 + 4)
#define                                 G1FIS4_bit          BANKMASK(COG1FIS), 4
// COG1FIS<G1FIS5>
extern volatile __bit                   G1FIS5              __at(0x34DD);	// @ (0x69B * 8 + 5)
#define                                 G1FIS5_bit          BANKMASK(COG1FIS), 5
// COG1FIS<G1FIS6>
extern volatile __bit                   G1FIS6              __at(0x34DE);	// @ (0x69B * 8 + 6)
#define                                 G1FIS6_bit          BANKMASK(COG1FIS), 6
// COG1FSIM<G1FSIM0>
extern volatile __bit                   G1FSIM0             __at(0x34E0);	// @ (0x69C * 8 + 0)
#define                                 G1FSIM0_bit         BANKMASK(COG1FSIM), 0
// COG1FSIM<G1FSIM1>
extern volatile __bit                   G1FSIM1             __at(0x34E1);	// @ (0x69C * 8 + 1)
#define                                 G1FSIM1_bit         BANKMASK(COG1FSIM), 1
// COG1FSIM<G1FSIM2>
extern volatile __bit                   G1FSIM2             __at(0x34E2);	// @ (0x69C * 8 + 2)
#define                                 G1FSIM2_bit         BANKMASK(COG1FSIM), 2
// COG1FSIM<G1FSIM3>
extern volatile __bit                   G1FSIM3             __at(0x34E3);	// @ (0x69C * 8 + 3)
#define                                 G1FSIM3_bit         BANKMASK(COG1FSIM), 3
// COG1FSIM<G1FSIM4>
extern volatile __bit                   G1FSIM4             __at(0x34E4);	// @ (0x69C * 8 + 4)
#define                                 G1FSIM4_bit         BANKMASK(COG1FSIM), 4
// COG1FSIM<G1FSIM5>
extern volatile __bit                   G1FSIM5             __at(0x34E5);	// @ (0x69C * 8 + 5)
#define                                 G1FSIM5_bit         BANKMASK(COG1FSIM), 5
// COG1FSIM<G1FSIM6>
extern volatile __bit                   G1FSIM6             __at(0x34E6);	// @ (0x69C * 8 + 6)
#define                                 G1FSIM6_bit         BANKMASK(COG1FSIM), 6
// COG1CON0<G1LD>
extern volatile __bit                   G1LD                __at(0x34BE);	// @ (0x697 * 8 + 6)
#define                                 G1LD_bit            BANKMASK(COG1CON0), 6
// COG1CON0<G1MD0>
extern volatile __bit                   G1MD0               __at(0x34B8);	// @ (0x697 * 8 + 0)
#define                                 G1MD0_bit           BANKMASK(COG1CON0), 0
// COG1CON0<G1MD1>
extern volatile __bit                   G1MD1               __at(0x34B9);	// @ (0x697 * 8 + 1)
#define                                 G1MD1_bit           BANKMASK(COG1CON0), 1
// COG1CON0<G1MD2>
extern volatile __bit                   G1MD2               __at(0x34BA);	// @ (0x697 * 8 + 2)
#define                                 G1MD2_bit           BANKMASK(COG1CON0), 2
// COG1PHF<G1PHF0>
extern volatile __bit                   G1PHF0              __at(0x3490);	// @ (0x692 * 8 + 0)
#define                                 G1PHF0_bit          BANKMASK(COG1PHF), 0
// COG1PHF<G1PHF1>
extern volatile __bit                   G1PHF1              __at(0x3491);	// @ (0x692 * 8 + 1)
#define                                 G1PHF1_bit          BANKMASK(COG1PHF), 1
// COG1PHF<G1PHF2>
extern volatile __bit                   G1PHF2              __at(0x3492);	// @ (0x692 * 8 + 2)
#define                                 G1PHF2_bit          BANKMASK(COG1PHF), 2
// COG1PHF<G1PHF3>
extern volatile __bit                   G1PHF3              __at(0x3493);	// @ (0x692 * 8 + 3)
#define                                 G1PHF3_bit          BANKMASK(COG1PHF), 3
// COG1PHF<G1PHF4>
extern volatile __bit                   G1PHF4              __at(0x3494);	// @ (0x692 * 8 + 4)
#define                                 G1PHF4_bit          BANKMASK(COG1PHF), 4
// COG1PHF<G1PHF5>
extern volatile __bit                   G1PHF5              __at(0x3495);	// @ (0x692 * 8 + 5)
#define                                 G1PHF5_bit          BANKMASK(COG1PHF), 5
// COG1PHR<G1PHR0>
extern volatile __bit                   G1PHR0              __at(0x3488);	// @ (0x691 * 8 + 0)
#define                                 G1PHR0_bit          BANKMASK(COG1PHR), 0
// COG1PHR<G1PHR1>
extern volatile __bit                   G1PHR1              __at(0x3489);	// @ (0x691 * 8 + 1)
#define                                 G1PHR1_bit          BANKMASK(COG1PHR), 1
// COG1PHR<G1PHR2>
extern volatile __bit                   G1PHR2              __at(0x348A);	// @ (0x691 * 8 + 2)
#define                                 G1PHR2_bit          BANKMASK(COG1PHR), 2
// COG1PHR<G1PHR3>
extern volatile __bit                   G1PHR3              __at(0x348B);	// @ (0x691 * 8 + 3)
#define                                 G1PHR3_bit          BANKMASK(COG1PHR), 3
// COG1PHR<G1PHR4>
extern volatile __bit                   G1PHR4              __at(0x348C);	// @ (0x691 * 8 + 4)
#define                                 G1PHR4_bit          BANKMASK(COG1PHR), 4
// COG1PHR<G1PHR5>
extern volatile __bit                   G1PHR5              __at(0x348D);	// @ (0x691 * 8 + 5)
#define                                 G1PHR5_bit          BANKMASK(COG1PHR), 5
// COG1CON1<G1POLA>
extern volatile __bit                   G1POLA              __at(0x34C0);	// @ (0x698 * 8 + 0)
#define                                 G1POLA_bit          BANKMASK(COG1CON1), 0
// COG1CON1<G1POLB>
extern volatile __bit                   G1POLB              __at(0x34C1);	// @ (0x698 * 8 + 1)
#define                                 G1POLB_bit          BANKMASK(COG1CON1), 1
// COG1CON1<G1POLC>
extern volatile __bit                   G1POLC              __at(0x34C2);	// @ (0x698 * 8 + 2)
#define                                 G1POLC_bit          BANKMASK(COG1CON1), 2
// COG1CON1<G1POLD>
extern volatile __bit                   G1POLD              __at(0x34C3);	// @ (0x698 * 8 + 3)
#define                                 G1POLD_bit          BANKMASK(COG1CON1), 3
// COG1CON1<G1RDBS>
extern volatile __bit                   G1RDBS              __at(0x34C7);	// @ (0x698 * 8 + 7)
#define                                 G1RDBS_bit          BANKMASK(COG1CON1), 7
// COG1RIS<G1RIS0>
extern volatile __bit                   G1RIS0              __at(0x34C8);	// @ (0x699 * 8 + 0)
#define                                 G1RIS0_bit          BANKMASK(COG1RIS), 0
// COG1RIS<G1RIS1>
extern volatile __bit                   G1RIS1              __at(0x34C9);	// @ (0x699 * 8 + 1)
#define                                 G1RIS1_bit          BANKMASK(COG1RIS), 1
// COG1RIS<G1RIS2>
extern volatile __bit                   G1RIS2              __at(0x34CA);	// @ (0x699 * 8 + 2)
#define                                 G1RIS2_bit          BANKMASK(COG1RIS), 2
// COG1RIS<G1RIS3>
extern volatile __bit                   G1RIS3              __at(0x34CB);	// @ (0x699 * 8 + 3)
#define                                 G1RIS3_bit          BANKMASK(COG1RIS), 3
// COG1RIS<G1RIS4>
extern volatile __bit                   G1RIS4              __at(0x34CC);	// @ (0x699 * 8 + 4)
#define                                 G1RIS4_bit          BANKMASK(COG1RIS), 4
// COG1RIS<G1RIS5>
extern volatile __bit                   G1RIS5              __at(0x34CD);	// @ (0x699 * 8 + 5)
#define                                 G1RIS5_bit          BANKMASK(COG1RIS), 5
// COG1RIS<G1RIS6>
extern volatile __bit                   G1RIS6              __at(0x34CE);	// @ (0x699 * 8 + 6)
#define                                 G1RIS6_bit          BANKMASK(COG1RIS), 6
// COG1RSIM<G1RSIM0>
extern volatile __bit                   G1RSIM0             __at(0x34D0);	// @ (0x69A * 8 + 0)
#define                                 G1RSIM0_bit         BANKMASK(COG1RSIM), 0
// COG1RSIM<G1RSIM1>
extern volatile __bit                   G1RSIM1             __at(0x34D1);	// @ (0x69A * 8 + 1)
#define                                 G1RSIM1_bit         BANKMASK(COG1RSIM), 1
// COG1RSIM<G1RSIM2>
extern volatile __bit                   G1RSIM2             __at(0x34D2);	// @ (0x69A * 8 + 2)
#define                                 G1RSIM2_bit         BANKMASK(COG1RSIM), 2
// COG1RSIM<G1RSIM3>
extern volatile __bit                   G1RSIM3             __at(0x34D3);	// @ (0x69A * 8 + 3)
#define                                 G1RSIM3_bit         BANKMASK(COG1RSIM), 3
// COG1RSIM<G1RSIM4>
extern volatile __bit                   G1RSIM4             __at(0x34D4);	// @ (0x69A * 8 + 4)
#define                                 G1RSIM4_bit         BANKMASK(COG1RSIM), 4
// COG1RSIM<G1RSIM5>
extern volatile __bit                   G1RSIM5             __at(0x34D5);	// @ (0x69A * 8 + 5)
#define                                 G1RSIM5_bit         BANKMASK(COG1RSIM), 5
// COG1RSIM<G1RSIM6>
extern volatile __bit                   G1RSIM6             __at(0x34D6);	// @ (0x69A * 8 + 6)
#define                                 G1RSIM6_bit         BANKMASK(COG1RSIM), 6
// COG1STR<G1SDATA>
extern volatile __bit                   G1SDATA             __at(0x34FC);	// @ (0x69F * 8 + 4)
#define                                 G1SDATA_bit         BANKMASK(COG1STR), 4
// COG1STR<G1SDATB>
extern volatile __bit                   G1SDATB             __at(0x34FD);	// @ (0x69F * 8 + 5)
#define                                 G1SDATB_bit         BANKMASK(COG1STR), 5
// COG1STR<G1SDATC>
extern volatile __bit                   G1SDATC             __at(0x34FE);	// @ (0x69F * 8 + 6)
#define                                 G1SDATC_bit         BANKMASK(COG1STR), 6
// COG1STR<G1SDATD>
extern volatile __bit                   G1SDATD             __at(0x34FF);	// @ (0x69F * 8 + 7)
#define                                 G1SDATD_bit         BANKMASK(COG1STR), 7
// COG1STR<G1STRA>
extern volatile __bit                   G1STRA              __at(0x34F8);	// @ (0x69F * 8 + 0)
#define                                 G1STRA_bit          BANKMASK(COG1STR), 0
// COG1STR<G1STRB>
extern volatile __bit                   G1STRB              __at(0x34F9);	// @ (0x69F * 8 + 1)
#define                                 G1STRB_bit          BANKMASK(COG1STR), 1
// COG1STR<G1STRC>
extern volatile __bit                   G1STRC              __at(0x34FA);	// @ (0x69F * 8 + 2)
#define                                 G1STRC_bit          BANKMASK(COG1STR), 2
// COG1STR<G1STRD>
extern volatile __bit                   G1STRD              __at(0x34FB);	// @ (0x69F * 8 + 3)
#define                                 G1STRD_bit          BANKMASK(COG1STR), 3
// SSP1CON2<GCEN>
extern volatile __bit                   GCEN                __at(0x10B7);	// @ (0x216 * 8 + 7)
#define                                 GCEN_bit            BANKMASK(SSP1CON2), 7
// INTCON<GIE>
extern volatile __bit                   GIE                 __at(0x5F);	// @ (0xB * 8 + 7)
#define                                 GIE_bit             BANKMASK(INTCON), 7
// ADCON0<GO>
extern volatile __bit                   GO                  __at(0x4E9);	// @ (0x9D * 8 + 1)
#define                                 GO_bit              BANKMASK(ADCON0), 1
// ADCON0<GO_nDONE>
extern volatile __bit                   GO_nDONE            __at(0x4E9);	// @ (0x9D * 8 + 1)
#define                                 GO_nDONE_bit        BANKMASK(ADCON0), 1
// OSCSTAT<HFIOFL>
extern volatile __bit                   HFIOFL              __at(0x4D3);	// @ (0x9A * 8 + 3)
#define                                 HFIOFL_bit          BANKMASK(OSCSTAT), 3
// OSCSTAT<HFIOFR>
extern volatile __bit                   HFIOFR              __at(0x4D4);	// @ (0x9A * 8 + 4)
#define                                 HFIOFR_bit          BANKMASK(OSCSTAT), 4
// OSCSTAT<HFIOFS>
extern volatile __bit                   HFIOFS              __at(0x4D0);	// @ (0x9A * 8 + 0)
#define                                 HFIOFS_bit          BANKMASK(OSCSTAT), 0
// INLVLA<INLVLA0>
extern volatile __bit                   INLVLA0             __at(0x1C60);	// @ (0x38C * 8 + 0)
#define                                 INLVLA0_bit         BANKMASK(INLVLA), 0
// INLVLA<INLVLA1>
extern volatile __bit                   INLVLA1             __at(0x1C61);	// @ (0x38C * 8 + 1)
#define                                 INLVLA1_bit         BANKMASK(INLVLA), 1
// INLVLA<INLVLA2>
extern volatile __bit                   INLVLA2             __at(0x1C62);	// @ (0x38C * 8 + 2)
#define                                 INLVLA2_bit         BANKMASK(INLVLA), 2
// INLVLA<INLVLA3>
extern volatile __bit                   INLVLA3             __at(0x1C63);	// @ (0x38C * 8 + 3)
#define                                 INLVLA3_bit         BANKMASK(INLVLA), 3
// INLVLA<INLVLA4>
extern volatile __bit                   INLVLA4             __at(0x1C64);	// @ (0x38C * 8 + 4)
#define                                 INLVLA4_bit         BANKMASK(INLVLA), 4
// INLVLA<INLVLA5>
extern volatile __bit                   INLVLA5             __at(0x1C65);	// @ (0x38C * 8 + 5)
#define                                 INLVLA5_bit         BANKMASK(INLVLA), 5
// INLVLB<INLVLB4>
extern volatile __bit                   INLVLB4             __at(0x1C6C);	// @ (0x38D * 8 + 4)
#define                                 INLVLB4_bit         BANKMASK(INLVLB), 4
// INLVLB<INLVLB5>
extern volatile __bit                   INLVLB5             __at(0x1C6D);	// @ (0x38D * 8 + 5)
#define                                 INLVLB5_bit         BANKMASK(INLVLB), 5
// INLVLB<INLVLB6>
extern volatile __bit                   INLVLB6             __at(0x1C6E);	// @ (0x38D * 8 + 6)
#define                                 INLVLB6_bit         BANKMASK(INLVLB), 6
// INLVLB<INLVLB7>
extern volatile __bit                   INLVLB7             __at(0x1C6F);	// @ (0x38D * 8 + 7)
#define                                 INLVLB7_bit         BANKMASK(INLVLB), 7
// INLVLC<INLVLC0>
extern volatile __bit                   INLVLC0             __at(0x1C70);	// @ (0x38E * 8 + 0)
#define                                 INLVLC0_bit         BANKMASK(INLVLC), 0
// INLVLC<INLVLC1>
extern volatile __bit                   INLVLC1             __at(0x1C71);	// @ (0x38E * 8 + 1)
#define                                 INLVLC1_bit         BANKMASK(INLVLC), 1
// INLVLC<INLVLC2>
extern volatile __bit                   INLVLC2             __at(0x1C72);	// @ (0x38E * 8 + 2)
#define                                 INLVLC2_bit         BANKMASK(INLVLC), 2
// INLVLC<INLVLC3>
extern volatile __bit                   INLVLC3             __at(0x1C73);	// @ (0x38E * 8 + 3)
#define                                 INLVLC3_bit         BANKMASK(INLVLC), 3
// INLVLC<INLVLC4>
extern volatile __bit                   INLVLC4             __at(0x1C74);	// @ (0x38E * 8 + 4)
#define                                 INLVLC4_bit         BANKMASK(INLVLC), 4
// INLVLC<INLVLC5>
extern volatile __bit                   INLVLC5             __at(0x1C75);	// @ (0x38E * 8 + 5)
#define                                 INLVLC5_bit         BANKMASK(INLVLC), 5
// INLVLC<INLVLC6>
extern volatile __bit                   INLVLC6             __at(0x1C76);	// @ (0x38E * 8 + 6)
#define                                 INLVLC6_bit         BANKMASK(INLVLC), 6
// INLVLC<INLVLC7>
extern volatile __bit                   INLVLC7             __at(0x1C77);	// @ (0x38E * 8 + 7)
#define                                 INLVLC7_bit         BANKMASK(INLVLC), 7
// INTCON<INTE>
extern volatile __bit                   INTE                __at(0x5C);	// @ (0xB * 8 + 4)
#define                                 INTE_bit            BANKMASK(INTCON), 4
// OPTION_REG<INTEDG>
extern volatile __bit                   INTEDG              __at(0x4AE);	// @ (0x95 * 8 + 6)
#define                                 INTEDG_bit          BANKMASK(OPTION_REG), 6
// INTCON<INTF>
extern volatile __bit                   INTF                __at(0x59);	// @ (0xB * 8 + 1)
#define                                 INTF_bit            BANKMASK(INTCON), 1
// IOCAF<IOCAF0>
extern volatile __bit                   IOCAF0              __at(0x1C98);	// @ (0x393 * 8 + 0)
#define                                 IOCAF0_bit          BANKMASK(IOCAF), 0
// IOCAF<IOCAF1>
extern volatile __bit                   IOCAF1              __at(0x1C99);	// @ (0x393 * 8 + 1)
#define                                 IOCAF1_bit          BANKMASK(IOCAF), 1
// IOCAF<IOCAF2>
extern volatile __bit                   IOCAF2              __at(0x1C9A);	// @ (0x393 * 8 + 2)
#define                                 IOCAF2_bit          BANKMASK(IOCAF), 2
// IOCAF<IOCAF3>
extern volatile __bit                   IOCAF3              __at(0x1C9B);	// @ (0x393 * 8 + 3)
#define                                 IOCAF3_bit          BANKMASK(IOCAF), 3
// IOCAF<IOCAF4>
extern volatile __bit                   IOCAF4              __at(0x1C9C);	// @ (0x393 * 8 + 4)
#define                                 IOCAF4_bit          BANKMASK(IOCAF), 4
// IOCAF<IOCAF5>
extern volatile __bit                   IOCAF5              __at(0x1C9D);	// @ (0x393 * 8 + 5)
#define                                 IOCAF5_bit          BANKMASK(IOCAF), 5
// IOCAN<IOCAN0>
extern volatile __bit                   IOCAN0              __at(0x1C90);	// @ (0x392 * 8 + 0)
#define                                 IOCAN0_bit          BANKMASK(IOCAN), 0
// IOCAN<IOCAN1>
extern volatile __bit                   IOCAN1              __at(0x1C91);	// @ (0x392 * 8 + 1)
#define                                 IOCAN1_bit          BANKMASK(IOCAN), 1
// IOCAN<IOCAN2>
extern volatile __bit                   IOCAN2              __at(0x1C92);	// @ (0x392 * 8 + 2)
#define                                 IOCAN2_bit          BANKMASK(IOCAN), 2
// IOCAN<IOCAN3>
extern volatile __bit                   IOCAN3              __at(0x1C93);	// @ (0x392 * 8 + 3)
#define                                 IOCAN3_bit          BANKMASK(IOCAN), 3
// IOCAN<IOCAN4>
extern volatile __bit                   IOCAN4              __at(0x1C94);	// @ (0x392 * 8 + 4)
#define                                 IOCAN4_bit          BANKMASK(IOCAN), 4
// IOCAN<IOCAN5>
extern volatile __bit                   IOCAN5              __at(0x1C95);	// @ (0x392 * 8 + 5)
#define                                 IOCAN5_bit          BANKMASK(IOCAN), 5
// IOCAP<IOCAP0>
extern volatile __bit                   IOCAP0              __at(0x1C88);	// @ (0x391 * 8 + 0)
#define                                 IOCAP0_bit          BANKMASK(IOCAP), 0
// IOCAP<IOCAP1>
extern volatile __bit                   IOCAP1              __at(0x1C89);	// @ (0x391 * 8 + 1)
#define                                 IOCAP1_bit          BANKMASK(IOCAP), 1
// IOCAP<IOCAP2>
extern volatile __bit                   IOCAP2              __at(0x1C8A);	// @ (0x391 * 8 + 2)
#define                                 IOCAP2_bit          BANKMASK(IOCAP), 2
// IOCAP<IOCAP3>
extern volatile __bit                   IOCAP3              __at(0x1C8B);	// @ (0x391 * 8 + 3)
#define                                 IOCAP3_bit          BANKMASK(IOCAP), 3
// IOCAP<IOCAP4>
extern volatile __bit                   IOCAP4              __at(0x1C8C);	// @ (0x391 * 8 + 4)
#define                                 IOCAP4_bit          BANKMASK(IOCAP), 4
// IOCAP<IOCAP5>
extern volatile __bit                   IOCAP5              __at(0x1C8D);	// @ (0x391 * 8 + 5)
#define                                 IOCAP5_bit          BANKMASK(IOCAP), 5
// IOCBF<IOCBF4>
extern volatile __bit                   IOCBF4              __at(0x1CB4);	// @ (0x396 * 8 + 4)
#define                                 IOCBF4_bit          BANKMASK(IOCBF), 4
// IOCBF<IOCBF5>
extern volatile __bit                   IOCBF5              __at(0x1CB5);	// @ (0x396 * 8 + 5)
#define                                 IOCBF5_bit          BANKMASK(IOCBF), 5
// IOCBF<IOCBF6>
extern volatile __bit                   IOCBF6              __at(0x1CB6);	// @ (0x396 * 8 + 6)
#define                                 IOCBF6_bit          BANKMASK(IOCBF), 6
// IOCBF<IOCBF7>
extern volatile __bit                   IOCBF7              __at(0x1CB7);	// @ (0x396 * 8 + 7)
#define                                 IOCBF7_bit          BANKMASK(IOCBF), 7
// IOCBN<IOCBN4>
extern volatile __bit                   IOCBN4              __at(0x1CAC);	// @ (0x395 * 8 + 4)
#define                                 IOCBN4_bit          BANKMASK(IOCBN), 4
// IOCBN<IOCBN5>
extern volatile __bit                   IOCBN5              __at(0x1CAD);	// @ (0x395 * 8 + 5)
#define                                 IOCBN5_bit          BANKMASK(IOCBN), 5
// IOCBN<IOCBN6>
extern volatile __bit                   IOCBN6              __at(0x1CAE);	// @ (0x395 * 8 + 6)
#define                                 IOCBN6_bit          BANKMASK(IOCBN), 6
// IOCBN<IOCBN7>
extern volatile __bit                   IOCBN7              __at(0x1CAF);	// @ (0x395 * 8 + 7)
#define                                 IOCBN7_bit          BANKMASK(IOCBN), 7
// IOCBP<IOCBP4>
extern volatile __bit                   IOCBP4              __at(0x1CA4);	// @ (0x394 * 8 + 4)
#define                                 IOCBP4_bit          BANKMASK(IOCBP), 4
// IOCBP<IOCBP5>
extern volatile __bit                   IOCBP5              __at(0x1CA5);	// @ (0x394 * 8 + 5)
#define                                 IOCBP5_bit          BANKMASK(IOCBP), 5
// IOCBP<IOCBP6>
extern volatile __bit                   IOCBP6              __at(0x1CA6);	// @ (0x394 * 8 + 6)
#define                                 IOCBP6_bit          BANKMASK(IOCBP), 6
// IOCBP<IOCBP7>
extern volatile __bit                   IOCBP7              __at(0x1CA7);	// @ (0x394 * 8 + 7)
#define                                 IOCBP7_bit          BANKMASK(IOCBP), 7
// IOCCF<IOCCF0>
extern volatile __bit                   IOCCF0              __at(0x1CC8);	// @ (0x399 * 8 + 0)
#define                                 IOCCF0_bit          BANKMASK(IOCCF), 0
// IOCCF<IOCCF1>
extern volatile __bit                   IOCCF1              __at(0x1CC9);	// @ (0x399 * 8 + 1)
#define                                 IOCCF1_bit          BANKMASK(IOCCF), 1
// IOCCF<IOCCF2>
extern volatile __bit                   IOCCF2              __at(0x1CCA);	// @ (0x399 * 8 + 2)
#define                                 IOCCF2_bit          BANKMASK(IOCCF), 2
// IOCCF<IOCCF3>
extern volatile __bit                   IOCCF3              __at(0x1CCB);	// @ (0x399 * 8 + 3)
#define                                 IOCCF3_bit          BANKMASK(IOCCF), 3
// IOCCF<IOCCF4>
extern volatile __bit                   IOCCF4              __at(0x1CCC);	// @ (0x399 * 8 + 4)
#define                                 IOCCF4_bit          BANKMASK(IOCCF), 4
// IOCCF<IOCCF5>
extern volatile __bit                   IOCCF5              __at(0x1CCD);	// @ (0x399 * 8 + 5)
#define                                 IOCCF5_bit          BANKMASK(IOCCF), 5
// IOCCF<IOCCF6>
extern volatile __bit                   IOCCF6              __at(0x1CCE);	// @ (0x399 * 8 + 6)
#define                                 IOCCF6_bit          BANKMASK(IOCCF), 6
// IOCCF<IOCCF7>
extern volatile __bit                   IOCCF7              __at(0x1CCF);	// @ (0x399 * 8 + 7)
#define                                 IOCCF7_bit          BANKMASK(IOCCF), 7
// IOCCN<IOCCN0>
extern volatile __bit                   IOCCN0              __at(0x1CC0);	// @ (0x398 * 8 + 0)
#define                                 IOCCN0_bit          BANKMASK(IOCCN), 0
// IOCCN<IOCCN1>
extern volatile __bit                   IOCCN1              __at(0x1CC1);	// @ (0x398 * 8 + 1)
#define                                 IOCCN1_bit          BANKMASK(IOCCN), 1
// IOCCN<IOCCN2>
extern volatile __bit                   IOCCN2              __at(0x1CC2);	// @ (0x398 * 8 + 2)
#define                                 IOCCN2_bit          BANKMASK(IOCCN), 2
// IOCCN<IOCCN3>
extern volatile __bit                   IOCCN3              __at(0x1CC3);	// @ (0x398 * 8 + 3)
#define                                 IOCCN3_bit          BANKMASK(IOCCN), 3
// IOCCN<IOCCN4>
extern volatile __bit                   IOCCN4              __at(0x1CC4);	// @ (0x398 * 8 + 4)
#define                                 IOCCN4_bit          BANKMASK(IOCCN), 4
// IOCCN<IOCCN5>
extern volatile __bit                   IOCCN5              __at(0x1CC5);	// @ (0x398 * 8 + 5)
#define                                 IOCCN5_bit          BANKMASK(IOCCN), 5
// IOCCN<IOCCN6>
extern volatile __bit                   IOCCN6              __at(0x1CC6);	// @ (0x398 * 8 + 6)
#define                                 IOCCN6_bit          BANKMASK(IOCCN), 6
// IOCCN<IOCCN7>
extern volatile __bit                   IOCCN7              __at(0x1CC7);	// @ (0x398 * 8 + 7)
#define                                 IOCCN7_bit          BANKMASK(IOCCN), 7
// IOCCP<IOCCP0>
extern volatile __bit                   IOCCP0              __at(0x1CB8);	// @ (0x397 * 8 + 0)
#define                                 IOCCP0_bit          BANKMASK(IOCCP), 0
// IOCCP<IOCCP1>
extern volatile __bit                   IOCCP1              __at(0x1CB9);	// @ (0x397 * 8 + 1)
#define                                 IOCCP1_bit          BANKMASK(IOCCP), 1
// IOCCP<IOCCP2>
extern volatile __bit                   IOCCP2              __at(0x1CBA);	// @ (0x397 * 8 + 2)
#define                                 IOCCP2_bit          BANKMASK(IOCCP), 2
// IOCCP<IOCCP3>
extern volatile __bit                   IOCCP3              __at(0x1CBB);	// @ (0x397 * 8 + 3)
#define                                 IOCCP3_bit          BANKMASK(IOCCP), 3
// IOCCP<IOCCP4>
extern volatile __bit                   IOCCP4              __at(0x1CBC);	// @ (0x397 * 8 + 4)
#define                                 IOCCP4_bit          BANKMASK(IOCCP), 4
// IOCCP<IOCCP5>
extern volatile __bit                   IOCCP5              __at(0x1CBD);	// @ (0x397 * 8 + 5)
#define                                 IOCCP5_bit          BANKMASK(IOCCP), 5
// IOCCP<IOCCP6>
extern volatile __bit                   IOCCP6              __at(0x1CBE);	// @ (0x397 * 8 + 6)
#define                                 IOCCP6_bit          BANKMASK(IOCCP), 6
// IOCCP<IOCCP7>
extern volatile __bit                   IOCCP7              __at(0x1CBF);	// @ (0x397 * 8 + 7)
#define                                 IOCCP7_bit          BANKMASK(IOCCP), 7
// INTCON<IOCIE>
extern volatile __bit                   IOCIE               __at(0x5B);	// @ (0xB * 8 + 3)
#define                                 IOCIE_bit           BANKMASK(INTCON), 3
// INTCON<IOCIF>
extern volatile __bit                   IOCIF               __at(0x58);	// @ (0xB * 8 + 0)
#define                                 IOCIF_bit           BANKMASK(INTCON), 0
// OSCCON<IRCF0>
extern volatile __bit                   IRCF0               __at(0x4CB);	// @ (0x99 * 8 + 3)
#define                                 IRCF0_bit           BANKMASK(OSCCON), 3
// OSCCON<IRCF1>
extern volatile __bit                   IRCF1               __at(0x4CC);	// @ (0x99 * 8 + 4)
#define                                 IRCF1_bit           BANKMASK(OSCCON), 4
// OSCCON<IRCF2>
extern volatile __bit                   IRCF2               __at(0x4CD);	// @ (0x99 * 8 + 5)
#define                                 IRCF2_bit           BANKMASK(OSCCON), 5
// OSCCON<IRCF3>
extern volatile __bit                   IRCF3               __at(0x4CE);	// @ (0x99 * 8 + 6)
#define                                 IRCF3_bit           BANKMASK(OSCCON), 6
// LATA<LATA0>
extern volatile __bit                   LATA0               __at(0x860);	// @ (0x10C * 8 + 0)
#define                                 LATA0_bit           BANKMASK(LATA), 0
// LATA<LATA1>
extern volatile __bit                   LATA1               __at(0x861);	// @ (0x10C * 8 + 1)
#define                                 LATA1_bit           BANKMASK(LATA), 1
// LATA<LATA2>
extern volatile __bit                   LATA2               __at(0x862);	// @ (0x10C * 8 + 2)
#define                                 LATA2_bit           BANKMASK(LATA), 2
// LATA<LATA4>
extern volatile __bit                   LATA4               __at(0x864);	// @ (0x10C * 8 + 4)
#define                                 LATA4_bit           BANKMASK(LATA), 4
// LATA<LATA5>
extern volatile __bit                   LATA5               __at(0x865);	// @ (0x10C * 8 + 5)
#define                                 LATA5_bit           BANKMASK(LATA), 5
// LATB<LATB4>
extern volatile __bit                   LATB4               __at(0x86C);	// @ (0x10D * 8 + 4)
#define                                 LATB4_bit           BANKMASK(LATB), 4
// LATB<LATB5>
extern volatile __bit                   LATB5               __at(0x86D);	// @ (0x10D * 8 + 5)
#define                                 LATB5_bit           BANKMASK(LATB), 5
// LATB<LATB6>
extern volatile __bit                   LATB6               __at(0x86E);	// @ (0x10D * 8 + 6)
#define                                 LATB6_bit           BANKMASK(LATB), 6
// LATB<LATB7>
extern volatile __bit                   LATB7               __at(0x86F);	// @ (0x10D * 8 + 7)
#define                                 LATB7_bit           BANKMASK(LATB), 7
// LATC<LATC0>
extern volatile __bit                   LATC0               __at(0x870);	// @ (0x10E * 8 + 0)
#define                                 LATC0_bit           BANKMASK(LATC), 0
// LATC<LATC1>
extern volatile __bit                   LATC1               __at(0x871);	// @ (0x10E * 8 + 1)
#define                                 LATC1_bit           BANKMASK(LATC), 1
// LATC<LATC2>
extern volatile __bit                   LATC2               __at(0x872);	// @ (0x10E * 8 + 2)
#define                                 LATC2_bit           BANKMASK(LATC), 2
// LATC<LATC3>
extern volatile __bit                   LATC3               __at(0x873);	// @ (0x10E * 8 + 3)
#define                                 LATC3_bit           BANKMASK(LATC), 3
// LATC<LATC4>
extern volatile __bit                   LATC4               __at(0x874);	// @ (0x10E * 8 + 4)
#define                                 LATC4_bit           BANKMASK(LATC), 4
// LATC<LATC5>
extern volatile __bit                   LATC5               __at(0x875);	// @ (0x10E * 8 + 5)
#define                                 LATC5_bit           BANKMASK(LATC), 5
// LATC<LATC6>
extern volatile __bit                   LATC6               __at(0x876);	// @ (0x10E * 8 + 6)
#define                                 LATC6_bit           BANKMASK(LATC), 6
// LATC<LATC7>
extern volatile __bit                   LATC7               __at(0x877);	// @ (0x10E * 8 + 7)
#define                                 LATC7_bit           BANKMASK(LATC), 7
// CLC1SEL0<LC1D1S0>
extern volatile __bit                   LC1D1S0             __at(0x7890);	// @ (0xF12 * 8 + 0)
#define                                 LC1D1S0_bit         BANKMASK(CLC1SEL0), 0
// CLC1SEL0<LC1D1S1>
extern volatile __bit                   LC1D1S1             __at(0x7891);	// @ (0xF12 * 8 + 1)
#define                                 LC1D1S1_bit         BANKMASK(CLC1SEL0), 1
// CLC1SEL0<LC1D1S2>
extern volatile __bit                   LC1D1S2             __at(0x7892);	// @ (0xF12 * 8 + 2)
#define                                 LC1D1S2_bit         BANKMASK(CLC1SEL0), 2
// CLC1SEL0<LC1D1S3>
extern volatile __bit                   LC1D1S3             __at(0x7893);	// @ (0xF12 * 8 + 3)
#define                                 LC1D1S3_bit         BANKMASK(CLC1SEL0), 3
// CLC1SEL0<LC1D1S4>
extern volatile __bit                   LC1D1S4             __at(0x7894);	// @ (0xF12 * 8 + 4)
#define                                 LC1D1S4_bit         BANKMASK(CLC1SEL0), 4
// CLC1SEL1<LC1D2S0>
extern volatile __bit                   LC1D2S0             __at(0x7898);	// @ (0xF13 * 8 + 0)
#define                                 LC1D2S0_bit         BANKMASK(CLC1SEL1), 0
// CLC1SEL1<LC1D2S1>
extern volatile __bit                   LC1D2S1             __at(0x7899);	// @ (0xF13 * 8 + 1)
#define                                 LC1D2S1_bit         BANKMASK(CLC1SEL1), 1
// CLC1SEL1<LC1D2S2>
extern volatile __bit                   LC1D2S2             __at(0x789A);	// @ (0xF13 * 8 + 2)
#define                                 LC1D2S2_bit         BANKMASK(CLC1SEL1), 2
// CLC1SEL1<LC1D2S3>
extern volatile __bit                   LC1D2S3             __at(0x789B);	// @ (0xF13 * 8 + 3)
#define                                 LC1D2S3_bit         BANKMASK(CLC1SEL1), 3
// CLC1SEL1<LC1D2S4>
extern volatile __bit                   LC1D2S4             __at(0x789C);	// @ (0xF13 * 8 + 4)
#define                                 LC1D2S4_bit         BANKMASK(CLC1SEL1), 4
// CLC1SEL2<LC1D3S0>
extern volatile __bit                   LC1D3S0             __at(0x78A0);	// @ (0xF14 * 8 + 0)
#define                                 LC1D3S0_bit         BANKMASK(CLC1SEL2), 0
// CLC1SEL2<LC1D3S1>
extern volatile __bit                   LC1D3S1             __at(0x78A1);	// @ (0xF14 * 8 + 1)
#define                                 LC1D3S1_bit         BANKMASK(CLC1SEL2), 1
// CLC1SEL2<LC1D3S2>
extern volatile __bit                   LC1D3S2             __at(0x78A2);	// @ (0xF14 * 8 + 2)
#define                                 LC1D3S2_bit         BANKMASK(CLC1SEL2), 2
// CLC1SEL2<LC1D3S3>
extern volatile __bit                   LC1D3S3             __at(0x78A3);	// @ (0xF14 * 8 + 3)
#define                                 LC1D3S3_bit         BANKMASK(CLC1SEL2), 3
// CLC1SEL2<LC1D3S4>
extern volatile __bit                   LC1D3S4             __at(0x78A4);	// @ (0xF14 * 8 + 4)
#define                                 LC1D3S4_bit         BANKMASK(CLC1SEL2), 4
// CLC1SEL3<LC1D4S0>
extern volatile __bit                   LC1D4S0             __at(0x78A8);	// @ (0xF15 * 8 + 0)
#define                                 LC1D4S0_bit         BANKMASK(CLC1SEL3), 0
// CLC1SEL3<LC1D4S1>
extern volatile __bit                   LC1D4S1             __at(0x78A9);	// @ (0xF15 * 8 + 1)
#define                                 LC1D4S1_bit         BANKMASK(CLC1SEL3), 1
// CLC1SEL3<LC1D4S2>
extern volatile __bit                   LC1D4S2             __at(0x78AA);	// @ (0xF15 * 8 + 2)
#define                                 LC1D4S2_bit         BANKMASK(CLC1SEL3), 2
// CLC1SEL3<LC1D4S3>
extern volatile __bit                   LC1D4S3             __at(0x78AB);	// @ (0xF15 * 8 + 3)
#define                                 LC1D4S3_bit         BANKMASK(CLC1SEL3), 3
// CLC1SEL3<LC1D4S4>
extern volatile __bit                   LC1D4S4             __at(0x78AC);	// @ (0xF15 * 8 + 4)
#define                                 LC1D4S4_bit         BANKMASK(CLC1SEL3), 4
// CLC1CON<LC1EN>
extern volatile __bit                   LC1EN               __at(0x7887);	// @ (0xF10 * 8 + 7)
#define                                 LC1EN_bit           BANKMASK(CLC1CON), 7
// CLC1GLS0<LC1G1D1N>
extern volatile __bit                   LC1G1D1N            __at(0x78B0);	// @ (0xF16 * 8 + 0)
#define                                 LC1G1D1N_bit        BANKMASK(CLC1GLS0), 0
// CLC1GLS0<LC1G1D1T>
extern volatile __bit                   LC1G1D1T            __at(0x78B1);	// @ (0xF16 * 8 + 1)
#define                                 LC1G1D1T_bit        BANKMASK(CLC1GLS0), 1
// CLC1GLS0<LC1G1D2N>
extern volatile __bit                   LC1G1D2N            __at(0x78B2);	// @ (0xF16 * 8 + 2)
#define                                 LC1G1D2N_bit        BANKMASK(CLC1GLS0), 2
// CLC1GLS0<LC1G1D2T>
extern volatile __bit                   LC1G1D2T            __at(0x78B3);	// @ (0xF16 * 8 + 3)
#define                                 LC1G1D2T_bit        BANKMASK(CLC1GLS0), 3
// CLC1GLS0<LC1G1D3N>
extern volatile __bit                   LC1G1D3N            __at(0x78B4);	// @ (0xF16 * 8 + 4)
#define                                 LC1G1D3N_bit        BANKMASK(CLC1GLS0), 4
// CLC1GLS0<LC1G1D3T>
extern volatile __bit                   LC1G1D3T            __at(0x78B5);	// @ (0xF16 * 8 + 5)
#define                                 LC1G1D3T_bit        BANKMASK(CLC1GLS0), 5
// CLC1GLS0<LC1G1D4N>
extern volatile __bit                   LC1G1D4N            __at(0x78B6);	// @ (0xF16 * 8 + 6)
#define                                 LC1G1D4N_bit        BANKMASK(CLC1GLS0), 6
// CLC1GLS0<LC1G1D4T>
extern volatile __bit                   LC1G1D4T            __at(0x78B7);	// @ (0xF16 * 8 + 7)
#define                                 LC1G1D4T_bit        BANKMASK(CLC1GLS0), 7
// CLC1POL<LC1G1POL>
extern volatile __bit                   LC1G1POL            __at(0x7888);	// @ (0xF11 * 8 + 0)
#define                                 LC1G1POL_bit        BANKMASK(CLC1POL), 0
// CLC1GLS1<LC1G2D1N>
extern volatile __bit                   LC1G2D1N            __at(0x78B8);	// @ (0xF17 * 8 + 0)
#define                                 LC1G2D1N_bit        BANKMASK(CLC1GLS1), 0
// CLC1GLS1<LC1G2D1T>
extern volatile __bit                   LC1G2D1T            __at(0x78B9);	// @ (0xF17 * 8 + 1)
#define                                 LC1G2D1T_bit        BANKMASK(CLC1GLS1), 1
// CLC1GLS1<LC1G2D2N>
extern volatile __bit                   LC1G2D2N            __at(0x78BA);	// @ (0xF17 * 8 + 2)
#define                                 LC1G2D2N_bit        BANKMASK(CLC1GLS1), 2
// CLC1GLS1<LC1G2D2T>
extern volatile __bit                   LC1G2D2T            __at(0x78BB);	// @ (0xF17 * 8 + 3)
#define                                 LC1G2D2T_bit        BANKMASK(CLC1GLS1), 3
// CLC1GLS1<LC1G2D3N>
extern volatile __bit                   LC1G2D3N            __at(0x78BC);	// @ (0xF17 * 8 + 4)
#define                                 LC1G2D3N_bit        BANKMASK(CLC1GLS1), 4
// CLC1GLS1<LC1G2D3T>
extern volatile __bit                   LC1G2D3T            __at(0x78BD);	// @ (0xF17 * 8 + 5)
#define                                 LC1G2D3T_bit        BANKMASK(CLC1GLS1), 5
// CLC1GLS1<LC1G2D4N>
extern volatile __bit                   LC1G2D4N            __at(0x78BE);	// @ (0xF17 * 8 + 6)
#define                                 LC1G2D4N_bit        BANKMASK(CLC1GLS1), 6
// CLC1GLS1<LC1G2D4T>
extern volatile __bit                   LC1G2D4T            __at(0x78BF);	// @ (0xF17 * 8 + 7)
#define                                 LC1G2D4T_bit        BANKMASK(CLC1GLS1), 7
// CLC1POL<LC1G2POL>
extern volatile __bit                   LC1G2POL            __at(0x7889);	// @ (0xF11 * 8 + 1)
#define                                 LC1G2POL_bit        BANKMASK(CLC1POL), 1
// CLC1GLS2<LC1G3D1N>
extern volatile __bit                   LC1G3D1N            __at(0x78C0);	// @ (0xF18 * 8 + 0)
#define                                 LC1G3D1N_bit        BANKMASK(CLC1GLS2), 0
// CLC1GLS2<LC1G3D1T>
extern volatile __bit                   LC1G3D1T            __at(0x78C1);	// @ (0xF18 * 8 + 1)
#define                                 LC1G3D1T_bit        BANKMASK(CLC1GLS2), 1
// CLC1GLS2<LC1G3D2N>
extern volatile __bit                   LC1G3D2N            __at(0x78C2);	// @ (0xF18 * 8 + 2)
#define                                 LC1G3D2N_bit        BANKMASK(CLC1GLS2), 2
// CLC1GLS2<LC1G3D2T>
extern volatile __bit                   LC1G3D2T            __at(0x78C3);	// @ (0xF18 * 8 + 3)
#define                                 LC1G3D2T_bit        BANKMASK(CLC1GLS2), 3
// CLC1GLS2<LC1G3D3N>
extern volatile __bit                   LC1G3D3N            __at(0x78C4);	// @ (0xF18 * 8 + 4)
#define                                 LC1G3D3N_bit        BANKMASK(CLC1GLS2), 4
// CLC1GLS2<LC1G3D3T>
extern volatile __bit                   LC1G3D3T            __at(0x78C5);	// @ (0xF18 * 8 + 5)
#define                                 LC1G3D3T_bit        BANKMASK(CLC1GLS2), 5
// CLC1GLS2<LC1G3D4N>
extern volatile __bit                   LC1G3D4N            __at(0x78C6);	// @ (0xF18 * 8 + 6)
#define                                 LC1G3D4N_bit        BANKMASK(CLC1GLS2), 6
// CLC1GLS2<LC1G3D4T>
extern volatile __bit                   LC1G3D4T            __at(0x78C7);	// @ (0xF18 * 8 + 7)
#define                                 LC1G3D4T_bit        BANKMASK(CLC1GLS2), 7
// CLC1POL<LC1G3POL>
extern volatile __bit                   LC1G3POL            __at(0x788A);	// @ (0xF11 * 8 + 2)
#define                                 LC1G3POL_bit        BANKMASK(CLC1POL), 2
// CLC1GLS3<LC1G4D1N>
extern volatile __bit                   LC1G4D1N            __at(0x78C8);	// @ (0xF19 * 8 + 0)
#define                                 LC1G4D1N_bit        BANKMASK(CLC1GLS3), 0
// CLC1GLS3<LC1G4D1T>
extern volatile __bit                   LC1G4D1T            __at(0x78C9);	// @ (0xF19 * 8 + 1)
#define                                 LC1G4D1T_bit        BANKMASK(CLC1GLS3), 1
// CLC1GLS3<LC1G4D2N>
extern volatile __bit                   LC1G4D2N            __at(0x78CA);	// @ (0xF19 * 8 + 2)
#define                                 LC1G4D2N_bit        BANKMASK(CLC1GLS3), 2
// CLC1GLS3<LC1G4D2T>
extern volatile __bit                   LC1G4D2T            __at(0x78CB);	// @ (0xF19 * 8 + 3)
#define                                 LC1G4D2T_bit        BANKMASK(CLC1GLS3), 3
// CLC1GLS3<LC1G4D3N>
extern volatile __bit                   LC1G4D3N            __at(0x78CC);	// @ (0xF19 * 8 + 4)
#define                                 LC1G4D3N_bit        BANKMASK(CLC1GLS3), 4
// CLC1GLS3<LC1G4D3T>
extern volatile __bit                   LC1G4D3T            __at(0x78CD);	// @ (0xF19 * 8 + 5)
#define                                 LC1G4D3T_bit        BANKMASK(CLC1GLS3), 5
// CLC1GLS3<LC1G4D4N>
extern volatile __bit                   LC1G4D4N            __at(0x78CE);	// @ (0xF19 * 8 + 6)
#define                                 LC1G4D4N_bit        BANKMASK(CLC1GLS3), 6
// CLC1GLS3<LC1G4D4T>
extern volatile __bit                   LC1G4D4T            __at(0x78CF);	// @ (0xF19 * 8 + 7)
#define                                 LC1G4D4T_bit        BANKMASK(CLC1GLS3), 7
// CLC1POL<LC1G4POL>
extern volatile __bit                   LC1G4POL            __at(0x788B);	// @ (0xF11 * 8 + 3)
#define                                 LC1G4POL_bit        BANKMASK(CLC1POL), 3
// CLC1CON<LC1INTN>
extern volatile __bit                   LC1INTN             __at(0x7883);	// @ (0xF10 * 8 + 3)
#define                                 LC1INTN_bit         BANKMASK(CLC1CON), 3
// CLC1CON<LC1INTP>
extern volatile __bit                   LC1INTP             __at(0x7884);	// @ (0xF10 * 8 + 4)
#define                                 LC1INTP_bit         BANKMASK(CLC1CON), 4
// CLC1CON<LC1MODE0>
extern volatile __bit                   LC1MODE0            __at(0x7880);	// @ (0xF10 * 8 + 0)
#define                                 LC1MODE0_bit        BANKMASK(CLC1CON), 0
// CLC1CON<LC1MODE1>
extern volatile __bit                   LC1MODE1            __at(0x7881);	// @ (0xF10 * 8 + 1)
#define                                 LC1MODE1_bit        BANKMASK(CLC1CON), 1
// CLC1CON<LC1MODE2>
extern volatile __bit                   LC1MODE2            __at(0x7882);	// @ (0xF10 * 8 + 2)
#define                                 LC1MODE2_bit        BANKMASK(CLC1CON), 2
// CLC1CON<LC1OUT>
extern volatile __bit                   LC1OUT              __at(0x7885);	// @ (0xF10 * 8 + 5)
#define                                 LC1OUT_bit          BANKMASK(CLC1CON), 5
// CLC1POL<LC1POL>
extern volatile __bit                   LC1POL              __at(0x788F);	// @ (0xF11 * 8 + 7)
#define                                 LC1POL_bit          BANKMASK(CLC1POL), 7
// CLC2SEL0<LC2D1S0>
extern volatile __bit                   LC2D1S0             __at(0x78E0);	// @ (0xF1C * 8 + 0)
#define                                 LC2D1S0_bit         BANKMASK(CLC2SEL0), 0
// CLC2SEL0<LC2D1S1>
extern volatile __bit                   LC2D1S1             __at(0x78E1);	// @ (0xF1C * 8 + 1)
#define                                 LC2D1S1_bit         BANKMASK(CLC2SEL0), 1
// CLC2SEL0<LC2D1S2>
extern volatile __bit                   LC2D1S2             __at(0x78E2);	// @ (0xF1C * 8 + 2)
#define                                 LC2D1S2_bit         BANKMASK(CLC2SEL0), 2
// CLC2SEL0<LC2D1S3>
extern volatile __bit                   LC2D1S3             __at(0x78E3);	// @ (0xF1C * 8 + 3)
#define                                 LC2D1S3_bit         BANKMASK(CLC2SEL0), 3
// CLC2SEL0<LC2D1S4>
extern volatile __bit                   LC2D1S4             __at(0x78E4);	// @ (0xF1C * 8 + 4)
#define                                 LC2D1S4_bit         BANKMASK(CLC2SEL0), 4
// CLC2SEL1<LC2D2S0>
extern volatile __bit                   LC2D2S0             __at(0x78E8);	// @ (0xF1D * 8 + 0)
#define                                 LC2D2S0_bit         BANKMASK(CLC2SEL1), 0
// CLC2SEL1<LC2D2S1>
extern volatile __bit                   LC2D2S1             __at(0x78E9);	// @ (0xF1D * 8 + 1)
#define                                 LC2D2S1_bit         BANKMASK(CLC2SEL1), 1
// CLC2SEL1<LC2D2S2>
extern volatile __bit                   LC2D2S2             __at(0x78EA);	// @ (0xF1D * 8 + 2)
#define                                 LC2D2S2_bit         BANKMASK(CLC2SEL1), 2
// CLC2SEL1<LC2D2S3>
extern volatile __bit                   LC2D2S3             __at(0x78EB);	// @ (0xF1D * 8 + 3)
#define                                 LC2D2S3_bit         BANKMASK(CLC2SEL1), 3
// CLC2SEL1<LC2D2S4>
extern volatile __bit                   LC2D2S4             __at(0x78EC);	// @ (0xF1D * 8 + 4)
#define                                 LC2D2S4_bit         BANKMASK(CLC2SEL1), 4
// CLC2SEL2<LC2D3S0>
extern volatile __bit                   LC2D3S0             __at(0x78F0);	// @ (0xF1E * 8 + 0)
#define                                 LC2D3S0_bit         BANKMASK(CLC2SEL2), 0
// CLC2SEL2<LC2D3S1>
extern volatile __bit                   LC2D3S1             __at(0x78F1);	// @ (0xF1E * 8 + 1)
#define                                 LC2D3S1_bit         BANKMASK(CLC2SEL2), 1
// CLC2SEL2<LC2D3S2>
extern volatile __bit                   LC2D3S2             __at(0x78F2);	// @ (0xF1E * 8 + 2)
#define                                 LC2D3S2_bit         BANKMASK(CLC2SEL2), 2
// CLC2SEL2<LC2D3S3>
extern volatile __bit                   LC2D3S3             __at(0x78F3);	// @ (0xF1E * 8 + 3)
#define                                 LC2D3S3_bit         BANKMASK(CLC2SEL2), 3
// CLC2SEL2<LC2D3S4>
extern volatile __bit                   LC2D3S4             __at(0x78F4);	// @ (0xF1E * 8 + 4)
#define                                 LC2D3S4_bit         BANKMASK(CLC2SEL2), 4
// CLC2SEL3<LC2D4S0>
extern volatile __bit                   LC2D4S0             __at(0x78F8);	// @ (0xF1F * 8 + 0)
#define                                 LC2D4S0_bit         BANKMASK(CLC2SEL3), 0
// CLC2SEL3<LC2D4S1>
extern volatile __bit                   LC2D4S1             __at(0x78F9);	// @ (0xF1F * 8 + 1)
#define                                 LC2D4S1_bit         BANKMASK(CLC2SEL3), 1
// CLC2SEL3<LC2D4S2>
extern volatile __bit                   LC2D4S2             __at(0x78FA);	// @ (0xF1F * 8 + 2)
#define                                 LC2D4S2_bit         BANKMASK(CLC2SEL3), 2
// CLC2SEL3<LC2D4S3>
extern volatile __bit                   LC2D4S3             __at(0x78FB);	// @ (0xF1F * 8 + 3)
#define                                 LC2D4S3_bit         BANKMASK(CLC2SEL3), 3
// CLC2SEL3<LC2D4S4>
extern volatile __bit                   LC2D4S4             __at(0x78FC);	// @ (0xF1F * 8 + 4)
#define                                 LC2D4S4_bit         BANKMASK(CLC2SEL3), 4
// CLC2CON<LC2EN>
extern volatile __bit                   LC2EN               __at(0x78D7);	// @ (0xF1A * 8 + 7)
#define                                 LC2EN_bit           BANKMASK(CLC2CON), 7
// CLC2GLS0<LC2G1D1N>
extern volatile __bit                   LC2G1D1N            __at(0x7900);	// @ (0xF20 * 8 + 0)
#define                                 LC2G1D1N_bit        BANKMASK(CLC2GLS0), 0
// CLC2GLS0<LC2G1D1T>
extern volatile __bit                   LC2G1D1T            __at(0x7901);	// @ (0xF20 * 8 + 1)
#define                                 LC2G1D1T_bit        BANKMASK(CLC2GLS0), 1
// CLC2GLS0<LC2G1D2N>
extern volatile __bit                   LC2G1D2N            __at(0x7902);	// @ (0xF20 * 8 + 2)
#define                                 LC2G1D2N_bit        BANKMASK(CLC2GLS0), 2
// CLC2GLS0<LC2G1D2T>
extern volatile __bit                   LC2G1D2T            __at(0x7903);	// @ (0xF20 * 8 + 3)
#define                                 LC2G1D2T_bit        BANKMASK(CLC2GLS0), 3
// CLC2GLS0<LC2G1D3N>
extern volatile __bit                   LC2G1D3N            __at(0x7904);	// @ (0xF20 * 8 + 4)
#define                                 LC2G1D3N_bit        BANKMASK(CLC2GLS0), 4
// CLC2GLS0<LC2G1D3T>
extern volatile __bit                   LC2G1D3T            __at(0x7905);	// @ (0xF20 * 8 + 5)
#define                                 LC2G1D3T_bit        BANKMASK(CLC2GLS0), 5
// CLC2GLS0<LC2G1D4N>
extern volatile __bit                   LC2G1D4N            __at(0x7906);	// @ (0xF20 * 8 + 6)
#define                                 LC2G1D4N_bit        BANKMASK(CLC2GLS0), 6
// CLC2GLS0<LC2G1D4T>
extern volatile __bit                   LC2G1D4T            __at(0x7907);	// @ (0xF20 * 8 + 7)
#define                                 LC2G1D4T_bit        BANKMASK(CLC2GLS0), 7
// CLC2POL<LC2G1POL>
extern volatile __bit                   LC2G1POL            __at(0x78D8);	// @ (0xF1B * 8 + 0)
#define                                 LC2G1POL_bit        BANKMASK(CLC2POL), 0
// CLC2GLS1<LC2G2D1N>
extern volatile __bit                   LC2G2D1N            __at(0x7908);	// @ (0xF21 * 8 + 0)
#define                                 LC2G2D1N_bit        BANKMASK(CLC2GLS1), 0
// CLC2GLS1<LC2G2D1T>
extern volatile __bit                   LC2G2D1T            __at(0x7909);	// @ (0xF21 * 8 + 1)
#define                                 LC2G2D1T_bit        BANKMASK(CLC2GLS1), 1
// CLC2GLS1<LC2G2D2N>
extern volatile __bit                   LC2G2D2N            __at(0x790A);	// @ (0xF21 * 8 + 2)
#define                                 LC2G2D2N_bit        BANKMASK(CLC2GLS1), 2
// CLC2GLS1<LC2G2D2T>
extern volatile __bit                   LC2G2D2T            __at(0x790B);	// @ (0xF21 * 8 + 3)
#define                                 LC2G2D2T_bit        BANKMASK(CLC2GLS1), 3
// CLC2GLS1<LC2G2D3N>
extern volatile __bit                   LC2G2D3N            __at(0x790C);	// @ (0xF21 * 8 + 4)
#define                                 LC2G2D3N_bit        BANKMASK(CLC2GLS1), 4
// CLC2GLS1<LC2G2D3T>
extern volatile __bit                   LC2G2D3T            __at(0x790D);	// @ (0xF21 * 8 + 5)
#define                                 LC2G2D3T_bit        BANKMASK(CLC2GLS1), 5
// CLC2GLS1<LC2G2D4N>
extern volatile __bit                   LC2G2D4N            __at(0x790E);	// @ (0xF21 * 8 + 6)
#define                                 LC2G2D4N_bit        BANKMASK(CLC2GLS1), 6
// CLC2GLS1<LC2G2D4T>
extern volatile __bit                   LC2G2D4T            __at(0x790F);	// @ (0xF21 * 8 + 7)
#define                                 LC2G2D4T_bit        BANKMASK(CLC2GLS1), 7
// CLC2POL<LC2G2POL>
extern volatile __bit                   LC2G2POL            __at(0x78D9);	// @ (0xF1B * 8 + 1)
#define                                 LC2G2POL_bit        BANKMASK(CLC2POL), 1
// CLC2GLS2<LC2G3D1N>
extern volatile __bit                   LC2G3D1N            __at(0x7910);	// @ (0xF22 * 8 + 0)
#define                                 LC2G3D1N_bit        BANKMASK(CLC2GLS2), 0
// CLC2GLS2<LC2G3D1T>
extern volatile __bit                   LC2G3D1T            __at(0x7911);	// @ (0xF22 * 8 + 1)
#define                                 LC2G3D1T_bit        BANKMASK(CLC2GLS2), 1
// CLC2GLS2<LC2G3D2N>
extern volatile __bit                   LC2G3D2N            __at(0x7912);	// @ (0xF22 * 8 + 2)
#define                                 LC2G3D2N_bit        BANKMASK(CLC2GLS2), 2
// CLC2GLS2<LC2G3D2T>
extern volatile __bit                   LC2G3D2T            __at(0x7913);	// @ (0xF22 * 8 + 3)
#define                                 LC2G3D2T_bit        BANKMASK(CLC2GLS2), 3
// CLC2GLS2<LC2G3D3N>
extern volatile __bit                   LC2G3D3N            __at(0x7914);	// @ (0xF22 * 8 + 4)
#define                                 LC2G3D3N_bit        BANKMASK(CLC2GLS2), 4
// CLC2GLS2<LC2G3D3T>
extern volatile __bit                   LC2G3D3T            __at(0x7915);	// @ (0xF22 * 8 + 5)
#define                                 LC2G3D3T_bit        BANKMASK(CLC2GLS2), 5
// CLC2GLS2<LC2G3D4N>
extern volatile __bit                   LC2G3D4N            __at(0x7916);	// @ (0xF22 * 8 + 6)
#define                                 LC2G3D4N_bit        BANKMASK(CLC2GLS2), 6
// CLC2GLS2<LC2G3D4T>
extern volatile __bit                   LC2G3D4T            __at(0x7917);	// @ (0xF22 * 8 + 7)
#define                                 LC2G3D4T_bit        BANKMASK(CLC2GLS2), 7
// CLC2POL<LC2G3POL>
extern volatile __bit                   LC2G3POL            __at(0x78DA);	// @ (0xF1B * 8 + 2)
#define                                 LC2G3POL_bit        BANKMASK(CLC2POL), 2
// CLC2GLS3<LC2G4D1N>
extern volatile __bit                   LC2G4D1N            __at(0x7918);	// @ (0xF23 * 8 + 0)
#define                                 LC2G4D1N_bit        BANKMASK(CLC2GLS3), 0
// CLC2GLS3<LC2G4D1T>
extern volatile __bit                   LC2G4D1T            __at(0x7919);	// @ (0xF23 * 8 + 1)
#define                                 LC2G4D1T_bit        BANKMASK(CLC2GLS3), 1
// CLC2GLS3<LC2G4D2N>
extern volatile __bit                   LC2G4D2N            __at(0x791A);	// @ (0xF23 * 8 + 2)
#define                                 LC2G4D2N_bit        BANKMASK(CLC2GLS3), 2
// CLC2GLS3<LC2G4D2T>
extern volatile __bit                   LC2G4D2T            __at(0x791B);	// @ (0xF23 * 8 + 3)
#define                                 LC2G4D2T_bit        BANKMASK(CLC2GLS3), 3
// CLC2GLS3<LC2G4D3N>
extern volatile __bit                   LC2G4D3N            __at(0x791C);	// @ (0xF23 * 8 + 4)
#define                                 LC2G4D3N_bit        BANKMASK(CLC2GLS3), 4
// CLC2GLS3<LC2G4D3T>
extern volatile __bit                   LC2G4D3T            __at(0x791D);	// @ (0xF23 * 8 + 5)
#define                                 LC2G4D3T_bit        BANKMASK(CLC2GLS3), 5
// CLC2GLS3<LC2G4D4N>
extern volatile __bit                   LC2G4D4N            __at(0x791E);	// @ (0xF23 * 8 + 6)
#define                                 LC2G4D4N_bit        BANKMASK(CLC2GLS3), 6
// CLC2GLS3<LC2G4D4T>
extern volatile __bit                   LC2G4D4T            __at(0x791F);	// @ (0xF23 * 8 + 7)
#define                                 LC2G4D4T_bit        BANKMASK(CLC2GLS3), 7
// CLC2POL<LC2G4POL>
extern volatile __bit                   LC2G4POL            __at(0x78DB);	// @ (0xF1B * 8 + 3)
#define                                 LC2G4POL_bit        BANKMASK(CLC2POL), 3
// CLC2CON<LC2INTN>
extern volatile __bit                   LC2INTN             __at(0x78D3);	// @ (0xF1A * 8 + 3)
#define                                 LC2INTN_bit         BANKMASK(CLC2CON), 3
// CLC2CON<LC2INTP>
extern volatile __bit                   LC2INTP             __at(0x78D4);	// @ (0xF1A * 8 + 4)
#define                                 LC2INTP_bit         BANKMASK(CLC2CON), 4
// CLC2CON<LC2MODE0>
extern volatile __bit                   LC2MODE0            __at(0x78D0);	// @ (0xF1A * 8 + 0)
#define                                 LC2MODE0_bit        BANKMASK(CLC2CON), 0
// CLC2CON<LC2MODE1>
extern volatile __bit                   LC2MODE1            __at(0x78D1);	// @ (0xF1A * 8 + 1)
#define                                 LC2MODE1_bit        BANKMASK(CLC2CON), 1
// CLC2CON<LC2MODE2>
extern volatile __bit                   LC2MODE2            __at(0x78D2);	// @ (0xF1A * 8 + 2)
#define                                 LC2MODE2_bit        BANKMASK(CLC2CON), 2
// CLC2CON<LC2OUT>
extern volatile __bit                   LC2OUT              __at(0x78D5);	// @ (0xF1A * 8 + 5)
#define                                 LC2OUT_bit          BANKMASK(CLC2CON), 5
// CLC2POL<LC2POL>
extern volatile __bit                   LC2POL              __at(0x78DF);	// @ (0xF1B * 8 + 7)
#define                                 LC2POL_bit          BANKMASK(CLC2POL), 7
// CLC3SEL0<LC3D1S0>
extern volatile __bit                   LC3D1S0             __at(0x7930);	// @ (0xF26 * 8 + 0)
#define                                 LC3D1S0_bit         BANKMASK(CLC3SEL0), 0
// CLC3SEL0<LC3D1S1>
extern volatile __bit                   LC3D1S1             __at(0x7931);	// @ (0xF26 * 8 + 1)
#define                                 LC3D1S1_bit         BANKMASK(CLC3SEL0), 1
// CLC3SEL0<LC3D1S2>
extern volatile __bit                   LC3D1S2             __at(0x7932);	// @ (0xF26 * 8 + 2)
#define                                 LC3D1S2_bit         BANKMASK(CLC3SEL0), 2
// CLC3SEL0<LC3D1S3>
extern volatile __bit                   LC3D1S3             __at(0x7933);	// @ (0xF26 * 8 + 3)
#define                                 LC3D1S3_bit         BANKMASK(CLC3SEL0), 3
// CLC3SEL0<LC3D1S4>
extern volatile __bit                   LC3D1S4             __at(0x7934);	// @ (0xF26 * 8 + 4)
#define                                 LC3D1S4_bit         BANKMASK(CLC3SEL0), 4
// CLC3SEL1<LC3D2S0>
extern volatile __bit                   LC3D2S0             __at(0x7938);	// @ (0xF27 * 8 + 0)
#define                                 LC3D2S0_bit         BANKMASK(CLC3SEL1), 0
// CLC3SEL1<LC3D2S1>
extern volatile __bit                   LC3D2S1             __at(0x7939);	// @ (0xF27 * 8 + 1)
#define                                 LC3D2S1_bit         BANKMASK(CLC3SEL1), 1
// CLC3SEL1<LC3D2S2>
extern volatile __bit                   LC3D2S2             __at(0x793A);	// @ (0xF27 * 8 + 2)
#define                                 LC3D2S2_bit         BANKMASK(CLC3SEL1), 2
// CLC3SEL1<LC3D2S3>
extern volatile __bit                   LC3D2S3             __at(0x793B);	// @ (0xF27 * 8 + 3)
#define                                 LC3D2S3_bit         BANKMASK(CLC3SEL1), 3
// CLC3SEL1<LC3D2S4>
extern volatile __bit                   LC3D2S4             __at(0x793C);	// @ (0xF27 * 8 + 4)
#define                                 LC3D2S4_bit         BANKMASK(CLC3SEL1), 4
// CLC3SEL2<LC3D3S0>
extern volatile __bit                   LC3D3S0             __at(0x7940);	// @ (0xF28 * 8 + 0)
#define                                 LC3D3S0_bit         BANKMASK(CLC3SEL2), 0
// CLC3SEL2<LC3D3S1>
extern volatile __bit                   LC3D3S1             __at(0x7941);	// @ (0xF28 * 8 + 1)
#define                                 LC3D3S1_bit         BANKMASK(CLC3SEL2), 1
// CLC3SEL2<LC3D3S2>
extern volatile __bit                   LC3D3S2             __at(0x7942);	// @ (0xF28 * 8 + 2)
#define                                 LC3D3S2_bit         BANKMASK(CLC3SEL2), 2
// CLC3SEL2<LC3D3S3>
extern volatile __bit                   LC3D3S3             __at(0x7943);	// @ (0xF28 * 8 + 3)
#define                                 LC3D3S3_bit         BANKMASK(CLC3SEL2), 3
// CLC3SEL2<LC3D3S4>
extern volatile __bit                   LC3D3S4             __at(0x7944);	// @ (0xF28 * 8 + 4)
#define                                 LC3D3S4_bit         BANKMASK(CLC3SEL2), 4
// CLC3SEL3<LC3D4S0>
extern volatile __bit                   LC3D4S0             __at(0x7948);	// @ (0xF29 * 8 + 0)
#define                                 LC3D4S0_bit         BANKMASK(CLC3SEL3), 0
// CLC3SEL3<LC3D4S1>
extern volatile __bit                   LC3D4S1             __at(0x7949);	// @ (0xF29 * 8 + 1)
#define                                 LC3D4S1_bit         BANKMASK(CLC3SEL3), 1
// CLC3SEL3<LC3D4S2>
extern volatile __bit                   LC3D4S2             __at(0x794A);	// @ (0xF29 * 8 + 2)
#define                                 LC3D4S2_bit         BANKMASK(CLC3SEL3), 2
// CLC3SEL3<LC3D4S3>
extern volatile __bit                   LC3D4S3             __at(0x794B);	// @ (0xF29 * 8 + 3)
#define                                 LC3D4S3_bit         BANKMASK(CLC3SEL3), 3
// CLC3SEL3<LC3D4S4>
extern volatile __bit                   LC3D4S4             __at(0x794C);	// @ (0xF29 * 8 + 4)
#define                                 LC3D4S4_bit         BANKMASK(CLC3SEL3), 4
// CLC3CON<LC3EN>
extern volatile __bit                   LC3EN               __at(0x7927);	// @ (0xF24 * 8 + 7)
#define                                 LC3EN_bit           BANKMASK(CLC3CON), 7
// CLC3GLS0<LC3G1D1N>
extern volatile __bit                   LC3G1D1N            __at(0x7950);	// @ (0xF2A * 8 + 0)
#define                                 LC3G1D1N_bit        BANKMASK(CLC3GLS0), 0
// CLC3GLS0<LC3G1D1T>
extern volatile __bit                   LC3G1D1T            __at(0x7951);	// @ (0xF2A * 8 + 1)
#define                                 LC3G1D1T_bit        BANKMASK(CLC3GLS0), 1
// CLC3GLS0<LC3G1D2N>
extern volatile __bit                   LC3G1D2N            __at(0x7952);	// @ (0xF2A * 8 + 2)
#define                                 LC3G1D2N_bit        BANKMASK(CLC3GLS0), 2
// CLC3GLS0<LC3G1D2T>
extern volatile __bit                   LC3G1D2T            __at(0x7953);	// @ (0xF2A * 8 + 3)
#define                                 LC3G1D2T_bit        BANKMASK(CLC3GLS0), 3
// CLC3GLS0<LC3G1D3N>
extern volatile __bit                   LC3G1D3N            __at(0x7954);	// @ (0xF2A * 8 + 4)
#define                                 LC3G1D3N_bit        BANKMASK(CLC3GLS0), 4
// CLC3GLS0<LC3G1D3T>
extern volatile __bit                   LC3G1D3T            __at(0x7955);	// @ (0xF2A * 8 + 5)
#define                                 LC3G1D3T_bit        BANKMASK(CLC3GLS0), 5
// CLC3GLS0<LC3G1D4N>
extern volatile __bit                   LC3G1D4N            __at(0x7956);	// @ (0xF2A * 8 + 6)
#define                                 LC3G1D4N_bit        BANKMASK(CLC3GLS0), 6
// CLC3GLS0<LC3G1D4T>
extern volatile __bit                   LC3G1D4T            __at(0x7957);	// @ (0xF2A * 8 + 7)
#define                                 LC3G1D4T_bit        BANKMASK(CLC3GLS0), 7
// CLC3POL<LC3G1POL>
extern volatile __bit                   LC3G1POL            __at(0x7928);	// @ (0xF25 * 8 + 0)
#define                                 LC3G1POL_bit        BANKMASK(CLC3POL), 0
// CLC3GLS1<LC3G2D1N>
extern volatile __bit                   LC3G2D1N            __at(0x7958);	// @ (0xF2B * 8 + 0)
#define                                 LC3G2D1N_bit        BANKMASK(CLC3GLS1), 0
// CLC3GLS1<LC3G2D1T>
extern volatile __bit                   LC3G2D1T            __at(0x7959);	// @ (0xF2B * 8 + 1)
#define                                 LC3G2D1T_bit        BANKMASK(CLC3GLS1), 1
// CLC3GLS1<LC3G2D2N>
extern volatile __bit                   LC3G2D2N            __at(0x795A);	// @ (0xF2B * 8 + 2)
#define                                 LC3G2D2N_bit        BANKMASK(CLC3GLS1), 2
// CLC3GLS1<LC3G2D2T>
extern volatile __bit                   LC3G2D2T            __at(0x795B);	// @ (0xF2B * 8 + 3)
#define                                 LC3G2D2T_bit        BANKMASK(CLC3GLS1), 3
// CLC3GLS1<LC3G2D3N>
extern volatile __bit                   LC3G2D3N            __at(0x795C);	// @ (0xF2B * 8 + 4)
#define                                 LC3G2D3N_bit        BANKMASK(CLC3GLS1), 4
// CLC3GLS1<LC3G2D3T>
extern volatile __bit                   LC3G2D3T            __at(0x795D);	// @ (0xF2B * 8 + 5)
#define                                 LC3G2D3T_bit        BANKMASK(CLC3GLS1), 5
// CLC3GLS1<LC3G2D4N>
extern volatile __bit                   LC3G2D4N            __at(0x795E);	// @ (0xF2B * 8 + 6)
#define                                 LC3G2D4N_bit        BANKMASK(CLC3GLS1), 6
// CLC3GLS1<LC3G2D4T>
extern volatile __bit                   LC3G2D4T            __at(0x795F);	// @ (0xF2B * 8 + 7)
#define                                 LC3G2D4T_bit        BANKMASK(CLC3GLS1), 7
// CLC3POL<LC3G2POL>
extern volatile __bit                   LC3G2POL            __at(0x7929);	// @ (0xF25 * 8 + 1)
#define                                 LC3G2POL_bit        BANKMASK(CLC3POL), 1
// CLC3GLS2<LC3G3D1N>
extern volatile __bit                   LC3G3D1N            __at(0x7960);	// @ (0xF2C * 8 + 0)
#define                                 LC3G3D1N_bit        BANKMASK(CLC3GLS2), 0
// CLC3GLS2<LC3G3D1T>
extern volatile __bit                   LC3G3D1T            __at(0x7961);	// @ (0xF2C * 8 + 1)
#define                                 LC3G3D1T_bit        BANKMASK(CLC3GLS2), 1
// CLC3GLS2<LC3G3D2N>
extern volatile __bit                   LC3G3D2N            __at(0x7962);	// @ (0xF2C * 8 + 2)
#define                                 LC3G3D2N_bit        BANKMASK(CLC3GLS2), 2
// CLC3GLS2<LC3G3D2T>
extern volatile __bit                   LC3G3D2T            __at(0x7963);	// @ (0xF2C * 8 + 3)
#define                                 LC3G3D2T_bit        BANKMASK(CLC3GLS2), 3
// CLC3GLS2<LC3G3D3N>
extern volatile __bit                   LC3G3D3N            __at(0x7964);	// @ (0xF2C * 8 + 4)
#define                                 LC3G3D3N_bit        BANKMASK(CLC3GLS2), 4
// CLC3GLS2<LC3G3D3T>
extern volatile __bit                   LC3G3D3T            __at(0x7965);	// @ (0xF2C * 8 + 5)
#define                                 LC3G3D3T_bit        BANKMASK(CLC3GLS2), 5
// CLC3GLS2<LC3G3D4N>
extern volatile __bit                   LC3G3D4N            __at(0x7966);	// @ (0xF2C * 8 + 6)
#define                                 LC3G3D4N_bit        BANKMASK(CLC3GLS2), 6
// CLC3GLS2<LC3G3D4T>
extern volatile __bit                   LC3G3D4T            __at(0x7967);	// @ (0xF2C * 8 + 7)
#define                                 LC3G3D4T_bit        BANKMASK(CLC3GLS2), 7
// CLC3POL<LC3G3POL>
extern volatile __bit                   LC3G3POL            __at(0x792A);	// @ (0xF25 * 8 + 2)
#define                                 LC3G3POL_bit        BANKMASK(CLC3POL), 2
// CLC3GLS3<LC3G4D1N>
extern volatile __bit                   LC3G4D1N            __at(0x7968);	// @ (0xF2D * 8 + 0)
#define                                 LC3G4D1N_bit        BANKMASK(CLC3GLS3), 0
// CLC3GLS3<LC3G4D1T>
extern volatile __bit                   LC3G4D1T            __at(0x7969);	// @ (0xF2D * 8 + 1)
#define                                 LC3G4D1T_bit        BANKMASK(CLC3GLS3), 1
// CLC3GLS3<LC3G4D2N>
extern volatile __bit                   LC3G4D2N            __at(0x796A);	// @ (0xF2D * 8 + 2)
#define                                 LC3G4D2N_bit        BANKMASK(CLC3GLS3), 2
// CLC3GLS3<LC3G4D2T>
extern volatile __bit                   LC3G4D2T            __at(0x796B);	// @ (0xF2D * 8 + 3)
#define                                 LC3G4D2T_bit        BANKMASK(CLC3GLS3), 3
// CLC3GLS3<LC3G4D3N>
extern volatile __bit                   LC3G4D3N            __at(0x796C);	// @ (0xF2D * 8 + 4)
#define                                 LC3G4D3N_bit        BANKMASK(CLC3GLS3), 4
// CLC3GLS3<LC3G4D3T>
extern volatile __bit                   LC3G4D3T            __at(0x796D);	// @ (0xF2D * 8 + 5)
#define                                 LC3G4D3T_bit        BANKMASK(CLC3GLS3), 5
// CLC3GLS3<LC3G4D4N>
extern volatile __bit                   LC3G4D4N            __at(0x796E);	// @ (0xF2D * 8 + 6)
#define                                 LC3G4D4N_bit        BANKMASK(CLC3GLS3), 6
// CLC3GLS3<LC3G4D4T>
extern volatile __bit                   LC3G4D4T            __at(0x796F);	// @ (0xF2D * 8 + 7)
#define                                 LC3G4D4T_bit        BANKMASK(CLC3GLS3), 7
// CLC3POL<LC3G4POL>
extern volatile __bit                   LC3G4POL            __at(0x792B);	// @ (0xF25 * 8 + 3)
#define                                 LC3G4POL_bit        BANKMASK(CLC3POL), 3
// CLC3CON<LC3INTN>
extern volatile __bit                   LC3INTN             __at(0x7923);	// @ (0xF24 * 8 + 3)
#define                                 LC3INTN_bit         BANKMASK(CLC3CON), 3
// CLC3CON<LC3INTP>
extern volatile __bit                   LC3INTP             __at(0x7924);	// @ (0xF24 * 8 + 4)
#define                                 LC3INTP_bit         BANKMASK(CLC3CON), 4
// CLC3CON<LC3MODE0>
extern volatile __bit                   LC3MODE0            __at(0x7920);	// @ (0xF24 * 8 + 0)
#define                                 LC3MODE0_bit        BANKMASK(CLC3CON), 0
// CLC3CON<LC3MODE1>
extern volatile __bit                   LC3MODE1            __at(0x7921);	// @ (0xF24 * 8 + 1)
#define                                 LC3MODE1_bit        BANKMASK(CLC3CON), 1
// CLC3CON<LC3MODE2>
extern volatile __bit                   LC3MODE2            __at(0x7922);	// @ (0xF24 * 8 + 2)
#define                                 LC3MODE2_bit        BANKMASK(CLC3CON), 2
// CLC3CON<LC3OUT>
extern volatile __bit                   LC3OUT              __at(0x7925);	// @ (0xF24 * 8 + 5)
#define                                 LC3OUT_bit          BANKMASK(CLC3CON), 5
// CLC3POL<LC3POL>
extern volatile __bit                   LC3POL              __at(0x792F);	// @ (0xF25 * 8 + 7)
#define                                 LC3POL_bit          BANKMASK(CLC3POL), 7
// OSCSTAT<LFIOFR>
extern volatile __bit                   LFIOFR              __at(0x4D1);	// @ (0x9A * 8 + 1)
#define                                 LFIOFR_bit          BANKMASK(OSCSTAT), 1
// PMCON1<LWLO>
extern volatile __bit                   LWLO                __at(0xCAD);	// @ (0x195 * 8 + 5)
#define                                 LWLO_bit            BANKMASK(PMCON1), 5
// CMOUT<MC1OUT>
extern volatile __bit                   MC1OUT              __at(0x8A8);	// @ (0x115 * 8 + 0)
#define                                 MC1OUT_bit          BANKMASK(CMOUT), 0
// CMOUT<MC2OUT>
extern volatile __bit                   MC2OUT              __at(0x8A9);	// @ (0x115 * 8 + 1)
#define                                 MC2OUT_bit          BANKMASK(CMOUT), 1
// CLCDATA<MCLC1OUT>
extern volatile __bit                   MCLC1OUT            __at(0x7878);	// @ (0xF0F * 8 + 0)
#define                                 MCLC1OUT_bit        BANKMASK(CLCDATA), 0
// CLCDATA<MCLC2OUT>
extern volatile __bit                   MCLC2OUT            __at(0x7879);	// @ (0xF0F * 8 + 1)
#define                                 MCLC2OUT_bit        BANKMASK(CLCDATA), 1
// CLCDATA<MCLC3OUT>
extern volatile __bit                   MCLC3OUT            __at(0x787A);	// @ (0xF0F * 8 + 2)
#define                                 MCLC3OUT_bit        BANKMASK(CLCDATA), 2
// OSCSTAT<MFIOFR>
extern volatile __bit                   MFIOFR              __at(0x4D2);	// @ (0x9A * 8 + 2)
#define                                 MFIOFR_bit          BANKMASK(OSCSTAT), 2
// SSP1MSK<MSK0>
extern volatile __bit                   MSK0                __at(0x1098);	// @ (0x213 * 8 + 0)
#define                                 MSK0_bit            BANKMASK(SSP1MSK), 0
// SSP1MSK<MSK1>
extern volatile __bit                   MSK1                __at(0x1099);	// @ (0x213 * 8 + 1)
#define                                 MSK1_bit            BANKMASK(SSP1MSK), 1
// SSP1MSK<MSK2>
extern volatile __bit                   MSK2                __at(0x109A);	// @ (0x213 * 8 + 2)
#define                                 MSK2_bit            BANKMASK(SSP1MSK), 2
// SSP1MSK<MSK3>
extern volatile __bit                   MSK3                __at(0x109B);	// @ (0x213 * 8 + 3)
#define                                 MSK3_bit            BANKMASK(SSP1MSK), 3
// SSP1MSK<MSK4>
extern volatile __bit                   MSK4                __at(0x109C);	// @ (0x213 * 8 + 4)
#define                                 MSK4_bit            BANKMASK(SSP1MSK), 4
// SSP1MSK<MSK5>
extern volatile __bit                   MSK5                __at(0x109D);	// @ (0x213 * 8 + 5)
#define                                 MSK5_bit            BANKMASK(SSP1MSK), 5
// SSP1MSK<MSK6>
extern volatile __bit                   MSK6                __at(0x109E);	// @ (0x213 * 8 + 6)
#define                                 MSK6_bit            BANKMASK(SSP1MSK), 6
// SSP1MSK<MSK7>
extern volatile __bit                   MSK7                __at(0x109F);	// @ (0x213 * 8 + 7)
#define                                 MSK7_bit            BANKMASK(SSP1MSK), 7
// ODCONA<ODA0>
extern volatile __bit                   ODA0                __at(0x1460);	// @ (0x28C * 8 + 0)
#define                                 ODA0_bit            BANKMASK(ODCONA), 0
// ODCONA<ODA1>
extern volatile __bit                   ODA1                __at(0x1461);	// @ (0x28C * 8 + 1)
#define                                 ODA1_bit            BANKMASK(ODCONA), 1
// ODCONA<ODA2>
extern volatile __bit                   ODA2                __at(0x1462);	// @ (0x28C * 8 + 2)
#define                                 ODA2_bit            BANKMASK(ODCONA), 2
// ODCONA<ODA4>
extern volatile __bit                   ODA4                __at(0x1464);	// @ (0x28C * 8 + 4)
#define                                 ODA4_bit            BANKMASK(ODCONA), 4
// ODCONA<ODA5>
extern volatile __bit                   ODA5                __at(0x1465);	// @ (0x28C * 8 + 5)
#define                                 ODA5_bit            BANKMASK(ODCONA), 5
// ODCONB<ODB4>
extern volatile __bit                   ODB4                __at(0x146C);	// @ (0x28D * 8 + 4)
#define                                 ODB4_bit            BANKMASK(ODCONB), 4
// ODCONB<ODB5>
extern volatile __bit                   ODB5                __at(0x146D);	// @ (0x28D * 8 + 5)
#define                                 ODB5_bit            BANKMASK(ODCONB), 5
// ODCONB<ODB6>
extern volatile __bit                   ODB6                __at(0x146E);	// @ (0x28D * 8 + 6)
#define                                 ODB6_bit            BANKMASK(ODCONB), 6
// ODCONB<ODB7>
extern volatile __bit                   ODB7                __at(0x146F);	// @ (0x28D * 8 + 7)
#define                                 ODB7_bit            BANKMASK(ODCONB), 7
// ODCONC<ODC0>
extern volatile __bit                   ODC0                __at(0x1470);	// @ (0x28E * 8 + 0)
#define                                 ODC0_bit            BANKMASK(ODCONC), 0
// ODCONC<ODC1>
extern volatile __bit                   ODC1                __at(0x1471);	// @ (0x28E * 8 + 1)
#define                                 ODC1_bit            BANKMASK(ODCONC), 1
// ODCONC<ODC2>
extern volatile __bit                   ODC2                __at(0x1472);	// @ (0x28E * 8 + 2)
#define                                 ODC2_bit            BANKMASK(ODCONC), 2
// ODCONC<ODC3>
extern volatile __bit                   ODC3                __at(0x1473);	// @ (0x28E * 8 + 3)
#define                                 ODC3_bit            BANKMASK(ODCONC), 3
// ODCONC<ODC4>
extern volatile __bit                   ODC4                __at(0x1474);	// @ (0x28E * 8 + 4)
#define                                 ODC4_bit            BANKMASK(ODCONC), 4
// ODCONC<ODC5>
extern volatile __bit                   ODC5                __at(0x1475);	// @ (0x28E * 8 + 5)
#define                                 ODC5_bit            BANKMASK(ODCONC), 5
// ODCONC<ODC6>
extern volatile __bit                   ODC6                __at(0x1476);	// @ (0x28E * 8 + 6)
#define                                 ODC6_bit            BANKMASK(ODCONC), 6
// ODCONC<ODC7>
extern volatile __bit                   ODC7                __at(0x1477);	// @ (0x28E * 8 + 7)
#define                                 ODC7_bit            BANKMASK(ODCONC), 7
// RC1STA<OERR>
extern volatile __bit                   OERR                __at(0xCE9);	// @ (0x19D * 8 + 1)
#define                                 OERR_bit            BANKMASK(RC1STA), 1
// OPA1CON<OPA1EN>
extern volatile __bit                   OPA1EN              __at(0x288F);	// @ (0x511 * 8 + 7)
#define                                 OPA1EN_bit          BANKMASK(OPA1CON), 7
// OPA1CON<OPA1PCH0>
extern volatile __bit                   OPA1PCH0            __at(0x2888);	// @ (0x511 * 8 + 0)
#define                                 OPA1PCH0_bit        BANKMASK(OPA1CON), 0
// OPA1CON<OPA1PCH1>
extern volatile __bit                   OPA1PCH1            __at(0x2889);	// @ (0x511 * 8 + 1)
#define                                 OPA1PCH1_bit        BANKMASK(OPA1CON), 1
// OPA1CON<OPA1SP>
extern volatile __bit                   OPA1SP              __at(0x288E);	// @ (0x511 * 8 + 6)
#define                                 OPA1SP_bit          BANKMASK(OPA1CON), 6
// OPA1CON<OPA1UG>
extern volatile __bit                   OPA1UG              __at(0x288C);	// @ (0x511 * 8 + 4)
#define                                 OPA1UG_bit          BANKMASK(OPA1CON), 4
// OPA2CON<OPA2EN>
extern volatile __bit                   OPA2EN              __at(0x28AF);	// @ (0x515 * 8 + 7)
#define                                 OPA2EN_bit          BANKMASK(OPA2CON), 7
// OPA2CON<OPA2PCH0>
extern volatile __bit                   OPA2PCH0            __at(0x28A8);	// @ (0x515 * 8 + 0)
#define                                 OPA2PCH0_bit        BANKMASK(OPA2CON), 0
// OPA2CON<OPA2PCH1>
extern volatile __bit                   OPA2PCH1            __at(0x28A9);	// @ (0x515 * 8 + 1)
#define                                 OPA2PCH1_bit        BANKMASK(OPA2CON), 1
// OPA2CON<OPA2SP>
extern volatile __bit                   OPA2SP              __at(0x28AE);	// @ (0x515 * 8 + 6)
#define                                 OPA2SP_bit          BANKMASK(OPA2CON), 6
// OPA2CON<OPA2UG>
extern volatile __bit                   OPA2UG              __at(0x28AC);	// @ (0x515 * 8 + 4)
#define                                 OPA2UG_bit          BANKMASK(OPA2CON), 4
// PIE2<OSFIE>
extern volatile __bit                   OSFIE               __at(0x497);	// @ (0x92 * 8 + 7)
#define                                 OSFIE_bit           BANKMASK(PIE2), 7
// PIR2<OSFIF>
extern volatile __bit                   OSFIF               __at(0x97);	// @ (0x12 * 8 + 7)
#define                                 OSFIF_bit           BANKMASK(PIR2), 7
// OSCSTAT<OSTS>
extern volatile __bit                   OSTS                __at(0x4D5);	// @ (0x9A * 8 + 5)
#define                                 OSTS_bit            BANKMASK(OSCSTAT), 5
// CCPTMRS<P3TSEL0>
extern volatile __bit                   P3TSEL0             __at(0x14F4);	// @ (0x29E * 8 + 4)
#define                                 P3TSEL0_bit         BANKMASK(CCPTMRS), 4
// CCPTMRS<P3TSEL1>
extern volatile __bit                   P3TSEL1             __at(0x14F5);	// @ (0x29E * 8 + 5)
#define                                 P3TSEL1_bit         BANKMASK(CCPTMRS), 5
// CCPTMRS<P4TSEL0>
extern volatile __bit                   P4TSEL0             __at(0x14F6);	// @ (0x29E * 8 + 6)
#define                                 P4TSEL0_bit         BANKMASK(CCPTMRS), 6
// CCPTMRS<P4TSEL1>
extern volatile __bit                   P4TSEL1             __at(0x14F7);	// @ (0x29E * 8 + 7)
#define                                 P4TSEL1_bit         BANKMASK(CCPTMRS), 7
// SSP1CON3<PCIE>
extern volatile __bit                   PCIE                __at(0x10BE);	// @ (0x217 * 8 + 6)
#define                                 PCIE_bit            BANKMASK(SSP1CON3), 6
// INTCON<PEIE>
extern volatile __bit                   PEIE                __at(0x5E);	// @ (0xB * 8 + 6)
#define                                 PEIE_bit            BANKMASK(INTCON), 6
// SSP1CON2<PEN>
extern volatile __bit                   PEN                 __at(0x10B2);	// @ (0x216 * 8 + 2)
#define                                 PEN_bit             BANKMASK(SSP1CON2), 2
// OSCSTAT<PLLR>
extern volatile __bit                   PLLR                __at(0x4D6);	// @ (0x9A * 8 + 6)
#define                                 PLLR_bit            BANKMASK(OSCSTAT), 6
// PPSLOCK<PPSLOCKED>
extern volatile __bit                   PPSLOCKED           __at(0x7078);	// @ (0xE0F * 8 + 0)
#define                                 PPSLOCKED_bit       BANKMASK(PPSLOCK), 0
// OPTION_REG<PS0>
extern volatile __bit                   PS0                 __at(0x4A8);	// @ (0x95 * 8 + 0)
#define                                 PS0_bit             BANKMASK(OPTION_REG), 0
// OPTION_REG<PS1>
extern volatile __bit                   PS1                 __at(0x4A9);	// @ (0x95 * 8 + 1)
#define                                 PS1_bit             BANKMASK(OPTION_REG), 1
// OPTION_REG<PS2>
extern volatile __bit                   PS2                 __at(0x4AA);	// @ (0x95 * 8 + 2)
#define                                 PS2_bit             BANKMASK(OPTION_REG), 2
// OPTION_REG<PSA>
extern volatile __bit                   PSA                 __at(0x4AB);	// @ (0x95 * 8 + 3)
#define                                 PSA_bit             BANKMASK(OPTION_REG), 3
// PWM3DCH<PWM3DCH0>
extern volatile __bit                   PWM3DCH0            __at(0x30C0);	// @ (0x618 * 8 + 0)
#define                                 PWM3DCH0_bit        BANKMASK(PWM3DCH), 0
// PWM3DCH<PWM3DCH1>
extern volatile __bit                   PWM3DCH1            __at(0x30C1);	// @ (0x618 * 8 + 1)
#define                                 PWM3DCH1_bit        BANKMASK(PWM3DCH), 1
// PWM3DCH<PWM3DCH2>
extern volatile __bit                   PWM3DCH2            __at(0x30C2);	// @ (0x618 * 8 + 2)
#define                                 PWM3DCH2_bit        BANKMASK(PWM3DCH), 2
// PWM3DCH<PWM3DCH3>
extern volatile __bit                   PWM3DCH3            __at(0x30C3);	// @ (0x618 * 8 + 3)
#define                                 PWM3DCH3_bit        BANKMASK(PWM3DCH), 3
// PWM3DCH<PWM3DCH4>
extern volatile __bit                   PWM3DCH4            __at(0x30C4);	// @ (0x618 * 8 + 4)
#define                                 PWM3DCH4_bit        BANKMASK(PWM3DCH), 4
// PWM3DCH<PWM3DCH5>
extern volatile __bit                   PWM3DCH5            __at(0x30C5);	// @ (0x618 * 8 + 5)
#define                                 PWM3DCH5_bit        BANKMASK(PWM3DCH), 5
// PWM3DCH<PWM3DCH6>
extern volatile __bit                   PWM3DCH6            __at(0x30C6);	// @ (0x618 * 8 + 6)
#define                                 PWM3DCH6_bit        BANKMASK(PWM3DCH), 6
// PWM3DCH<PWM3DCH7>
extern volatile __bit                   PWM3DCH7            __at(0x30C7);	// @ (0x618 * 8 + 7)
#define                                 PWM3DCH7_bit        BANKMASK(PWM3DCH), 7
// PWM3DCL<PWM3DCL0>
extern volatile __bit                   PWM3DCL0            __at(0x30BE);	// @ (0x617 * 8 + 6)
#define                                 PWM3DCL0_bit        BANKMASK(PWM3DCL), 6
// PWM3DCL<PWM3DCL1>
extern volatile __bit                   PWM3DCL1            __at(0x30BF);	// @ (0x617 * 8 + 7)
#define                                 PWM3DCL1_bit        BANKMASK(PWM3DCL), 7
// PWM3CON<PWM3EN>
extern volatile __bit                   PWM3EN              __at(0x30CF);	// @ (0x619 * 8 + 7)
#define                                 PWM3EN_bit          BANKMASK(PWM3CON), 7
// PWM3CON<PWM3OUT>
extern volatile __bit                   PWM3OUT             __at(0x30CD);	// @ (0x619 * 8 + 5)
#define                                 PWM3OUT_bit         BANKMASK(PWM3CON), 5
// PWM3CON<PWM3POL>
extern volatile __bit                   PWM3POL             __at(0x30CC);	// @ (0x619 * 8 + 4)
#define                                 PWM3POL_bit         BANKMASK(PWM3CON), 4
// PWM4DCH<PWM4DCH0>
extern volatile __bit                   PWM4DCH0            __at(0x30D8);	// @ (0x61B * 8 + 0)
#define                                 PWM4DCH0_bit        BANKMASK(PWM4DCH), 0
// PWM4DCH<PWM4DCH1>
extern volatile __bit                   PWM4DCH1            __at(0x30D9);	// @ (0x61B * 8 + 1)
#define                                 PWM4DCH1_bit        BANKMASK(PWM4DCH), 1
// PWM4DCH<PWM4DCH2>
extern volatile __bit                   PWM4DCH2            __at(0x30DA);	// @ (0x61B * 8 + 2)
#define                                 PWM4DCH2_bit        BANKMASK(PWM4DCH), 2
// PWM4DCH<PWM4DCH3>
extern volatile __bit                   PWM4DCH3            __at(0x30DB);	// @ (0x61B * 8 + 3)
#define                                 PWM4DCH3_bit        BANKMASK(PWM4DCH), 3
// PWM4DCH<PWM4DCH4>
extern volatile __bit                   PWM4DCH4            __at(0x30DC);	// @ (0x61B * 8 + 4)
#define                                 PWM4DCH4_bit        BANKMASK(PWM4DCH), 4
// PWM4DCH<PWM4DCH5>
extern volatile __bit                   PWM4DCH5            __at(0x30DD);	// @ (0x61B * 8 + 5)
#define                                 PWM4DCH5_bit        BANKMASK(PWM4DCH), 5
// PWM4DCH<PWM4DCH6>
extern volatile __bit                   PWM4DCH6            __at(0x30DE);	// @ (0x61B * 8 + 6)
#define                                 PWM4DCH6_bit        BANKMASK(PWM4DCH), 6
// PWM4DCH<PWM4DCH7>
extern volatile __bit                   PWM4DCH7            __at(0x30DF);	// @ (0x61B * 8 + 7)
#define                                 PWM4DCH7_bit        BANKMASK(PWM4DCH), 7
// PWM4DCL<PWM4DCL0>
extern volatile __bit                   PWM4DCL0            __at(0x30D6);	// @ (0x61A * 8 + 6)
#define                                 PWM4DCL0_bit        BANKMASK(PWM4DCL), 6
// PWM4DCL<PWM4DCL1>
extern volatile __bit                   PWM4DCL1            __at(0x30D7);	// @ (0x61A * 8 + 7)
#define                                 PWM4DCL1_bit        BANKMASK(PWM4DCL), 7
// PWM4CON<PWM4EN>
extern volatile __bit                   PWM4EN              __at(0x30E7);	// @ (0x61C * 8 + 7)
#define                                 PWM4EN_bit          BANKMASK(PWM4CON), 7
// PWM4CON<PWM4OUT>
extern volatile __bit                   PWM4OUT             __at(0x30E5);	// @ (0x61C * 8 + 5)
#define                                 PWM4OUT_bit         BANKMASK(PWM4CON), 5
// PWM4CON<PWM4POL>
extern volatile __bit                   PWM4POL             __at(0x30E4);	// @ (0x61C * 8 + 4)
#define                                 PWM4POL_bit         BANKMASK(PWM4CON), 4
// PORTA<RA0>
extern volatile __bit                   RA0                 __at(0x60);	// @ (0xC * 8 + 0)
#define                                 RA0_bit             BANKMASK(PORTA), 0
// PORTA<RA1>
extern volatile __bit                   RA1                 __at(0x61);	// @ (0xC * 8 + 1)
#define                                 RA1_bit             BANKMASK(PORTA), 1
// PORTA<RA2>
extern volatile __bit                   RA2                 __at(0x62);	// @ (0xC * 8 + 2)
#define                                 RA2_bit             BANKMASK(PORTA), 2
// PORTA<RA3>
extern volatile __bit                   RA3                 __at(0x63);	// @ (0xC * 8 + 3)
#define                                 RA3_bit             BANKMASK(PORTA), 3
// PORTA<RA4>
extern volatile __bit                   RA4                 __at(0x64);	// @ (0xC * 8 + 4)
#define                                 RA4_bit             BANKMASK(PORTA), 4
// PORTA<RA5>
extern volatile __bit                   RA5                 __at(0x65);	// @ (0xC * 8 + 5)
#define                                 RA5_bit             BANKMASK(PORTA), 5
// PORTB<RB4>
extern volatile __bit                   RB4                 __at(0x6C);	// @ (0xD * 8 + 4)
#define                                 RB4_bit             BANKMASK(PORTB), 4
// PORTB<RB5>
extern volatile __bit                   RB5                 __at(0x6D);	// @ (0xD * 8 + 5)
#define                                 RB5_bit             BANKMASK(PORTB), 5
// PORTB<RB6>
extern volatile __bit                   RB6                 __at(0x6E);	// @ (0xD * 8 + 6)
#define                                 RB6_bit             BANKMASK(PORTB), 6
// PORTB<RB7>
extern volatile __bit                   RB7                 __at(0x6F);	// @ (0xD * 8 + 7)
#define                                 RB7_bit             BANKMASK(PORTB), 7
// PORTC<RC0>
extern volatile __bit                   RC0                 __at(0x70);	// @ (0xE * 8 + 0)
#define                                 RC0_bit             BANKMASK(PORTC), 0
// PORTC<RC1>
extern volatile __bit                   RC1                 __at(0x71);	// @ (0xE * 8 + 1)
#define                                 RC1_bit             BANKMASK(PORTC), 1
// PORTC<RC2>
extern volatile __bit                   RC2                 __at(0x72);	// @ (0xE * 8 + 2)
#define                                 RC2_bit             BANKMASK(PORTC), 2
// PORTC<RC3>
extern volatile __bit                   RC3                 __at(0x73);	// @ (0xE * 8 + 3)
#define                                 RC3_bit             BANKMASK(PORTC), 3
// PORTC<RC4>
extern volatile __bit                   RC4                 __at(0x74);	// @ (0xE * 8 + 4)
#define                                 RC4_bit             BANKMASK(PORTC), 4
// PORTC<RC5>
extern volatile __bit                   RC5                 __at(0x75);	// @ (0xE * 8 + 5)
#define                                 RC5_bit             BANKMASK(PORTC), 5
// PORTC<RC6>
extern volatile __bit                   RC6                 __at(0x76);	// @ (0xE * 8 + 6)
#define                                 RC6_bit             BANKMASK(PORTC), 6
// PORTC<RC7>
extern volatile __bit                   RC7                 __at(0x77);	// @ (0xE * 8 + 7)
#define                                 RC7_bit             BANKMASK(PORTC), 7
// SSP1CON2<RCEN>
extern volatile __bit                   RCEN                __at(0x10B3);	// @ (0x216 * 8 + 3)
#define                                 RCEN_bit            BANKMASK(SSP1CON2), 3
// BAUD1CON<RCIDL>
extern volatile __bit                   RCIDL               __at(0xCFE);	// @ (0x19F * 8 + 6)
#define                                 RCIDL_bit           BANKMASK(BAUD1CON), 6
// PIE1<RCIE>
extern volatile __bit                   RCIE                __at(0x48D);	// @ (0x91 * 8 + 5)
#define                                 RCIE_bit            BANKMASK(PIE1), 5
// PIR1<RCIF>
extern volatile __bit                   RCIF                __at(0x8D);	// @ (0x11 * 8 + 5)
#define                                 RCIF_bit            BANKMASK(PIR1), 5
// PMCON1<RD>
extern volatile __bit                   RD                  __at(0xCA8);	// @ (0x195 * 8 + 0)
#define                                 RD_bit              BANKMASK(PMCON1), 0
// SSP1CON2<RSEN>
extern volatile __bit                   RSEN                __at(0x10B1);	// @ (0x216 * 8 + 1)
#define                                 RSEN_bit            BANKMASK(SSP1CON2), 1
// RC1STA<RX9>
extern volatile __bit                   RX9                 __at(0xCEE);	// @ (0x19D * 8 + 6)
#define                                 RX9_bit             BANKMASK(RC1STA), 6
// RC1STA<RX9D>
extern volatile __bit                   RX9D                __at(0xCE8);	// @ (0x19D * 8 + 0)
#define                                 RX9D_bit            BANKMASK(RC1STA), 0
// SSP1STAT<R_nW>
extern volatile __bit                   R_nW                __at(0x10A2);	// @ (0x214 * 8 + 2)
#define                                 R_nW_bit            BANKMASK(SSP1STAT), 2
// SSP1CON3<SBCDE>
extern volatile __bit                   SBCDE               __at(0x10BA);	// @ (0x217 * 8 + 2)
#define                                 SBCDE_bit           BANKMASK(SSP1CON3), 2
// BORCON<SBOREN>
extern volatile __bit                   SBOREN              __at(0x8B7);	// @ (0x116 * 8 + 7)
#define                                 SBOREN_bit          BANKMASK(BORCON), 7
// SSP1CON3<SCIE>
extern volatile __bit                   SCIE                __at(0x10BD);	// @ (0x217 * 8 + 5)
#define                                 SCIE_bit            BANKMASK(SSP1CON3), 5
// BAUD1CON<SCKP>
extern volatile __bit                   SCKP                __at(0xCFC);	// @ (0x19F * 8 + 4)
#define                                 SCKP_bit            BANKMASK(BAUD1CON), 4
// OSCCON<SCS0>
extern volatile __bit                   SCS0                __at(0x4C8);	// @ (0x99 * 8 + 0)
#define                                 SCS0_bit            BANKMASK(OSCCON), 0
// OSCCON<SCS1>
extern volatile __bit                   SCS1                __at(0x4C9);	// @ (0x99 * 8 + 1)
#define                                 SCS1_bit            BANKMASK(OSCCON), 1
// SSP1CON3<SDAHT>
extern volatile __bit                   SDAHT               __at(0x10BB);	// @ (0x217 * 8 + 3)
#define                                 SDAHT_bit           BANKMASK(SSP1CON3), 3
// SSP1CON2<SEN>
extern volatile __bit                   SEN                 __at(0x10B0);	// @ (0x216 * 8 + 0)
#define                                 SEN_bit             BANKMASK(SSP1CON2), 0
// TX1STA<SENDB>
extern volatile __bit                   SENDB               __at(0xCF3);	// @ (0x19E * 8 + 3)
#define                                 SENDB_bit           BANKMASK(TX1STA), 3
// SLRCONA<SLRA0>
extern volatile __bit                   SLRA0               __at(0x1860);	// @ (0x30C * 8 + 0)
#define                                 SLRA0_bit           BANKMASK(SLRCONA), 0
// SLRCONA<SLRA1>
extern volatile __bit                   SLRA1               __at(0x1861);	// @ (0x30C * 8 + 1)
#define                                 SLRA1_bit           BANKMASK(SLRCONA), 1
// SLRCONA<SLRA2>
extern volatile __bit                   SLRA2               __at(0x1862);	// @ (0x30C * 8 + 2)
#define                                 SLRA2_bit           BANKMASK(SLRCONA), 2
// SLRCONA<SLRA4>
extern volatile __bit                   SLRA4               __at(0x1864);	// @ (0x30C * 8 + 4)
#define                                 SLRA4_bit           BANKMASK(SLRCONA), 4
// SLRCONA<SLRA5>
extern volatile __bit                   SLRA5               __at(0x1865);	// @ (0x30C * 8 + 5)
#define                                 SLRA5_bit           BANKMASK(SLRCONA), 5
// SLRCONB<SLRB4>
extern volatile __bit                   SLRB4               __at(0x186C);	// @ (0x30D * 8 + 4)
#define                                 SLRB4_bit           BANKMASK(SLRCONB), 4
// SLRCONB<SLRB5>
extern volatile __bit                   SLRB5               __at(0x186D);	// @ (0x30D * 8 + 5)
#define                                 SLRB5_bit           BANKMASK(SLRCONB), 5
// SLRCONB<SLRB6>
extern volatile __bit                   SLRB6               __at(0x186E);	// @ (0x30D * 8 + 6)
#define                                 SLRB6_bit           BANKMASK(SLRCONB), 6
// SLRCONB<SLRB7>
extern volatile __bit                   SLRB7               __at(0x186F);	// @ (0x30D * 8 + 7)
#define                                 SLRB7_bit           BANKMASK(SLRCONB), 7
// SLRCONC<SLRC0>
extern volatile __bit                   SLRC0               __at(0x1870);	// @ (0x30E * 8 + 0)
#define                                 SLRC0_bit           BANKMASK(SLRCONC), 0
// SLRCONC<SLRC1>
extern volatile __bit                   SLRC1               __at(0x1871);	// @ (0x30E * 8 + 1)
#define                                 SLRC1_bit           BANKMASK(SLRCONC), 1
// SLRCONC<SLRC2>
extern volatile __bit                   SLRC2               __at(0x1872);	// @ (0x30E * 8 + 2)
#define                                 SLRC2_bit           BANKMASK(SLRCONC), 2
// SLRCONC<SLRC3>
extern volatile __bit                   SLRC3               __at(0x1873);	// @ (0x30E * 8 + 3)
#define                                 SLRC3_bit           BANKMASK(SLRCONC), 3
// SLRCONC<SLRC4>
extern volatile __bit                   SLRC4               __at(0x1874);	// @ (0x30E * 8 + 4)
#define                                 SLRC4_bit           BANKMASK(SLRCONC), 4
// SLRCONC<SLRC5>
extern volatile __bit                   SLRC5               __at(0x1875);	// @ (0x30E * 8 + 5)
#define                                 SLRC5_bit           BANKMASK(SLRCONC), 5
// SLRCONC<SLRC6>
extern volatile __bit                   SLRC6               __at(0x1876);	// @ (0x30E * 8 + 6)
#define                                 SLRC6_bit           BANKMASK(SLRCONC), 6
// SLRCONC<SLRC7>
extern volatile __bit                   SLRC7               __at(0x1877);	// @ (0x30E * 8 + 7)
#define                                 SLRC7_bit           BANKMASK(SLRCONC), 7
// SSP1STAT<SMP>
extern volatile __bit                   SMP                 __at(0x10A7);	// @ (0x214 * 8 + 7)
#define                                 SMP_bit             BANKMASK(SSP1STAT), 7
// OSCSTAT<SOSCR>
extern volatile __bit                   SOSCR               __at(0x4D7);	// @ (0x9A * 8 + 7)
#define                                 SOSCR_bit           BANKMASK(OSCSTAT), 7
// RC1STA<SPEN>
extern volatile __bit                   SPEN                __at(0xCEF);	// @ (0x19D * 8 + 7)
#define                                 SPEN_bit            BANKMASK(RC1STA), 7
// OSCCON<SPLLEN>
extern volatile __bit                   SPLLEN              __at(0x4CF);	// @ (0x99 * 8 + 7)
#define                                 SPLLEN_bit          BANKMASK(OSCCON), 7
// RC1STA<SREN>
extern volatile __bit                   SREN                __at(0xCED);	// @ (0x19D * 8 + 5)
#define                                 SREN_bit            BANKMASK(RC1STA), 5
// SSP1ADD<SSP1ADD0>
extern volatile __bit                   SSP1ADD0            __at(0x1090);	// @ (0x212 * 8 + 0)
#define                                 SSP1ADD0_bit        BANKMASK(SSP1ADD), 0
// SSP1ADD<SSP1ADD1>
extern volatile __bit                   SSP1ADD1            __at(0x1091);	// @ (0x212 * 8 + 1)
#define                                 SSP1ADD1_bit        BANKMASK(SSP1ADD), 1
// SSP1ADD<SSP1ADD2>
extern volatile __bit                   SSP1ADD2            __at(0x1092);	// @ (0x212 * 8 + 2)
#define                                 SSP1ADD2_bit        BANKMASK(SSP1ADD), 2
// SSP1ADD<SSP1ADD3>
extern volatile __bit                   SSP1ADD3            __at(0x1093);	// @ (0x212 * 8 + 3)
#define                                 SSP1ADD3_bit        BANKMASK(SSP1ADD), 3
// SSP1ADD<SSP1ADD4>
extern volatile __bit                   SSP1ADD4            __at(0x1094);	// @ (0x212 * 8 + 4)
#define                                 SSP1ADD4_bit        BANKMASK(SSP1ADD), 4
// SSP1ADD<SSP1ADD5>
extern volatile __bit                   SSP1ADD5            __at(0x1095);	// @ (0x212 * 8 + 5)
#define                                 SSP1ADD5_bit        BANKMASK(SSP1ADD), 5
// SSP1ADD<SSP1ADD6>
extern volatile __bit                   SSP1ADD6            __at(0x1096);	// @ (0x212 * 8 + 6)
#define                                 SSP1ADD6_bit        BANKMASK(SSP1ADD), 6
// SSP1ADD<SSP1ADD7>
extern volatile __bit                   SSP1ADD7            __at(0x1097);	// @ (0x212 * 8 + 7)
#define                                 SSP1ADD7_bit        BANKMASK(SSP1ADD), 7
// SSP1BUF<SSP1BUF0>
extern volatile __bit                   SSP1BUF0            __at(0x1088);	// @ (0x211 * 8 + 0)
#define                                 SSP1BUF0_bit        BANKMASK(SSP1BUF), 0
// SSP1BUF<SSP1BUF1>
extern volatile __bit                   SSP1BUF1            __at(0x1089);	// @ (0x211 * 8 + 1)
#define                                 SSP1BUF1_bit        BANKMASK(SSP1BUF), 1
// SSP1BUF<SSP1BUF2>
extern volatile __bit                   SSP1BUF2            __at(0x108A);	// @ (0x211 * 8 + 2)
#define                                 SSP1BUF2_bit        BANKMASK(SSP1BUF), 2
// SSP1BUF<SSP1BUF3>
extern volatile __bit                   SSP1BUF3            __at(0x108B);	// @ (0x211 * 8 + 3)
#define                                 SSP1BUF3_bit        BANKMASK(SSP1BUF), 3
// SSP1BUF<SSP1BUF4>
extern volatile __bit                   SSP1BUF4            __at(0x108C);	// @ (0x211 * 8 + 4)
#define                                 SSP1BUF4_bit        BANKMASK(SSP1BUF), 4
// SSP1BUF<SSP1BUF5>
extern volatile __bit                   SSP1BUF5            __at(0x108D);	// @ (0x211 * 8 + 5)
#define                                 SSP1BUF5_bit        BANKMASK(SSP1BUF), 5
// SSP1BUF<SSP1BUF6>
extern volatile __bit                   SSP1BUF6            __at(0x108E);	// @ (0x211 * 8 + 6)
#define                                 SSP1BUF6_bit        BANKMASK(SSP1BUF), 6
// SSP1BUF<SSP1BUF7>
extern volatile __bit                   SSP1BUF7            __at(0x108F);	// @ (0x211 * 8 + 7)
#define                                 SSP1BUF7_bit        BANKMASK(SSP1BUF), 7
// PIE1<SSP1IE>
extern volatile __bit                   SSP1IE              __at(0x48B);	// @ (0x91 * 8 + 3)
#define                                 SSP1IE_bit          BANKMASK(PIE1), 3
// PIR1<SSP1IF>
extern volatile __bit                   SSP1IF              __at(0x8B);	// @ (0x11 * 8 + 3)
#define                                 SSP1IF_bit          BANKMASK(PIR1), 3
// SSP1MSK<SSP1MSK0>
extern volatile __bit                   SSP1MSK0            __at(0x1098);	// @ (0x213 * 8 + 0)
#define                                 SSP1MSK0_bit        BANKMASK(SSP1MSK), 0
// SSP1MSK<SSP1MSK1>
extern volatile __bit                   SSP1MSK1            __at(0x1099);	// @ (0x213 * 8 + 1)
#define                                 SSP1MSK1_bit        BANKMASK(SSP1MSK), 1
// SSP1MSK<SSP1MSK2>
extern volatile __bit                   SSP1MSK2            __at(0x109A);	// @ (0x213 * 8 + 2)
#define                                 SSP1MSK2_bit        BANKMASK(SSP1MSK), 2
// SSP1MSK<SSP1MSK3>
extern volatile __bit                   SSP1MSK3            __at(0x109B);	// @ (0x213 * 8 + 3)
#define                                 SSP1MSK3_bit        BANKMASK(SSP1MSK), 3
// SSP1MSK<SSP1MSK4>
extern volatile __bit                   SSP1MSK4            __at(0x109C);	// @ (0x213 * 8 + 4)
#define                                 SSP1MSK4_bit        BANKMASK(SSP1MSK), 4
// SSP1MSK<SSP1MSK5>
extern volatile __bit                   SSP1MSK5            __at(0x109D);	// @ (0x213 * 8 + 5)
#define                                 SSP1MSK5_bit        BANKMASK(SSP1MSK), 5
// SSP1MSK<SSP1MSK6>
extern volatile __bit                   SSP1MSK6            __at(0x109E);	// @ (0x213 * 8 + 6)
#define                                 SSP1MSK6_bit        BANKMASK(SSP1MSK), 6
// SSP1MSK<SSP1MSK7>
extern volatile __bit                   SSP1MSK7            __at(0x109F);	// @ (0x213 * 8 + 7)
#define                                 SSP1MSK7_bit        BANKMASK(SSP1MSK), 7
// SSP1CON1<SSPEN>
extern volatile __bit                   SSPEN               __at(0x10AD);	// @ (0x215 * 8 + 5)
#define                                 SSPEN_bit           BANKMASK(SSP1CON1), 5
// SSP1CON1<SSPM0>
extern volatile __bit                   SSPM0               __at(0x10A8);	// @ (0x215 * 8 + 0)
#define                                 SSPM0_bit           BANKMASK(SSP1CON1), 0
// SSP1CON1<SSPM1>
extern volatile __bit                   SSPM1               __at(0x10A9);	// @ (0x215 * 8 + 1)
#define                                 SSPM1_bit           BANKMASK(SSP1CON1), 1
// SSP1CON1<SSPM2>
extern volatile __bit                   SSPM2               __at(0x10AA);	// @ (0x215 * 8 + 2)
#define                                 SSPM2_bit           BANKMASK(SSP1CON1), 2
// SSP1CON1<SSPM3>
extern volatile __bit                   SSPM3               __at(0x10AB);	// @ (0x215 * 8 + 3)
#define                                 SSPM3_bit           BANKMASK(SSP1CON1), 3
// SSP1CON1<SSPOV>
extern volatile __bit                   SSPOV               __at(0x10AE);	// @ (0x215 * 8 + 6)
#define                                 SSPOV_bit           BANKMASK(SSP1CON1), 6
// PCON<STKOVF>
extern volatile __bit                   STKOVF              __at(0x4B7);	// @ (0x96 * 8 + 7)
#define                                 STKOVF_bit          BANKMASK(PCON), 7
// PCON<STKUNF>
extern volatile __bit                   STKUNF              __at(0x4B6);	// @ (0x96 * 8 + 6)
#define                                 STKUNF_bit          BANKMASK(PCON), 6
// WDTCON<SWDTEN>
extern volatile __bit                   SWDTEN              __at(0x4B8);	// @ (0x97 * 8 + 0)
#define                                 SWDTEN_bit          BANKMASK(WDTCON), 0
// TX1STA<SYNC>
extern volatile __bit                   SYNC                __at(0xCF4);	// @ (0x19E * 8 + 4)
#define                                 SYNC_bit            BANKMASK(TX1STA), 4
// OPTION_REG<T0CS>
extern volatile __bit                   T0CS                __at(0x4AD);	// @ (0x95 * 8 + 5)
#define                                 T0CS_bit            BANKMASK(OPTION_REG), 5
// INTCON<T0IE>
extern volatile __bit                   T0IE                __at(0x5D);	// @ (0xB * 8 + 5)
#define                                 T0IE_bit            BANKMASK(INTCON), 5
// INTCON<T0IF>
extern volatile __bit                   T0IF                __at(0x5A);	// @ (0xB * 8 + 2)
#define                                 T0IF_bit            BANKMASK(INTCON), 2
// OPTION_REG<T0SE>
extern volatile __bit                   T0SE                __at(0x4AC);	// @ (0x95 * 8 + 4)
#define                                 T0SE_bit            BANKMASK(OPTION_REG), 4
// T1CON<T1CKPS0>
extern volatile __bit                   T1CKPS0             __at(0xC4);	// @ (0x18 * 8 + 4)
#define                                 T1CKPS0_bit         BANKMASK(T1CON), 4
// T1CON<T1CKPS1>
extern volatile __bit                   T1CKPS1             __at(0xC5);	// @ (0x18 * 8 + 5)
#define                                 T1CKPS1_bit         BANKMASK(T1CON), 5
// T1GCON<T1GGO_nDONE>
extern volatile __bit                   T1GGO_nDONE         __at(0xCB);	// @ (0x19 * 8 + 3)
#define                                 T1GGO_nDONE_bit     BANKMASK(T1GCON), 3
// T1GCON<T1GPOL>
extern volatile __bit                   T1GPOL              __at(0xCE);	// @ (0x19 * 8 + 6)
#define                                 T1GPOL_bit          BANKMASK(T1GCON), 6
// T1GCON<T1GSPM>
extern volatile __bit                   T1GSPM              __at(0xCC);	// @ (0x19 * 8 + 4)
#define                                 T1GSPM_bit          BANKMASK(T1GCON), 4
// T1GCON<T1GSS0>
extern volatile __bit                   T1GSS0              __at(0xC8);	// @ (0x19 * 8 + 0)
#define                                 T1GSS0_bit          BANKMASK(T1GCON), 0
// T1GCON<T1GSS1>
extern volatile __bit                   T1GSS1              __at(0xC9);	// @ (0x19 * 8 + 1)
#define                                 T1GSS1_bit          BANKMASK(T1GCON), 1
// T1GCON<T1GTM>
extern volatile __bit                   T1GTM               __at(0xCD);	// @ (0x19 * 8 + 5)
#define                                 T1GTM_bit           BANKMASK(T1GCON), 5
// T1GCON<T1GVAL>
extern volatile __bit                   T1GVAL              __at(0xCA);	// @ (0x19 * 8 + 2)
#define                                 T1GVAL_bit          BANKMASK(T1GCON), 2
// T1CON<T1OSCEN>
extern volatile __bit                   T1OSCEN             __at(0xC3);	// @ (0x18 * 8 + 3)
#define                                 T1OSCEN_bit         BANKMASK(T1CON), 3
// T2CON<T2CKPS0>
extern volatile __bit                   T2CKPS0             __at(0xE0);	// @ (0x1C * 8 + 0)
#define                                 T2CKPS0_bit         BANKMASK(T2CON), 0
// T2CON<T2CKPS1>
extern volatile __bit                   T2CKPS1             __at(0xE1);	// @ (0x1C * 8 + 1)
#define                                 T2CKPS1_bit         BANKMASK(T2CON), 1
// T2CON<T2OUTPS0>
extern volatile __bit                   T2OUTPS0            __at(0xE3);	// @ (0x1C * 8 + 3)
#define                                 T2OUTPS0_bit        BANKMASK(T2CON), 3
// T2CON<T2OUTPS1>
extern volatile __bit                   T2OUTPS1            __at(0xE4);	// @ (0x1C * 8 + 4)
#define                                 T2OUTPS1_bit        BANKMASK(T2CON), 4
// T2CON<T2OUTPS2>
extern volatile __bit                   T2OUTPS2            __at(0xE5);	// @ (0x1C * 8 + 5)
#define                                 T2OUTPS2_bit        BANKMASK(T2CON), 5
// T2CON<T2OUTPS3>
extern volatile __bit                   T2OUTPS3            __at(0xE6);	// @ (0x1C * 8 + 6)
#define                                 T2OUTPS3_bit        BANKMASK(T2CON), 6
// T4CON<T4CKPS0>
extern volatile __bit                   T4CKPS0             __at(0x20B8);	// @ (0x417 * 8 + 0)
#define                                 T4CKPS0_bit         BANKMASK(T4CON), 0
// T4CON<T4CKPS1>
extern volatile __bit                   T4CKPS1             __at(0x20B9);	// @ (0x417 * 8 + 1)
#define                                 T4CKPS1_bit         BANKMASK(T4CON), 1
// T4CON<T4OUTPS0>
extern volatile __bit                   T4OUTPS0            __at(0x20BB);	// @ (0x417 * 8 + 3)
#define                                 T4OUTPS0_bit        BANKMASK(T4CON), 3
// T4CON<T4OUTPS1>
extern volatile __bit                   T4OUTPS1            __at(0x20BC);	// @ (0x417 * 8 + 4)
#define                                 T4OUTPS1_bit        BANKMASK(T4CON), 4
// T4CON<T4OUTPS2>
extern volatile __bit                   T4OUTPS2            __at(0x20BD);	// @ (0x417 * 8 + 5)
#define                                 T4OUTPS2_bit        BANKMASK(T4CON), 5
// T4CON<T4OUTPS3>
extern volatile __bit                   T4OUTPS3            __at(0x20BE);	// @ (0x417 * 8 + 6)
#define                                 T4OUTPS3_bit        BANKMASK(T4CON), 6
// T6CON<T6CKPS0>
extern volatile __bit                   T6CKPS0             __at(0x20F0);	// @ (0x41E * 8 + 0)
#define                                 T6CKPS0_bit         BANKMASK(T6CON), 0
// T6CON<T6CKPS1>
extern volatile __bit                   T6CKPS1             __at(0x20F1);	// @ (0x41E * 8 + 1)
#define                                 T6CKPS1_bit         BANKMASK(T6CON), 1
// T6CON<T6OUTPS0>
extern volatile __bit                   T6OUTPS0            __at(0x20F3);	// @ (0x41E * 8 + 3)
#define                                 T6OUTPS0_bit        BANKMASK(T6CON), 3
// T6CON<T6OUTPS1>
extern volatile __bit                   T6OUTPS1            __at(0x20F4);	// @ (0x41E * 8 + 4)
#define                                 T6OUTPS1_bit        BANKMASK(T6CON), 4
// T6CON<T6OUTPS2>
extern volatile __bit                   T6OUTPS2            __at(0x20F5);	// @ (0x41E * 8 + 5)
#define                                 T6OUTPS2_bit        BANKMASK(T6CON), 5
// T6CON<T6OUTPS3>
extern volatile __bit                   T6OUTPS3            __at(0x20F6);	// @ (0x41E * 8 + 6)
#define                                 T6OUTPS3_bit        BANKMASK(T6CON), 6
// OPTION_REG<TMR0CS>
extern volatile __bit                   TMR0CS              __at(0x4AD);	// @ (0x95 * 8 + 5)
#define                                 TMR0CS_bit          BANKMASK(OPTION_REG), 5
// INTCON<TMR0IE>
extern volatile __bit                   TMR0IE              __at(0x5D);	// @ (0xB * 8 + 5)
#define                                 TMR0IE_bit          BANKMASK(INTCON), 5
// INTCON<TMR0IF>
extern volatile __bit                   TMR0IF              __at(0x5A);	// @ (0xB * 8 + 2)
#define                                 TMR0IF_bit          BANKMASK(INTCON), 2
// OPTION_REG<TMR0SE>
extern volatile __bit                   TMR0SE              __at(0x4AC);	// @ (0x95 * 8 + 4)
#define                                 TMR0SE_bit          BANKMASK(OPTION_REG), 4
// T1CON<TMR1CS0>
extern volatile __bit                   TMR1CS0             __at(0xC6);	// @ (0x18 * 8 + 6)
#define                                 TMR1CS0_bit         BANKMASK(T1CON), 6
// T1CON<TMR1CS1>
extern volatile __bit                   TMR1CS1             __at(0xC7);	// @ (0x18 * 8 + 7)
#define                                 TMR1CS1_bit         BANKMASK(T1CON), 7
// T1GCON<TMR1GE>
extern volatile __bit                   TMR1GE              __at(0xCF);	// @ (0x19 * 8 + 7)
#define                                 TMR1GE_bit          BANKMASK(T1GCON), 7
// PIE1<TMR1GIE>
extern volatile __bit                   TMR1GIE             __at(0x48F);	// @ (0x91 * 8 + 7)
#define                                 TMR1GIE_bit         BANKMASK(PIE1), 7
// PIR1<TMR1GIF>
extern volatile __bit                   TMR1GIF             __at(0x8F);	// @ (0x11 * 8 + 7)
#define                                 TMR1GIF_bit         BANKMASK(PIR1), 7
// PIE1<TMR1IE>
extern volatile __bit                   TMR1IE              __at(0x488);	// @ (0x91 * 8 + 0)
#define                                 TMR1IE_bit          BANKMASK(PIE1), 0
// PIR1<TMR1IF>
extern volatile __bit                   TMR1IF              __at(0x88);	// @ (0x11 * 8 + 0)
#define                                 TMR1IF_bit          BANKMASK(PIR1), 0
// T1CON<TMR1ON>
extern volatile __bit                   TMR1ON              __at(0xC0);	// @ (0x18 * 8 + 0)
#define                                 TMR1ON_bit          BANKMASK(T1CON), 0
// PIE1<TMR2IE>
extern volatile __bit                   TMR2IE              __at(0x489);	// @ (0x91 * 8 + 1)
#define                                 TMR2IE_bit          BANKMASK(PIE1), 1
// PIR1<TMR2IF>
extern volatile __bit                   TMR2IF              __at(0x89);	// @ (0x11 * 8 + 1)
#define                                 TMR2IF_bit          BANKMASK(PIR1), 1
// T2CON<TMR2ON>
extern volatile __bit                   TMR2ON              __at(0xE2);	// @ (0x1C * 8 + 2)
#define                                 TMR2ON_bit          BANKMASK(T2CON), 2
// PIE2<TMR4IE>
extern volatile __bit                   TMR4IE              __at(0x491);	// @ (0x92 * 8 + 1)
#define                                 TMR4IE_bit          BANKMASK(PIE2), 1
// PIR2<TMR4IF>
extern volatile __bit                   TMR4IF              __at(0x91);	// @ (0x12 * 8 + 1)
#define                                 TMR4IF_bit          BANKMASK(PIR2), 1
// T4CON<TMR4ON>
extern volatile __bit                   TMR4ON              __at(0x20BA);	// @ (0x417 * 8 + 2)
#define                                 TMR4ON_bit          BANKMASK(T4CON), 2
// PIE2<TMR6IE>
extern volatile __bit                   TMR6IE              __at(0x492);	// @ (0x92 * 8 + 2)
#define                                 TMR6IE_bit          BANKMASK(PIE2), 2
// PIR2<TMR6IF>
extern volatile __bit                   TMR6IF              __at(0x92);	// @ (0x12 * 8 + 2)
#define                                 TMR6IF_bit          BANKMASK(PIR2), 2
// T6CON<TMR6ON>
extern volatile __bit                   TMR6ON              __at(0x20F2);	// @ (0x41E * 8 + 2)
#define                                 TMR6ON_bit          BANKMASK(T6CON), 2
// ADCON2<TRIGSEL0>
extern volatile __bit                   TRIGSEL0            __at(0x4FC);	// @ (0x9F * 8 + 4)
#define                                 TRIGSEL0_bit        BANKMASK(ADCON2), 4
// ADCON2<TRIGSEL1>
extern volatile __bit                   TRIGSEL1            __at(0x4FD);	// @ (0x9F * 8 + 5)
#define                                 TRIGSEL1_bit        BANKMASK(ADCON2), 5
// ADCON2<TRIGSEL2>
extern volatile __bit                   TRIGSEL2            __at(0x4FE);	// @ (0x9F * 8 + 6)
#define                                 TRIGSEL2_bit        BANKMASK(ADCON2), 6
// ADCON2<TRIGSEL3>
extern volatile __bit                   TRIGSEL3            __at(0x4FF);	// @ (0x9F * 8 + 7)
#define                                 TRIGSEL3_bit        BANKMASK(ADCON2), 7
// TRISA<TRISA0>
extern volatile __bit                   TRISA0              __at(0x460);	// @ (0x8C * 8 + 0)
#define                                 TRISA0_bit          BANKMASK(TRISA), 0
// TRISA<TRISA1>
extern volatile __bit                   TRISA1              __at(0x461);	// @ (0x8C * 8 + 1)
#define                                 TRISA1_bit          BANKMASK(TRISA), 1
// TRISA<TRISA2>
extern volatile __bit                   TRISA2              __at(0x462);	// @ (0x8C * 8 + 2)
#define                                 TRISA2_bit          BANKMASK(TRISA), 2
// TRISA<TRISA4>
extern volatile __bit                   TRISA4              __at(0x464);	// @ (0x8C * 8 + 4)
#define                                 TRISA4_bit          BANKMASK(TRISA), 4
// TRISA<TRISA5>
extern volatile __bit                   TRISA5              __at(0x465);	// @ (0x8C * 8 + 5)
#define                                 TRISA5_bit          BANKMASK(TRISA), 5
// TRISB<TRISB4>
extern volatile __bit                   TRISB4              __at(0x46C);	// @ (0x8D * 8 + 4)
#define                                 TRISB4_bit          BANKMASK(TRISB), 4
// TRISB<TRISB5>
extern volatile __bit                   TRISB5              __at(0x46D);	// @ (0x8D * 8 + 5)
#define                                 TRISB5_bit          BANKMASK(TRISB), 5
// TRISB<TRISB6>
extern volatile __bit                   TRISB6              __at(0x46E);	// @ (0x8D * 8 + 6)
#define                                 TRISB6_bit          BANKMASK(TRISB), 6
// TRISB<TRISB7>
extern volatile __bit                   TRISB7              __at(0x46F);	// @ (0x8D * 8 + 7)
#define                                 TRISB7_bit          BANKMASK(TRISB), 7
// TRISC<TRISC0>
extern volatile __bit                   TRISC0              __at(0x470);	// @ (0x8E * 8 + 0)
#define                                 TRISC0_bit          BANKMASK(TRISC), 0
// TRISC<TRISC1>
extern volatile __bit                   TRISC1              __at(0x471);	// @ (0x8E * 8 + 1)
#define                                 TRISC1_bit          BANKMASK(TRISC), 1
// TRISC<TRISC2>
extern volatile __bit                   TRISC2              __at(0x472);	// @ (0x8E * 8 + 2)
#define                                 TRISC2_bit          BANKMASK(TRISC), 2
// TRISC<TRISC3>
extern volatile __bit                   TRISC3              __at(0x473);	// @ (0x8E * 8 + 3)
#define                                 TRISC3_bit          BANKMASK(TRISC), 3
// TRISC<TRISC4>
extern volatile __bit                   TRISC4              __at(0x474);	// @ (0x8E * 8 + 4)
#define                                 TRISC4_bit          BANKMASK(TRISC), 4
// TRISC<TRISC5>
extern volatile __bit                   TRISC5              __at(0x475);	// @ (0x8E * 8 + 5)
#define                                 TRISC5_bit          BANKMASK(TRISC), 5
// TRISC<TRISC6>
extern volatile __bit                   TRISC6              __at(0x476);	// @ (0x8E * 8 + 6)
#define                                 TRISC6_bit          BANKMASK(TRISC), 6
// TRISC<TRISC7>
extern volatile __bit                   TRISC7              __at(0x477);	// @ (0x8E * 8 + 7)
#define                                 TRISC7_bit          BANKMASK(TRISC), 7
// TX1STA<TRMT>
extern volatile __bit                   TRMT                __at(0xCF1);	// @ (0x19E * 8 + 1)
#define                                 TRMT_bit            BANKMASK(TX1STA), 1
// FVRCON<TSEN>
extern volatile __bit                   TSEN                __at(0x8BD);	// @ (0x117 * 8 + 5)
#define                                 TSEN_bit            BANKMASK(FVRCON), 5
// FVRCON<TSRNG>
extern volatile __bit                   TSRNG               __at(0x8BC);	// @ (0x117 * 8 + 4)
#define                                 TSRNG_bit           BANKMASK(FVRCON), 4
// OSCTUNE<TUN0>
extern volatile __bit                   TUN0                __at(0x4C0);	// @ (0x98 * 8 + 0)
#define                                 TUN0_bit            BANKMASK(OSCTUNE), 0
// OSCTUNE<TUN1>
extern volatile __bit                   TUN1                __at(0x4C1);	// @ (0x98 * 8 + 1)
#define                                 TUN1_bit            BANKMASK(OSCTUNE), 1
// OSCTUNE<TUN2>
extern volatile __bit                   TUN2                __at(0x4C2);	// @ (0x98 * 8 + 2)
#define                                 TUN2_bit            BANKMASK(OSCTUNE), 2
// OSCTUNE<TUN3>
extern volatile __bit                   TUN3                __at(0x4C3);	// @ (0x98 * 8 + 3)
#define                                 TUN3_bit            BANKMASK(OSCTUNE), 3
// OSCTUNE<TUN4>
extern volatile __bit                   TUN4                __at(0x4C4);	// @ (0x98 * 8 + 4)
#define                                 TUN4_bit            BANKMASK(OSCTUNE), 4
// OSCTUNE<TUN5>
extern volatile __bit                   TUN5                __at(0x4C5);	// @ (0x98 * 8 + 5)
#define                                 TUN5_bit            BANKMASK(OSCTUNE), 5
// TX1STA<TX9>
extern volatile __bit                   TX9                 __at(0xCF6);	// @ (0x19E * 8 + 6)
#define                                 TX9_bit             BANKMASK(TX1STA), 6
// TX1STA<TX9D>
extern volatile __bit                   TX9D                __at(0xCF0);	// @ (0x19E * 8 + 0)
#define                                 TX9D_bit            BANKMASK(TX1STA), 0
// TX1STA<TXEN>
extern volatile __bit                   TXEN                __at(0xCF5);	// @ (0x19E * 8 + 5)
#define                                 TXEN_bit            BANKMASK(TX1STA), 5
// PIE1<TXIE>
extern volatile __bit                   TXIE                __at(0x48C);	// @ (0x91 * 8 + 4)
#define                                 TXIE_bit            BANKMASK(PIE1), 4
// PIR1<TXIF>
extern volatile __bit                   TXIF                __at(0x8C);	// @ (0x11 * 8 + 4)
#define                                 TXIF_bit            BANKMASK(PIR1), 4
// SSP1STAT<UA>
extern volatile __bit                   UA                  __at(0x10A1);	// @ (0x214 * 8 + 1)
#define                                 UA_bit              BANKMASK(SSP1STAT), 1
// VREGCON<VREGPM>
extern volatile __bit                   VREGPM              __at(0xCB9);	// @ (0x197 * 8 + 1)
#define                                 VREGPM_bit          BANKMASK(VREGCON), 1
// SSP1CON1<WCOL>
extern volatile __bit                   WCOL                __at(0x10AF);	// @ (0x215 * 8 + 7)
#define                                 WCOL_bit            BANKMASK(SSP1CON1), 7
// WDTCON<WDTPS0>
extern volatile __bit                   WDTPS0              __at(0x4B9);	// @ (0x97 * 8 + 1)
#define                                 WDTPS0_bit          BANKMASK(WDTCON), 1
// WDTCON<WDTPS1>
extern volatile __bit                   WDTPS1              __at(0x4BA);	// @ (0x97 * 8 + 2)
#define                                 WDTPS1_bit          BANKMASK(WDTCON), 2
// WDTCON<WDTPS2>
extern volatile __bit                   WDTPS2              __at(0x4BB);	// @ (0x97 * 8 + 3)
#define                                 WDTPS2_bit          BANKMASK(WDTCON), 3
// WDTCON<WDTPS3>
extern volatile __bit                   WDTPS3              __at(0x4BC);	// @ (0x97 * 8 + 4)
#define                                 WDTPS3_bit          BANKMASK(WDTCON), 4
// WDTCON<WDTPS4>
extern volatile __bit                   WDTPS4              __at(0x4BD);	// @ (0x97 * 8 + 5)
#define                                 WDTPS4_bit          BANKMASK(WDTCON), 5
// WPUA<WPUA0>
extern volatile __bit                   WPUA0               __at(0x1060);	// @ (0x20C * 8 + 0)
#define                                 WPUA0_bit           BANKMASK(WPUA), 0
// WPUA<WPUA1>
extern volatile __bit                   WPUA1               __at(0x1061);	// @ (0x20C * 8 + 1)
#define                                 WPUA1_bit           BANKMASK(WPUA), 1
// WPUA<WPUA2>
extern volatile __bit                   WPUA2               __at(0x1062);	// @ (0x20C * 8 + 2)
#define                                 WPUA2_bit           BANKMASK(WPUA), 2
// WPUA<WPUA3>
extern volatile __bit                   WPUA3               __at(0x1063);	// @ (0x20C * 8 + 3)
#define                                 WPUA3_bit           BANKMASK(WPUA), 3
// WPUA<WPUA4>
extern volatile __bit                   WPUA4               __at(0x1064);	// @ (0x20C * 8 + 4)
#define                                 WPUA4_bit           BANKMASK(WPUA), 4
// WPUA<WPUA5>
extern volatile __bit                   WPUA5               __at(0x1065);	// @ (0x20C * 8 + 5)
#define                                 WPUA5_bit           BANKMASK(WPUA), 5
// WPUB<WPUB4>
extern volatile __bit                   WPUB4               __at(0x106C);	// @ (0x20D * 8 + 4)
#define                                 WPUB4_bit           BANKMASK(WPUB), 4
// WPUB<WPUB5>
extern volatile __bit                   WPUB5               __at(0x106D);	// @ (0x20D * 8 + 5)
#define                                 WPUB5_bit           BANKMASK(WPUB), 5
// WPUB<WPUB6>
extern volatile __bit                   WPUB6               __at(0x106E);	// @ (0x20D * 8 + 6)
#define                                 WPUB6_bit           BANKMASK(WPUB), 6
// WPUB<WPUB7>
extern volatile __bit                   WPUB7               __at(0x106F);	// @ (0x20D * 8 + 7)
#define                                 WPUB7_bit           BANKMASK(WPUB), 7
// WPUC<WPUC0>
extern volatile __bit                   WPUC0               __at(0x1070);	// @ (0x20E * 8 + 0)
#define                                 WPUC0_bit           BANKMASK(WPUC), 0
// WPUC<WPUC1>
extern volatile __bit                   WPUC1               __at(0x1071);	// @ (0x20E * 8 + 1)
#define                                 WPUC1_bit           BANKMASK(WPUC), 1
// WPUC<WPUC2>
extern volatile __bit                   WPUC2               __at(0x1072);	// @ (0x20E * 8 + 2)
#define                                 WPUC2_bit           BANKMASK(WPUC), 2
// WPUC<WPUC3>
extern volatile __bit                   WPUC3               __at(0x1073);	// @ (0x20E * 8 + 3)
#define                                 WPUC3_bit           BANKMASK(WPUC), 3
// WPUC<WPUC4>
extern volatile __bit                   WPUC4               __at(0x1074);	// @ (0x20E * 8 + 4)
#define                                 WPUC4_bit           BANKMASK(WPUC), 4
// WPUC<WPUC5>
extern volatile __bit                   WPUC5               __at(0x1075);	// @ (0x20E * 8 + 5)
#define                                 WPUC5_bit           BANKMASK(WPUC), 5
// WPUC<WPUC6>
extern volatile __bit                   WPUC6               __at(0x1076);	// @ (0x20E * 8 + 6)
#define                                 WPUC6_bit           BANKMASK(WPUC), 6
// WPUC<WPUC7>
extern volatile __bit                   WPUC7               __at(0x1077);	// @ (0x20E * 8 + 7)
#define                                 WPUC7_bit           BANKMASK(WPUC), 7
// PMCON1<WR>
extern volatile __bit                   WR                  __at(0xCA9);	// @ (0x195 * 8 + 1)
#define                                 WR_bit              BANKMASK(PMCON1), 1
// PMCON1<WREN>
extern volatile __bit                   WREN                __at(0xCAA);	// @ (0x195 * 8 + 2)
#define                                 WREN_bit            BANKMASK(PMCON1), 2
// PMCON1<WRERR>
extern volatile __bit                   WRERR               __at(0xCAB);	// @ (0x195 * 8 + 3)
#define                                 WRERR_bit           BANKMASK(PMCON1), 3
// BAUD1CON<WUE>
extern volatile __bit                   WUE                 __at(0xCF9);	// @ (0x19F * 8 + 1)
#define                                 WUE_bit             BANKMASK(BAUD1CON), 1
// ZCD1CON<ZCD1EN>
extern volatile __bit                   ZCD1EN              __at(0x8E7);	// @ (0x11C * 8 + 7)
#define                                 ZCD1EN_bit          BANKMASK(ZCD1CON), 7
// ZCD1CON<ZCD1INTN>
extern volatile __bit                   ZCD1INTN            __at(0x8E0);	// @ (0x11C * 8 + 0)
#define                                 ZCD1INTN_bit        BANKMASK(ZCD1CON), 0
// ZCD1CON<ZCD1INTP>
extern volatile __bit                   ZCD1INTP            __at(0x8E1);	// @ (0x11C * 8 + 1)
#define                                 ZCD1INTP_bit        BANKMASK(ZCD1CON), 1
// ZCD1CON<ZCD1OUT>
extern volatile __bit                   ZCD1OUT             __at(0x8E5);	// @ (0x11C * 8 + 5)
#define                                 ZCD1OUT_bit         BANKMASK(ZCD1CON), 5
// ZCD1CON<ZCD1POL>
extern volatile __bit                   ZCD1POL             __at(0x8E4);	// @ (0x11C * 8 + 4)
#define                                 ZCD1POL_bit         BANKMASK(ZCD1CON), 4
// PIE3<ZCDIE>
extern volatile __bit                   ZCDIE               __at(0x49C);	// @ (0x93 * 8 + 4)
#define                                 ZCDIE_bit           BANKMASK(PIE3), 4
// PIR3<ZCDIF>
extern volatile __bit                   ZCDIF               __at(0x9C);	// @ (0x13 * 8 + 4)
#define                                 ZCDIF_bit           BANKMASK(PIR3), 4
// STATUS<ZERO>
extern volatile __bit                   ZERO                __at(0x1A);	// @ (0x3 * 8 + 2)
#define                                 ZERO_bit            BANKMASK(STATUS), 2
// STATUS_SHAD<Z_SHAD>
extern volatile __bit                   Z_SHAD              __at(0x7F22);	// @ (0xFE4 * 8 + 2)
#define                                 Z_SHAD_bit          BANKMASK(STATUS_SHAD), 2
// PCON<nBOR>
extern volatile __bit                   nBOR                __at(0x4B0);	// @ (0x96 * 8 + 0)
#define                                 nBOR_bit            BANKMASK(PCON), 0
// STATUS<nPD>
extern volatile __bit                   nPD                 __at(0x1B);	// @ (0x3 * 8 + 3)
#define                                 nPD_bit             BANKMASK(STATUS), 3
// PCON<nPOR>
extern volatile __bit                   nPOR                __at(0x4B1);	// @ (0x96 * 8 + 1)
#define                                 nPOR_bit            BANKMASK(PCON), 1
// PCON<nRI>
extern volatile __bit                   nRI                 __at(0x4B2);	// @ (0x96 * 8 + 2)
#define                                 nRI_bit             BANKMASK(PCON), 2
// PCON<nRMCLR>
extern volatile __bit                   nRMCLR              __at(0x4B3);	// @ (0x96 * 8 + 3)
#define                                 nRMCLR_bit          BANKMASK(PCON), 3
// PCON<nRWDT>
extern volatile __bit                   nRWDT               __at(0x4B4);	// @ (0x96 * 8 + 4)
#define                                 nRWDT_bit           BANKMASK(PCON), 4
// T1CON<nT1SYNC>
extern volatile __bit                   nT1SYNC             __at(0xC2);	// @ (0x18 * 8 + 2)
#define                                 nT1SYNC_bit         BANKMASK(T1CON), 2
// STATUS<nTO>
extern volatile __bit                   nTO                 __at(0x1C);	// @ (0x3 * 8 + 4)
#define                                 nTO_bit             BANKMASK(STATUS), 4
// OPTION_REG<nWPUEN>
extern volatile __bit                   nWPUEN              __at(0x4AF);	// @ (0x95 * 8 + 7)
#define                                 nWPUEN_bit          BANKMASK(OPTION_REG), 7

#endif // _PIC16F1709_H_
