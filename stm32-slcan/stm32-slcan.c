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

#include "stm32-slcan.h"

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

    /* Enable clocks for GPIO port A (for GPIO_USART2_TX) and USART2. */
   rcc_periph_clock_enable(RCC_AFIO);
   rcc_periph_clock_enable(RCC_USART2);
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

static int can_speed(int index) {
    int ret;

    /*
	S0 = 10 kBaud
	S1 = 20 kBaud
	S2 = 50 kBaud
	S3 = 100 kBaud
	S4 = 125 kBaud
	S5 = 250 kBaud
	S6 = 500 kBaud
	S7 = 800 kBaud
	S8 = 1 MBaud

	  10 kBaud Sample Point 75.0 % : 36MHZ / 180 = 200kHz -> TQ 20   SJW + TS1 15 TS2 5
	  20 kBaud Sample Point 75.0 % : 36MHZ /  90 = 400kHz -> TQ 20   SJW + TS1 15 TS2 5
	  50 kBaud Sample Point 75.0 % : 36MHZ /  36 = 1MHz   -> TQ 20   SJW + TS1 15 TS2 5
	 100 kBaud Sample Point 80.0 % : 36MHZ /  36 = 1MHz   -> TQ 10   SJW + TS1  8 TS2 2
	 125 kBaud Sample Point 87.5 % : 36MHZ /  18 = 2MHz   -> TQ 16   SJW + TS1 14 TS2 2 
	 250 kBaud Sample Point 87.5 % : 36MHZ /   9 = 4MHz   -> TQ 16   SJW + TS1 14 TS2 2
	 500 kBaud Sample Point 87.5 % : 36MHZ /   9 = 4MHz   -> TQ  8   SJW + TS1  7 TS2 1
	 800 kBaud Sample Point 86.7 % : 36MHZ /   3 = 12MHz  -> TQ 15   SJW + TS1 13 TS2 2
	1000 kBaud Sample Point 88.9 % : 36MHZ /   2 = 18MHz  -> TQ 18   SJW + TS1 16 TS2 2

    	TTCM: Time triggered comm mode
	ABOM: Automatic bus-off management
	AWUM: Automatic wakeup mode
	NART: No automatic retransmission
	RFLM: Receive FIFO locked mode
	TXFP: Transmit FIFO priority
     */
    switch(index) {
    case 0: ret = can_init(CAN1, false, true, false, false, false, false,
			CAN_BTR_SJW_1TQ, CAN_BTR_TS1_14TQ, CAN_BTR_TS2_5TQ, 180, false, false);
	break;
    case 1: ret = can_init(CAN1, false, true, false, false, false, false,
 			CAN_BTR_SJW_1TQ, CAN_BTR_TS1_14TQ, CAN_BTR_TS2_5TQ,  90, false, false);
	break;
    case 2: ret = can_init(CAN1, false, true, false, false, false, false,
			CAN_BTR_SJW_1TQ, CAN_BTR_TS1_14TQ, CAN_BTR_TS2_5TQ,  36, false, false);
	break;
    case 3: ret = can_init(CAN1, false, true, false, false, false, false,
			CAN_BTR_SJW_1TQ, CAN_BTR_TS1_8TQ, CAN_BTR_TS2_2TQ,   36, false, false);
	break;
    case 4: ret = can_init(CAN1, false, true, false, false, false, false,
			CAN_BTR_SJW_1TQ, CAN_BTR_TS1_13TQ, CAN_BTR_TS2_2TQ,  18, false, false);
	break;
    case 5: ret = can_init(CAN1, false, true, false, false, false, false,
			CAN_BTR_SJW_1TQ, CAN_BTR_TS1_13TQ, CAN_BTR_TS2_2TQ,   9, false, false);
	break;
    case 6: ret = can_init(CAN1, false, true, false, false, false, false,
			CAN_BTR_SJW_1TQ, CAN_BTR_TS1_7TQ, CAN_BTR_TS2_1TQ,    9, false, false);
	break;
    case 7: ret = can_init(CAN1, false, true, false, false, false, false,
			CAN_BTR_SJW_1TQ, CAN_BTR_TS1_12TQ, CAN_BTR_TS2_2TQ,   3, false, false);
	break;
    case 8: ret = can_init(CAN1, false, true, false, false, false, false,
			CAN_BTR_SJW_1TQ, CAN_BTR_TS1_15TQ, CAN_BTR_TS2_2TQ,    2, false, false);
	break;
    default:
	ret = -1;
	break;
    }
    return ret;
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
#if 0
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
#endif
    if (can_speed(5)) {
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

static int can_send(void) {
    static uint8_t data[8] = { 0, 0, 0, 0, 0 };

    if (can_transmit(CAN1, 0x0000fffe,	/* (EX/ST)ID: CAN ID */
			 true,		/* IDE: CAN ID extended? */
			 false,		/* RTR: Request transmit? */
			 5,		/* DLC: Data length */
     data) == -1) {
	 return -1;
    }
    return 0;

}
void sys_tick_handler(void) {

    /* We call this handler every 1ms so every 1ms = 0.001s
     * resulting in 1Hz message rate.
     */

    /* Transmit CAN frame. */
    counter++;
    if (counter == 1000) {
	counter = 0;
	status ^= 0x01;
	gpio_set(GPIOC, GPIO13);	/* LED green off */
    }
}

void usb_lp_can_rx0_isr(void) {
    uint32_t id, fmi;
    bool ext, rtr;
    uint8_t dlc, data[8];

    can_receive(CAN1, 0, false, &id, &ext, &rtr, &fmi, &dlc, data);

    /* doing something useful */

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
