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
TxD   A9             3V3    3V3    B10
      A10                           B1
      A11                           B0
      A12                           A7
      A15                           A6
      B3       STM32F103C8T6        A5
Sniff B4                            A4
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

#include "analyzer.h"
#include "can.h"
#include "sniffer.h"
#include "timer.h"
#include "usart.h"

volatile uint32_t counter;
volatile uint8_t commands_pending;
volatile uint32_t milliseconds = 0;
volatile uint8_t status;
extern volatile uint32_t pulse_duration, old_timestamp, new_timestamp, printlock;
uint8_t d_data[8];
extern volatile uint8_t loco_table_head;
extern volatile uint8_t loco_table_tail;

extern struct loco_status loco_table_status[32];
extern volatile struct loco_status loco_command;
volatile struct loco_status loco_command_old;

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

void sys_tick_handler(void) {
    /* We call this handler every 1ms so every 1ms = 0.001s */
    counter++;
    milliseconds++;
    if (counter == 500) {
	counter = 0;
	gpio_toggle(GPIOC, GPIO13);	/* toggle onboard LED */
    }
}

static void send_mm_can(void) {
}

static void check_loco_command_table(void) {
    printf("*NEW*\n");
}

int main(void) {
    status = 0;
    commands_pending = 0;
    memset(loco_table_status, 0, sizeof(loco_table_status));
    loco_table_head = 0;
    loco_table_tail = 0;

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    gpio_setup();
    usart_setup();
    can_setup();
    timer_setup();

    systick_setup();


    /* endless loop */
    while (1) {
	if (printlock == 2) {
	    OSCI_PIN_ON;
	    printlock = 1;
	    mm_print();
	    if ((loco_command.address == loco_command_old.address) && 
		(loco_command.speed == loco_command_old.speed) &&
		(loco_command.function == loco_command_old.function))
		check_loco_command_table();
	    printlock = 0;
            loco_command_old = loco_command;
	    OSCI_PIN_OFF;
	}
    }
    return 0;
}
