/**
 * ec2_cksum.c - Calculate SiLabs EC2 specific checksum
 *
 * Copied from src/ec2drv/boot.c of the ec2drv project:
 *   (C) Ricky White 2007
 *   Liscence GPL V2
 *   http://ec2drv.sourceforge.net
 */
#ifndef __EC2_CKSUM_H__
#define __EC2_CKSUM_H__

#include <stdint.h>

/** Calculates the same checksum as boot_calc_page_cksum asks the EC2/3 to do 
	but does but on a local	block of data.

	There's a little difference in the SiLabs code compared to CCITT-CRC16
	(although the polynomial looks the same): 
 
	For real CRC, the MSB test (&0x8000) should occur _before_ shifting the
	value, not afterwards
	(see http://en.wikipedia.org/wiki/Cyclic_redundancy_check ).
	Or use C after shifting, instead of MSB before shifting. 
 
	And CCITT-CRC16 uses (IMHO) 0xFFFF for initialisation, not 0. But that's
	not important for the strength of the CRC. 
	-- Kolja

	\param data buffer contains 1 page worth of data to calculate a checksum
				for (512 bytes)
	\returns	Calculated checksum.
*/
uint16_t boot_local_calc_page_cksum(uint8_t *data);

#endif // __EC2_CKSUM_H__
