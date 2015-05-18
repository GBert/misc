/* ir-nec-decoder.c - handle NEC IR Pulse/Space protocol
 *
 * Copyright (C) 2010 by Mauro Carvalho Chehab
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


enum nec_state { STATE_INACTIVE,
	STATE_HEADER_SPACE,
	STATE_BIT_PULSE,
	STATE_BIT_SPACE,
	STATE_TRAILER_PULSE,
	STATE_TRAILER_SPACE,
};

#define NEC_NBITS               32
#define NEC_UNIT                562500  /* ns */
#define NEC_HEADER_PULSE        (16 * NEC_UNIT)
#define NECX_HEADER_PULSE       (8  * NEC_UNIT) /* Less common NEC variant */
#define NEC_HEADER_SPACE        (8  * NEC_UNIT)
#define NEC_REPEAT_SPACE        (4  * NEC_UNIT)
#define NEC_BIT_PULSE           (1  * NEC_UNIT)
#define NEC_BIT_0_SPACE         (1  * NEC_UNIT)
#define NEC_BIT_1_SPACE         (3  * NEC_UNIT)
#define NEC_TRAILER_PULSE       (1  * NEC_UNIT)
#define NEC_TRAILER_SPACE       (10 * NEC_UNIT) /* even longer in reality */
#define NECX_REPEAT_BITS        1

/* timer 50us */

/*                                              /  50us grid
   start1     pulse 9000us        / 8100 - 9900 / 162 - 198
   start2     pulse 4500us        / 4050 - 4950 /  81 -  99
   data_start pulse  560us        /  500 - 600  /  10 -  12
   data_h     pulse (2250-560) us / 1500 - 1850 /  30 -  37
   data_l     pulse  560us        /  500 - 600  /  10 -  12

   repeat1    pulse 9000us        / 8100 - 9900 / 162 - 198
   repeat2    pulse 2250us        / 2000 - 2500 /  40 -  50
   repeat3    pulse  560us        /  500 - 600  /  10 -  12

   format:    address , address ^ 0xff, command , command ^ 0xff
   LSB first
*/

uint8_t address;
uint8_t not_address;
uint8_t command;
uint8_t not_command;

uint8_t ir_nec_decode_state;
uint8_t ir_nec_decode_bits;
uint8_t ir_nec_decode_valid;
uint32_t nec_code;

void ir_nec_decode(uint8_t stoppwatch) {

    switch (ir_nec_decode_state) {

    case STATE_INACTIVE:
	ir_nec_decode_state = STATE_HEADER_SPACE;
        return;
    case STATE_HEADER_SPACE:
	if ((stoppwatch > 161 ) && (stopwatch < 199))
	    ir_nec_decode_state = STATE_HEADER_PULSE;
        return;
    case STATE_HEADER_PULSE:
	if ((stoppwatch > 80 ) && (stopwatch < 100))
	    ir_nec_decode_state = STATE_BIT_PULSE;
        return;
    case STATE_BIT_PULSE:
	if ((stoppwatch > 9 ) && (stopwatch < 13))
	    ir_nec_decode_state = STATE_BIT_SPACE;
        return;
    
    case STATE_BIT_SPACE:
	if ((stoppwatch > 9 ) && (stopwatch < 13))
	    nec_code >>=1;
	if ((stoppwatch  29 ) && (stopwatch < 38))
	    nec_code &= 0x80000000;
	    nec_code >>=1;
	if (ir_nec_decode_bits == NEC_NBITS)
	    ir_nec_decode_state = STATE_TRAILER_PULSE;
	else
	    ir_nec_decode_state = STATE_BIT_PULSE;
        return;

    case STATE_TRAILER_PULSE:
	ir_nec_decode_state = STATE_TRAILER_SPACE;
	return;

    case STATE_TRAILER_SPACE:
        ir_nec_decode_state = STATE_INACTIVE;
	return;

    /* back to start */
    ir_nec_decode_state = STATE_INACTIVE;
}
