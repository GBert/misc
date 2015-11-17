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

#ifndef INTELHEX_H
#define INTELHEX_H

/**
 * Callback run when data is found in hex file
 * 
 * @param addr Start address of block
 * @param len  Length of binary data in bytes
 * @param data Binary data
 */
typedef void (intelhex_datacallback)(unsigned int addr,
                                     unsigned int len,
                                     char        *data);

/**
 * Parse intel hex file of given name
 * 
 * In case of an error, simply calls exit() after outputing error
 * message to stderr
 * 
 * @param fname Filename suitable for passing to fopen()
 * @param cb    Callback that's run whenever data is parsed
 */
void intelhex_parse(const char            *fname,
                    intelhex_datacallback *cb);

#endif /* INTELHEX_H */
