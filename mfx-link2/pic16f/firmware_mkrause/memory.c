/**
  MEMORY Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    memory.c

  @Summary
    This is the generated driver implementation file for the MEMORY driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This file provides implementations of driver APIs for MEMORY.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F1709
        Driver Version    :  2.0.2
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB             :  MPLAB X 5.45
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "memory.h"

/**
  Section: Flash Module APIs
*/

uint16_t FLASH_ReadWord(uint16_t flashAddr)
{
    uint8_t GIEBitValue = INTCONbits.GIE;   // Save interrupt enable

    INTCONbits.GIE = 0;     // Disable interrupts
    PMADRL = (flashAddr & 0x00FF);
    PMADRH = ((flashAddr & 0xFF00) >> 8);

    PMCON1bits.CFGS = 0;    // Deselect Configuration space
    PMCON1bits.RD = 1;      // Initiate Read
    NOP();
    NOP();
    INTCONbits.GIE = GIEBitValue;	// Restore interrupt enable

    return ((uint16_t)((PMDATH << 8) | PMDATL));
}

//void FLASH_WriteWord(uint16_t flashAddr, uint16_t *ramBuf, uint16_t word)
//{
//    uint16_t blockStartAddr = (uint16_t)(flashAddr & ((END_FLASH-1) ^ (ERASE_FLASH_BLOCKSIZE-1)));
//    uint8_t offset = (uint8_t)(flashAddr & (ERASE_FLASH_BLOCKSIZE-1));
//    uint8_t i;
//
//    // Entire row will be erased, read and save the existing data
//    for (i=0; i<ERASE_FLASH_BLOCKSIZE; i++)
//    {
//        ramBuf[i] = FLASH_ReadWord((blockStartAddr+i));
//    }
//
//    // Write at offset
//    ramBuf[offset] = word;
//
//    // Writes ramBuf to current block
//    FLASH_WriteBlock(blockStartAddr, ramBuf);
//}

int8_t FLASH_WriteBlock(uint16_t writeAddr, uint16_t *flashWordArray)
{
    uint16_t    blockStartAddr  = (uint16_t )(writeAddr & ((END_FLASH-1) ^ (ERASE_FLASH_BLOCKSIZE-1)));
    uint8_t     GIEBitValue = INTCONbits.GIE;   // Save interrupt enable
    uint8_t i;

    // Flash write must start at the beginning of a row
    if( writeAddr != blockStartAddr )
    {
        return -1;
    }

    INTCONbits.GIE = 0;         // Disable interrupts

    // Block erase sequence
    FLASH_EraseBlock(writeAddr);

    // Block write sequence
    PMCON1bits.CFGS = 0;    // Deselect Configuration space
    PMCON1bits.WREN = 1;    // Enable wrties
    PMCON1bits.LWLO = 1;    // Only load write latches

    for (i=0; i<WRITE_FLASH_BLOCKSIZE; i++)
    {
        // Load lower 8 bits of write address
        PMADRL = (writeAddr & 0xFF);
        // Load upper 6 bits of write address
        PMADRH = ((writeAddr & 0xFF00) >> 8);

	// Load data in current address
        PMDATL = (uint8_t)flashWordArray[i];
        PMDATH = ((flashWordArray[i] & 0xFF00) >> 8);

        if(i == (WRITE_FLASH_BLOCKSIZE-1))
        {
            // Start Flash program memory write
            PMCON1bits.LWLO = 0;
        }

        PMCON2 = 0x55;
        PMCON2 = 0xAA;
        PMCON1bits.WR = 1;
        NOP();
        NOP();

	    writeAddr++;
    }

    PMCON1bits.WREN = 0;       // Disable writes
    INTCONbits.GIE = GIEBitValue;   // Restore interrupt enable

    return 0;
}

void FLASH_EraseBlock(uint16_t startAddr)
{
    uint8_t GIEBitValue = INTCONbits.GIE;   // Save interrupt enable


    INTCONbits.GIE = 0; // Disable interrupts
    // Load lower 8 bits of erase address boundary
    PMADRL = (startAddr & 0xFF);
    // Load upper 6 bits of erase address boundary
    PMADRH = ((startAddr & 0xFF00) >> 8);

    // Block erase sequence
    PMCON1bits.CFGS = 0;    // Deselect Configuration space
    PMCON1bits.FREE = 1;    // Specify an erase operation
    PMCON1bits.WREN = 1;    // Allows erase cycles

    // Start of required sequence to initiate erase
    PMCON2 = 0x55;
    PMCON2 = 0xAA;
    PMCON1bits.WR = 1;      // Set WR bit to begin erase
    NOP();
    NOP();

    PMCON1bits.WREN = 0;       // Disable writes
    INTCONbits.GIE = GIEBitValue;	// Restore interrupt enable
}
/**
 End of File
*/
