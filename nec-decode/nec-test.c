/* nec-test.c - handle NEC IR Pulse/Space protocol
 * based on ir-nec-decoder.c from Mauro Carvalho Chehab
 *
 * This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include <stdio.h>
#include <stdint.h>

enum nec_state { STATE_INACTIVE,
    STATE_HEADER_SPACE,
    STATE_BIT_PULSE,
    STATE_BIT_SPACE,
    STATE_TRAILER_PULSE,
    STATE_TRAILER_SPACE
};

#define TIMER_GRID		64000	/* 64us */
#define MARGIN			30	/* in percent */

#define	T_LOW(x)	(( x * NEC_UNIT / TIMER_GRID) * (100 - MARGIN)/100)
#define	T_HIGH(x)	(( x * NEC_UNIT / TIMER_GRID) * (100 + MARGIN)/100)

#define NEC_NBITS               32
#define NEC_UNIT                562500	/* ns */
#define NEC_HEADER_PULSE        (16 * NEC_UNIT)
#define NECX_HEADER_PULSE       (8  * NEC_UNIT)	/* Less common NEC variant */
#define NEC_HEADER_SPACE        (8  * NEC_UNIT)
#define NEC_REPEAT_SPACE        (4  * NEC_UNIT)
#define NEC_BIT_PULSE           (1  * NEC_UNIT)
#define NEC_BIT_0_SPACE         (1  * NEC_UNIT)
#define NEC_BIT_1_SPACE         (3  * NEC_UNIT)
#define NEC_TRAILER_PULSE       (1  * NEC_UNIT)
#define NEC_TRAILER_SPACE       (10 * NEC_UNIT)	/* even longer in reality */
#define NECX_REPEAT_BITS        1

uint8_t test_data []   = { 140, 70, 8,  8,  8,  8,  8, 26,  8, 26,   8,  8,  8, 26,  8,  8,  8, 26,
				    8,  8,  8, 26,  8,  8,  8,  8,   8, 26,  8,  8,  8, 26,  8,  8,
				    8, 26,  8, 26,  8, 26,  8, 26,   8,  8,  8,  8,  8,  8,  8,  8,
				    8,  8,  8,  8,  8,  8,  8,  8,   8, 26,  8, 26,  8, 26,  8, 26,  8, 255 };

uint8_t test_data_r [] = { 140, 45, 11, 255};

uint8_t test_data_f [] = { 140, 70, 8,  8,  8,  8,  8, 26,  8, 26,   8,  8,  8, 26,  8,  8,  8, 26,
				    8,  8,  8, 26,  8,  8,  8,  8,   8, 26,  8,  8,  8, 26,  8,  8,
				    8, 26, 99, 26,  8, 26,  8, 26,   8,  8,  8,  8,  8,  8,  8,  8,
				    8,  8,  8,  8,  8,  8,  8,  8,   8, 26,  8, 26,  8, 26,  8, 26, 8, 255 };

/* timer 50us */

/*                                              /  64us grid
   start1     pulse 9000us        / 8100 - 9900 / 126 - 155
   start2     pulse 4500us        / 4050 - 4950 /  63 -  77
   data_start pulse  560us        /  500 - 600  /   7 -  10
   data_h     pulse (2250-560) us / 1500 - 1850 /  23 -  29
   data_l     pulse  560us        /  500 - 600  /   7 -  10

   repeat1    pulse 9000us        / 8100 - 9900 / 126 - 155
   repeat2    pulse 2250us        / 2000 - 2500 /  31 -  40
   repeat3    pulse  560us        /  500 - 600  /   7 -  10

   pause after NEC code 255*64us = 16.32 ms

   format:    address , address ^ 0xff, command , command ^ 0xff
   LSB first
*/

uint8_t address;
uint8_t not_address;
uint8_t command;
uint8_t not_command;

uint8_t ir_nec_decode_state;
uint8_t ir_nec_decode_bits;
uint8_t ir_nec_data_valid;
uint32_t nec_code;

void ir_nec_decode(uint8_t stopwatch) {

    switch (ir_nec_decode_state) {

    case STATE_INACTIVE:
	if ((stopwatch > 125) && (stopwatch < 156))
	    ir_nec_decode_state = STATE_HEADER_SPACE;
	break;

    case STATE_HEADER_SPACE:
	if ((stopwatch > 62) && (stopwatch < 78)) {
	    /* we got the start sequence -> old data is now invalid */
	    ir_nec_data_valid = 0;
	    ir_nec_decode_bits = 0;
	    ir_nec_decode_state = STATE_BIT_PULSE;
	    break;
	}
	/* is this a repeat sequence ? */
	if ((stopwatch > 30) && (stopwatch < 41)) {
	    /* if ir_nec_decode_bits == 32 the repeat sequence could be valid */
	    ir_nec_decode_state = STATE_TRAILER_PULSE;
	    break;
	}

    case STATE_BIT_PULSE:
	if ((stopwatch > 6) && (stopwatch < 11)) {
	    ir_nec_decode_state = STATE_BIT_SPACE;
	    break;
	}

    case STATE_BIT_SPACE:
	nec_code >>= 1;
	if ((stopwatch > 22) && (stopwatch < 30))
	    nec_code |= 0x80000000;
	/* valid data anyway ? */
	else if ((stopwatch < 7) || (stopwatch > 10)) {
	    /* TODO */
	    ir_nec_decode_state = STATE_INACTIVE;
	    break;
	}

	ir_nec_decode_bits++;

	if (ir_nec_decode_bits == NEC_NBITS)
	    ir_nec_decode_state = STATE_TRAILER_PULSE;
	else
	    ir_nec_decode_state = STATE_BIT_PULSE;
	break;

    case STATE_TRAILER_PULSE:
	if ((stopwatch > 6) && (stopwatch < 11)) {
	    ir_nec_decode_state = STATE_TRAILER_SPACE;
	    break;
	}

    case STATE_TRAILER_SPACE:
	/* 255 means timer overflow - pause after NEC code sent */
	if (stopwatch == 255) {
	    ir_nec_decode_state = STATE_INACTIVE;
	    /* we got valid data if the sequence before was valid - for repeat needed */
	    if (ir_nec_decode_bits == NEC_NBITS)
		ir_nec_data_valid = 1;
	    break;
	}
    default:
	ir_nec_decode_state = STATE_INACTIVE;
    }
}

void feed_state_machine(uint8_t * data, int length) {
    int i;
    uint8_t old_state;
    for (i = 0; i < length; i++) {
	old_state = ir_nec_decode_state;
	ir_nec_decode(data[i]);
	printf("stopwatch %03d state %d -> %d  bit %02d\n", data[i], old_state, ir_nec_decode_state, ir_nec_decode_bits);
    }
    if (ir_nec_data_valid)
	printf("state machine : valid data 0x%08X\n\n", nec_code);
    else
	printf("state machine : no valid data\n\n");
}

int main(int argc, char **argv) {
    ir_nec_decode_state = STATE_INACTIVE;

    feed_state_machine(test_data,   sizeof(test_data));
    feed_state_machine(test_data_r, sizeof(test_data_r));
    feed_state_machine(test_data_f, sizeof(test_data_f));
    feed_state_machine(test_data_r, sizeof(test_data_r));
    feed_state_machine(test_data,   sizeof(test_data));
    feed_state_machine(test_data_r, sizeof(test_data_r));

    return 0;
}
