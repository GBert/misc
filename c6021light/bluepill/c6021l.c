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

/*
      B12        MICRO USB         GND
      B13                          GND
      B14                          3V3
      B15   RESET    GND    GND  RESET
      A8    BUTTON  *BOOT1 *BOOT0  B11 I2C2 SDA
      A9             3V3    3V3    B10 I2C2 SCL
      A10                           B1
      A11                           B0
      A12                           A7
      A15                           A6
      B3       STM32F103C8T6        A5
      B4                            A4
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

#include "c6021l.h"

extern struct ring output_ring;
extern struct ring input_ring;
volatile unsigned int counter;
volatile uint32_t i2c_val;
volatile uint32_t i2c_address;
volatile uint8_t status;
volatile uint8_t commands_pending;
uint8_t d_data[8];
volatile uint8_t i2c_data[8];
uint32_t reading, writing;
volatile uint8_t *write_p;

static void gpio_setup(void) {
    /*  GPIOB & GPIOC clock */
    /* B8 & B9 CAN  - B10 & B11 I2C */
    rcc_periph_clock_enable(RCC_GPIOB);
    /* C12 LED */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Preconfigure LED */
    gpio_set(GPIOC, GPIO13);	/* LED green off */

    /* Preconfigure Osci pin CAN */
    gpio_clear(GPIOC, GPIO14);

    /* Preconfigure Osci pin */
    gpio_clear(GPIOC, GPIO15);

    /* Configure LED&Osci GPIO */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO14);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO15);
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

static void gpio_debug(int n) {

    switch(n) {
    case 0:
	gpio_clear(GPIOC, GPIO14);
	gpio_clear(GPIOC, GPIO15);
	break;
    case 1:
	gpio_set(GPIOC, GPIO14);
	gpio_clear(GPIOC, GPIO15);
	break;
    case 2:
	gpio_clear(GPIOC, GPIO14);
	gpio_set(GPIOC, GPIO15);
	break;
    case -1:
	gpio_set(GPIOC, GPIO14);
	gpio_set(GPIOC, GPIO15);
	break;
    }
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
			CAN_BTR_SJW_1TQ, CAN_BTR_TS1_15TQ, CAN_BTR_TS2_2TQ,   2, false, false);
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
    can_filter_id_mask_32bit_init(
				 0,	/* Filter ID */
				 0,	/* CAN ID */
				 0,	/* CAN ID mask */
				 0,	/* FIFO assignment (here: FIFO0) */
				 true);	/* Enable the filter. */

    /* Enable CAN RX interrupt. */
    can_enable_irq(CAN1, CAN_IER_FMPIE0);
}

static void send_can_data(uint32_t val) {
    bool ext, rtr;
    uint8_t dlc, data[8] = {0};
    uint32_t id;
    int ret;

    rtr = 0; 
    ext = 1;
    dlc = 6;
    id = 0x00160300;
    data[0] = (val >> 24) & 0xff;
    data[1] = (val >> 16) & 0xff;
    data[2] = (val >>  8) & 0xff;
    data[3] = val & 0xff;

    ret = can_transmit(CAN1, id, ext, rtr, dlc, data);
    gpio_debug(ret);
}

static void i2c_slave_init(uint8_t ownaddress) {
    rcc_periph_clock_enable(RCC_I2C2);
    rcc_periph_clock_enable(RCC_AFIO);

    nvic_enable_irq(NVIC_I2C2_EV_IRQ);

    /* configure I2C2 pins */
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C2_SDA);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C2_SCL);

    i2c_reset(I2C2);
    i2c_peripheral_disable(I2C2);

    i2c_set_speed(I2C2, i2c_speed_sm_100k, I2C_CR2_FREQ_36MHZ);
    i2c_set_own_7bit_slave_address(I2C2, ownaddress);
    i2c_enable_interrupt(I2C2, I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN);
    i2c_peripheral_enable(I2C2);

    // slave needs to acknowledge on receiving bytes
    // set it after enabling Peripheral i.e. PE = 1
    i2c_enable_ack(I2C2);
}

void sys_tick_handler(void) {

    /* We call this handler every 1ms so every 1ms = 0.001s
     * resulting in 1Hz message rate.
     */

    /* Transmit CAN frame. */
    counter++;
    if (counter == 500) {
	counter = 0;
	gpio_toggle(GPIOC, GPIO13);	/* toggle green LED */
        //send_can_data();
    }
}


void usb_lp_can_rx0_isr(void) {
    uint32_t id;
    bool ext, rtr;
    uint8_t fmi, dlc, data[8];

    can_receive(CAN1, 0, false, &id, &ext, &rtr, &fmi, &dlc, data, NULL);

    can_fifo_release(CAN1, 0);
}

void i2c2_ev_isr(void) {
    uint32_t sr1, sr2;


    sr1 = I2C_SR1(I2C2);
    i2c_val = sr1;

    // Address matched (Slave)
    if (sr1 & I2C_SR1_ADDR) {
	reading = 0;
	writing = 3;
	write_p = i2c_data;
        //Clear the ADDR sequence by reading SR2.
        sr2 = I2C_SR2(I2C2);
        (void) sr2;
    }
    // Receive buffer not empty
    else if (sr1 & I2C_SR1_RxNE) {
	if (reading == 0) {
	    i2c_data[0] = i2c_get_data(I2C2) >> 1; 
	    i2c_data[1] = 0xFE;
	}
	if (reading == 1) {
	    i2c_data[2] = i2c_get_data(I2C2);
	}
        //ignore more than 2 bytes reaing
	if (reading >= 2)
	    return;
	reading++;
    }
    // Transmit buffer empty & Data byte transfer not finished
    else if ((sr1 & I2C_SR1_TxE) && !(sr1 & I2C_SR1_BTF)) {
	 i2c_send_data(I2C2, *write_p++);
        //send data to master in MSB order
    }
    // done by master by sending STOP
    //this event happens when slave is in Recv mode at the end of communication
    else if (sr1 & I2C_SR1_STOPF) {
	send_can_data((i2c_data[0] << 16) | (i2c_data[1] << 8) | (i2c_data[2]));
	i2c_peripheral_enable(I2C2);
	i2c_send_data(I2C2, *write_p++);
    }
    //this event happens when slave is in transmit mode at the end of communication
    else if (sr1 & I2C_SR1_AF) {
        //(void) I2C_SR1(I2C1);
        I2C_SR1(I2C2) &= ~(I2C_SR1_AF);
    }
}

int main(void) {
    uint32_t i2c_val_old;
    status = 0;
    commands_pending = 0;
    i2c_val_old = 0;

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    gpio_setup();
    can_setup();
    i2c_slave_init(0x7f);

    systick_setup();


    /* endless loop */
    while (1) {
	if (i2c_val_old != i2c_val) {
	    send_can_data(i2c_val);
	    i2c_val_old = i2c_val;
	}
    }
    return 0;
}
