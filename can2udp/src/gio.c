/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "can2lan.h"

#define POLY 0x1021

uint16_t UpdateCRC(unsigned short CRC_acc, unsigned char CRC_input) {
    int i;

    /* Create the CRC "dividend" for polynomial arithmetic
       (binary arithmetic with no carries) */
    CRC_acc = CRC_acc ^ (CRC_input << 8);
    /* "Divide" the poly into the dividend using CRC XOR subtraction
       CRC_acc holds the "remainder" of each divide. Only complete
       this division for 8 bits since input is 1 byte */
    for (i = 0; i < 8; i++)
    {
        /* Check if the MSB is set (if MSB is 1, then the POLY
           can "divide" into the "dividend") */
        if ((CRC_acc & 0x8000) == 0x8000)
        {
           /* if so, shift the CRC value, and XOR "subtract" the poly */
           CRC_acc = CRC_acc << 1;
           CRC_acc ^= POLY;
        }
        else
        {
            /* if not, just shift the CRC value */
            CRC_acc = CRC_acc << 1;
        }
    }
    /* Return the final remainder (CRC value) */
    return CRC_acc;
}

uint8_t * read_config_file(char *filename, uint32_t *nbytes, char compressed, uint16_t *crc) {
    int rc;
    struct stat st;
    FILE *fp;
    /* char *s; */
    uint8_t *config;

    rc = stat(filename, &st);
    if (rc < 0) {
        printf("%s: error stat failed for %s\n", __func__, filename);
        return NULL;
    }
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("%s: error fopen failed for %s\n", __func__, filename);
        return NULL;
    }
    *nbytes = st.st_size;
    config = (uint8_t *)calloc(*nbytes, sizeof(uint8_t));
    if (config == NULL) {
        printf("%s: error calloc failed for %s\n", __func__, filename);
        return NULL;
    }
    rc = fread((void *)config, 1, *nbytes, fp);
    if ((rc != *nbytes)) {
        printf("%s: error fread failed for %s\n", __func__, filename);
        return NULL;
    }
    return config;
}
