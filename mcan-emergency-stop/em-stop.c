/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/*
 * This file is derived from the libopencm3 project examples 
 */

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/rcc.h>

volatile unsigned int counter;
volatile uint8_t status;

static void gpio_setup(void) {
    /* Enable Alternate Function clock */
    rcc_periph_clock_enable(RCC_AFIO);

    /* Enable GPIOC clock */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Preconfigure LED */
    gpio_set(GPIOC, GPIO13);	/* LED green off */

    /* Configure LED GPIO */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
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

static void can_setup(void) {
    /* Enable peripheral clocks */
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_CAN1);

    AFIO_MAPR |= AFIO_MAPR_CAN1_REMAP_PORTB;

    /* Configure CAN pin: RX (input pull-up) */
    gpio_set_mode(GPIO_BANK_CAN1_PB_RX, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_CAN1_PB_RX);
    gpio_set(GPIO_BANK_CAN1_PB_RX, GPIO_CAN1_PB_RX);

    /* Configure CAN pin: TX */
    gpio_set_mode(GPIO_BANK_CAN1_PB_TX, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_CAN1_PB_TX);

    /* NVIC setup */
    nvic_enable_irq(NVIC_USB_LP_CAN_RX0_IRQ);
    nvic_set_priority(NVIC_USB_LP_CAN_RX0_IRQ, 1);

    /* Reset CAN */
    can_reset(CAN1);

    /* CAN cell init.
     * Setting the bitrate to 250kHz. APB1 = 36MHz, 
     * prescaler = 9 -> 4MHz time quanta frequency.
     * 1tq sync + 9tq bit segment1 (TS1) + 6tq bit segment2 (TS2) = 
     * 16time quanto per bit period, therefor 4MHz/16 = 250kHz
     */
    if (can_init(CAN1,
		 false,		/* TTCM: Time triggered comm mode? */
		 true,		/* ABOM: Automatic bus-off management? */
		 false,		/* AWUM: Automatic wakeup mode? */
		 false,		/* NART: No automatic retransmission? */
		 false,		/* RFLM: Receive FIFO locked mode? */
		 false,		/* TXFP: Transmit FIFO priority? */
		 CAN_BTR_SJW_1TQ,
		 CAN_BTR_TS1_9TQ,
		 CAN_BTR_TS2_6TQ,
		 9,
		 false,
		 false))	{
	gpio_clear(GPIOC, GPIO13);	/* LED green on */

	/* Die because we failed to initialize. */
	while (1)
	    __asm__("nop");
    }

    /* CAN filter 0 init. */
    can_filter_id_mask_32bit_init(CAN1,
				 0,	/* Filter ID */
				 0,	/* CAN ID */
				 0,	/* CAN ID mask */
				 0,	/* FIFO assignment (here: FIFO0) */
				 true);	/* Enable the filter. */

    /* Enable CAN RX interrupt. */
    can_enable_irq(CAN1, CAN_IER_FMPIE0);
}

void sys_tick_handler(void) {
    static uint8_t data[5] = { 0, 0, 0, 0, 0 };

    /* We call this handler every 1ms so every 1ms = 0.001s
     * resulting in 1Hz message rate.
     */

    /* Transmit CAN frame. */
    counter++;
    if (counter == 1000) {
	counter = 0;
	status ^= 0x01;
	if (status) {
	    data[4] = 1;
	} else {
	    data[4] = 0;
	}
           
	if (can_transmit(CAN1, 0x0000fffe,	/* (EX/ST)ID: CAN ID */
			 true,		/* IDE: CAN ID extended? */
			 false,		/* RTR: Request transmit? */
			 5,		/* DLC: Data length */
			 data) == -1) {
	    gpio_set(GPIOC, GPIO13);	/* LED green off */
	}
    }
}

void usb_lp_can_rx0_isr(void) {
    uint32_t id, fmi;
    bool ext, rtr;
    uint8_t dlc, data[8];

    can_receive(CAN1, 0, false, &id, &ext, &rtr, &fmi, &dlc, data);

    /* check for extended id, dlc = 5 and id 0x0000xxxx */
    if ((ext) && (dlc == 5) && !(id & 0xffff0000)) {
       /* M*rklin Start/Stop CMD
	  00004711 [5] 00 00 00 00 00 -> Stop
	  00004711 [5] 00 00 00 00 01 -> Start
	*/
	if (data[4] && 0x01)
	    gpio_clear(GPIOC, GPIO13);
	else
	    gpio_set(GPIOC, GPIO13);
    }

    can_fifo_release(CAN1, 0);
}

int main(void) {
    status = 0;

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    gpio_setup();
    can_setup();

    systick_setup();

    /* endless loop */
    while (1) ;	

    return 0;
}
