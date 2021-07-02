//***************************************************************
//
// MFX-LINK2 Firmware 
//
//***************************************************************
//
// * 2021/07 V1.1 Initial Release
//
// ADC Reference is FVR at 4.096V
//
// ADC measured values for ISense
// IBT2 RS=10kOhm per Channel (result in 5 kOhm because Sense Outputs are parallel)
// MFX_LINK2 10kOhm to GND
// RSense total 3,33kOhm

// Offset values (measured Isense at no load) depends on hardware 
// and is different for each halfbridge
// Calibration:
// Read raw values with no Load and input Settings Offset-Values (pos and neg)
//and save
// Gain values for higher currents depends on hardware
// Gain is 1 /gain value

// measured adc values after offset correction 
// I      Hex pos/ne    Dez pos/neg
// 560mA =4A/49          74/73
// 3.0A  =1AF / 1A5     431/421
// 5.4A =314 2FF        781/762
// linear regression deliveres ~ 0,007 as slope 1/0.007= ~142...
// use about 142 as gain values

// U_In: R1=10kOhm R2=1.8kOhm
// 4.096 max ADC voltage -> max UIn = 26,85V

// used resources:
// TMR2 = System timer 1ms intervall
// TMR4 = Timer for ADC measur start
// TMR6 = Timer for I-Max off delay
// COG = Invertation of Data in for second IBT2 half bridge
// ADC = measuremnt of current and supply voltage IBT2
// SSP1 = I2C interface for LCD Display
// EUSART1 = serial interface for communication 115200,8,n,1
// WDT = watchdog timer 32ms
// PPS = peripheral pin select COG, I2C, EUSART


#include "config.h"
#include "i2c_lcd_states.h"
#include "memory.h"



#define BLINK_FAST 0xCCCC
#define BLINK_HEARTBEAT_BOOSTER_ON 0xFFF0
#define BLINK_HEARTBEAT_BOOSTER_OFF 0xF000

//ADC channels:
// RC0 AN4   I_Sense
// RA2 AN2   Poti Imax
// RC6 AN8   U:n
#define ADCON_POTI		0x09 //0 00010 = AN2 0 1
#define ADCON_ISENSE    0x11 //0 00100 = AN4 0 1
#define ADCON_UIN       0x21 //0 01000 = AN8 0 1

typedef enum {CH_ISENSE, CH_UIN} ADC_CHANNEL_ACTIVE_T; 

// Bitmuster fuer user defined LCD-character  Abschaltschwelle 
// 11100
// 00100
// 00100
// 10101
// 01110
// 00100
// 00100
// 00111
const uint8_t CharTrip[8] =  { 0x1C, 0x04, 0x04, 0x15, 0x0E, 0x04, 0x04, 0x07 };

typedef struct
{
    uint8_t validflag ; //0x99 if data is valid
    uint8_t imax;       //1..85 in 100mA steps
    uint16_t t_imax;    //delay time for imax off (1ms per bit)   
    uint16_t umin;      //minimum voltage level in 100mV steps 
    uint16_t umax;      //maximum voltage level in 100mV steps 
    uint16_t pos_offset; //IBT2 offset value Isense
    uint16_t neg_offset; //IBT2 offset value Isense
    uint16_t pos_gain;    //IBT2 reciprocal gain value Isense   
    uint16_t neg_gain;    //IBT2 reciprocal gain value Isense    
} SETTINGS_T;

typedef union
{
   SETTINGS_T values;
   uint8_t bytevals[sizeof(SETTINGS_T)]; 
} EEP_SETTINGS_T;

//__at(0x1F80)

const EEP_SETTINGS_T EEP_SettingsRom  __at(0x1F80) = {EEP_VALID_FLAG, I_MAXDEFAULT, I_MAX_DELAY, 0, 220, 
                    ISENSE_POSOFFSET,ISENSE_NEGOFFSET, POS_GAIN, NEG_GAIN};

EEP_SETTINGS_T EEP_Settings;

const uint8_t HelpString[]=
"Booster V1.1\r\n\
Help:\r\n\
!<CR> Get Status\r\n\
V<CR> Get Version\r\n\
U<CR> Get Voltage\r\n\
UMIN<CR> Get Umin\r\n\
UMIN=xx.x<CR> Set Umin(00.0...26.0\r\n\
UMAX<CR> Get Umax\r\n\
UMAX=xx.x<CR> Set Umax(00.0...26.0\r\n\
I<CR> Get Current\r\n\
IR<CR> Get raw Current\r\n\
IC<CR> Get corrected Current\r\n\
IMAX<CR> Get I_max A\r\n\
IMAX=x.x<CR> Set I_max A (0.1...8.5)\r\n\
T<CR> Get I_max delay ms\r\n\
T=xxx<CR> Set I_max delay ms(1...255)\r\n\
OP<CR> Get positive Offset\r\n\
OP=xxx<CR> Set positive Offset(1...255)\r\n\
ON<CR> Get negative Offset\r\n\
ON=xxx<CR> Set negative Offset(1...255)\r\n\
GP<CR> Get positive gain\r\n\
GP=xxx<CR> Set positive gain(1...255)\r\n\
GN<CR> Get negative gain\r\n\
GN=xxx<CR> Set negative gain(1...255)\r\n\
SA<CR> Save nonvolatile\r\n\
R<CR> Recall nonvolatile\r\n\
";

typedef union
{
    uint8_t byteval[2];
    uint16_t uintval;
    int16_t intval;
    
} UNION16BIT_T;

typedef struct
{
    unsigned DO_10MS_TASK:1;
    unsigned DO_100MS_TASK:1;
    unsigned DO_1S_TASK:1;
    unsigned DO_PARSECMD:1;
    unsigned DO_SENDHELP:1;
    unsigned DO_REDRAW:1;
} SYSTASKBITS_T;

typedef union
{
struct
{
    unsigned IN_DATA_DETECT:1;
    unsigned BOOSTER_IMAX_DETECT:1;
    unsigned BOOSTER_IMAX_WARNING:1;
    unsigned BOOSTER_UMAX_DETECT:1;
    unsigned BOOSTER_UMIN_DETECT:1;
    
} bits;
uint8_t byteval;
} BOOSTER_STATUS_T;

uint8_t rx_buff[RX_BUFSIZE];
uint8_t rx_dat;
uint8_t rx_wr_index;
uint8_t rx_rd_index;

uint8_t tx_buff[TX_BUFSIZE];
uint8_t tx_count;
uint8_t tx_wr_index;
uint8_t tx_rd_index;
uint16_t tx_helpindex;


volatile SYSTASKBITS_T systaskstat;
volatile uint8_t count1ms;
volatile uint8_t count10ms;
volatile uint8_t count100ms;
volatile uint8_t count1s;
volatile BOOSTER_STATUS_T booster_status;
BOOSTER_STATUS_T booster_drawn_status;
volatile uint16_t imax_Toffcount;
volatile uint8_t imax_poscount;
volatile uint8_t imax_negcount;

//ADC_CHANNEL_ACTIVE_T adc_ch_selection;
volatile uint16_t adc_value;

volatile uint16_t adc_sense_possumm;
volatile uint16_t adc_sense_possummmain;
volatile uint16_t adc_sense_negsumm;
volatile uint16_t adc_sense_negsummmain;
volatile uint8_t adc_sense_posctr;
volatile uint8_t adc_sense_negctr;

volatile uint16_t adc_uin_summ;
volatile uint16_t adc_uin_summmain;
volatile uint8_t adc_uin_ctr;

uint8_t in_pulsecount;
uint16_t led_blink;

volatile uint16_t imax_pos_adc;
volatile uint16_t imax_neg_adc;

uint16_t uin_100mV;
uint16_t uin_raw;

uint16_t i_pos_raw;
uint16_t i_pos_corr;
uint16_t i_pos100mA;
uint16_t i_neg_raw;
uint16_t i_neg_corr;
uint16_t i_neg100mA;
uint8_t draw_settings_state;
uint8_t draw_settings_count;

void ADC_DelayStart(void);
void Calc_Imax_ADC(void);

void __interrupt() isr(void)
//void isr(void) __interrupt(0) 
{
    if(IOCIF)
    { //interrupt on change (data in gleisbox)
        in_pulsecount++;
        if(in_pulsecount == 0)
            in_pulsecount = 0xFF;
        //check adc channel isense and start new conversion cycle
        ADGO=0;
        TMR4=0;
        PR4=16;
        ADIF=0;
        
        IOCCF = 0;
    }
    if(ADIF && ADIE)
    {
        adc_value = ADRES;
        ADCON2 = 0x00; //TMR4 Match auto conversion start
        if(ADCON0 == ADCON_ISENSE)
        {
            if(IOC_PORT)
            {
                if(adc_value >= imax_pos_adc)
                { //I max pos reached
                    if(T6CONbits.TMR6ON == 0)
                    {
                        imax_Toffcount = EEP_Settings.values.t_imax;
                        if(imax_Toffcount!=0)
                        {
                            booster_status.bits.BOOSTER_IMAX_WARNING = 1;
                            TMR6 = 0;
                            T6CON = 0x03; //0 0000 0 11 prescale 1:64 Tcount=2ms
                            PR6 = 125;
                            TMR6IF = 0;
                            TMR6IE = 1;
                            T6CONbits.TMR6ON = 1;
                        }
                        else
                        {
                            LATB4 = 0;
                            booster_status.bits.BOOSTER_IMAX_DETECT = 1;
                            led_blink = BLINK_FAST;
                        }
                    }
                    if(imax_poscount < 4)
                        imax_poscount++;
                }
                else if(T6CONbits.TMR6ON) //if(booster_status.bits.BOOSTER_IMAX_WARNING)
                {
                    if(adc_value < imax_pos_adc - 5)
                    {
                        if(imax_poscount)
                            imax_poscount--;
                        if((imax_poscount == 0)&&(imax_negcount == 0))
                        {
                            TMR6IE = 0;
                            T6CONbits.TMR6ON = 0;
                            booster_status.bits.BOOSTER_IMAX_WARNING = 0;
                        }
                    }
                }
                adc_sense_possumm += adc_value;
                adc_sense_posctr++;
                if((adc_sense_posctr & 0x1F) == 0)
                {
                    adc_sense_possummmain = adc_sense_possumm;
                    adc_sense_possumm = 0;
                    adc_sense_posctr = 0;
                }
            }
            else
            {
                if(adc_value >= imax_neg_adc)
                { //I max neg reached
                    if(T6CONbits.TMR6ON == 0)
                    {
                       imax_Toffcount = EEP_Settings.values.t_imax;
                        if(imax_Toffcount!=0)
                        {
                            booster_status.bits.BOOSTER_IMAX_WARNING = 1;
                            TMR6 = 0;
                            T6CON = 0x03; //0 0000 0 11 prescale 1:64 Tcount=2ms
                            PR6 = 125;
                            TMR6IF = 0;
                            TMR6IE = 1;
                            T6CONbits.TMR6ON = 1;
                        }
                        else
                        {
                            LATB4 = 0;
                            booster_status.bits.BOOSTER_IMAX_DETECT = 1;
                            led_blink = BLINK_FAST;
                        }
                    }
                    if(imax_negcount < 4)
                        imax_negcount++;
                }
                else if(T6CONbits.TMR6ON) //if(booster_status.bits.BOOSTER_IMAX_WARNING)
                {
                    if(adc_value < imax_neg_adc - 5)
                    {
                        if(imax_negcount)
                            imax_negcount--;
                        if((imax_poscount == 0)&&(imax_negcount == 0))
                        {
                            TMR6IE = 0;
                            T6CONbits.TMR6ON = 0;
                            booster_status.bits.BOOSTER_IMAX_WARNING = 0;
                        }
                    }
                }
                adc_sense_negsumm += adc_value;
                adc_sense_negctr++;
                if((adc_sense_negctr & 0x1F) == 0)
                {
                    adc_sense_negsummmain = adc_sense_negsumm;
                    adc_sense_negsumm = 0;
                    adc_sense_negctr = 0;
                }
            }
            PR4 = 20; //8탎 to next conversion start
        }
        else if(ADCON0 == ADCON_UIN)
        {
            adc_uin_summ += adc_value; // (ADRESH << 8) + ADRESL;
            adc_uin_ctr++;
            if((adc_uin_ctr & 0x07) == 0)
            {
                adc_uin_summmain = adc_uin_summ;
                adc_uin_summ = 0;
                adc_uin_ctr = 0;
                if(LATB4)
                {
                    ADCON0 = ADCON_ISENSE;
                    //delay next conversion start
                    PR4 = 20; //10탎 to delay
                }
                else
                {
                    PR4 = 100; //50탎 to next measure
                }
            }
        }
        else
        {
            ADCON0 = ADCON_ISENSE;
            //delay next conversion start
            PR4 = 20; //10탎 to delay 
        }
        TMR4 = 0;
        ADCON2 = 0b11000000; //TMR4 Match auto conversion start
        ADIF = 0;
    }
    if(RCIF && RCIE)
    {
        rx_dat = RC1REG;
        RCIF = 0;
        rx_buff[rx_wr_index++] = rx_dat;
        if(rx_wr_index == RX_BUFSIZE)
        {
            rx_wr_index = 0;
        }
        if(rx_dat == 0x0D)
        {
            systaskstat.DO_PARSECMD = 1;
        }
    }
    if(TMR6IE & TMR6IF)
    {
        TMR6IF = 0;
        if(imax_Toffcount == 0)
        {
            LATB4 = 0;
            booster_status.bits.BOOSTER_IMAX_DETECT = 1;
            booster_status.bits.BOOSTER_IMAX_WARNING = 0;
            TMR6IE = 0;
            T6CONbits.TMR6ON = 0;
            led_blink = BLINK_FAST;
        }
        else
        {
            imax_Toffcount--;
        }
    }

}

/* RA4 SDA I2C
 * RA5 SCL I2C
 * RC1 RxD UART
 * RC2 TxD UART
 * RC3 Rail Signal
 * RB5 RPWM
 * RB6 LPWM
 */

void pps_init(void)
{
    //    PPSLOCK = 0x55;
    //    PPSLOCK = 0xaa;
    //    PPSLOCK = 0;		// unlock PPS
    /* I2C */
    SSPCLKPPS = 0x05;
    RA5PPS = 0b10000; // RA5 output SCL
    SSPDATPPS = 0x04;
    RA4PPS = 0b10001; // RA4 output SDA
    /* USART */
    RXPPS = 0b10001; // input  EUSART RX -> RC1
    RC2PPS = 0b10100; // RC2 output TX/CK
    /* CLC */
    /* RB4PPS = 0b00100;		// LC1OUT -> ENABLE */
    /* COG */
    COGINPPS = 0b10011; // RC3 PULSE
    RB5PPS = 0b01001; // COG1B -> RPWM
    RB6PPS = 0b01000; // COG1A -> LPWM
    //    PPSLOCK = 0x55;
    //    PPSLOCK = 0xaa;
    //    PPSLOCK = 1;		// lock PPS
}

void system_init(void)
{
    // switch off analog
    OSCCON = 0b11110000; // Configure oscillator
    //         1------- use PLL to get 4x8 Mhz (system clock)
    //         -1110--- 8 MHz internal oscillator (instruction clock)
    //         ------00 oscillator selected with INTOSC
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ADCON0 = 0;
    ADCON1 = 0;
    ADCON2 = 0;
    CM1CON0 = 0;
    CM1CON1 = 0;
    CM2CON0 = 0;
    CM2CON1 = 0;
    //init WDT to 32ms
    WDTCON = 0x0B; //00 00101 1  00101=1:1024 (Interval 32 ms nominal) SWDTEN=1
    /* I2C MSSP 40001729B.pdf page 302 */
    TRISA4 = 1;
    TRISA5 = 1;
    TRISC0 = 0;
    /* USART */
    TRISC1 = 1;
    TRISC2 = 0;
    /* RA2&RC0 analog input */
    TRISA2 = 1;
    TRISB5 = 0;
    TRISB6 = 0;
    TRISC0 = 1;
    TRISC3 = 1; /* Rail Data */
    TRISB4 = 0; /* Enable */
    TRISC5 = 0; /* LED */
    // setup interrupt events
    //clear all relevant interrupt flags
    SSP1IF = 0;
    TMR1IF = 0;
    CCP1IF = 0;
    // activate IOC on Rail Data
    IOCCP3 = 1;
    IOCCN3 = 1;

}

void i2c_init(void)
{
    /* Initialise I2C MSSP Master */
    SSP1CON1 = 0b00101000;
    SSP1CON2 = 0x00;
    SSP1CON3 = 0x00;
    /* Master at 100KHz */
    /* clock = FOSC/(4 * (SSPADD + 1)) */
    SSP1ADD = SSP1ADD_VAL;
    /* Slew rate disabled */
    SSP1STAT = 0b11000000;
}

void ad_init(void)
{
    FVRCON = 0x8F; //1 x 0 0 11 11 Comp=4.096V ADC=4.096V
    /* RA2&RC0&RC6 analog */
    ANSELA = 0x04; //1 << 2;
    ANSELC = 0x41; // 1 << 0;
    /* right justified ; FOSC/64 ;VREF- GND & VREF+ FVR */
    ADCON1 = 0b10100011;
    ADCON2 = 0b11000000; //TMR4 Match auto conversion start
}

void uart_init(void)
{
    WPU_RX = 1;
    TX9 = 1; // 8-bit transmission
    TX9D = 1; //  one extra stop bit
    TXEN = 1; // transmit enabled
    SYNC = 0; // asynchronous mode
    BRGH = 1; // high speed
    SPEN = 1; // enable serial port (configures RX/DT and TX/CK pins as serial port pins)
    RX9 = 0; // 8-bit reception
    CREN = 1; // enable receiver
    BRG16 = USE_BRG16; // 8-bit baud rate generator
    SPBRG = SBRG_VAL; // calculated by defines
    RCIF = 0;
    RCIE = 1;
}

//void timer0_init(void) {
//    TMR0CS = 0;			// FOSC / 4
//    PSA = 0;			// use prescaler
//    PS1 = 1;			// prescaler 1:8
//    TMR0 = TIMER0_VAL;
//}

void timer2_init(void)
{
    //init Timer2 to 1ms tick
    T2CON = 0b00000111;
    //        -0000--- postscaler 1:1
    //        -----1-- timer on
    //        ------11 prescaler 1:64 (overflow every 16ms)
    TMR2 = 0; // reset timer2
    PR2 = 125; //this will force Iint every 1ms
    TMR2IF = 0;
}

void timer4_init(void)
{
    TMR4ON = 0;
    T4CON = 0x01;
    //        -0000--- postscaler 1:1
    //        -----1-- timer on
    //        ------01 prescaler 1:4 (Tcount = 1/(32MHz/4)*4=0.5탎
}

void clc_init(void)
{
    CLC1SEL0 = 0;
    CLC1SEL1 = 0;
    //LC1OE = 1;
    //LC1EN = 1;
}

void cog_init(void)
{
    COG1CON0 = 0;
    COG1CON1 = 0;
    COG1RIS = 0x1; /* COG1PPS rising event		*/
    COG1RSIM = 0x0; /* no rising delay		*/
    COG1FIS = 0x1; /* COG1PPS falling event	*/
    COG1FSIM = 0x0; /* no falling delay		*/
    COG1ASD0 = 0; /* don't use shutdown		*/
    COG1ASD1 = 0;
    COG1STR = 0; /* don't use steering control	*/
    COG1DBR = 0; /* no dead band			*/
    COG1DBF = 0;
    COG1BLKR = 0; /* don't use blanking		*/
    COG1BLKF = 0;
    COG1PHR = 0; /* normal phase			*/
    COG1PHF = 0;

    COG1CON0 = 0b10001100;
    /* 1-------	G1EN COG1 Enable
   -0------	G1LD0 no buffer
   ---01---	G1CS Fosc clock source
   -----100	G1MD COG Half Bridge mode */
}

//*********************************************************
// Nonvolatile settings read / write
//*********************************************************

void Write_Settings(void)
{
    uint8_t i;
    uint16_t buf[32];
    uint16_t flashadr = HEF_STARTADRESS; //nonvolatile memory in flash

    for(i = 0; i < 32; i++)
    {
        buf[i] = 0xFFFF;
    }
    for(i = 0; i<sizeof (EEP_Settings); i++)
    {
        buf[i & 0x1F] = (uint16_t) EEP_Settings.bytevals[i];
        if(((i & 0x1F) == 0x1F) || (i == sizeof (EEP_Settings) - 1))
        {
            FLASH_WriteBlock(flashadr, &buf[0]);
            flashadr += 32;
        }
    }
}

void Read_Settings(void)
{
    union
    {
        uint16_t ui16;
        uint8_t ui8[2];
    } uu16;
    uint8_t nrofbytes;
    uint8_t i;
    
    nrofbytes = sizeof (EEP_Settings);
    uu16.ui16 = FLASH_ReadWord(HEF_STARTADRESS);
    if(uu16.ui8[0] == EEP_VALID_FLAG)
    {
        for(i = 0; i<sizeof (EEP_Settings); i++)
        {
            uu16.ui16 = FLASH_ReadWord(HEF_STARTADRESS + i);
            EEP_Settings.bytevals[i] = uu16.ui8[0];
        }
    }
    else
    {
        Write_Settings();
    }
}

//*********************************************************
// ADC dependend Functuions
//*********************************************************

void ADC_StartDelay(void)
{
    ADGO = 0; //stop current Conversion
    //delay next conversion start 5 us
    TMR4 = 0;
    ADIF = 0;
    PR4 = 30; //15탎 to delay
    ADIE = 1;
}

void ADC_SetISense(void)
{
//    adc_ch_selection = CH_ISENSE;
    ADCON0 = ADCON_ISENSE;
    ADC_StartDelay();
}

void ADC_SetUinSense(void)
{
//    adc_ch_selection = CH_UIN;
    ADC_StartDelay();
    ADCON0 = ADCON_UIN;
    adc_uin_summ = 0;
    adc_uin_ctr = 0;
}

void Calc_Ineg(void)
{
    uint16_t mathdummy;
    
    di();
    mathdummy = adc_sense_negsummmain;
    ei();
    i_neg_raw = mathdummy >> 5;
    i_neg_corr = i_neg_raw - EEP_Settings.values.neg_offset;
    if(i_neg_corr & 0x8000)
    {
        i_neg_corr = 0;
        i_neg100mA = 0;
    }
    else
    {
        mathdummy = i_neg_corr * 10;
        i_neg100mA = mathdummy / EEP_Settings.values.neg_gain;
    }
}

void Calc_Ipos(void)
{
    uint16_t mathdummy;
    
    di();
    mathdummy = adc_sense_possummmain;
    ei();
    i_pos_raw = mathdummy >> 5;
    i_pos_corr = i_pos_raw - EEP_Settings.values.pos_offset;
    if(i_pos_corr & 0x8000)
    {
        i_pos_corr = 0;
        i_pos100mA = 0;
    }
    else
    {
        mathdummy = i_pos_corr * 10;
        i_pos100mA = mathdummy / EEP_Settings.values.pos_gain;
    }
}

//void Calc_Uin(void)
//{
//    //no load  500mA
//    //20V=306 304 (U=/ADC/38,8) 0.0258
//    //18V=2B9 2B6 
//    //16V=269 
//    //14V=21F 21C       0.02578
//    //12V=1CF
//    //10V=182       0.0259
//    uint16_t mathdummy;
//    
//    di();
//    uin_raw = adc_uin_summmain;
//    ei();
//    uin_raw = uin_raw >> 3;
//    mathdummy = uin_raw;
//    mathdummy *= 10u;
//    mathdummy = mathdummy / 39u;
//    mathdummy += 2;
//    uin_100mV = (uint16_t) mathdummy;
//    if(uin_100mV > EEP_Settings.values.umax)
//    {
//        booster_status.bits.BOOSTER_UMAX_DETECT = 1;
//        LATB4 = 0;
//    }
//    else
//    {
//        booster_status.bits.BOOSTER_UMAX_DETECT = 0;
//    }
//    if(uin_100mV < EEP_Settings.values.umin)
//    {
//        booster_status.bits.BOOSTER_UMIN_DETECT = 1;
//        LATB4 = 0;
//    }
//    else
//    {
//        booster_status.bits.BOOSTER_UMIN_DETECT = 0;
//    }
//}
void Calc_Uin(void)
{
    //no load  500mA
    //20V=306 304 (U=/ADC/38,8) 0.0258
    //18V=2B9 2B6 
    //16V=269 
    //14V=21F 21C       0.02578
    //12V=1CF
    //10V=182       0.0259
    uint32_t mathdummy;
    
    di();
    uin_raw = adc_uin_summmain;
    ei();
    uin_raw = uin_raw >> 3;
    mathdummy = (uint32_t) uin_raw;
    mathdummy *= 100u;
    mathdummy = mathdummy / 500u;
    uin_100mV = (uint16_t) mathdummy;
    if(uin_100mV > EEP_Settings.values.umax)
    {
        booster_status.bits.BOOSTER_UMAX_DETECT = 1;
        LATB4 = 0;
    }
    else
    {
        booster_status.bits.BOOSTER_UMAX_DETECT = 0;
    }
    if(uin_100mV < EEP_Settings.values.umin)
    {
        booster_status.bits.BOOSTER_UMIN_DETECT = 1;
        LATB4 = 0;
    }
    else
    {
        booster_status.bits.BOOSTER_UMIN_DETECT = 0;
    }
}


void Calc_Imax_ADC(void)
{
    uint16_t mathdummy;
    
    mathdummy = (uint16_t) EEP_Settings.values.imax * EEP_Settings.values.pos_gain;
    mathdummy = mathdummy / 10;
    mathdummy += EEP_Settings.values.pos_offset;
    di();
    imax_pos_adc = mathdummy;
    ei();
    mathdummy = (uint16_t) EEP_Settings.values.imax * EEP_Settings.values.neg_gain;
    mathdummy = mathdummy / 10;
    mathdummy += EEP_Settings.values.neg_offset;
    di();
    imax_neg_adc = mathdummy;
    ei();
#ifdef USE_BARGRAPH
    LCD_BargraphSetFullScale(imax_pos_adc, imax_neg_adc);
#endif    
}

//*********************************************************
// Conversion and Helper functions
//*********************************************************
//void prints(char *psource, uint8_t *pdest)
//{
//    while (*psource)
//    {
//        *(pdest++) = (uint8_t)*(psource++);
//    }
//}

char nibble_to_hex(uint8_t c)
{
    char nibble;
    
    nibble = (c & 0x0f) + '0';
    if(nibble >= 0x3a)
        nibble += 7;
    return (nibble);
}

void Draw_10BitHex(uint8_t x, uint8_t y, uint16_t val10bit)
{
    UNION16BIT_T shift_val;
    uint8_t nibble;
    
    shift_val.uintval = val10bit;
    LCD_gotobufxy(x, y);
    nibble = shift_val.byteval[1] & 0x0F;
    LCD_putchbuf(nibble_to_hex(nibble));
    nibble = (shift_val.byteval[0] >> 4) & 0x0F;
    LCD_putchbuf(nibble_to_hex(nibble));
    nibble = shift_val.byteval[0] & 0x0F;
    LCD_putchbuf(nibble_to_hex(nibble));
}

void U16ToHex(uint16_t val16bit, uint8_t *pdest)
{
    UNION16BIT_T shift_val;
    uint8_t nibble;
    
    shift_val.uintval = val16bit;
    nibble = (shift_val.byteval[1] >> 4) & 0x0F;
    *pdest++ = nibble_to_hex(nibble);
    nibble = shift_val.byteval[1] & 0x0F;
    *pdest++ = nibble_to_hex(nibble);
    nibble = (shift_val.byteval[0] >> 4) & 0x0F;
    *pdest++ = nibble_to_hex(nibble);
    nibble = shift_val.byteval[0] & 0x0F;
    *pdest++ = nibble_to_hex(nibble);
}

void U16ToDez(uint16_t val16bit, uint8_t *pdest, uint8_t do16bit)
{
    uint16_t rest;
    uint8_t digit;

    rest = val16bit;
    if(do16bit)
    {
        digit = 0x30u;
        while(rest >= 10000)
        {
            digit++;
            rest -= 10000;
        }
        *(pdest++) = digit;
        digit = 0x30u;
        while(rest >= 1000)
        {
            digit++;
            rest -= 1000;
        }
        *(pdest++) = digit;
    }
    digit = 0x30u;
    while(rest >= 100)
    {
        digit++;
        rest -= 100;
    }
    *(pdest++) = digit;
    digit = 0x30u;
    while(rest >= 10)
    {
        digit++;
        rest -= 10;
    }
    *(pdest++) = digit;
    *pdest = (uint8_t) rest + 0x30u;
}

//*********************************************************
// Serial interface functions
//*********************************************************

void Tx_Putc(uint8_t c)
{
    tx_buff[tx_wr_index++] = c;
    if(tx_wr_index == TX_BUFSIZE)
        tx_wr_index = 0;
    tx_count++;
}

void SendAnswer(uint8_t *pdat, uint8_t nrofbytes)
{
    while(nrofbytes)
    {
        Tx_Putc(*(pdat++));
        nrofbytes--;
    }
}

void SendString(const unsigned char *pstr)
{
    while(*pstr != 0)
    {
        Tx_Putc(*(pstr++));
    }
}

uint8_t ParseNumber(uint8_t *psource, uint16_t *pdest, uint16_t min, uint16_t max, uint8_t kommaexpected)
{
    uint8_t *p_temp;
    uint8_t asc_nums = 0;
    uint8_t kommadet = 0;
    uint16_t val = 0;
    
    p_temp = psource;
    while((*p_temp != 0x0D)&&(*p_temp != 0))
    {
        if((*p_temp <= '9')&&(*p_temp >= '0'))
        {
            asc_nums++;
            if(kommadet == 1)
            { //dec point aready detected shift
                *(p_temp - 1) = *p_temp;
            }
        }
        else if(*p_temp == '.')
        {
            kommadet = 1;
        }
        else
        {
            return 0; //wrong char detected
        }
        p_temp++;
    }
    if(kommaexpected != kommadet)
    {
        return 0; //no komma detected
    }
    if((asc_nums > 0)&&(asc_nums < 6))
    {
        if(asc_nums == 5)
        {
            val += ((*psource++) - 0x30) * 10000;
            asc_nums--;
        }
        if(asc_nums == 4)
        {
            val += ((*psource++) - 0x30) * 1000;
            asc_nums--;
        }
        if(asc_nums == 3)
        {
            val += ((*psource++) - 0x30) * 100;
            asc_nums--;
        }
        if(asc_nums == 2)
        {
            val += ((*psource++) - 0x30) * 10;
            asc_nums--;
        }
        if(asc_nums == 1)
        {
            val += ((*psource++) - 0x30);
            asc_nums--;
        }
    }
    else
    {
        return 0;
    }
    if((val < min) || (val > max))
    {
        return 0;
    }
    *pdest = val;
    return 1;
}


void Send_MaxVal_Hold(void)
{
    if(booster_status.bits.BOOSTER_IMAX_DETECT)
    {
       SendString(":IS MAX BOOSTER IS OFF"); 
    }
}

void DoParseCmd(void)
{
    uint8_t dummy[6];
    uint16_t dummy_ui16;
    uint8_t err = 1; //default
    
    switch(rx_buff[0])
    {
        case '?':
        case 'h':
        case 'H':
            tx_count = 1;
            tx_helpindex = 0;
            systaskstat.DO_SENDHELP = 1;
            err = 0;
            break;
        case 'V': //Send Version
            SendString("Booster MFX-Link V1.0\r\n");
            err = 0;
            break;
        case '!': //Send status
            if(LATB4)
            {
                SendString("On");
            }
            else
            {
                SendString("Off:");
            }
            if(booster_status.bits.BOOSTER_IMAX_DETECT)
            {
                SendString("IMAX");
            }
            else if(booster_status.bits.BOOSTER_UMAX_DETECT)
            {
                SendString("UMAX");
            }
            else if(booster_status.bits.BOOSTER_UMIN_DETECT)
            {
                SendString("UMIN");
            }
            else if(booster_status.bits.IN_DATA_DETECT == 0)
            {
                SendString("NODAT");
            }
            Tx_Putc(0x0D);
            err = 0;
            break;
        case 'U': //Send voltage U_in
            if(rx_buff[1] == 'M')
            {
                if((rx_buff[2] == 'I')&&(rx_buff[3] == 'N'))
                {
                    if(rx_buff[4] == '=')
                    { //set Umin
                        err = 0;
                        if(ParseNumber(&rx_buff[5], &dummy_ui16, 0, 240, 1))
                        {
                            EEP_Settings.values.umin = dummy_ui16;
                            rx_buff[4] = 0x0D; // send Umin 
                        }
                        else
                        {
                            SendString("ERR(0.0 ... 24.0)\r\n");
                        }
                    }
                    if(rx_buff[4] == 0x0D)
                    { //send U_min
                        err = 0;
                        SendString("UMIN=");
                        U16ToDez(EEP_Settings.values.umin, &dummy[0], 1);
                        Tx_Putc(dummy[2]);
                        Tx_Putc(dummy[3]);
                        Tx_Putc('.');
                        Tx_Putc(dummy[4]);
                        Tx_Putc('V');
                        Tx_Putc(0x0D);
                    }
                }
                if((rx_buff[2] == 'A')&&(rx_buff[3] == 'X'))
                {
                    if(rx_buff[4] == '=')
                    { //set Uax
                        err = 0;
                        if(ParseNumber(&rx_buff[5], &dummy_ui16, 0, 240, 1))
                        {
                            EEP_Settings.values.umax = dummy_ui16;
                            rx_buff[4] = 0x0D; // send Umin 
                        }
                        else
                        {
                            SendString("ERR(8.0 ... 24.0)\r\n");
                        }
                    }
                    if(rx_buff[4] == 0x0D)
                    { //send U_min
                        err = 0;
                        SendString("UMAX=");
                        U16ToDez(EEP_Settings.values.umax, &dummy[0], 1);
                        Tx_Putc(dummy[2]);
                        Tx_Putc(dummy[3]);
                        Tx_Putc('.');
                        Tx_Putc(dummy[4]);
                        Tx_Putc('V');
                        Tx_Putc(0x0D);
                    }
                }
            }
            if(rx_buff[1] == 0x0D)
            {
                err = 0;
                SendString("U=");
                U16ToDez(uin_100mV, &dummy[0], 1);
                Tx_Putc(dummy[2]);
                Tx_Putc(dummy[3]);
                Tx_Putc('.');
                Tx_Putc(dummy[4]);
                Tx_Putc('V');
                Tx_Putc(0x0D);
            }
            break;
        case 'I': //Isense commands
            if((rx_buff[1] == 'M')&&(rx_buff[2] == 'A')&&(rx_buff[3] == 'X'))
            {
                if(rx_buff[4] == '=')
                { //Set I_Max
                    err = 0;
                    if(ParseNumber(&rx_buff[5], &dummy_ui16, 0, 240, 1))
                    {
                        EEP_Settings.values.imax = (uint8_t) dummy_ui16;
                        Calc_Imax_ADC();
                        rx_buff[4] = 0x0D; // send Umin 
                    }
                    else
                    {
                        SendString("ERR(0.1 ... 8.5)\r\n");
                    }
                }
                if(rx_buff[4] == 0x0D)
                {
                    // send I_Max off
                    err = 0;
                    SendString("IMAX=");
                    U16ToDez((uint16_t) EEP_Settings.values.imax, &dummy[0], 0);
                    Tx_Putc(dummy[1]);
                    Tx_Putc('.');
                    Tx_Putc(dummy[2]);
                    Tx_Putc('A');
                    Tx_Putc(0x0D);
                }
            }
            else if(rx_buff[1] == 0x0D)
            { //send I sense
                err = 0;
                SendString("I=");
                U16ToDez(i_pos100mA, &dummy[0], 0);
                Tx_Putc(dummy[1]);
                Tx_Putc('.');
                Tx_Putc(dummy[2]);
                Tx_Putc('A');
                Tx_Putc(':');
                U16ToDez(i_neg100mA, &dummy[0], 0);
                Tx_Putc(dummy[1]);
                Tx_Putc('.');
                Tx_Putc(dummy[2]);
                Tx_Putc('A');
                Send_MaxVal_Hold();
                Tx_Putc(0x0D);
                
            }
            else if(rx_buff[1] == 'R')
            { //send Iraw
                err = 0;
                SendString("IR=");
                U16ToHex(i_pos_raw, &dummy[0]);
                SendAnswer(&dummy[0], 4);
                Tx_Putc('h');
                Tx_Putc(':');
                U16ToHex(i_neg_raw, &dummy[0]);
                SendAnswer(&dummy[0], 4);
                Tx_Putc('h');
                Send_MaxVal_Hold();
                Tx_Putc(0x0D);
            }
            else if(rx_buff[1] == 'C')
            { //send Iraw
                err = 0;
                SendString("IC=");
                U16ToHex(i_pos_corr, &dummy[0]);
                SendAnswer(&dummy[0], 4);
                Tx_Putc('h');
                Tx_Putc(':');
                U16ToHex(i_neg_corr, &dummy[0]);
                SendAnswer(&dummy[0], 4);
                Tx_Putc('h');
                Send_MaxVal_Hold();
                Tx_Putc(0x0D);
            }
            break;
        case 'T': //I_max off delay read/write 
            if(rx_buff[1] == '=')
            { //Set I_Max DELAY
                err = 0;
                if(ParseNumber(&rx_buff[2], &dummy_ui16, 0, 999, 0))
                {
                    EEP_Settings.values.t_imax = dummy_ui16;
                    rx_buff[1] = 0x0D; // send Umin 
                }
                else
                {
                    SendString("ERR(0 ... 999)\r\n");
                }
            }
            if(rx_buff[1] == 0x0D)
            {
                SendString("T=");
                U16ToDez((uint16_t) EEP_Settings.values.t_imax, &dummy[0], 1);
                SendAnswer(&dummy[1], 4);
                Tx_Putc(0x0D);
                err = 0;
            }
            break;
        case 'O': //Isense offset (pos/neg) commands
            if(rx_buff[1] == 'P')
            {
                if(rx_buff[2] == '=')
                { //Set Offset positive
                    err = 0;
                    if(ParseNumber(&rx_buff[3], &dummy_ui16, 0, 255, 0))
                    {
                        EEP_Settings.values.pos_offset = dummy_ui16;
                        Calc_Imax_ADC();
                        rx_buff[2] = 0x0D; // send Umin 
                    }
                    else
                    {
                        SendString("ERR(0 ... 255)\r\n");
                    }
                }
                if(rx_buff[2] == 0x0D)
                {
                    SendString("OP=");
                    U16ToDez((uint16_t) EEP_Settings.values.pos_offset, &dummy[0], 0);
                    SendAnswer(&dummy[0], 3);
                    Tx_Putc(0x0D);
                    err = 0;
                }
            }
            if(rx_buff[1] == 'N')
            {
                if(rx_buff[2] == '=')
                { //Set Offset negative
                    err = 0;
                    if(ParseNumber(&rx_buff[3], &dummy_ui16, 0, 255, 0))
                    {
                        EEP_Settings.values.neg_offset = dummy_ui16;
                        Calc_Imax_ADC();
                        rx_buff[2] = 0x0D; // send Umin 
                    }
                    else
                    {
                        SendString("ERR(0 ... 255)\r\n");
                    }
                }
                if(rx_buff[2] == 0x0D)
                {
                    SendString("ON=");
                    U16ToDez((uint16_t) EEP_Settings.values.neg_offset, &dummy[0], 0);
                    SendAnswer(&dummy[0], 3);
                    Tx_Putc(0x0D);
                    err = 0;
                }
            }
            break;
        case 'G': //Isense gain (pos/neg) commands
            if(rx_buff[1] == 'P')
            {
                if(rx_buff[2] == '=')
                { //Set Offset positive
                    err = 0;
                    if(ParseNumber(&rx_buff[3], &dummy_ui16, 0, 255, 0))
                    {
                        EEP_Settings.values.pos_gain = dummy_ui16;
                        Calc_Imax_ADC();
                        rx_buff[2] = 0x0D; // send Umin 
                    }
                    else
                    {
                        SendString("ERR(0 ... 255)\r\n");
                    }
                }
                if(rx_buff[2] == 0x0D)
                {
                    SendString("GP=");
                    U16ToDez((uint16_t) EEP_Settings.values.pos_gain, &dummy[0], 0);
                    SendAnswer(&dummy[0], 3);
                    Tx_Putc(0x0D);
                    err = 0;
                }
            }
            if(rx_buff[1] == 'N')
            {
                if(rx_buff[2] == '=')
                { //Set Offset negative
                    err = 0;
                    if(ParseNumber(&rx_buff[3], &dummy_ui16, 0, 255, 0))
                    {
                        EEP_Settings.values.neg_gain = dummy_ui16;
                        Calc_Imax_ADC();
                        rx_buff[2] = 0x0D; // send Umin 
                    }
                    else
                    {
                        SendString("ERR(0 ... 255)\r\n");
                    }
                }
                if(rx_buff[2] == 0x0D)
                {
                    SendString("GN=");
                    U16ToDez((uint16_t) EEP_Settings.values.neg_gain, &dummy[0], 0);
                    SendAnswer(&dummy[0], 3);
                    Tx_Putc(0x0D);
                    err = 0;
                }
            }
            break;
        case 'S': //SA = save nonvolatile
            if((rx_buff[1] == 'A')&&(rx_buff[2] == 0x0D))
            {
                Write_Settings();
                SendString("SAVED\r\n");
                err = 0;
            }
            break;
        case 'R': //recall nonvolatile
            if(rx_buff[1] == 0x0D)
            {
                Read_Settings();
                SendString("RECALLED\r\n");
                err = 0;
            }
            break;
       default:
            break;
    }
    if(err == 1)
    {
        SendString("ERR:unknown command\r\n");
    }
    rx_wr_index = 0;
}

//*********************************************************
// Daw LCD functions
//*********************************************************

void Draw_ToffdelaySetting(void)
{
    uint8_t dummy[6];
    
    U16ToDez((uint16_t) EEP_Settings.values.t_imax, &dummy[0], 1);
    LCD_gotobufxy(5, 1);
    LCD_putchbuf('T'); //trip char
    LCD_putchbuf(dummy[2]);
    LCD_putchbuf(dummy[3]);
    LCD_putchbuf(dummy[4]);
    LCD_putchbuf('m');
    LCD_putchbuf('s');
}

void Draw_UminSetting(void)
{
    uint8_t dummy[4];
    
    U16ToDez((uint16_t) EEP_Settings.values.umin, &dummy[0], 0);
    LCD_gotobufxy(5, 1);
    LCD_putchbuf('U'); //trip char
    LCD_putchbuf('L');
    LCD_putchbuf(dummy[0]);
    LCD_putchbuf(dummy[1]);
    LCD_putchbuf('.');
    LCD_putchbuf(dummy[2]);
    LCD_putchbuf('V');
}

void Draw_UmaxSetting(void)
{
    uint8_t dummy[4];
    
    U16ToDez((uint16_t) EEP_Settings.values.umax, &dummy[0], 0);
    LCD_gotobufxy(5, 1);
    LCD_putchbuf('U'); //trip char
    LCD_putchbuf('H');
    LCD_putchbuf(dummy[0]);
    LCD_putchbuf(dummy[1]);
    LCD_putchbuf('.');
    LCD_putchbuf(dummy[2]);
    LCD_putchbuf('V');
}

void Draw_ImaxSetting(void)
{
    uint8_t dummy[4];
    
    U16ToDez((uint16_t) EEP_Settings.values.imax, &dummy[0], 0);
    LCD_gotobufxy(5, 1);
    LCD_putchbuf(CHAR_TRIP); //trip char
    LCD_putchbuf(dummy[1]);
    LCD_putchbuf('.');
    LCD_putchbuf(dummy[2]);
    LCD_putchbuf('A');
    LCD_putchbuf(' ');
    LCD_putchbuf(' ');
   // LCD_putchbuf(' ');
}

void Draw_Uin(void)
{
    uint8_t dummy[6];

    U16ToDez(uin_100mV, &dummy[0], 1);
    LCD_gotobufxy(12, 2);
    if(dummy[2] == '0')
        dummy[2] = ' ';
    LCD_putchbuf(dummy[2]);
    LCD_putchbuf(dummy[3]);
    LCD_putchbuf('.');
    LCD_putchbuf(dummy[4]);
    if(booster_status.bits.BOOSTER_UMAX_DETECT)
        LCD_putchbuf('H');
    else if(booster_status.bits.BOOSTER_UMIN_DETECT)
        LCD_putchbuf('L');
    else
        LCD_putchbuf('V');
}

void Draw_IsenseVal(uint8_t val8)
{
    uint8_t dummy[6];
    
    U16ToDez(val8, &dummy[0], 0);
    LCD_putchbuf(dummy[1]);
    LCD_putchbuf('.');
    LCD_putchbuf(dummy[2]);
    LCD_putchbuf('A');
}

void Draw_Isense(void)
{
    uint8_t senseval8;
    uint16_t corrval16;
    uint8_t c;
    
    if(i_neg_corr>i_pos_corr)
    {
        c=CHAR_NEG;    
        senseval8=(uint8_t)i_neg100mA;
        corrval16=i_neg_corr;
    }
    else
    {
        c=CHAR_POS;
        senseval8=(uint8_t)i_pos100mA;
        corrval16=i_pos_corr;
    }
    if(LATB4)
    { //Booster is on
        LCD_gotobufxy(12, 1);
        LCD_putchbuf(c);
        Draw_IsenseVal(senseval8);
#ifdef USE_BARGRAPH
        if(corrval16<3)
            corrval16 = 0;
        LCD_DrawBargraph(corrval16,c==CHAR_POS);
#else            
        Draw_10BitHex(1, 2, i_pos_corr);
        Draw_10BitHex(5, 2, i_neg_corr);
#endif        
    }
    else
    { //Booster is off, check if last Isense must be saved
        if(booster_status.bits.BOOSTER_IMAX_DETECT == 0)
        {
            LCD_putsbufxy(12, 1, " ---A");
        }
        else
        {
            if((count1s & 0x01) == 0)
            {
                LCD_gotobufxy(13, 1);
                Draw_IsenseVal(senseval8);
            }
            else
            {
                LCD_putsbufxy(13, 1, "    ");
            }
        }
    }
}

void DrawOnOff(void)
{
    if(LATB4)
        LCD_putsbufxy(1, 1, "On ");
    else
        LCD_putsbufxy(1, 1, "Off");
}

void DrawMessage(void)
{
    if(booster_status.bits.BOOSTER_IMAX_DETECT)
    {
        LCD_putsbufxy(1, 2, "!! IMAX !!");
    }
    else if(booster_status.bits.BOOSTER_UMAX_DETECT)
    {
        LCD_putsbufxy(1, 2, "   UMAX   ");
    }
    else if(booster_status.bits.BOOSTER_UMIN_DETECT)
    {
        LCD_putsbufxy(1, 2, "   UMIN   ");
    }
    else if(booster_status.bits.IN_DATA_DETECT == 0)
    {
        LCD_putsbufxy(1, 2, "No Raildat");
    }
}

void Draw_HartBeat(void)
{
    if(count1s & 0x01)
    {
        if(booster_status.bits.IN_DATA_DETECT)
        {
            LCD_putsbufxy(11, 2, "*");
        }
        else
        {
            LCD_putsbufxy(11, 2, ".");
        }
    }
    else
    {
        LCD_putsbufxy(11, 2, " ");
    }
}

void Draw_Settings(void)
{

    if(LATB4)
    {
        Draw_ImaxSetting();
    }
    else
    {
        draw_settings_count++;
        if((draw_settings_count & 0x1F) == 0)
        {
            switch(draw_settings_state++)
            {
                case 0: Draw_ImaxSetting();
                    break;
                case 1: Draw_ToffdelaySetting();
                    break;
                case 2: Draw_UminSetting();
                    break;
                case 3: Draw_UmaxSetting();
                    draw_settings_state = 0;
                    break;
            }
        }
    }
}

void Draw_LCD(void)
{
    DrawOnOff();
    Draw_Settings();
    Draw_Isense();
    DrawMessage();
    Draw_HartBeat();
    Draw_Uin();
}

void DoBlink(void)
{
    if(led_blink & 0x8000u)
    {
        LED = 0; //LED on
        led_blink = led_blink << 1;
        led_blink++;
    }
    else
    {
        LED = 1; //LED off
        led_blink = led_blink << 1;
    }
}

void Clear_ISense(void)
{
    i_pos100mA = 0;
    i_neg100mA = 0;
    i_pos_corr = 0;
    i_neg_corr = 0;
    i_pos_raw = 0;
    i_neg_raw = 0;
    adc_sense_possummmain = 0;
    adc_sense_negsummmain = 0;
    imax_negcount = 0;
    imax_poscount = 0;
    adc_sense_posctr = 0;
    adc_sense_negctr = 0;
}

void SysTimer_DoTask(void)
{
    count1ms++;
    if(count1ms == 10)
    {
        systaskstat.DO_10MS_TASK = 1;
        if(in_pulsecount < 3)
        { // we got 10ms without pulses
            // disable H-Bridge
            if((LATB4) || (booster_status.bits.BOOSTER_IMAX_DETECT))
            {
                Clear_ISense();
                ADC_SetUinSense();
                TMR6IE = 0;
                T6CONbits.TMR6ON = 0;
                booster_status.bits.BOOSTER_IMAX_WARNING = 0;
                led_blink = BLINK_HEARTBEAT_BOOSTER_OFF;
                booster_status.bits.BOOSTER_IMAX_DETECT = 0;
            }
            LATB4 = 0;
            booster_status.bits.IN_DATA_DETECT = 0;
        }
        else if(in_pulsecount > 10)
        {
            booster_status.bits.IN_DATA_DETECT = 1;
            if(LATB4 == 0)
            {
                if((booster_status.bits.BOOSTER_IMAX_DETECT == 0)
                        &&(booster_status.bits.BOOSTER_UMAX_DETECT == 0)
                        &&(booster_status.bits.BOOSTER_UMIN_DETECT == 0))
                {
                    LATB4 = 1;
                    led_blink = BLINK_HEARTBEAT_BOOSTER_ON;
                    Clear_ISense();
                    ADC_SetISense();
                    LCD_putsbufxy(1, 2, "          ");
                }
            }
        }
        in_pulsecount = 0;
        count1ms = 0;
        count10ms++;
        if(count10ms == 10)
        {
            systaskstat.DO_100MS_TASK = 1;
            systaskstat.DO_REDRAW = 1;
            count10ms = 0;
            count100ms++;
            if((count100ms & 0x01) == 1)
            { //every 200ms measure U_in
                if(booster_status.bits.BOOSTER_IMAX_WARNING == 0)
                {
                    ADC_SetUinSense();
                }
            }
            else
            {
                DoBlink();
            }
            if(count100ms == 10)
            {
                count1s++;
                count100ms = 0;
            }
        }
    }
}

void main(void)
{
    //HW init
    NOP();
    CLRWDT(); 
    pps_init();
    system_init();
    uart_init();
    i2c_init();
    ad_init();
    timer2_init();
    timer4_init();
    cog_init();
    led_blink = BLINK_HEARTBEAT_BOOSTER_OFF;
    //read settings from flash
    Read_Settings();
    //init lcd screen
    LCD_SetBLOn();
    //init LCD state maschine
    LCD_Init();
    Draw_LCD();
    //start ADC with poti sense 
    ADC_SetUinSense(); //ADC_SetPotiSense();
    TMR4ON = 1;
    //enable interrupts
    ADIE = 1;
    IOCIE = 1;
    PEIE = 1;
    //calculate imax ADC values
    Calc_Imax_ADC();
    ei();
    while(1)
    {
        CLRWDT();
        if(TMR2IF)
        {
            TMR2IF = 0;
            SysTimer_DoTask();
        }
        if(systaskstat.DO_10MS_TASK)
        {
            systaskstat.DO_10MS_TASK = 0;
            switch(count10ms)
            {
                case 1:
                    Calc_Uin();
                    break;
                case 2:
                    if(LATB4)
                    {
                        Calc_Ipos();
                    }
                    break;
                case 3:
                    if(LATB4)
                    {
                        Calc_Ineg();
                    }
                default:
                    break;
            }
        }
        if(systaskstat.DO_100MS_TASK)
        {
            systaskstat.DO_100MS_TASK = 0;
        }
        if(systaskstat.DO_REDRAW)
        { //redraw LCD display if idle
            if(LCD_GetIdle())
            {
                Draw_LCD();
                systaskstat.DO_REDRAW = 0;
            }
        }
        LCD_DoTask(); //do LCD communication / refresh
        if(tx_count)
        { //transmitt serial data if any
            if(TRMT)
            {
                if(systaskstat.DO_SENDHELP)
                {//send long help string here because is much bigger than buffer
                    if(HelpString[tx_helpindex] == 0)
                    {
                        systaskstat.DO_SENDHELP = 0;
                        tx_count = 0;
                    }
                    else
                    {
                        TXREG1 = HelpString[tx_helpindex++];
                    }
                }
                else
                {
                    TXREG1 = tx_buff[tx_rd_index++];
                    if(tx_rd_index == TX_BUFSIZE)
                        tx_rd_index = 0;
                    tx_count--;
                }
            }
        }
        if(systaskstat.DO_PARSECMD)
        { //parse incomming command
            systaskstat.DO_PARSECMD = 0;
            DoParseCmd();
        }
    }
}
