#include <stdio.h>
#include <stdint.h>
/* #include <type.h> */

uint8_t rpt, key_press, key_state, KEY_PIN;
uint8_t ct0, ct1;


void isr(uint8_t pins) {
  uint8_t i;
 
  i = key_state ^ ~pins;		/* pin changed ? */
  ct0 = ~( ct0 & i );			/* reset or count ct0 */
  ct1 = ct0 ^ (ct1 & i);		/* reset or count ct1 */
  i &= ct0 & ct1;			/* count until roll over ? */
  key_state ^= i;			/* then toggle debounced state */
  key_press |= key_state & i;		/* 0->1: key press detect */
 
  if( (key_state & REPEAT_MASK) == 0 )	/* check repeat function */
     rpt = REPEAT_START;		/* start delay */
  if( --rpt == 0 ){
    rpt = REPEAT_NEXT;			/* repeat delay */
    key_rpt |= key_state & REPEAT_MASK;
  }
}

int main(int argc, char **argv) {

    return 0;
}
