#ifndef __FX2_UTIL_H__
#define __FX2_UTIL_H__

// Read TRM p.15-115 for an explanation on this. 
// A single nop is sufficient for default setup but like that we're on 
// the safe side. 
#define	NOP		__asm nop __endasm
#define	SYNCDELAY	NOP; NOP; NOP; NOP

#endif // __FX2_UTIL_H__
