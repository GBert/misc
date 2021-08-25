#ifndef BITMASK_H
#define BITMASK_H

#define BIT_MASK(a, b) (((unsigned) -1 >> (31 - (b))) & ~((1U << (a)) - 1))

#define MASK_LOW_NIBBLE  0x0f
#define MASK_HIGH_NIBBLE 0xf0
#define MASK_LOW_BYTE  0x00ff
#define MASK_HIGH_BYTE 0xff00
#define MASK_LOW_WORD  0x0000ffff
#define MASK_HIGH_WORD 0xffff0000
#define MASK_LOW_BYTE_OF_HIGH_WORD  0x00ff0000
#define MASK_HIGH_BYTE_OF_HIGH_WORD 0xff000000

#define SHIFT_0_BYTE  0
#define SHIFT_1_BYTE  8
#define SHIFT_2_BYTE 16
#define SHIFT_3_BYTE 24
#define SHIFT_4_BYTE 32

#endif
