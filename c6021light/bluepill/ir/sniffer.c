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
extern volatile uint8_t command_repeat;

struct loco_status loco_table_status[32];
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

static void send_mm_emergency_stop(uint16_t address) {
    bool ext, rtr;
    uint8_t dlc, data[8];
    uint32_t id;

    rtr = 0;
    ext = 1;
    id = 0x00000300;
    dlc = 5;
    data[0] = 0;
    data[1] = 0;
    data[2] = (address >> 8) & 0xff;
    data[3] = address & 0xff;
    data[4] = 3;
    can_transmit(CAN1, id, ext, rtr, dlc, data);
}


static void send_mm_function(uint16_t address, uint8_t function, uint32_t value) {
    bool ext, rtr;
    uint8_t dlc, data[8];
    uint32_t id;

    rtr = 0;
    ext = 1;

    id = 0x000C0300;
    dlc = 6;
    data[0] = 0;
    data[1] = 0;
    data[2] = (address >> 8) & 0xff;
    data[3] = address & 0xff;
    data[4] = function;
    if (value) {
	data[5] = 1;
    } else {
	data[5] = 0;
    }
    can_transmit(CAN1, id, ext, rtr, dlc, data);
}

static void send_mm_can(uint16_t address, uint16_t speed, uint8_t direction) {
    bool ext, rtr;
    uint8_t dlc, data[8];
    uint32_t id;

    rtr = 0;
    ext = 1;

    data[0] = 0;
    data[1] = 0;
    data[2] = (address >> 8) & 0xff;
    data[3] = address & 0xff;
    if (direction) {
	dlc = 5;
	id = 0x000A0300;
	data[4] = direction;
    } else {
	dlc = 6;
	id = 0x00080300;
	data[4] = (speed >> 8) & 0xff;
	data[5] = speed & 0xff;
    }
    can_transmit(CAN1, id, ext, rtr, dlc, data);
}

static void check_loco_command_table(void) {
    uint8_t idx, i;
    uint16_t speed;
    uint32_t mask;
    // search loco address in table
    for (idx = 0; idx <= 31; idx++) {
	if (loco_table_status[idx].address == loco_command.address)
	    break;
    }
    // not found
    if (idx == 32) {
	loco_table_status[loco_table_head].address = loco_command.address;
	idx = loco_table_head;
	loco_table_head++;
    }
    // valid entry found if idx != max
    if (idx <= 31) {
	// first check speed
	if (loco_table_status[idx].speed != (loco_command.speed & 0x7FFF)) {
	    if ((loco_table_status[idx].speed & 0x3FFF) != (loco_command.speed & 0x3FFF)) {
		if ((loco_command.speed & 0x3FFF) == 0) {
		    send_mm_can(loco_command.address, 0, 0);
		} else if ((loco_command.speed & 0x3FFF) == 1) {
		    send_mm_emergency_stop(loco_command.address);
		} else {
		    // IR has only 14 speed steps
		    speed = ((loco_command.speed & 0x0F) + 1) * 64 - 1;
		    send_mm_can(loco_command.address, speed, 0);
		}
		loco_table_status[idx].speed = (loco_table_status[idx].speed & 0x4000) | (loco_command.speed & 0x3FFF);
	    }
	    if (loco_command.speed & 0x8000) {
		if ((loco_table_status[idx].speed & 0x4000) != (loco_command.speed & 0x4000)) {
		    if (loco_command.speed & 0x4000)
			send_mm_can(loco_command.address, 0, 2);
		    else
			send_mm_can(loco_command.address, 0, 1);
		}
		loco_table_status[idx].speed = (loco_command.speed & 0x4000) | (loco_table_status[idx].speed & 0x3FFF);
	    }
	}
	if ((loco_table_status[idx].function & loco_command.mask) != (loco_command.function & loco_command.mask)) {
	    mask = 1;
	    for (i = 0; i < 32; i++) {
		if (mask & loco_command.mask) {
		    if ((loco_table_status[idx].function & mask) != (loco_command.function & mask)) {
			send_mm_function(loco_command.address, i, loco_command.function & mask);
			if (loco_command.function & mask)
			    bit_set(loco_table_status[idx].function, i);
			else
			    bit_clear(loco_table_status[idx].function, i);
		    }
		}
		mask <<= 1;
	    }
	}
    }
}

int main(void) {
    status = 0;
    commands_pending = 0;
    command_repeat = 0;
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
	    if (command_repeat == 1)
		check_loco_command_table();
	    mm_print();
	    printlock = 0;
	    loco_command_old = loco_command;
	    OSCI_PIN_OFF;
	}
    }
    return 0;
}
