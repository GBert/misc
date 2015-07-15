/*
 * Copyright (C) 2005-2015 Darron Broad
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

#ifndef _IO_H
#define _IO_H

/* I/O bit rules */
#define PGD_OUT_FLIP  (0x0001)	/* invert pgd o/p */
#define PGC_OUT_FLIP  (0x0002)	/* invert pgc o/p */
#define VPP_OUT_FLIP  (0x0004)	/* invert vpp o/p */
#define PGD_IN_FLIP   (0x0008)	/* invert pgd i/p */
#define PGD_IN_PULLUP (0x0010)	/* pgd o/p high for pgd i/p */
#define PGM_OUT_FLIP  (0x0020)	/* invert pgm o/p */
#define VPP_OUT_CLOCK (0x0040)	/* vpp clocks a T-type flip-flop */
/* GPIO bit rules */
#define PGD_RELEASE   (0x0100)	/* pgd released on exit */
#define PGC_RELEASE   (0x0200)	/* pgc released on exit */
#define PGM_RELEASE   (0x0400)	/* pgm released on exit */
#define VPP_RELEASE   (0x0800)	/* vpp released on exit */
#define VPP_RUN       (0x1000)	/* vpp high on exit if not released */
#define BB_LOCK       (0x2000)	/* gpio-bb shift with lock */
#define ALT_RELEASE   (0x4000)	/* re-enable ALT function if released */

/* I/O backends */
#define IONONE  (0)
#define IOTTY   (1)     /* TTY/TTYUSB                     */
#define IORPI   (2)     /* RPI GPIO DIRECT/VELLEMAN K8048 */
#define IOI2C   (3)     /* MCP23017 I2C                   */
#define IOBB    (4)     /* LINUX BIT-BANG DRIVER          */

/* Default GPIO pins */
#define GPIO_PGM_DISABLED (255)
#define GPIO_VPP  (9)
#define GPIO_PGC  (10)
#define GPIO_PGDO (11)
#define GPIO_PGDI (11)
#define GPIO_PGM  (GPIO_PGM_DISABLED)

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
void io_signal();
void io_signal_on();
void io_signal_off();
void io_config(void);
int io_open(void);
void io_release(void);
void io_close(int);
void io_exit(int);
char *io_fault(int);
char *io_error(void);
void io_usleep(uint32_t);
void io_set_pgm(uint8_t);
void io_set_vpp(uint8_t);
void io_set_pgd(uint8_t);
void io_set_pgc(uint8_t);
uint8_t io_get_pgd(void);
void io_configure(uint8_t);
void io_data_input(void);
uint32_t io_clock_in_bits(uint32_t, uint32_t, uint8_t);
void io_data_output(uint8_t);
void io_clock_out_bits(uint32_t, uint32_t, uint32_t, uint8_t);
uint8_t io_clock_bit_4phase(uint8_t, uint8_t);
uint32_t io_clock_bits_4phase(uint8_t, uint32_t, uint32_t);
uint32_t io_program_in(uint8_t);
void io_program_out(uint32_t, uint8_t);
void io_program_feedback(char);
void io_test0(int, int);
void io_test1(int);
void io_test2(int);
void io_test3(int);
void io_test4(int);
void io_test5(int);
int io_test_out(int, int, uint8_t);
int io_test_in(int, int, uint8_t *);
char *io_test_err(int);
int io_test_command(int, int, uint8_t *, int, uint32_t *, int);
int io_test_switch(int);
int io_test_lasterror(int);

#endif /* !_IO_H */
