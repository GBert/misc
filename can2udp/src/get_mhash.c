#include <stdio.h>
#include <stdint.h>

/*

  CS2 CAN hash generation

  hash' = highword ^ lowword
  
  xxxxxxx11 0xxxxxxxx
  ^^^^^^^    ^^^^^^^^
   \\\\\\\   ||||||||
    \\\\\\-- ||||||||
     \\\\\\ \||||||||
  000xxxxxx xxxxxxxxx

 */
uint16_t generateHash(uint32_t uid) {
    uint16_t hash, highword, lowword;

    highword = uid >> 16;
    lowword = uid & 0xFFFF;
    hash = highword ^ lowword;
    hash = (((hash << 3) & 0xFF00) | 0x0300) | (hash & 0x7F);
    return hash;
}

int main(int argc, char **argv) {
    uint32_t uid;
    uint16_t hash;

    if (argc == 1) {
	for (uid = 0; uid < 0x100000000UL; uid++) {
	    hash = generateHash(uid);
	    if (hash == 0x3b6a)
		printf("uid 0x%08x -> hash 0x%04x\n", uid, hash);
	}
	return 0;
    }

    if (argc != 2) {
        printf("error: usage %s hex_uid\n", argv[0]);
        return -1;
    }
    sscanf(argv[1], "%x", &uid);
    printf("uid 0x%08x -> hash 0x%04x\n", uid, generateHash(uid));
}

