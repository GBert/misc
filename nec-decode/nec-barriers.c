#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define TIMER_GRID		64000	/* 64us */
#define MARGIN			30	/* in percent */

#define	T_LOW(x)	((( x * (100 - MARGIN)) / TIMER_GRID)/ 100)
/* round up (+0.5) */
#define	T_HIGH(x)	((( x * (100 + MARGIN)) / TIMER_GRID + 50 )/ 100)

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

#define NEC_HEADER_PULSE_L	T_LOW(NEC_HEADER_PULSE)
#define NEC_HEADER_PULSE_H	T_HIGH(NEC_HEADER_PULSE)
#define NEC_HEADER_SPACE_L	T_LOW(NEC_HEADER_SPACE)
#define NEC_HEADER_SPACE_H	T_HIGH(NEC_HEADER_SPACE)
#define NEC_REPEAT_SPACE_L	T_LOW(NEC_REPEAT_SPACE)
#define NEC_REPEAT_SPACE_H	T_HIGH(NEC_REPEAT_SPACE)
#define NEC_BIT_PULSE_L		T_LOW(NEC_BIT_PULSE)
#define NEC_BIT_PULSE_H		T_HIGH(NEC_BIT_PULSE)
#define NEC_BIT_TRAILER_L	T_LOW(NEC_BIT_TRAILER)
#define NEC_BIT_TRAILER_H	T_HIGH(NEC_BIT_TRAILER)

int main(int argc, char **argv) {
    double nominal;

    nominal = (double ) NEC_HEADER_PULSE / TIMER_GRID;
    printf("NEC_HEADER_PULSE nominal: %f low: %d high: %d\n",
	nominal, (int ) T_LOW(NEC_HEADER_PULSE), (int ) T_HIGH(NEC_HEADER_PULSE));

    nominal = (double ) NEC_HEADER_SPACE / TIMER_GRID;
    printf("NEC_HEADER_SPACE nominal: %f low: %d high: %d\n",
	nominal, (int ) T_LOW(NEC_HEADER_SPACE), (int ) T_HIGH(NEC_HEADER_SPACE));

    nominal = (double ) NEC_BIT_PULSE / TIMER_GRID;
    printf("NEC_BIT_PULSE nominal: %f low: %d high: %d\n",
	nominal, (int ) T_LOW(NEC_BIT_PULSE), (int ) T_HIGH(NEC_BIT_PULSE));

    nominal = (double ) NEC_BIT_PULSE / TIMER_GRID;
    printf("NEC_BIT_PULSE nominal: %f low: %d high: %d\n",
	nominal, (int ) T_LOW(NEC_BIT_PULSE), (int ) T_HIGH(NEC_BIT_PULSE));
    nominal = (double ) NEC_BIT_PULSE / TIMER_GRID;
    printf("NEC_BIT_PULSE nominal: %f low: %d high: %d\n",
	nominal, (int ) T_LOW(NEC_BIT_PULSE), (int ) T_HIGH(NEC_BIT_PULSE));

    return 0;
}
