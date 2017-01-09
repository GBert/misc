/*
 * Copyright (C) 2005-2017 Darron Broad
 * All rights reserved.
 * 
 * This file is part of Pickle Microchip PIC ICSP.
 * 
 * Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation. 
 * 
 * Pickle Microchip PIC ICSP is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along
 * with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
 */

#ifndef _ICSPIO_H
#define _ICSPIO_H

/******************************************************************************
 * ICSP I/O
 */

/* command i/o */
#define RESYNC (256)
#define EOT (0x04)
#define ACK (0x06)
#define NAK (0x15)

/*
 * CONSTANT CMD_LED        = 0x01  ; 0x00..0x3F        SET K8048 LEDS
 * CONSTANT CMD_SWITCH     = 0x02  ;                   GET K8048 SWITCHES
 * CONSTANT CMD_SLEEP      = 0x10  ;                   SLEEP UNTIL WATCHDOG TIME-OUT
 * CONSTANT CMD_WATCHDOG   = 0x11  ; 1|0               SET WATCHDOG ENABLE/DISABLE
 * CONSTANT CMD_CLOCK      = 0x12  ; 0..7              SET INTERNAL RC CLOCK DIVIDER
 * CONSTANT CMD_DIRECTION  = 0x20  ; 0..5 0x00..0xFF   SET PORT DATA DIRECTION
 * CONSTANT CMD_OUTPUT     = 0x21  ; 0..5 0x00..0xFF   SET PORT DATA OUTPUT
 * CONSTANT CMD_INPUT      = 0x22  ; 0..5              GET PORT DATA INPUT
 * CONSTANT CMD_ANALOG     = 0x30  ; 0..N|0xFF         SET ANALOG CHANNEL ENABLE/DISABLE
 * CONSTANT CMD_SAMPLE     = 0x31  ;                   GET ANALOG CHANNEL INPUT
 * CONSTANT CMD_VREF       = 0x38  ; 0..15|0xFF        SET VREF VOLTAGE LEVEL OR DISABLE
 * CONSTANT CMD_EEREAD     = 0x40  ; ADDRESS           READ DATA EEPROM
 * CONSTANT CMD_EEWRITE    = 0x41  ; ADDRESS DATA      WRITE DATA EEPROM
 * CONSTANT CMD_READ       = 0x50  ; ADDRESH ADDRESL   READ PROGRAM FLASH
 * CONSTANT CMD_WRITE      = 0x51  ; 		       UNIMPLIMENTED
 * CONSTANT CMD_ARG8       = 0xF0  ; 0..0xFF           SEND AN 8-BIT ARG & GET AN 8-BIT ANSWER
 * CONSTANT CMD_ARG16      = 0xF1  ; 0..0xFFFF         SEND A 16-BIT ARG & GET AN 8-BIT ANSWER
 * CONSTANT CMD_ARG24      = 0xF2  ; 0..0xFFFFFF       SEND A 24-BIT ARG & GET AN 8-BIT ANSWER
 * CONSTANT CMD_ARG32      = 0xF3  ; 0..0xFFFFFFFF     SEND A 32-BIT ARG & GET AN 8-BIT ANSWER
 * CONSTANT CMD_TEST       = 0xFE  ; 0..0xFF           SEND AN 8-BIT TEST ARG & GET NO REPLY
 * CONSTANT CMD_ERROR      = 0xFF  ;                   GET LAST FIRMWARE ERROR
 */
#define CMD_LED       (0x01)
#define CMD_SWITCH    (0x02)
#define CMD_SLEEP     (0x10)
#define CMD_WATCHDOG  (0x11)
#define CMD_CLOCK     (0x12)
#define CMD_DIRECTION (0x20)
#define CMD_OUTPUT    (0x21)
#define CMD_INPUT     (0x22)
#define CMD_ANALOG    (0x30)
#define CMD_SAMPLE    (0x31)
#define CMD_VREF      (0x38)
#define CMD_EEREAD    (0x40)
#define CMD_EEWRITE   (0x41)
#define CMD_READ      (0x50)
#define CMD_WRITE     (0x51)
#define CMD_ARG8      (0xF0)
#define CMD_ARG16     (0xF1)
#define CMD_ARG24     (0xF2)
#define CMD_ARG32     (0xF3)
#define CMD_TEST      (0xFE)
#define CMD_ERROR     (0xFF)

/* Analog sample delay */
#define FWSAMPLE  (200)
/* EEPROM write delay */
#define FWEEPROM  (200)

/*
 * CONSTANT ERRNONE     = 0    ; OK
 * CONSTANT ERRTIMEOUT  = 1    ; CLOCK TIMED OUT
 * CONSTANT ERRPROTOCOL = 2    ; INVALID STOP OR START BIT
 * CONSTANT ERRPARITY   = 3    ; INVALID PARITY BIT
 * CONSTANT ERRNOTSUP   = 4    ; COMMAND NOT SUPPORTED
 * CONSTANT ERRINVALID  = 5    ; INVALID RESPONSE
 */
#define ERRNONE     (0)
#define ERRTIMEOUT  (1)
#define ERRPROTOCOL (2)
#define ERRPARITY   (3)
#define ERRNOTSUP   (4)
#define ERRINVALID  (5)

/*****************************************************************************/

/* prototypes */
int icspio_out(int, int, uint8_t);
int icspio_in(int, int, uint8_t *);
char *icspio_err(int);
int icspio_command(int, int, uint8_t *, int, uint32_t *, int);
int icspio_switch(int);
int icspio_lasterror(int);

#endif /* !_ICSPIO_H */
