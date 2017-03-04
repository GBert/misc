/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "main.h"

void i2c_start(void) {
    SSP1CON2bits.SEN = 1;
    while(SSP1CON2bits.SEN);
}

void i2c_restart(void) {
    SSP1CON2bits.RSEN = 1;
    while(SSP1CON2bits.RSEN);
}

void i2c_stop(void) {
    SSP1CON2bits.PEN = 1;
    while(SSP1CON2bits.PEN);
}

void i2c_sendACK(void) {
    SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKEN = 1;
}

void i2c_sendNACK(void) {
    SSP1CON2bits.ACKDT = 1;
    SSP1CON2bits.ACKEN = 1;
}

int8_t i2c_write(uint8_t data) {
    LATCbits.LATC0 = 1;
    SSPBUF = data;
    if (SSP1CON1bits.WCOL)
	return -1;
    while(SSP1STATbits.BF);
    LATCbits.LATC0 = 0;
    return 0;
}
