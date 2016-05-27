/*
 * timing_fx2_48mhz.c - Timing functions for Cypress EZ-USB FX2 @ 48 MHz
 *
 * Copyright (c) 2012, David Imhoff <dimhoff.devel@gmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors may
 *       be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __TIMING_H__
#define __TIMING_H__

/**
 * Wait 1 microseconds
 *
 * This function waits 1 microseconds. The calling of the function has been
 * taken into account.
 */
void wait_1us() __naked;

/**
 * Wait 5 microseconds
 *
 * This function waits 5 microseconds. The calling of the function has been
 * taken into account.
 */
void wait_5us() __naked;

/**
 * Wait 'times' * 10 microseconds
 *
 * This function waits exactly 'times' * 10 microseconds. The passing of 
 * the argument and calling the function has been taken into account.
 *
 * @param times		The amount of 10us intervals to wait, 0 means max.
 */
void wait_10us(unsigned char times) __naked;

#endif // __TIMING_H__
