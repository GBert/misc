/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CONFIG_H
#define	CONFIG_H

// PIC16F1709 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#if __DEBUG == 1
  #pragma config WDTE = OFF
#else
  #pragma config WDTE = ON       // Watchdog Timer Enable (WDT disabled)
#endif
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PPS1WAY = OFF    // Peripheral Pin Select one-way control (The PPSLOCK bit can be set and cleared repeatedly by software)
#pragma config ZCDDIS = ON      // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR)
#pragma config PLLEN = OFF      // Phase Lock Loop enable (4x PLL is enabled when software sets the SPLLEN bit)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "pic16f1709.h"

//IOs
// 1  VDD
// 2  RA5   SCL I2C     IN
// 3  RA4   SDA I2C     IN
// 4  MCLR/RA3          IN
// 5  LED               OUT (low active)
// 6  RC4               n.c.
// 7  RC3   PULSE       IN  (data from Gleisbox)
// 8  RC6               n.c.
// 9  RC7               n.c.
// 10 RB7               n.c.

// 11 RB6   LPWM         OUT (data to IBT2)
// 12 RB5   RPWM         OUT (data to IBT2)
// 13 RB4   ENABLE       OUT (enable IBT2)  
// 14 RC2   TX (RS232)   OUT
// 15 RC1   RX (RS232)   IN
// 16 RC0 AN4  ISENSE    IN_ANALOG (current sense)
// 17 RA2 AN2  POTI      IN_ANALOG (overcurrent set)
// 18 RA1/PGC            IN (debug)
// 19 RA0/PGD            IN (debug) 
// 20 GND

#define TRISAWERT 0xFF
#define ANSELAWERT 0x04
#define TRISBWERT 0xFF
#define ANSELBWERT 0x00
#define TRISCWERT 0xCF
#define ANSELCWERT 0x01

//Frequencies
#define _XTAL_FREQ	32000000
#define XTAL_FREQ	(_XTAL_FREQ)
#define FCYC		(_XTAL_FREQ/4L) // target device instruction clock freqency

//IO masks
#define IOC_DIGITALINMSK  0x08 //RC3
#define IOC_PORT PORTCbits.RC3
#define LED LATCbits.LATC5

//LCD Display
#define LCD_01_ADDRESS  0x27
#define I2C_BAUDRATE	100000
// I2C baud rate calculation
// SCL pin clock period = ((ADD<7:0> + 1) *4)/FOSC
#define SSP1ADD_VAL	( (((_XTAL_FREQ / I2C_BAUDRATE) / 2) - 1) / 2)
#define LCD_WAIT_DELAY 80
#define LCD_WAIT_INITIAL 0x0400;

//ADC channels
#define ADCCHANNEL_POTI_IMAX 0x05    //AN2
#define ADCCHANNEL_POTI_TAUS 0x06
#define ADCCHANNEL_SENSE	0x04    //AN4

//UART
#define BAUDRATE	115200
#define USE_BRG16	0
#define USE_BRGH	1
// USART calculating Baud Rate Generator
// if BRGH = 0 => FOSC/[64 (n + 1)]
// if BRGH = 1 => FOSC/[16 (n + 1)]
// avoid rounding errors

#if USE_BRGH == 0
#define SBRG_VAL	( (((_XTAL_FREQ / BAUDRATE) / 32) - 1) / 2 )
#else
#define SBRG_VAL	( (((_XTAL_FREQ / BAUDRATE) / 8) - 1) / 2 )
#endif

/* BUFFER_SIZE */
#define RX_BUFSIZE 16
#define TX_BUFSIZE 32
#define WPU_RX WPUCbits.WPUC1 //RC1 = RX

#define HEF_STARTADRESS 0x1F80 //nonvolatile memory in flash

#define USE_BARGRAPH 1
#define BARGRAPH_X 1
#define BARGRAPH_Y 2
#define BARGRAPH_CHARS 10

// project specific default values for nonvolatile settings
#define EEP_VALID_FLAG 0x99
#define ISENSE_POSOFFSET 0x26 //default values for settings initialization
#define ISENSE_NEGOFFSET 0x17 //default values for settings initialization
#define I_MAXDEFAULT 40u //I_max in 0,1A steps here 2.5A
#define I_MAX_DELAY 1u //* 1ms until off
#define POS_GAIN 145u //default values for settings initialization
#define NEG_GAIN 141u //default values for settings initialization


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

