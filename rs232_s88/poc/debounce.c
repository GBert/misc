/* inspred by Peter Dannegger danni@specs.de     */

#include <stdio.h>
#include <stdint.h>

#define REPEAT_MASK	255
#define REPEAT_START    50
#define REPEAT_NEXT     20

uint8_t test_data1[] = { 0x00, 0x02, 0x03, 0x02, 0x03, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x80 };

uint8_t rpt, key_rpt, key_press, key_state, KEY_PIN;
uint8_t ct0, ct1;

const char *bit_rep[16] = {
    "0000", "0001", "0010", "0011",
    "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011",
    "1100", "1101", "1110", "1111",
};

void print_byte(uint8_t byte) {
    printf("%s%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);
}

void isr(uint8_t pins) {
    uint8_t i;

    i = key_state ^ ~pins;	/* pin changed ? */
    ct0 = ~(ct0 & i);		/* reset or count ct0 */
    ct1 = ct0 ^ (ct1 & i);	/* reset or count ct1 */
    i &= ct0 & ct1;		/* count until roll over ? */
    key_state ^= i;		/* then toggle debounced state */
    key_press |= key_state & i;	/* 0->1: key press detect */

#if 0
    if ((key_state & REPEAT_MASK) == 0)	/* check repeat function */
	rpt = REPEAT_START;	/* start delay */
    if (--rpt == 0) {
	rpt = REPEAT_NEXT;	/* repeat delay */
	key_rpt |= key_state & REPEAT_MASK;
    }
#endif
    printf("ct0 0b");
    print_byte(ct0);
    printf("\nct1 0b");
    print_byte(ct1);
    printf("\npin 0b");
    print_byte(key_press);
    printf("\n\n");
}

void feed_debouncer(uint8_t * data, int length) {
    int i;
    for (i = 0; i < length; i++) {
	isr(data[i]);
    }
}

int main(int argc, char **argv) {
    feed_debouncer(test_data1, sizeof(test_data1));
    return 0;
}
