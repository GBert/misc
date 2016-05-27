/**
 * ec2_cksum.c - Calculace SiLabs EC2 specific checksum
 *
 * Copied from ec2-newer/src/ec2drv/boot.c:
 *   (C) Ricky White 2007
 *   Liscence GPL V2
 */
#include "ec2_cksum.h"

uint16_t boot_local_calc_page_cksum(uint8_t *data)
{ 
	int i,j; 
	uint16_t cksum = 0;
 
	for(i = 0; i<512; i++) 
	{ 
		cksum ^= (uint16_t)data[i] << 8; 
 
		for(j = 0; j < 8; j++) 
		{ 
			cksum <<= 1; 
			if(cksum & 0x8000) cksum ^= 0x1021; 
		}; 
	}; 
 
	return cksum; 
}
