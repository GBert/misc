/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "can.h"
#include "mm-can.h"
#include <stdint.h>
#include <stdbool.h>

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

void can_setup(void) {
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

void send_can_data(uint32_t val) {
    bool ext, rtr;
    uint8_t dlc, data[8] = {0};
    uint32_t id;

    rtr = 0; 
    ext = 1;
    dlc = 8;
    id = 0x00840300;
    data[0] = (val >> 24) & 0xff;
    data[1] = (val >> 16) & 0xff;
    data[2] = (val >>  8) & 0xff;
    data[3] = val & 0xff;

    can_transmit(CAN1, id, ext, rtr, dlc, data);
}

void usb_lp_can_rx0_isr(void) {
    uint32_t id;
    bool ext, rtr;
    uint8_t fmi, dlc, data[8];

    can_receive(CAN1, 0, false, &id, &ext, &rtr, &fmi, &dlc, data, NULL);

    can_fifo_release(CAN1, 0);
}
