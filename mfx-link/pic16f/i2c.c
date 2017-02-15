#include "main.h"

void i2c_Init(void) {
    //Initialise I2C MSSP
    //Master 100KHz

    SSP1CON1 = 0b00101000;	//I2C enabled Master mode
    SSP1CON2 = 0x00;
    //I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
    SSP1ADD = SSP1ADD_VAL;

    SSP1STAT = 0b11000000;	//Slew rate disabled
}

void i2c_start() {
    SSP1CON2bits.SEN = 1;
    while(SSP1CON2bits.SEN);
}

void i2c_restart() {
    SSP1CON2bits.RSEN = 1;
    while(SSP1CON2bits.RSEN);
}

void i2c_stop() {
    SSP1CON2bits.PEN = 1;
    while(SSP1CON2bits.PEN);
}

void i2c_sendACK() {
    SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKEN = 1;
}

void i2c_sendNACK() {
    SSP1CON2bits.ACKDT = 1;
    SSP1CON2bits.ACKEN = 1;
}

char i2c_write(unsigned char data) {
    SSPBUF = data;
    if (SSP1CON1bits.WCOL)
	return -1;
    while(SSP1STATbits.BF);
    return 0;
}
