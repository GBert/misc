/**
 * pic tiny bootloader - descriptions of supported PICs
 * 
 * Source for data was:
 *   https://sourceforge.net/projects/tinybldlinux/
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

#include <stdlib.h>
#include "picdata.h"

static PicData pics[] = {
{ "16F876A/16F877A",               0x31, 0x2000 },
{ "16F873A/16F874A",               0x32, 0x1000 },
{ "16F87/16F88",                   0x33, 0x1000 },
{ "18F252/18F452/18F2520/18F4520", 0x41, 0x8000 },
{ "18F242/18F442/18F2420/18F4420", 0x42, 0x4000 },
{ "18F258/18F458",                 0x43, 0x8000 },
{ "18F248/18F448",                 0x44, 0x4000 },
{ "18F1320/18F2320",               0x45, 0x2000 },
{ "18F1220/18F2220",               0x46, 0x1000 },
{ "18F4320/",                      0x47, 0x2000 },
{ "18F4220",                       0x48, 0x1000 },
{ "18F6720/18F8720",               0x4A, 0x20000 },
{ "18F6620/18F8620",               0x4B, 0x10000 },
{ "18F6520/18F8520",               0x4C, 0x8000 },
{ "18F8680",                       0x4D, 0x10000 },
{ "18F2525/18F4525",               0x4E, 0xC000 },
{ "18F2620/18F4620",               0x4F, 0x10000 },
{ "18F2550/18F4550",               0x55, 0x8000 },
{ "18F2455/18F4455",               0x56, 0x6000 },
{ "18F4685",                       0x57, 0x18000 },
{ "18F_w/4k_flash",                0x60, 0x1000 },
{ "18F_w/8k_flash",                0x61, 0x2000 },
{ "18F_w/16k_flash",               0x62, 0x4000 },
{ "18F_w/24k_flash",               0x63, 0x4000 },
{ "18F_w/32k_flash",               0x64, 0x8000 },
{ "18F_w/48k_flash",               0x65, 0xC000 },
{ "18F_w/64k_flash",               0x66, 0x10000 },
{ "18F_w/80k_flash",               0x67, 0x14000 },
{ "18F_w/96k_flash",               0x68, 0x18000 },
{ "18F_w/128k_flash",              0x69, 0x20000 },
{ NULL, 0, 0 }
};

PicData *PicData_get(unsigned char idByte)
{
    PicData *p;
    
    for (p = pics; p->desc != NULL; p++)
    {
        if (p->idByte == idByte)
            return p;
    }
    
    return NULL;
}
