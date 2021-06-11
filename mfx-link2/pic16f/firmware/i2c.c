/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "main.h"

void i2c_wait(void) {
    /* wait for R/W and Buffer Full clearing */
    while (SSP1STAT & 0x05) ;
    /* wait for SEN, RSEN, PEN, RCEN, ACKEN clearing */
    while (SSP1CON2 & 0x1F) ;
}

void i2c_start(void) {
    i2c_wait();
    SSP1CON2bits.SEN = 1;
}

void i2c_restart(void) {
    i2c_wait();
    SSP1CON2bits.RSEN = 1;
}

void i2c_stop(void) {
    i2c_wait();
    SSP1CON2bits.PEN = 1;
}

void i2c_sendACK(void) {
    i2c_wait();
    SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKEN = 1;
}

void i2c_sendNACK(void) {
    i2c_wait();
    SSP1CON2bits.ACKDT = 1;
    SSP1CON2bits.ACKEN = 1;
}

int8_t i2c_write(volatile uint8_t data) {
    i2c_wait();
    SSPBUF = data;
    if (SSP1CON1bits.WCOL)
	return -1;
    return 0;
}
