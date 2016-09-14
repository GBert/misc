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

extern struct ring output_ring;
extern struct ring input_ring;
volatile unsigned int counter;
volatile uint8_t status;
volatile uint8_t commands_pending;
uint8_t d_data[8];

static void gpio_setup(void) {
    /* Enable GPIOA & GPIOB & GPIOC clock */
    /* A2 & A3 USART */
    rcc_periph_clock_enable(RCC_GPIOA);
    /* B8 & B9 CAN */
    rcc_periph_clock_enable(RCC_GPIOB);
    /* C12 LED */
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

    /* defaultt CAN setting 250 kBaud */ 
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

static int can_send(uint8_t *data) {

    if (can_transmit(CAN1, 0x0deb0deb,	/* (EX/ST)ID: CAN ID */
			 true,		/* IDE: CAN ID extended? */
			 false,		/* RTR: Request transmit? */
			 8,		/* DLC: Data length */
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
	/* printf("Hello World !\r\n"); */
	gpio_toggle(GPIOC, GPIO13);	/* toggle green LED */
#if 0
	if (can_send(d_data))
	    gpio_set(GPIOC, GPIO13);	/* LED green off */
#endif
    }
}

static void put_hex(uint8_t c) {
    uint8_t s[2];

    bin2hex(s, c);
    ring_write(&output_ring, s, 2);
}
    
void usb_lp_can_rx0_isr(void) {
    uint32_t id, fmi;
    bool ext, rtr;
    uint8_t i, dlc, data[8];
    char c;

    can_receive(CAN1, 0, false, &id, &ext, &rtr, &fmi, &dlc, data);

    if (rtr) {
	if (ext)
	    c = 'R';
	else
	    c = 'r';
    } else {
	if (ext)
	    c = 'T';
        else
	    c = 't';
    }
    ring_write_ch(&output_ring, c);
    if (ext) {
	c = (id >> 24) & 0xff;
	put_hex(c);
	c = (id >> 16) & 0xff;
	put_hex(c);
	c = (id >> 8) & 0xff;
	put_hex(c);
	c = id & 0xff;
	put_hex(c);
    } else {
	/* bits 11-9 */
	c = (id >> 8) & 0x07;
	c += 0x30;
	ring_write_ch(&output_ring, c);
	/* bits 8-1 */
	c = id & 0xff;
	put_hex(c);
    }
    c = (dlc & 0x0f) | 0x30;
    ring_write_ch(&output_ring, c);
    for (i = 0 ; i < dlc; i++)
	put_hex(data[i]);

    ring_write_ch(&output_ring, '\r');

    can_fifo_release(CAN1, 0);

    /* enable the transmitter now */
    USART_CR1(USART2) |= USART_CR1_TXEIE;
}

static uint32_t get_id(int nibbles) {
    int i;
    uint32_t id;
    char c;

    id = 0;
    for (i = 0; i < nibbles; i++) {
	c = ring_read_ch(&input_ring, NULL);
	id <<= 4;
	id |= nibble2bin(c);
    }
    return id;
}


static int slcan_command(void) {
    bool ext, rtr;
    uint8_t i, dlc, data[8];
    uint32_t id;
    int32_t ret;
    char c;
    bool send;

    id = 0;
    dlc = 0;
    ext = true;
    send = true;
    rtr = false;

    c = ring_read_ch(&input_ring, NULL);
    switch(c) {
    case 'T':
	id = get_id(8);
	dlc = get_id(1);
	break;
    case 't':
	ext = false;
	id = get_id(3);
	dlc = get_id(1);
	break;
    case 'R':
	rtr = true;
	ext = true;
	id = get_id(8);
	dlc = get_id(1);
	break;
    case 'r':
	rtr = true;
	ext = false;
	id = get_id(3);
	dlc = get_id(1);
	break;
    case 'S':
	c = ring_read_ch(&input_ring, NULL);
        can_speed(c);
	send = false;
	break;
    case 'v':
	send = false;
	break;
    case 'V':
	send = false;
	break;
    case 'C':
	send = false;
	break;
    default:
	send = false;
	break;
    }

    for ( i = 0; i < dlc; i++) {
	data[i] = (uint8_t) get_id(2); 
    }

    if (send)
	can_transmit(CAN1, id, ext, rtr, dlc, data);

#if 0 
	d_data[0] = input_ring.data[input_ring.begin-3];
	d_data[1] = input_ring.data[input_ring.begin-2];
	d_data[2] = input_ring.data[input_ring.begin-1];
	d_data[3] = input_ring.data[input_ring.begin];
	d_data[4] = 0;
	d_data[5] = input_ring.begin;
	d_data[6] = input_ring.end;
	d_data[7] = commands_pending;
#endif

    /* consume chars until eol reached */
    do {
	ret = ring_read_ch(&input_ring, NULL);
    } while (ret == '\r');

    if (commands_pending)
	commands_pending--;

    return 0;
}

int main(void) {
    status = 0;
    commands_pending = 0;

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    gpio_setup();
    can_setup();
    usart_setup();

    systick_setup();

    /* endless loop */
    while (1) {
	if (commands_pending)
	    slcan_command();
    }
    return 0;
}
