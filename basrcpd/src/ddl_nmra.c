/* +----------------------------------------------------------------------+ */
/* | DDL - Digital Direct for Linux                                       | */
/* +----------------------------------------------------------------------+ */
/* | Copyright (c) 2002 - 2003 Vogt IT                                    | */
/* +----------------------------------------------------------------------+ */
/* | This source file is subject of the GNU general public license 2,     | */
/* | that is bundled with this package in the file COPYING, and is        | */
/* | available at through the world-wide-web at                           | */
/* | http://www.gnu.org/licenses/gpl.txt                                  | */
/* | If you did not receive a copy of the PHP license and are unable to   | */
/* | obtain it through the world-wide-web, please send a note to          | */
/* | gpl-license@vogt-it.com so we can mail you a copy immediately.       | */
/* +----------------------------------------------------------------------+ */
/* | Authors: Torsten Vogt vogt@vogt-it.com                               | */
/* |          For more read ChangeLog file                                | */
/* +----------------------------------------------------------------------+ */

/***************************************************************/
/* erddcd - Electric Railroad Direct Digital Command Daemon    */
/*    generates without any other hardware digital commands    */
/*    to control electric model railroads                      */
/*                                                             */
/* file: nmra.c                                                */
/* job : implements routines to compute data for the           */
/*       NMRA-DCC protocol and send this data to               */
/*       the serial device.                                    */
/*                                                             */
/* Torsten Vogt, may 1999                                      */
/*                                                             */
/* last changes: Torsten Vogt, September 2000                  */
/*               Torsten Vogt, January 2001                    */
/*               Torsten Vogt, April 2001                      */
/*               For more read ChangeLog file                  */
/***************************************************************/

/**********************************************************************
 data format:

 look at the srcp specification

 protocol formats:

 N: NMRA multi function decoder with 7/14-bit address, 14/28/128 speed steps
 (implemented)

 NA: accessory digital decoders
 (implemented)

 service mode instruction packets for direct mode
 (verify cv, write cv, cv bit manipulation)
 (implemented)

 service mode instruction packets for PoM mode
 (verify cv, write cv, cv bit manipulation)
 (only write implemented)

 service mode instruction packets for address-only mode
 (verify address contents, write address contents)
 (implemented)

 service mode instruction packets for physical register addressing
 (verify register contents, write register contents)
 (implemented)

 service mode instruction packets paged cv addressing
 (verify/write register/cv contents)
 (implemented)

****************************************************************/

#include <time.h>

#include "ddl.h"
#include "ddl_nmra.h"
#include "syslogmessage.h"

static char *preamble = "111111111111111";

/* 230 is needed for all functions F0-F28 */
static const unsigned int BUFFERSIZE = 256;

/* internal offset of the long addresses */
static const unsigned int ADDR14BIT_OFFSET = 128;

/**
 * Erzeugung Paket für SPI Ausgabe. Was viel einfacher ist:
 * Aus Effizenzgründen kodieren wir hier bitweise, Konvertierung 
 * auf tatsächlich zu sendende SPI Bytes erfolgt dann direkt beim senden.
 * (ein 0 Bit wird dann 2 Byte lang).
 * Damit da bekannt ist, wieviele Bits gesendet werden müssen, wird diese
 * Anzahl im 1. Packetstream Byte übergeben.
 * Retunwert ist die Anzahl Byte, also erstes Löngenbyte plus alle notwendigen Datenbytes.
 */
int translateBitstream2Packetstream(bus_t busnumber, char *Bitstream,
                                    char *Packetstream)
{
  int i;
  int bitLen = strlen(Bitstream);
  if (bitLen > 255) {
    //Paket zu Gross, Abbruch
    return 0;
  }
  memset(Packetstream, 0, PKTSIZE);
  //Länge
  Packetstream[0] = (char)bitLen;
  for (i=0; i<bitLen; i++) {
    if (Bitstream[i] == '1') {
      Packetstream[(i / 8) + 1] |= 1 << (i % 8);
    }
  }
  return (bitLen / 8) + 2; //Inkl. Längenbyte
}


/*** Some useful functions to calculate NMRA-DCC bytes (char arrays) ***/

/**
  Transform the lower 8 bit of the input into a "bitstream" byte
  @par Input: int value
  @par Output: char* byte
*/
static void calc_single_byte(char *byte, int value)
{
    int i;
    int bit = 0x1;

    strncpy(byte, "00000000", 9);
    byte[8] = 0;

    for (i = 7; i >= 0; i--) {
        if (value & bit)
            byte[i] = '1';
        bit <<= 1;
    }
}

/* calculating address bytes: 11AAAAAA AAAAAAAA */
static void calc_14bit_address_byte(char *byte1, char *byte2, int address)
{
    calc_single_byte(byte2, address);
    calc_single_byte(byte1, 0xc0 | (address >> 8));
}

/* calculating speed byte2: 01DUSSSS  */
static void calc_baseline_speed_byte(char *byte, int direction, int speed,
                                     int func)
{
    calc_single_byte(byte, 0x40 | (direction << 5) | speed);
    if (func & 1)
        byte[3] = '1';
}

/* calculating speed byte: 01DSSSSS */
static void calc_28spst_speed_byte(char *byte, int direction, int speed)
{
    /* last significant speed bit is at pos 3 */

    if (speed > 1) {
        speed += 2;
        calc_single_byte(byte, 0x40 | (direction << 5) | (speed >> 1));
        if (speed & 1) {
            byte[3] = '1';
        }
    }
    else {
        calc_single_byte(byte, 0x40 | (direction << 5) | speed);
    }
}

/* calculating function byte: 100DDDDD */
static void calc_function_group_one_byte(char *byte, int func)
{
    /* mask out lower 5 function bits */
    func &= 0x1f;
    calc_single_byte(byte, 0x80 | (func >> 1));

    /* F0 or FL is out of order */
    if (func & 1)
        byte[3] = '1';
}

/* calculating function byte: 101SDDDD */
static void calc_function_group_two_byte(char *byte, int func, int lower)
{
    if (lower) {
        /* shift func bits to lower 4 bits and mask them */
        func = (func >> 5) & 0xf;
        /* set command for F5 to F8 */
        func |= 0xb0;
    }
    else {
        func = (func >> 9) & 0xf;
        func |= 0xa0;
    }
    calc_single_byte(byte, func);
}

static void calc_128spst_adv_op_bytes(char *byte1, char *byte2,
                                      int direction, int speed)
{
    strcpy(byte1, "00111111");
    calc_single_byte(byte2, speed);
    if (direction == 1)
        byte2[0] = '1';
}

static void xor_two_bytes(char *byte, char *byte1, char *byte2)
{

    int i;

    for (i = 0; i < 8; i++) {
        if (byte1[i] == byte2[i])
            byte[i] = '0';
        else
            byte[i] = '1';
    }
    byte[8] = 0;
}

/*** functions to generate NMRA-DCC data packets ***/

int comp_nmra_accessory(bus_t busnumber, int nr, int output, int activate,
                        int offset)
{
    /* command: NA <nr [0001-2044]> <outp [0,1]> <activate [0,1]>
       example: NA 0012 0 1  */

    char byte1[9];
    char byte2[9];
    char byte3[9];
    char bitstream[BUFFERSIZE];
    char packetstream[PKTSIZE];
//    char *p_packetstream;

    int address = 0;            /* of the decoder                */
    int pairnr = 0;             /* decoders have pair of outputs */

    int j;

    syslog_bus(busnumber, DBG_DEBUG,
               "command for NMRA protocol for accessory decoders "
               "(NA) received");

    /* no special error handling, it's job of the clients */
    if (nr < 1 || nr > 2044 || output < 0 || output > 1 ||
        activate < 0 || activate > 1)
        return 1;

    /* packet is not available */
//    p_packetstream = packetstream;

    /* calculate the real address of the decoder and the pair number 
     * of the switch */
    /* valid decoder addresses: 0..511 */
    address = ((nr - 1) / 4) + offset;
    pairnr = (nr - 1) % 4;

    /* address byte: 10AAAAAA (lower 6 bits) */
    calc_single_byte(byte1, 0x80 | (address & 0x3f));

    /* address and data 1AAACDDO upper 3 address bits are inverted */
    /* C =  activate, DD = pairnr */
    calc_single_byte(byte2,
                     0x80 | ((~address) & 0x1c0) >> 2 | activate << 3 |
                     pairnr << 1 | output);
    xor_two_bytes(byte3, byte2, byte1);

    /* putting all together in a 'bitstream' (char array) */
    memset(bitstream, 0, BUFFERSIZE);
    strcat(bitstream, preamble);
    strcat(bitstream, "0");
    strcat(bitstream, byte1);
    strcat(bitstream, "0");
    strcat(bitstream, byte2);
    strcat(bitstream, "0");
    strcat(bitstream, byte3);
    strcat(bitstream, "1");

    j = translateBitstream2Packetstream(busnumber, bitstream, packetstream);
    if (j > 0) {
    	send_packet(busnumber, packetstream, j, QNBACCPKT, 2);    
        return 0;
    }

    return 1;
}

/**
  Calculate up to 4 command sequences depending on the number of possible 
  functions (taken from INIT <BUS> GL ...) for up to 28 Functions
  due to the long bitstream in case of 28 functions the bitstream is
  split into to parts.
  @par Input: char *addrerrbyte Error detection code for address bytes(s)
              char *addrstream "bitstream" for preamble and the address byte(s)
              int func function bits
              int nfuncs number of functions
  @par Output: char* bitstream the resulting "bitstream"
  @par Output: char* bitstream2 the second  "bitstream"
*/
static void calc_function_stream(char *bitstream, char *bitstream2,
                                 char *addrerrbyte, char *addrstream,
                                 int func, int nfuncs)
{
    char funcbyte[9];
    char errdbyte[9];
    /* between two commands to the same address there has to be a gap
       of at least 5ms. 5ms are 10 packet bytes. one packet byte can take
       5 logical "1" (0x55) ==> 50 logical "1" are needed between 2 packets.
       The preamble holds 15 "1"s, so only 35 additional are needed
     */
    char wait[40] = "11111111111111111111111111111111111";

    calc_function_group_one_byte(funcbyte, func);
    xor_two_bytes(errdbyte, addrerrbyte, funcbyte);

    /* putting all together in a 'bitstream' (char array) (functions) */
    memset(bitstream2, 0, BUFFERSIZE);
    strcat(bitstream2, addrstream);
    strcat(bitstream2, funcbyte);
    strcat(bitstream2, "0");
    strcat(bitstream2, errdbyte);
    strcat(bitstream2, "1");
    if (nfuncs > 5) {
        calc_function_group_two_byte(funcbyte, func, true);
        xor_two_bytes(errdbyte, addrerrbyte, funcbyte);
        strcat(bitstream, wait);
        strcat(bitstream, addrstream);
        strcat(bitstream, funcbyte);
        strcat(bitstream, "0");
        strcat(bitstream, errdbyte);
        strcat(bitstream, "1");

        if (nfuncs > 8) {
            calc_function_group_two_byte(funcbyte, func, false);
            xor_two_bytes(errdbyte, addrerrbyte, funcbyte);
            strcat(bitstream2, wait);
            strcat(bitstream2, addrstream);
            strcat(bitstream2, funcbyte);
            strcat(bitstream2, "0");
            strcat(bitstream2, errdbyte);
            strcat(bitstream2, "1");
            if (nfuncs > 12) {
                char funcbyte2[9];
                strncpy(funcbyte2, "11011110", 9);
                funcbyte2[8] = 0;
                calc_single_byte(funcbyte, func >> 13);
                xor_two_bytes(errdbyte, addrerrbyte, funcbyte2);
                xor_two_bytes(errdbyte, errdbyte, funcbyte);
                strcat(bitstream, wait);
                strcat(bitstream, addrstream);
                strcat(bitstream, funcbyte2);
                strcat(bitstream, "0");
                strcat(bitstream, funcbyte);
                strcat(bitstream, "0");
                strcat(bitstream, errdbyte);
                strcat(bitstream, "1");
                if (nfuncs > 20) {
                    funcbyte2[7] = '1';
                    xor_two_bytes(errdbyte, addrerrbyte, funcbyte2);
                    calc_single_byte(funcbyte, func >> 21);
                    xor_two_bytes(errdbyte, errdbyte, funcbyte);
                    strcat(bitstream2, wait);
                    strcat(bitstream2, addrstream);
                    strcat(bitstream2, funcbyte2);
                    strcat(bitstream2, "0");
                    strcat(bitstream2, funcbyte);
                    strcat(bitstream2, "0");
                    strcat(bitstream2, errdbyte);
                    strcat(bitstream2, "1");
                }
            }
        }
    }
}

/**
  Calculate the "bitstream" for the cv programming sequence
  @par Input: char *addrerrbyte error detection byte of the address byte(s)
              int cv
              int value
              int verify
              int pom  if true generate PoM command
  @par Output: char* progstream the resulting "bitstream" for the
               program sequence
*/
static void calc_byte_program_stream(char *progstream, char *addrerrbyte,
                                     int cv, int value, int verify,
                                     int pom)
{
    char byte2[9];
    char byte3[9];
    char byte4[9];
    char byte5[9];

    memset(progstream, 0, BUFFERSIZE);
    /* calculating byte3: AAAAAAAA (rest of CV#) */
    calc_single_byte(byte3, cv);

    if (pom) {
        /* calculating byte2: 1110C1AA (instruction byte1) */
        calc_single_byte(byte2, 0xec | (cv >> 8));
    }
    else {
        /* calculating byte2: 011110AA (instruction byte1) */
        calc_single_byte(byte2, 0x7c | (cv >> 8));
    }
    if (verify) {
        byte2[4] = '0';
    }

    /* calculating byte4: DDDDDDDD (data) */
    calc_single_byte(byte4, value);

    /* calculating byte5: EEEEEEEE (error detection byte) */
    xor_two_bytes(byte5, addrerrbyte, byte2);
    xor_two_bytes(byte5, byte5, byte3);
    xor_two_bytes(byte5, byte5, byte4);

    strcat(progstream, byte2);
    strcat(progstream, "0");
    strcat(progstream, byte3);
    strcat(progstream, "0");
    strcat(progstream, byte4);
    strcat(progstream, "0");
    strcat(progstream, byte5);
    strcat(progstream, "1");
}

/**
  Calculate the "bitstream" for the cvbit programming sequence
  @par Input: char *addrerrbyte error detection byte of the address byte(s)
              int cv
              int value
              int verify
              int pom  if true generrate PoM command
  @par Output: char* progstream the resulting "bitstream" for the
               program sequence
*/
static void calc_bit_program_stream(char *progstream, char *addrerrbyte,
                                    int cv, int bit, int value, int verify,
                                    int pom)
{
    char byte2[9];
    char byte3[9];
    char byte4[9];
    char byte5[9];

    memset(progstream, 0, BUFFERSIZE);
    /* calculating byte3: AAAAAAAA (rest of CV#) */
    calc_single_byte(byte3, cv);

    if (pom) {
        /* calculating byte2: 111010AA (instruction byte1) */
        calc_single_byte(byte2, 0xe8 | (cv >> 8));
    }
    else {
        /* calculating byte2: 011110AA (instruction byte1) */
        calc_single_byte(byte2, 0x78 | (cv >> 8));
    }

    /* calculating byte4: 111CDBBB (data) */
    calc_single_byte(byte4, 0xf0 | (value << 3) | bit);
    if (verify) {
        byte4[3] = '0';
    }

    /* calculating byte5: EEEEEEEE (error detection byte) */
    xor_two_bytes(byte5, addrerrbyte, byte2);
    xor_two_bytes(byte5, byte5, byte3);
    xor_two_bytes(byte5, byte5, byte4);

    /* putting all together in a 'bitstream' (char array) */
    strcat(progstream, byte2);
    strcat(progstream, "0");
    strcat(progstream, byte3);
    strcat(progstream, "0");
    strcat(progstream, byte4);
    strcat(progstream, "0");
    strcat(progstream, byte5);
    strcat(progstream, "1");
}

/**
  Calculate the "bitstream" for 7 and 14 bit addresses
  @par Input: int address
              int mode  1 = 7 bit, 2 = 14 bit
  
  @par Output: char* addrstream the resulting "bitstream" for address byte(s)
               char* addrerrbyte the "bitstream" for error detection byte
*/
static void calc_address_stream(char *addrstream, char *addrerrbyte,
                                int address, int mode)
{
    char addrbyte[9];
    char addrbyte2[9];
    if (mode == 1) {
        /* calc 7 bit address - leading bit is zero */
        calc_single_byte(addrbyte, address & 0x7f);
        /* no second byte => error detection byte = addressbyte */
        memcpy(addrerrbyte, addrbyte, 9);

    }
    else {
        calc_14bit_address_byte(addrbyte, addrbyte2, address);
        xor_two_bytes(addrerrbyte, addrbyte, addrbyte2);
    }

    /* putting all together in a 'bitstream' (char array) (speed & direction) */
    memset(addrstream, 0, BUFFERSIZE);
    strcat(addrstream, preamble);
    strcat(addrstream, "0");
    strcat(addrstream, addrbyte);
    strcat(addrstream, "0");
    if (mode == 2) {
        strcat(addrstream, addrbyte2);
        strcat(addrstream, "0");
    }
}

/**
  Generate the packet for NMRA (multi)-function-decoder with 7-bit or 14-bit
  address and 14/28 or 128 speed steps and up to 28 functions
  @par Input: bus_t busnumber
              int address GL address
              int direction
              int speed
              int func function bits
              int nspeed number of speeds for this decoder
              int nfuncs number of functions
              int mode 1 == short address, 2 == long (2byte) address
  @return 0 == OK, 1 == Error
*/
void comp_nmra_multi_func(bus_t busnumber, gl_data_t *glp)
{
    char spdrbyte[9];
    char spdrbyte2[9];
    char errdbyte[9];
    char addrerrbyte[9];
    char addrstream[BUFFERSIZE];
    char bitstream[BUFFERSIZE];
    char bitstream2[BUFFERSIZE];
    char packetstream[PKTSIZE];
    char packetstream2_buf[PKTSIZE];
    char *packetstream2 = packetstream2_buf;

    int adr = 0;
    int j, jj;

	int mode = glp->protocolversion;
    int address = glp->id;
    int speed = glp->speed;
    int direction = glp->direction;
    uint32_t func = glp->funcs;
    uint8_t nspeed = glp->n_fs;
    uint8_t	nfuncs = glp->n_func;

    if (glp->speedchange & SCEMERG) {   // Emergency Stop
        speed = 1;
        direction = glp->cacheddirection;
        glp->speedchange &= ~SCEMERG;
    }
    else if (speed) speed++;        	// Never send FS1

  	if (speed > 127) speed = 127;
    glp->speedchange &= ~(SCSPEED | SCDIREC);   // handled now
    
    syslog_bus(busnumber, DBG_DEBUG,
               "command for NMRA protocol (N%d) received addr:%d "
               "dir:%d speed:%d nspeeds:%d nfunc:%d funcs %x",
               mode, address, direction, speed, nspeed, nfuncs,func);

    adr = address;

    if (mode == 2) {
        adr += ADDR14BIT_OFFSET;
    }
    /* no special error handling, it's job of the clients */
    if (address < 1 || address > 10239 || direction < 0 || direction > 1 ||
        speed < 0 || speed > (nspeed + 1) || (address > 127 && mode == 1))
        return;

    calc_address_stream(addrstream, addrerrbyte, address, mode);
    if (speed < 2 || nspeed < 15) {
        /* commands for stop and emergency stop are identical for
           14 and 28 speed steps. All decoders supporting 128
           speed steps also have to support 14/28 speed step commands
           ==> results in shorter refresh cycle if there are many 128
           speed steps decoders with speed=0, and a slightly faster 
           emergency stop for these decoders.
         */
        calc_baseline_speed_byte(spdrbyte, direction, speed, func);
    }
    else {
        if (nspeed > 29) {
            calc_128spst_adv_op_bytes(spdrbyte, spdrbyte2, direction,
                                      speed);
        }
        else {
            calc_28spst_speed_byte(spdrbyte, direction, speed);
        }
    }

    xor_two_bytes(errdbyte, addrerrbyte, spdrbyte);

    memset(bitstream, 0, BUFFERSIZE);
    strcat(bitstream, addrstream);
    strcat(bitstream, spdrbyte);
    strcat(bitstream, "0");
    if (nspeed > 29 && speed > 1) {
        strcat(bitstream, spdrbyte2);
        strcat(bitstream, "0");
        xor_two_bytes(errdbyte, errdbyte, spdrbyte2);
    }
    strcat(bitstream, errdbyte);
    strcat(bitstream, "1");

    if (nfuncs && (nspeed > 14)) {
        calc_function_stream(bitstream, bitstream2, addrerrbyte,
                             addrstream, func, nfuncs);
        j = translateBitstream2Packetstream(busnumber, bitstream, packetstream);
        jj = translateBitstream2Packetstream(busnumber, bitstream2, packetstream2);
    }
    else {
        j = translateBitstream2Packetstream(busnumber, bitstream, packetstream);
        packetstream2 = packetstream;
        jj = j;
    }
    if (j > 0 && jj > 0) {
        update_NMRAPacketPool(busnumber, adr, packetstream, j,
                              packetstream2, jj);
    	send_packet(busnumber, packetstream, j, QNBLOCOPKT, 2);    
        if (nfuncs && (nspeed > 14)) {
    		send_packet(busnumber, packetstream, jj, QNBLOCOPKT, 2);    
        }
    }

    return;
}

/**
  Write a configuration variable (cv) on the Main (operations mode
  programming). This is very similar to the service mode programming with
  the extension, that the decoder address is used. I.e. only the cv of
  the selected decoder is updated not all decoders.
  @par Input: bus_t busnumber
              int address
              int cv
              int value
              int mode 1 == short address, 2 == long (2byte) address
  @return ack 
*/
int protocol_nmra_sm_write_cvbyte_pom(bus_t busnumber, int address, int cv,
                                      int value, int mode)
{
    char addrerrbyte[9];
    char addrstream[BUFFERSIZE];
    char progstream[BUFFERSIZE];
    char bitstream[BUFFERSIZE];
    char packetstream[PKTSIZE];
    int j;
    int adr = 0;

    syslog_bus(busnumber, DBG_DEBUG,
               "WR Byte command for PoM %d received addr:%d CV:%d value:%d",
               mode, address, cv, value);
    cv -= 1;
    /* do not allow to change the address on the main ==> cv 1 is disabled */
    if (address < 1 || address > 10239 || cv < 1 || cv > 1023 ||
        value < 0 || value > 255 || (address > 127 && mode == 1))
        return -1;

    adr = address;
    if (mode == 2) {
        adr += ADDR14BIT_OFFSET;
    }

    calc_address_stream(addrstream, addrerrbyte, address, mode);
    calc_byte_program_stream(progstream, addrerrbyte, cv, value, false,
                             true);
    memset(bitstream, 0, BUFFERSIZE);
    strcat(bitstream, addrstream);
    strcat(bitstream, progstream);

    j = translateBitstream2Packetstream(busnumber, bitstream, packetstream);

    if (j > 0) {
    	send_packet(busnumber, packetstream, j, QNBLOCOPKT, 3);    
        return value;
    }
    return -1;
}

/**
  Write a single bit of a configuration variable (cv) on the Main.
  This is very similar to the service mode programming with the extension,
  that the decoder address is used. I.e. only the cv of the selected 
  decoder is updated not all decoders.
  @par Input: bus_t busnumber
              int address
              int cv
              int value
              int mode 1 == short address, 2 == long (2byte) address
  @return ack 
*/
int protocol_nmra_sm_write_cvbit_pom(bus_t busnumber, int address, int cv,
                                     int bit, int value, int mode)
{
    char addrerrbyte[9];
    char addrstream[BUFFERSIZE];
    char progstream[BUFFERSIZE];
    char bitstream[BUFFERSIZE];
    char packetstream[PKTSIZE];
    int j;
    int adr = 0;

    syslog_bus(busnumber, DBG_DEBUG,
               "WR Bit command for PoM %d received addr:%d CV:%d bit:%d value:%d",
               mode, address, cv, bit, value);
    cv -= 1;
    /* do not allow to change the address on the main ==> cv 1 is disabled */
    if (address < 1 || address > 10239 || cv < 1 || cv > 1023
        || bit < 0 || bit > 7 || value < 0 || value > 1
        || (address > 127 && mode == 1))
        return -1;

    adr = address;
    if (mode == 2) {
        adr += ADDR14BIT_OFFSET;
    }

    calc_address_stream(addrstream, addrerrbyte, address, mode);
    calc_bit_program_stream(progstream, addrerrbyte, cv, bit, value, false,
                            true);
    memset(bitstream, 0, BUFFERSIZE);
    strcat(bitstream, addrstream);
    strcat(bitstream, progstream);

    j = translateBitstream2Packetstream(busnumber, bitstream, packetstream);

    if (j > 0) {
    	send_packet(busnumber, packetstream, j, QNBLOCOPKT, 3);    
        return value;
    }
    return -1;
}

/**
 * The following function(s) supports the implementation of NMRA-
 * programmers. It is recommended to use a programming track to  
 * program your locos. In every case it is useful to stop the   
 * refresh-cycle on the track when using one of the following    
 * service mode functions.
 **/

static int sm_initialized = false;

static char resetstream[PKTSIZE];
static int rs_size = 0;
static char idlestream[PKTSIZE];
static int is_size = 0;
static char pagepresetstream[PKTSIZE];
static int ps_size = 0;

static char *longpreamble = "111111111111111111111111111111";
static char reset_packet[] =
    "1111111111111111111111111111110000000000000000000000000001";
static char page_preset_packet[] =
    "1111111111111111111111111111110011111010000000010011111001";
static char idle_packet[] =
    "1111111111111111111111111111110111111110000000000111111111";

static void sm_init(bus_t busnumber)
{
    memset(resetstream, 0, PKTSIZE);
    rs_size =
        translateBitstream2Packetstream(busnumber, reset_packet, resetstream);
    memset(idlestream, 0, PKTSIZE);
    is_size =
        translateBitstream2Packetstream(busnumber, idle_packet, idlestream);
    memset(pagepresetstream, 0, PKTSIZE);
    ps_size =
        translateBitstream2Packetstream(busnumber, page_preset_packet, pagepresetstream);
    sm_initialized = true;
}

#if 0
// TODO: obsolete code for UART to be replaced by code for SPI
/**
  Check the serial line for an acknowledgment (ack)
  @par Input: bus_t busnumber
  @return ack 
*/
static int scanACK(bus_t busnumber)
{
    int result, arg;
    result = ioctl(buses[busnumber].device.file.fd, TIOCMGET, &arg);
    if (result == -1) {
        syslog_bus(busnumber, DBG_ERROR,
                   "ioctl() failed: %s (errno = %d)\n",
                   strerror(errno), errno);
    }

    if ((result >= 0) && (!(arg & TIOCM_RI)))
        return 1;
    return 0;
}

/**
  Wait for the UART to write the packetstream
  check in the meantime if an ack is set
  @par Input: bus_t busnumber
  @return ack 
*/
static int waitUARTempty_scanACK(bus_t busnumber)
{
    int value = 0;
    int result = 0;
    int ack = 0;
    clock_t start = clock();
    do {                        /* wait until UART is empty */
        if (scanACK(busnumber))
            ack = 1;            /* scan ACK */
        /* prevent endless loop in case somone turned the power on */
        if (buses[busnumber].power_state == 1) {
            clock_t now = clock();
            float diff = (now - start);
            diff /= CLOCKS_PER_SEC;
            if (ack) {
                value = 1;
//                waitUARTempty(busnumber);
            }
            /* wait 300ms */
            if (diff > 0.3) {
//                waitUARTempty(busnumber);
                value = 1;
            }
        }
        else {
#if linux
            result =
                ioctl(buses[busnumber].device.file.fd, TIOCSERGETLSR,
                      &value);
#else
            result =
                ioctl(buses[busnumber].device.file.fd, TCSADRAIN, &value);
#endif
        }
        if (result == -1) {
            syslog_bus(busnumber, DBG_ERROR,
                       "ioctl() failed: %s (errno = %d)\n",
                       strerror(errno), errno);
        }
    } while (!value);

    /* if power is on do not turn it off */
//    if (!buses[busnumber].power_state)
//        tcflow(buses[busnumber].device.file.fd, TCOOFF);
    return ack;
}

/**
  Check if a given ack is valid 
  an ack is invalid if the line is permanently active
  @par Input: bus_t busnumber
              int ack
  @return ack if valid, 0 otherwise
*/
static int handleACK(bus_t busnumber, int ack)
{
    if (usleep(5000) == -1) {
        syslog_bus(busnumber, DBG_ERROR,
                   "usleep() failed in NMRA line %d: %s (errno = %d)",
                   __LINE__, strerror(errno), errno);
    }
    /* ack not supported */
    if ((ack == 1) && (scanACK(busnumber) == 1)) {
        return 0;
    }
    /* ack supported ==> send to client */
    else
        return ack;
}
#endif

/**
  Generate command stream to access a physical register
  @par Input: bus_t bus
              int reg
              int value
              int verify
  @par Output:char *packetstream
  @return length of packetstream
*/
#if 0
// TODO: obsolete code for UART to be replaced by code for SPI
static int calc_reg_stream(bus_t bus, int reg, int value, int verify,
                           char *packetstream)
{

    char byte1[9];
    char byte2[9];
    char byte3[9];
    char bitstream[BUFFERSIZE];
    int j;

    /* calculating byte1: 0111CRRR (instruction and number of register) */
    calc_single_byte(byte1, 0x78 | reg);
    if (verify) {
        byte1[4] = '0';
    }

    /* calculating byte2: DDDDDDDD (data) */
    calc_single_byte(byte2, value);

    /* calculating byte3 (error detection byte) */
    xor_two_bytes(byte3, byte1, byte2);

    /* putting all together in a 'bitstream' (char array) */
    memset(bitstream, 0, BUFFERSIZE);
    strcat(bitstream, longpreamble);
    strcat(bitstream, "0");
    strcat(bitstream, byte1);
    strcat(bitstream, "0");
    strcat(bitstream, byte2);
    strcat(bitstream, "0");
    strcat(bitstream, byte3);
    strcat(bitstream, "1");

    memset(packetstream, 0, PKTSIZE);
    j = translateBitstream2Packetstream(bus, bitstream, packetstream);
    return j;
}
#endif
/**
  Write or verify a byte of a physical register
  special case: a write to register 1 is the address only command
  0111C000 0AAAAAAA
  @par Input: bus_t bus
              int reg
              int value
              int verify
  @return 1 if successful, 0 otherwise
*/
static int protocol_nmra_sm_phregister(bus_t bus, int reg, int value,
                                       int verify)
{
    /* physical register addressing */

//    char packetstream[PKTSIZE];
//    char SendStream[4096];

//    int j, l, y, ack;

    syslog_bus(bus, DBG_DEBUG,
               "command for NMRA service mode instruction (SMPRA) received"
               " REG %d, Value %d", reg, value);

    /* no special error handling, it's job of the clients */
    if (reg < 1 || reg > 8 || value < 0 || value > 255)
        return -1;

    if (!sm_initialized)
        sm_init(bus);
#if 0
// TODO: obsolete code for UART to be replaced by code for SPI
    reg -= 1;
    j = calc_reg_stream(bus, reg, value, verify, packetstream);

    memset(SendStream, 0, 4096);

    if (!verify) {
        /* power-on cycle, at least 20 valid packets */
        for (l = 0; l < 50; l++)
            strcat(SendStream, idlestream);
        /* 3 or more reset packets */
        for (l = 0; l < 6; l++)
            strcat(SendStream, resetstream);
        /* 5 or more page preset packets */
        for (l = 0; l < 10; l++)
            strcat(SendStream, pagepresetstream);
        /* 6 or more reset packets */
        for (l = 0; l < 12; l++)
            strcat(SendStream, resetstream);
        /* 3 or more reset packets */
        for (l = 0; l < 12; l++)
            strcat(SendStream, resetstream);
        /* 5 or more write packets */
        for (l = 0; l < 20; l++)
            strcat(SendStream, packetstream);
        /* 6 or more reset or identical write packets */
        for (l = 0; l < 24; l++)
            strcat(SendStream, packetstream);
        /* 3 or more reset packets */
        for (l = 0; l < 24; l++)
            strcat(SendStream, resetstream);

        y = 50 * is_size + 54 * rs_size + 44 * j + 10 * ps_size;
    }
    else {
        /* power-on cycle, at least 20 valid packets */
        for (l = 0; l < 30; l++)
            strcat(SendStream, idlestream);
        /* 3 or more reset packets */
        for (l = 0; l < 5; l++)
            strcat(SendStream, resetstream);
        /* 7 or more verify packets */
        for (l = 0; l < 20; l++)
            strcat(SendStream, packetstream);
        y = 30 * is_size + 5 * rs_size + 20 * j + 0;
    }

    setSerialMode(bus, SDM_NMRA);
//    tcflow(buses[bus].device.file.fd, TCOON);

    l = write(buses[bus].device.file.fd, SendStream, y);
    if (l == -1) {
        syslog_bus(bus, DBG_ERROR,
                   "write() failed: %s (errno = %d)\n",
                   strerror(errno), errno);
    }

    ack = waitUARTempty_scanACK(bus);
    setSerialMode(bus, SDM_DEFAULT);

    return handleACK(bus, ack);
#endif
    return -1;
}

/**
  Write or verify a byte of a physical register using paged mode addressing
  @par Input: bus_t bus
              int page
              int reg
              int value
              int verify
  @return 1 if successful, 0 otherwise
*/
static int protocol_nmra_sm_page(bus_t bus, int page, int reg, int value,
                                 int verify)
{
    /* physical register addressing */

//    char packetstream[PKTSIZE];
//    char packetstream_page[PKTSIZE];
//    char SendStream[4096];

//    int j, k, l, y, ack;

    syslog_bus(bus, DBG_DEBUG,
               "command for NMRA service mode instruction (SMPRA) received"
               " PAGE %d, REG %d, Value %d", page, reg, value);

    /* no special error handling, it's job of the clients */
    if (reg < 0 || reg > 3 || page < 0 || page > 255 || value < 0
        || value > 255)
        return -1;

    if (!sm_initialized)
        sm_init(bus);
 #if 0
// TODO: obsolete code for UART to be replaced by code for SPI
    j = calc_reg_stream(bus, reg, value, verify, packetstream);
    /* set page to register 6 (number 5) */
    k = calc_reg_stream(bus, 5, page, false, packetstream_page);

    memset(SendStream, 0, 4096);

    /* power-on cycle, at least 20 valid packets */
    for (l = 0; l < 50; l++)
        strcat(SendStream, idlestream);
    /* 3 or more reset packets */
    for (l = 0; l < 6; l++)
        strcat(SendStream, resetstream);
    /* 5 or more writes to the page register */
    for (l = 0; l < 10; l++)
        strcat(SendStream, packetstream_page);
    /* 6 or more reset packets */
    for (l = 0; l < 12; l++)
        strcat(SendStream, resetstream);
    /* 3 or more reset packets */
    for (l = 0; l < 12; l++)
        strcat(SendStream, resetstream);
    /* 5 or more write packets */
    for (l = 0; l < 15; l++)
        strcat(SendStream, packetstream);
    /* 6 or more reset or identical write packets */
    for (l = 0; l < 15; l++)
        strcat(SendStream, packetstream);
    /* 3 or more reset packets */
    for (l = 0; l < 24; l++)
        strcat(SendStream, resetstream);

    y = 50 * is_size + 54 * rs_size + 30 * j + 10 * k;

    setSerialMode(bus, SDM_NMRA);
//    tcflow(buses[bus].device.file.fd, TCOON);

    l = write(buses[bus].device.file.fd, SendStream, y);
    if (l == -1) {
        syslog_bus(bus, DBG_ERROR,
                   "write() failed: %s (errno = %d)\n",
                   strerror(errno), errno);
    }

    ack = waitUARTempty_scanACK(bus);
    setSerialMode(bus, SDM_DEFAULT);

    return handleACK(bus, ack);
#endif
    return -1;
}

/**
  Write a byte to a physical register
  @par Input: bus_t bus
              int reg
              int value
  @return 1 if successful, 0 otherwise
*/
int protocol_nmra_sm_write_phregister(bus_t bus, int reg, int value)
{
    return protocol_nmra_sm_phregister(bus, reg, value, false);
}

/**
  Check the contens of a physical register
  @par Input: bus_t bus
              int reg
              int value
  @return 1 if the contens of the register equals reg, 0 otherwise
*/
int protocol_nmra_sm_verify_phregister(bus_t bus, int reg, int value)
{
    return protocol_nmra_sm_phregister(bus, reg, value, true);
}

/**
  Get the contens of a physical register
  @par Input: bus_t bus
              int reg
  @return the value of the register
*/
int protocol_nmra_sm_get_phregister(bus_t bus, int reg)
{
    int rc;
    int i;
    for (i = 0; i < 256; i++) {
        rc = protocol_nmra_sm_phregister(bus, reg, i, true);
        if (rc)
            break;
    }
    return i;
}

/**
  Verify/write a byte of a configuration variable (cv) 
  @par Input: bus_t busnumber
              int cv
              int value
              int verify
  @return 1 if successful, 0 otherwise
*/
static int protocol_nmra_sm_direct_cvbyte(bus_t busnumber, int cv,
                                          int value, int verify)
{
    /* direct cv access */
    char bitstream[BUFFERSIZE];
    char progstream[BUFFERSIZE];
//    char packetstream[PKTSIZE];
//    char SendStream[2048];
    char progerrbyte[9];
//    int j, l, ack = 0;

    cv -= 1;
    syslog_bus(busnumber, DBG_DEBUG,
               "command for NMRA service mode instruction (SMDWY) received");

    /* no special error handling, it's job of the clients */
    if (cv < 0 || cv > 1024 || value < 0 || value > 255)
        return -1;

    /* address only mode */
    if (!cv && (value < 128)) {
        return protocol_nmra_sm_phregister(busnumber, 1, value, verify);
    }

    if (!sm_initialized)
        sm_init(busnumber);
    /* putting all together in a 'bitstream' (char array) */
    strncpy(progerrbyte, "00000000", 9);
    progerrbyte[8] = 0;
    calc_byte_program_stream(progstream, progerrbyte, cv, value, verify,
                             false);
    memset(bitstream, 0, BUFFERSIZE);
    strcat(bitstream, longpreamble);
    strcat(bitstream, "0");
    strcat(bitstream, progstream);
#if 0
// TODO: obsolete code for UART to be replaced by code for SPI

    j = translateBitstream2Packetstream(busnumber, bitstream, packetstream);

    memset(SendStream, 0, 2048);

    if (!verify) {
        for (l = 0; l < 30; l++)
            strcat(SendStream, idlestream);
        for (l = 0; l < 15; l++)
            strcat(SendStream, resetstream);
        for (l = 0; l < 20; l++)
            strcat(SendStream, packetstream);
        l = 30 * is_size + 15 * rs_size + 20 * j;
    }
    else {
        for (l = 0; l < 30; l++)
            strcat(SendStream, idlestream);
        for (l = 0; l < 5; l++)
            strcat(SendStream, resetstream);
        for (l = 0; l < 20; l++)
            strcat(SendStream, packetstream);
        l = 30 * is_size + 5 * rs_size + 20 * j;
    }

    setSerialMode(busnumber, SDM_NMRA);
//    tcflow(buses[busnumber].device.file.fd, TCOON);
    ssize_t result = write(buses[busnumber].device.file.fd, SendStream, l);
    if (result == -1) {
        syslog_bus(busnumber, DBG_ERROR,
                   "write() failed: %s (errno = %d)\n",
                   strerror(errno), errno);
    }
    ack = waitUARTempty_scanACK(busnumber);
    setSerialMode(busnumber, SDM_DEFAULT);
    return handleACK(busnumber, ack);
#endif
    return -1;
}

/**
  Write a byte to a configuration variable (cv) 
  @par Input: bus_t bus
              int cv
              int value
  @return 1 if successful, 0 otherwise
*/
int protocol_nmra_sm_write_cvbyte(bus_t bus, int cv, int value)
{
    return protocol_nmra_sm_direct_cvbyte(bus, cv, value, false);
}

/**
  Verify the content of a configuration variable (cv) 
  @par Input: bus_t bus
              int cv
              int value
  @return 1 if the value matches, 0 otherwise
*/
int protocol_nmra_sm_verify_cvbyte(bus_t bus, int cv, int value)
{
    return protocol_nmra_sm_direct_cvbyte(bus, cv, value, true);
}

/**
  Verify/write a single bit of a configuration variable (cv) 
  @par Input: bus_t bus
              int cv
              int bit
              int value
              int verify
  @return 1 if successful, 0 otherwise
*/
static int protocol_nmra_sm_direct_cvbit(bus_t bus, int cv, int bit,
                                         int value, int verify)
{
    /* direct cv access */
    char progerrbyte[9];
    char bitstream[BUFFERSIZE];
    char progstream[BUFFERSIZE];
//    char packetstream[PKTSIZE];
//    char SendStream[2048];

//    int j, l, ack;

    cv -= 1;
    syslog_bus(bus, DBG_DEBUG,
               "command for NMRA service mode instruction (SMDWB) received");

    /* no special error handling, it's job of the clients */
    if (cv < 0 || cv > 1023 || bit < 0 || bit > 7 || value < 0
        || value > 1)
        return -1;

    if (!sm_initialized)
        sm_init(bus);

    strncpy(progerrbyte, "00000000", 9);
    progerrbyte[8] = 0;
    calc_bit_program_stream(progstream, progerrbyte, cv, bit, value,
                            verify, false);

    /* putting all together in a 'bitstream' (char array) */
    memset(bitstream, 0, BUFFERSIZE);
    strcat(bitstream, longpreamble);
    strcat(bitstream, "0");
    strcat(bitstream, progstream);
#if 0
// TODO: obsolete code for UART to be replaced by code for SPI
    j = translateBitstream2Packetstream(bus, bitstream, packetstream);

    memset(SendStream, 0, 2048);
    for (l = 0; l < 30; l++)
        strcat(SendStream, idlestream);
    for (l = 0; l < 15; l++)
        strcat(SendStream, resetstream);
    for (l = 0; l < 20; l++)
        strcat(SendStream, packetstream);
    l = 30 * is_size + 15 * rs_size + 20 * j;

    setSerialMode(bus, SDM_NMRA);
//    tcflow(buses[bus].device.file.fd, TCOON);
    ssize_t result = write(buses[bus].device.file.fd, SendStream, l);
    if (result == -1) {
        syslog_bus(bus, DBG_ERROR,
                   "write() failed: %s (errno = %d)\n",
                   strerror(errno), errno);
    }
    ack = waitUARTempty_scanACK(bus);
    setSerialMode(bus, SDM_DEFAULT);

    return handleACK(bus, ack);
#endif
    return -1;
}

/**
  Write a single bit of a configuration variable (cv) 
  @par Input: bus_t bus
              int cv
              int bit
              int value
  @return 1 if successful, 0 otherwise
*/
int protocol_nmra_sm_write_cvbit(bus_t bus, int cv, int bit, int value)
{
    return protocol_nmra_sm_direct_cvbit(bus, cv, bit, value, false);
}

/**
  Verify a single bit of a configuration variable (cv) 
  @par Input: bus_t bus
              int cv
              int bit
              int value
  @return 1 if the value matches, 0 otherwise
*/
int protocol_nmra_sm_verify_cvbit(bus_t bus, int cv, int bit, int value)
{
    return protocol_nmra_sm_direct_cvbit(bus, cv, bit, value, true);
}

/**
  Get cvbyte by verifying all bits -> constant runtime
  @par Input: bus_t bus
              int cv
  @return the value of the configuration variable (cv)
*/
int protocol_nmra_sm_get_cvbyte(bus_t busnumber, int cv)
{
    int bit;
    int rc = 0;
    for (bit = 7; bit >= 0; bit--) {
        rc <<= 1;               /* shift bits */
        rc += protocol_nmra_sm_direct_cvbit(busnumber, cv, bit, 1, true);
    }
    return rc;
}

/**
  Calclulate the page number for a given cv
  @par Input: int cv
  @return the page number
*/
static int calc_page(int cv)
{
    int page = (cv - 1) / 4 + 1;
    return page;
}

/**
  Write a configuration variable (cv) using paged mode addressing
  @par Input: bus_t bus
              int cv
              int value
  @return 1 if successful, 0 otherwise
*/
int protocol_nmra_sm_write_page(bus_t busnumber, int cv, int value)
{
    int page = calc_page(cv);
    return protocol_nmra_sm_page(busnumber, page, (cv - 1) & 3, value,
                                 false);
}

/**
  Verify the contens of a cv using paged mode addressing
  @par Input: bus_t bus
              int cv
	      int value
  @return 1 if the value matches, 0 otherwise
*/

int protocol_nmra_sm_verify_page(bus_t busnumber, int cv, int value)
{
    int page = calc_page(cv);
    return protocol_nmra_sm_page(busnumber, page, (cv - 1) & 3, value,
                                 true);
}

/**
  Get the contens of a cv using paged mode addressing
  @par Input: bus_t bus
              int cv
  @return the value of the configuration variable (cv)
*/
int protocol_nmra_sm_get_page(bus_t busnumber, int cv)
{
    int page = calc_page(cv);
    int rc;
    int i;
    for (i = 0; i < 256; i++) {
        rc = protocol_nmra_sm_page(busnumber, page, (cv - 1) & 3, i, true);
        if (rc)
            break;
    }
    return i;
}
