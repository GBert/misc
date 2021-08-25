#include <pic.h> 
#include <xc.h>
#include "main.h"
#include "usart.h"

/*
 VDD VCC       1----14 VSS GND
 RA5 SDA       2    13 RA0 ICSPDAT
 RA4 SCL       3    12 RA1 ICSPCLK
 RA3 !MCLR VPP 4    11 RA2
 RC5           5    10 RC0
 RC4           6     9 RC1 RX
 RC3           7-----8 RC2 TX
 */

#pragma config FOSC=INTOSC, PLLEN=OFF, MCLRE=ON, WDTE=ON
#pragma config LVP=ON, CLKOUTEN=OFF

#define RX_ELMNTS		32
#define I2C_slave_address	0x7F

#define CLOCK		32000000

#define BAUDRATE	115200

#if 1
// array for master to read from
volatile unsigned char I2C_Array_Tx[RX_ELMNTS] =
{0xAA,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,
0x77,0x66,0x55,0x44,0x33,0x22,0x11,0xFA,
0xEA,0xDA,0xCA,0xBA,0xFB,0xFC,0xFD,0xFE,
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
#endif

// array for master to write to
volatile unsigned char I2C_Array_Rx[RX_ELMNTS] = 0;

volatile unsigned char complete, c;
unsigned char rx_data, tx_data;
struct serial_buffer tx_fifo, rx_fifo;

unsigned char index_i2c = 0;     // index pointer
unsigned char junk = 0;          // used to place unnecessary data
unsigned char clear = 0x00;      // used to clear array location once
                                 // it has been read

/**************************** INITIALIZE PERIPHERALS **************************/

/* RA4 SDA I2C
 * RA5 SCL I2C
 * RC1 RxD UART
 * RC2 TxD UART
 */

void init_pps(void) {
    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCK = 0;                // unlock PPS

    /* I2C */
    SSPDATPPS = 0x04;
    RA4PPS = 0b10001;           // RA4 output SDA
    SSPCLKPPS = 0x05;
    RA5PPS = 0b10000;           // RA5 output SCL

    // set USART : RX on RC1 , TX on RA1 / 40001729B.pdf page 141
    RXPPS = 0b10001;            // input  EUSART RX -> RC1
    RC2PPS = 0b10100;           // RC2 output TX/CK

    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCK = 1;                // lock PPS
    GIE = 1;
}

/**************************** INITIALIZE ROUTINE ******************************/
void init_system(void) {
    OSCCON = 0b11110000;        // Configure oscillator
	     //1------- use PLL to get 4x8 Mhz (system clock)
	     //-1110--- 8 MHz internal oscillator (instruction clock)
	     //------00 oscillator selected with INTOSC

    // switch off analog
    ANSELA = 0;
    ANSELC = 0;
    ADCON0 = 0;
    ADCON1 = 0;
    ADCON2 = 0;
    CM1CON0 = 0;
    CM1CON1 = 0;
    CM2CON0 = 0;
    CM2CON1 = 0;

    OPTION_REG = 0b11010111;	// WPU disabled, INT on rising edge, FOSC/4
				// Prescaler assigned to TMR0, rate @ 1:256
    WDTCON = 0b00010111;	// Prescaler 1:65536
				// period is 2 sec (RESET value)
    PORTA = 0x00;		// Clear PORTC
    LATA = 0b00110000;		// set PORTA latches

    TRISC1 = 1;			// USART RX
    TRISC2 = 0;			// UASRT TX

    TRISA4 = 1;			// I2C SDA
    TRISA5 = 1;			// I2C SCL

}//end init_system
/******************************************************************************/

void init_uart(void) {
    TX9 = 0;                    // 8-bit transmission
    TXEN = 1;                   // transmit enabled
    SYNC = 0;                   // asynchronous mode
    BRGH = 1;                   // high speed
    SPEN = 1;                   // enable serial port (configures RX/DT and TX/CK pins as serial port pins)
    RX9 = 0;                    // 8-bit reception
    CREN = 1;                   // enable receiver
    BRG16 = USE_BRG16;          // 8-bit baud rate generator
    SPBRG = SBRG_VAL;           // calculated by defines
    RCIE = 1;
}

/******************************************************************************/
void init_i2c(void) {
				//I2C SLAVE MODULE SET UP 
    SSPSTAT = 0b10000000;	// Slew rate control disabled for standard
				// speed mode (100 kHz and 1 MHz)
    SSPCON1 = 0b00110110; 	// Enable serial port, I2C slave mode, 
				// 7-bit address
    SSPCON2bits.SEN = 1;	// Clock stretching is enabled
    SSPCON3bits.BOEN = 1;	// SSPBUF is updated and NACK is generated
                                // for a received address/data byte,
                                // ignoring the state of the SSPOV bit
                                // only if the BF bit = 0
    SSPCON3bits.SDAHT = 1;	// Minimum of 300 ns hold time on SDA after
                                // the falling edge of SCL
    SSPCON3bits.SBCDE = 1;	// Enable slave bus collision detect interrupts
    SSPADD = I2C_slave_address; // Load the slave address
    SSP1IF = 0;			// Clear the serial port interrupt flag
    BCL1IF = 0;			// Clear the bus collision interrupt flag
    BCL1IE = 1;			// Enable bus collision interrupts
    SSP1IE = 1;			// Enable serial port interrupts
    PEIE = 1;			// Enable peripheral interrupts
    GIE = 1;			// Enable global interrupts
}

/*************************** ISR ROUTINE **************************************/
void interrupt ISR(void) {
    if(SSP1IF) {				// check to see if SSP interrupt
        if(SSPSTATbits.R_nW) {			// Master read (R_nW = 1)
            if(!SSPSTATbits.D_nA) {		// last byte was an address (D_nA = 0)
                junk = SSPBUF;			// dummy read to clear BF bit
                index_i2c = 0;			// clear index pointer
                if(index_i2c < RX_ELMNTS) {	// Does data exceed number of allocated bytes?
                    SSPBUF = I2C_Array_Tx[index_i2c];	// load SSPBUF with data
                    I2C_Array_Tx[index_i2c++] = clear;	// clear that location and increment index
                } else {			// trying to exceed array size
                    junk = SSPBUF;		// dummy read to clear BF bit
                }
                SSPCON1bits.CKP = 1;		// release CLK
            }
            if(SSPSTATbits.D_nA) {		// last byte was data
                if (index_i2c < RX_ELMNTS) {	// Does data exceed number of allocated bytes?
                    SSPBUF = I2C_Array_Tx[index_i2c];	// load SSPBUF with data
                    I2C_Array_Tx[index_i2c++] = clear;	// clear that location and increment index
                } else {
                    junk = SSPBUF;	// dummy read to clear BF bit
                }
                SSPCON1bits.CKP = 1;	// release CLK
            }
        }
        
        if(!SSPSTATbits.R_nW) {			// master write (R_nW = 0)
            if(!SSPSTATbits.D_nA) {		// last byte was an address (D_nA = 0)
                junk = SSPBUF;			// read buffer to clear BF
		SSPCON1bits.CKP = 1;		// release CLK
            }
            if(SSPSTATbits.D_nA) {		// last byte was data (D_nA = 1)
            	if (index_i2c < RX_ELMNTS) {	// Does data exceed number of allocated bytes?
                    I2C_Array_Rx[index_i2c++] = SSPBUF;
                } else {			// load array with data
                    index_i2c = 0;		// reset the index pointer
                }
		if(SSPCON1bits.WCOL) {		// Did a write collision occur?
                    SSPCON1bits.WCOL = 0;	// clear WCOL bit
                    junk = SSPBUF;		// clear SSPBUF
		}
		SSPCON1bits.CKP = 1;		// release CLK
            }
    	}
    }

    if(BCL1IF) {				// Did a bus collision occur?
        junk = SSPBUF;				// clear SSPBUF
	BCL1IF = 0;				// clear BCLIF
	SSPCON1bits.CKP = 1;			// Release CLK
    }
    SSP1IF = 0;					// clear SSPIF

    if (RCIF) {
	RCIF = 0;
	putc(RC1REG);
	if (RC1REG == 0x0d)
	    complete = 1;
    }

    if (TXIF) {
	TXIF = 0;
	tx_data=getc();
	if (tx_data) {
	    TX1REG = tx_data;
	} else {
	    TXIE = 0;
        }
    }
}

/**************************** MAIN ROUTINE ************************************/

void main(void) { 
    init_system();		// call initialize routine
    init_pps();			// call initialize routine
    init_i2c();
    init_uart();
    while(1) {			// program will wait here for ISR to be called
        asm("CLRWDT");		// clear WDT
    }
}
