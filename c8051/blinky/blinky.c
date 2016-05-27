#include <compiler.h>             // Compiler declarations
#include "C8051F500_defs.h"           // SFR declarations

#define SYSCLK       24000000/8        // SYSCLK frequency in Hz

SBIT (LED, SFR_P1, 3);                 // LED='1' means ON

void OSCILLATOR_Init (void);
void PORT_Init (void);
void TIMER2_Init (unsigned int counts);

void Timer2_ISR(void) __interrupt (1) {
   TF2H = 0;                           // Clear Timer2 interrupt flag
   LED = !LED;                         // Change state of LED
}

void OSCILLATOR_Init (void) {
   unsigned char SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x84;                      // Set oscillator divide to /8

   SFRPAGE = SFRPAGE_save;
}

void PORT_Init (void) {
   unsigned char SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P1MDOUT |= 0x08;                    // Enable LED as a push-pull output
   P1SKIP  |= 0x08;                    // Skip the LED pin on the crossbar

   XBR2     = 0x40;                    // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

void TIMER2_Init (unsigned int counts) {
   unsigned char SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE_PAGE;

   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON  &= ~0x60;                    // Timer2 clocked based on T2XCLK;

   TMR2RL  = -counts;                  // Init reload values
   TMR2    = 0xFFFF;                   // Set to reload immediately
   ET2     = 1;                        // Enable Timer2 interrupts
   TR2     = 1;                        // Start Timer2

   SFRPAGE = SFRPAGE_save;
}

void main (void) {
   SFRPAGE = ACTIVE_PAGE;              // Configure for PCA0MD

   PCA0MD &= ~0x40;                    // Disable the watchdog timer

   OSCILLATOR_Init ();                 // Configure system clock

   PORT_Init ();                       // Initialize crossbar

   TIMER2_Init (SYSCLK / 12 / 10);     // Init Timer2 to generate interrupts
                                       // at a 10 Hz rate.

   EA = 1;                             // Enable global interrupts

   while (1);                          // Spin forever
}

