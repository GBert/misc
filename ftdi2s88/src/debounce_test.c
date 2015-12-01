/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BIT(x)		(1<<x)

uint8_t test_data [512];

const char *bit_rep[16] = {
    [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
    [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
    [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

void print_byte(uint8_t byte)
{
    printf("%s%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);
}

void print_bin(char *str, uint8_t c, int nl) {
    printf("%s:", str);
    print_byte(c);
    if (nl)
	printf("\n");
}

void check_data(uint8_t changed, uint8_t value) {
    int i;
    uint8_t mask;

    mask = 1;

    for (i=0; i<8; i++) {
	if (changed & mask) {
	    if (value & mask) {
		printf("bit %d -> 1\n", i);
	    } else {
		printf("bit %d -> 0\n", i);
	    }
	}
	mask <<=1;
    }
}

int main(int argc, char **argv) {
    int i;
    uint8_t ct0, ct1, c, state, old;

    ct0 = 0;
    ct1 = 0;
    c = 0;
    state = 0;
    old = 0;

    for (i=0; i < sizeof(test_data); i++)
	test_data[i] = i & 0xff;

    for (i=0; i < 32; i++) {
	// c = state ^ ~test_data[i];
	c = state ^ ~test_data[i];
	ct0 = ~( ct0 & c );
	ct1 = ct0 ^ (ct1 & c);
	print_bin("c  ", c, 0);
	print_bin(" old  ", old, 0);
	print_bin(" ct0", ct0, 1);
	print_bin("                            ct1", ct1, 1);
	printf("\n");
	c &= ct0 & ct1;
	print_bin("c: ", c, 0);
	print_bin("  bits", test_data[i], 1);
	check_data(c, test_data[i]);
	state ^= c;
	old |= state & c;

    }
    exit(0);
}
