/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/*
      B12        MICRO USB         GND
      B13                          GND
      B14                          3V3
      B15   RESET    GND    GND  RESET
      A8    BUTTON  *BOOT1 *BOOT0  B11
      A9             3V3    3V3    B10
      A10                           B1
      A11                           B0
      A12                           A7
      A15                           A6
      B3       STM32F103C8T6        A5
MM    B4                            A4
      B5                            A3
      B6                            A2
      B7              8M            A1
CANRX B8           32768            A0
CANTX B9                           C15
      5V       PC13     POWER      C14
      GND      LED      LED        C13
      3V3           SWD           VBAT
             3V3 DIO  DCLK GND
                 PA13 PA14

 */

#include "mm-can.h"
#include "mm-decode.h"
#include "can.h"

volatile uint32_t counter;
volatile uint8_t commands_pending;
volatile uint32_t milliseconds = 0;
volatile uint8_t status;

uint8_t d_data[8];

static void gpio_setup(void) {

    rcc_periph_clock_enable(RCC_GPIOA);
    /*  GPIOB & GPIOC clock */
    /* B8 & B9 CAN */
    rcc_periph_clock_enable(RCC_GPIOB);
    /* C12 LED */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Preconfigure LED */
    gpio_set(GPIOC, GPIO13);	/* LED green off */
    gpio_set(GPIOA, GPIO0);	/* LED green off */

    /* Configure LED&Osci GPIO */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);

}

static void systick_setup(void) {
    /* 72MHz / 8 => 9000000 counts per second */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    /* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
    /* SysTick interrupt every N clock pulses: set reload to N-1 */
    systick_set_reload(8999);

    systick_interrupt_enable();

    /* Start counting */
    systick_counter_enable();
}

uint32_t micros(void) {
    return (((9000 - systick_get_value()) / 8 ) + milliseconds * 1000);
}

void sys_tick_handler(void) {

    /* We call this handler every 1ms so every 1ms = 0.001s
     * resulting in 1Hz message rate.
     */

    /* Transmit CAN frame. */
    counter++;
    milliseconds++;
    if (counter == 500) {
	counter = 0;
	gpio_toggle(GPIOC, GPIO13);	/* toggle green LED */
	gpio_toggle(GPIOA, GPIO0);	/* toggle LED */
    }
}

int main(void) {
    status = 0;
    commands_pending = 0;

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    gpio_setup();
    can_setup();
    exti_setup();

    systick_setup();

    /* endless loop */
    while (1) {

    }
    return 0;
}
