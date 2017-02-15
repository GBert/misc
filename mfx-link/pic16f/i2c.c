#include <xc.h>
#include <pic16f1705.h>

void i2c_Init(void) {
    //Initialise I2C MSSP
    //Master 100KHz

    SSP1CON1 = 0b00101000;	//I2C enabled Master mode
    SSP1CON2 = 0x00;
    //I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
    SSP1ADD = 78;		//100KHz @ 32MHz Fosc

    SSP1STAT = 0b11000000;	//Slew rate disabled
}
