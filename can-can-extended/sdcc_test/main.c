/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "main.h"
// #include "usart.h"
#include "prototypes.h"
#include <i2c.h>

#pragma config XINST=OFF


#define MCP_IODIR_A 0x00
#define MCP_IPOL_A 0x02
#define MCP_OLAT_A 0x14
#define MCP_GPIO_A 0x12
#define MCP_IOCON_A 0x0A
#define MCP_GPINTEN_A 0x04

volatile unsigned char timer_ticks=0;

void init_port(void) {
    ADCON1 = 0x0F;		// Default all pins to digital
    LED_TRIS = 0;
}

void tick (void) {
    INTCONbits.TMR0IF = 0;
}

void init_timer(void) {
    // time period = 1/16MHz = 0.0625 us
    // prescaler period = .0625us * 256 = 16us
    // overflow period  = 16us * 256 = 4.096ms
    T0CONbits.T0PS0=1;    //Prescaler is divide by 256
    T0CONbits.T0PS1=1;
    T0CONbits.T0PS2=1;

    T0CONbits.PSA=0;      //Timer Clock Source is from Prescaler
    T0CONbits.T0CS=0;     //Prescaler gets clock from FCPU (16MHz)

    T0CONbits.T08BIT=1;   //8 BIT MODE

    // INTCONbits.TMR0IE=1;   //Enable TIMER0 Interrupt
    // INTCONbits.PEIE=1;     //Enable Peripheral Interrupt
    // INTCONbits.GIE=1;      //Enable INTs globally

    T0CONbits.TMR0ON=1;   //Now start the timer!
}

void MCP_write()
{
     WriteI2C(0x40);
}

void MCP_read()
{
     WriteI2C(0x41);
}

void MCP_addr()
{
     StartI2C();
     MCP_write();
}

void init_MCP()
{
     //IODIR A configuration
     MCP_addr();
     WriteI2C(MCP_IODIR_A);
     WriteI2C(0x00);
     StopI2C();

     //IPOL A configuration
     MCP_addr();
     WriteI2C(MCP_IPOL_A);
     WriteI2C(0x00);
     StopI2C();

     //OLAT A configuration
     MCP_addr();
     WriteI2C(MCP_OLAT_A);
     WriteI2C(0xFF);
     StopI2C();

     //IOCON A configuration
     MCP_addr();
     WriteI2C(MCP_IOCON_A);
     WriteI2C(0x00);
     StopI2C();

     //GPINTEN A configuration
     MCP_addr();
     WriteI2C(MCP_GPINTEN_A);
     WriteI2C(0x00);
     StopI2C();
}

void main(void) {
    unsigned char do_print=0;
    unsigned char ret=0;
    unsigned char TaskB='A';
    char c;

    init_port();
    init_timer();

    //infinite loop
    while(1) {
	if (INTCONbits.TMR0IF) {
	    tick();
	}
    }
}

void isr() __interrupt 1 {
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {
        // overflow every 4.096ms
        timer_ticks++;
        if (timer_ticks==20) {  // 80 ms
            LED = 0;		// LED OFF
        }
        if (timer_ticks==40) {  // 80 ms
            LED = 1;		// LED ON
        }
        if (timer_ticks==60) {  // 80 ms
            LED = 0;		// LED OFF
        }
        if (timer_ticks==250) { // 720 ms
            LED = 1;		// LED OFF
            timer_ticks=0;
        }
        INTCONbits.TMR0IF = 0;
    }
}
