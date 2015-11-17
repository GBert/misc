/**
 * pic tiny bootloader PC side software
 * 
 * Copyright (c) 2009, Joseph Heenan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Joseph Heenan nor the names of any other
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Joseph Heenan ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <ctype.h>
#ifdef WIN32
typedef enum
{
    false,
    true
}
bool;
#else /* WIN32 */
#include <unistd.h>
#include <stdbool.h>
#endif /* WIN32 */
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>

#include "intelhex.h"
#include "picdata.h"
#include "serial.h"

#define DBUGF(x) printf x
#define BLOCK_DUMP 1
/* flash write block size - should be 16 for 16Fs */
#define BLOCKSIZE 64

#define WRITEACK_TIMEOUT 600 /* milliseconds */

typedef struct Block
{
    unsigned int startAddr;
    unsigned int len;
    unsigned char data[64];
}
Block;

/**
 * Array of blocks that hex data is loaded into
 * 
 * List is terminated by an entry with len == 0
 */
Block blocks[2048];

/**
 * Global pointer to store current block been written into
 * for hexcallback to use
 */
Block *blockPtr = blocks;

int rts_reset;
char default_tty_device[] = "/dev/ttyUSB0";

static void print_usage(char *prg)
{
    fprintf(stderr, "\nUsage: %s -p <port> -s <speed> -r -f <file>\n", prg);
    fprintf(stderr, "\n");
    fprintf(stderr, "         -p <port>           tty device - default /dev/ttyUSB0\n");
    fprintf(stderr, "         -s <speed>          tty device speed - default 19200\n");
    fprintf(stderr, "         -r                  reset PIC via RTS\n");
    fprintf(stderr, "         -f <HEX file>       HEX file to upload\n");
    fprintf(stderr, "\n");
}

/**
 * Callback passed to intelhex code
 *
 * Assembles hex data from intel hex file into chunks upto
 * the blocksize
 */
static void hexcallback(unsigned int addr,
                        unsigned int len,
                        char        *data)
{
    unsigned int i;
    
    if (blockPtr->len != 0)
    {
        if (addr < blockPtr->startAddr + blockPtr->len)
        {
            fprintf(stderr, "Out of order entries in hex file "
                    "[%08x / %08x + %04x]\n",
                    addr, blockPtr->startAddr, blockPtr->len);
            exit(EXIT_FAILURE);
        }
    }
    /* store data for later use */
    if (blockPtr->len != 0 &&
        blockPtr->len != BLOCKSIZE &&
        blockPtr->startAddr + BLOCKSIZE > addr &&
        blockPtr->len + len <= BLOCKSIZE)
    {
        if (blockPtr->startAddr + blockPtr->len < addr)
        {
            unsigned int pad = addr - (blockPtr->startAddr + blockPtr->len);
            /* pad with 0xFF */
            for (i = 0; i < pad; i++)
            {
                blockPtr->data[blockPtr->len++] = 0xff;
            }
        }
        /* continuation of previous block */
    }
    else
    {
        if (blockPtr->len != 0)
        {
             /* move to next block if already stored some data */
            blockPtr++;
            if (blockPtr >= blocks + sizeof(blocks) / sizeof(*blocks))
            {
                fprintf(stderr, "'blocks' array too small, please "
                        "increase size\n");
                exit(EXIT_FAILURE);
            }
        }
        blockPtr->startAddr = addr;
    }
    
    for (i = 0; i < len; i++)
    {
        blockPtr->data[i + blockPtr->len] = data[i];
    }
    blockPtr->len += len;
}

/**
 * Write a command to the bootloader
 * 
 * @param fd     serial port fd for the bootloader
 * @param out    data to write. CRC will be added after buffer, input
 *               array must be large enough
 * @param outptr pointer to first free location in 'out'
 */
static void sendwrite(int fd, unsigned char *out, unsigned char *outptr)
{
    int ret;
    unsigned char *p;
    unsigned char crc = 0;
    unsigned char c = 0;

    /* calculate crc */
    for (p = out; p < outptr; p++)
        crc += *p;
    crc = (~crc) + 1;
    *outptr++ = crc;
    
    DBUGF(("Writing data:"));
    for (p = out; p < outptr; p++)
    {
        DBUGF(("%02x ", *p));
    }
    DBUGF(("\n"));

    ret = serial_write(fd, (char *)out, outptr-out);
    if (ret != outptr-out)
    {
        fprintf(stderr, "Failed to write to serial port [%d]\n",
                __LINE__);
        exit(EXIT_FAILURE);
    }
    
    ret = serial_readtimeout(fd, &c, 1, WRITEACK_TIMEOUT);
    if (ret == -2)
    {
        /* we could try again here */
        fprintf(stderr, "Timeout reading from serial port [%d]\n",
                __LINE__);
        exit(EXIT_FAILURE);
    }
    if (ret != 1)
    {
        fprintf(stderr, "Failed to read from serial port [%d]\n",
                __LINE__);
        exit(EXIT_FAILURE);
    }
    if (c != 'K')
    {
        /* we could try again here */
        fprintf(stderr, "Failed to write block [%02x]", c);
        exit(EXIT_FAILURE);
    }
}

/**
 * Trying sending C1 to enter the bootloader
 * 
 * @param fd     serial port
 * @param idByte Pointer for id byte to be returned in
 * @param quiet  Don't print any non-fatal messages
 * 
 * @retval 0  success
 * @retval -1 fatal error
 * @retval -2 timeout
 * @retval -3 unrecognised response
 */
static int tryC1(int fd, unsigned char *idByte, bool quiet)
{
    int ret;
    unsigned char buff[2];
    
    DBUGF(("Sending 0xC1\n"));
    ret = serial_write(fd, "\xC1", 1);
    if (ret != 1)
    {
        fprintf(stderr, "Failed to write to serial port [%d]\n", __LINE__);
        return -1;
    }
    
    ret = serial_readtimeout(fd, &buff[0], 1, 100);
    if (ret == -2)
    {
        if (!quiet)
          fprintf(stderr, "Timeout reading from serial port [%d]\n", __LINE__);
        return -2;
    }
    if (ret != 1)
    {
        fprintf(stderr, "Failed to read from serial port [%d]\n", __LINE__);
        return -1;
    }
    ret = serial_readtimeout(fd, &buff[1], 1, 100);
    if (ret == -2)
    {
        if (!quiet)
          fprintf(stderr, "Timeout reading from serial port [%d]\n", __LINE__);
        return -2;
    }
    if (ret != 1)
    {
        fprintf(stderr, "Failed to read from serial port [%d]\n", __LINE__);
        return -1;
    }
    DBUGF(("read: %02x %02x\n", buff[0], buff[1]));
    
    if (buff[1] != 'K')
    {
        if (!quiet)
          fprintf(stderr, "Unexpected character %02x\n", buff[1]);
        return -3;
    }
    
    *idByte = buff[0];
    return 0;
}

/**
 * Start the bootloader
 * 
 * This should ensure communication with the bootloader has been
 * successfully established then return
 * 
 * @param idByte Pointer for id byte received from bootloader to be returned in
 */
static void enterbootloader(int fd, unsigned char *idByte)
{
    int ret;
    int i;

    printf("Trying to enter bootloader...\n");
    

    /* first try just C1 */
    ret = tryC1(fd, idByte, true);
    if (ret == 0)
        return;
    if (ret == -1)
    {
        fprintf(stderr, "tryC1 failed\n");
        exit(EXIT_FAILURE);
    }
    
    if (ret == -3)
    {
        /* unrecognised response */
        fprintf(stderr, "There appears to be a device connected to "
                "this serial port that is not running the expected "
                "bootloader\n");
        exit(EXIT_FAILURE);
    }

    printf("Trying to talk to boot loader...");
    fflush(stdout);

    for (i = 0; i < 50; i++)
    {
	if (rts_reset) {
	    DBUGF((" pulse RTS line...\n"));
	    set_rts(fd, 1);
	    usleep(50*1000);
	    set_rts(fd, 0);
	    usleep(100*1000);
	}
        ret = tryC1(fd, idByte, true);
        if (ret == 0)
            return;
        if (i % 5 == 1)
        {
            printf(".");
            fflush(stdout);
        }
    }
    
    printf("\nNo response\n");
    printf("\n ** Please remove power from device briefly then reconnect it **\n\n");

    /* continue trying to talk without user seeing it */
    for (i = 0; i < 20; i++)
    {
        ret = tryC1(fd, idByte, true);
        if (ret == 0)
            return;
    }
    
    printf("Trying to talk to boot loader...");
    fflush(stdout);
    for (i = 0; i < 100; i++)
    {
        ret = tryC1(fd, idByte, true);
        if (ret == 0)
            return;
        if (i % 10 == 1)
        {
            printf(".");
            fflush(stdout);
        }
    }

    ret = tryC1(fd, idByte, false);
    if (ret == 0)
        return;
    fprintf(stderr, "tryC1 failed [%d]\n", ret);
    exit(EXIT_FAILURE);
}

static void writefirmware(int fd, PicData *pic)
{
    int lastprogress = 0;
    int progress;
    int bytesTotal = 0;
    int bytesWritten = 0;
    
    for (blockPtr = blocks; blockPtr->len != 0; blockPtr++)
        bytesTotal += blockPtr->len;
    
    printf("Progress:   0%%\b");
    fflush(stdout);

    for (blockPtr = blocks; blockPtr->len != 0; blockPtr++)
    {
        unsigned char out[128];
        unsigned int i;
        unsigned int addr = blockPtr->startAddr;
        unsigned char *outptr = out;
        
        /* calculate percentage process to nearest 10% */
        progress = bytesWritten * 10 / bytesTotal * 10;
        if (progress != lastprogress)
        {
            printf("\b\b%02d", progress);
            fflush(stdout);
            lastprogress = progress;
        }
        
        bytesWritten += blockPtr->len;
        
        if (addr < 0x800)
        {
            /* do not write to boot block, it's protected */
            DBUGF(("ignoring boot block\n"));
            continue;
        }
        if (addr == 0x00300000)
        {
            /* do not write to configuration block, it's protected */
            DBUGF(("ignoring config address\n"));
            continue;
        }
        if (addr >= 0x00f00000 &&
            addr <  0x01000000)
        {
            /* eeprom data */
            for (i = 0; i < blockPtr->len; i++)
            {
                outptr = out;
                *outptr++ = 0x40;
                *outptr++ = addr + i;
                *outptr++ = blockPtr->data[i];
                *outptr++ = 0; /* count */
                sendwrite(fd, out, outptr);
            }
            continue;
        }
        if (addr + blockPtr->len > pic->flashSize)
        {
            fprintf(stderr, "Block %08x - %08x is not inside the device's "
                   "flash blocks\n", addr, addr+blockPtr->len);
            exit(EXIT_FAILURE);
        }
        if ((addr & (BLOCKSIZE-1)) != 0)
        {
            fprintf(stderr, "Block %08x - %08x is not aligned to the device's "
                   "flash block size\n", addr, addr+blockPtr->len);
            exit(EXIT_FAILURE);
        }
        
        while (blockPtr->len < BLOCKSIZE)
        {
            /* pad block */
            blockPtr->data[blockPtr->len++] = 0xff;
        }
        
        DBUGF(("addr %08x len %d", blockPtr->startAddr, blockPtr->len));
        for (i = 0; i < blockPtr->len; i++)
        {
            DBUGF((" %02x", blockPtr->data[i]));
        }
        DBUGF(("\n"));
        
        /* format data to send to PIC */
        /* for PIC16F, you would not send the first addr byte */
        *outptr++ = addr >> 16;
        *outptr++ = addr >> 8;
        *outptr++ = addr;
        *outptr++ = blockPtr->len;
        for (i = 0; i < blockPtr->len; i++)
            *outptr++ = blockPtr->data[i];
        sendwrite(fd, out, outptr);
    }
    printf("\b\b\b100\n");
}

int main(int argc, char *argv[])
{
    char *fname = malloc(1024);
    char *port = malloc(64);
    int fd, baud, verbose, opt;
    unsigned char idByte;
    PicData *pic;

    /* set defaults */
    baud = 19200;
    strcpy(port,default_tty_device);
    verbose = 0;
    

    while ((opt = getopt(argc, argv, "p:s:f:rv?")) != -1) {
	switch (opt) {
	    case 'p':
		strcpy(port, optarg);
		break;
	    case 's':
		baud = strtoul(optarg, (char **)NULL, 10);
		break;
	    case 'f':
		strcpy(fname,optarg);
		break;
	    case 'r':
		rts_reset = 1;
		break;
	    case 'v':
		verbose = 1;
		break;
	    case '?':
		print_usage(basename(argv[0]));
		exit(0);
		break;
	    default:
		fprintf(stderr, "Unknown option %c\n", opt);
		print_usage(basename(argv[0]));
		exit(EXIT_FAILURE);
		break;
	    }
    }
    if (verbose) {
	/* TODO */
    }

/*    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "%s: missing parameters\n", argv[0]);
        fprintf(stderr, "syntax: %s <serial port> [<hex file>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port = argv[1];
    if (argc == 3)
        fname = argv[2];
*/
    
    memset(blocks, 0, sizeof(blocks));
    
    intelhex_parse(fname, hexcallback);

#ifdef BLOCK_DUMP
    /* dump out contents of blocks read */
    for (blockPtr = blocks; blockPtr->len != 0; blockPtr++)
    {
        unsigned int i;
        DBUGF(("addr %08x len %d", blockPtr->startAddr, blockPtr->len));
        for (i = 0; i < blockPtr->len; i++)
        {
            DBUGF((" %02x", blockPtr->data[i]));
        }
        DBUGF(("\n"));
    }
#endif /* BLOCK_DUMP */
    
    fd = serial_openport(port, baud, 1);
    if (fd == -1)
    {
        fprintf(stderr, "Failed to open serial port %s at baud %d\n",
                port, baud);
        exit(EXIT_FAILURE);
    }

    enterbootloader(fd, &idByte);
    
    printf("Bootloader has started\n");
    
    pic = PicData_get(idByte);
    if (pic == NULL)
    {
        fprintf(stderr, "Id byte %02x does not match a supported PIC\n",
                idByte);
        exit(EXIT_FAILURE);
    }
    DBUGF(("Found PIC: %s\n", pic->desc));
    
    printf("Writing firmware...\n");
    writefirmware(fd, pic);
    
    printf("Firmware written okay\n");

    serial_close(fd);
    
    return 0;
}
