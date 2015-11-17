/**
 * Code for loading intel hex files
 * 
 * Designed for output produced by the Microchip PIC MPLAB development tools.
 * 
 * Reference: http://en.wikipedia.org/wiki/Intel_HEX
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
typedef enum
{
    false,
    true
}
bool;
#else /* WIN32 */
#include <stdbool.h>
#endif /* WIN32 */

#include "intelhex.h"

#include "firmware-data.h"

#define DBUGF(x) printf x

/* decode a single ASCII hex character into decimal */
static unsigned int hextodec(int c)
{
    assert(isxdigit(c));
    if (isdigit(c))
        return c - '0';
    if (isupper(c))
      return c - 'A' + 10;
    return c - 'a' + 10;
}

/* decode a ASCII hex string of the given length */
static unsigned int decodehex(const char *p, int cnt)
{
    int i = 0;
    while (cnt-- > 0)
    {
        i = i * 16 + hextodec(*p++);
    }
    return i;
}

/* handle one line from a hex file
 *
 * returns true when the EOF marker is found in the hex data
 */
static int handleline(intelhex_datacallback *cb, char *line, unsigned int *haddr)
{
    unsigned int len;
    unsigned int addr;
    unsigned int type;
    unsigned char checksum = 0;
    char *p;

    if (*line == 0)
        return false;
    p = line + strlen(line) - 1;
    if (*p != '\n')
    {
        fprintf(stderr, "Overlong line read\n");
        exit(EXIT_FAILURE);
    }
    *p = 0;
    p--;
    if (*p == '\r')
        *p = 0;
    
    if (*line != ':')
    {
        fprintf(stderr, "Line does not begin with ':' (%s)\n", line);
        exit(EXIT_FAILURE);
    }
    if (strlen(line) < 11)
    {
        fprintf(stderr, "Line is too short to be valid (%s)\n", line);
        exit(EXIT_FAILURE);
    }
    
    /* rest of string should now be purely hex */
    for (p = line + 1; *p != 0; p++)
    {
        if (!isxdigit(*p))
        {
            fprintf(stderr, "Line contains non-hex characters (%s)\n", line);
            exit(EXIT_FAILURE);
        }
    }
    
    len  = decodehex(line + 1, 2);
    addr = decodehex(line + 3, 4);
    type = decodehex(line + 7, 2);
    DBUGF(("%s\n", line));
    DBUGF(("type = %d, addr = %08x, len = %d\n", type, addr, len));
    if (strlen(line) - 11 != len * 2)
    {
        fprintf(stderr, "Length field doesn't match (%s)\n", line);
        exit(EXIT_FAILURE);
    }
    
    /* check checksum is correct */
    for (p = line + 1; p < line + 11 + len * 2; p += 2)
    {
        unsigned int i;
        
        i = decodehex(p, 2);
        checksum += i;
    }
    if (checksum != 0)
    {
        fprintf(stderr, "Checksum is incorrect (%s)\n", line);
        exit(EXIT_FAILURE);
    }
    
    switch (type)
    {
        case 0:
        {
            unsigned int i;
            char data[16];
            /* data record */
            addr = (*haddr << 16) + addr;
            if (len == 0)
            {
                fprintf(stderr, "Length field incorrect for type 0 (%s)\n", line);
                exit(EXIT_FAILURE);
            }
            if (len > 16)
            {
                fprintf(stderr, "Too longer data for type 0 (%s)\n", line);
                exit(EXIT_FAILURE);
            }
            DBUGF(("data addr = %08x, len = %d\n", addr, len));
            for (i = 0; i < len; i++)
            {
                data[i] = decodehex(line + 9 + i * 2, 2);
            }
            cb(addr, len, data);
            break;
        }
        case 1:
            /* end of file marker */
            if (len != 0)
            {
                fprintf(stderr, "Length field incorrect for type 1 (%s)\n", line);
                exit(EXIT_FAILURE);
            }
            return true;
            break;
        case 4:
            /* high address */
            if (len != 2)
            {
                fprintf(stderr, "Length field incorrect for type 4 (%s)\n", line);
                exit(EXIT_FAILURE);
            }
            *haddr = decodehex(line + 9, 4);
            break;
        default:
            fprintf(stderr, "Line contains unknown type %d (%s)\n", type, line);
            exit(EXIT_FAILURE);
    }
    return false;
}


void intelhex_parse(const char *fname,
                    intelhex_datacallback *cb)
{
    FILE *f;
    bool eof = false;
    unsigned int haddr = 0; /* top 16 bits of address */
    char line[256];

    if (fname == NULL)
    {
        const char **ptr;
        char *p;
        /* use the embedded firmware */
        printf("Using data: %s\n",  version);
        for (ptr = firmware; *ptr[0] != 0; ptr++)
        {
            if (eof)
            {
                printf("EOF encountered early\n");
                exit(EXIT_FAILURE);
            }
            strcpy(line, *ptr);
            /* undo the obfuscation the perl did */
            for (p = line; *p != '\n'; p++)
            {
            	*p = *p - 15;
            }
            eof = handleline(cb, line, &haddr);
        }
        if (!eof)
        {
            printf("EOF not found\n");
            exit(EXIT_FAILURE);
        }

        return;
    }
    
    f = fopen(fname, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Failed to open '%s'\n", fname);
        exit(EXIT_FAILURE);
    }
    
    while (fgets(line, sizeof(line), f) != NULL && !eof)
    {
        eof = handleline(cb, line, &haddr);
    }

    if (ferror(f))
    {
        fprintf(stderr, "File read error occurred\n");
    }

    if (!feof(f))
    {
        fprintf(stderr, "Data after end of file\n");
        exit(EXIT_FAILURE);
    }
    
    fclose(f);
}
