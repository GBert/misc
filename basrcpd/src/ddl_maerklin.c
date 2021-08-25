// ddl_maerklin.c - adapted for basrcpd project 2018 by Rainer Müller
//
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
/* | Authors:   Torsten Vogt vogt@vogt-it.com                             | */
/* |                                                                      | */
/* +----------------------------------------------------------------------+ */

/***************************************************************/
/* erddcd - Electric Railroad Direct Digital Command Daemon    */
/*    generates without any other hardware digital commands    */
/*    to control electric model railroads                      */
/*                                                             */
/* file: maerklin.c                                            */
/* job : implements routines to compute data for the           */
/*       various Maerklin protocols and send this data to      */
/*       the serial device.                                    */
/*                                                             */
/* Torsten Vogt, January 1999                                  */
/*                                                             */
/* last changes: June 2000                                     */
/*               January 2001                                  */
/*                                                             */
/*
   thanks to Dieter Schaefer for testing and correcting the
   handling of the solenoids decoders (thr_protocol_maerklin_ms())
*/
/***************************************************************/

/**********************************************************************

 implemented protocols:

 M1: maerklin protocol type 1 (old)
 M2: maerklin protocol type 2 (new)
 M3: maerklin protocol type 2 (new) with 28 speed steps (Wikinger decoder)
 M4: maerklin protocol type 2 (new) with 256 addresses (Uhlenbrock)
 M5: maerklin protocol type 2 (new) with 27 speed steps (newer Maerklin decoders)
 MS: maerklin protocol for solenoids (type 1 and type 2)
 MF: maerklin_protocol for function decoders (old)

**********************************************************************/

#include "ddl.h"
#include "ddl_maerklin.h"
#include "srcp-gl.h"
#include "syslogmessage.h"


// Codiertabelle zur Adresswandlung binär -> ternär
static uint8_t mmadr_cod [256] = {
     85,  3,  1, 12, 15, 13,  4,  7,  5, 48, 51, 49, 60, 63, 61, 52,
     55, 53, 16, 19, 17, 28, 31, 29, 20, 23, 21,192,195,193,204,207,
    205,196,199,197,240,243,241,252,255,253,244,247,245,208,211,209,
    220,223,221,212,215,213, 64, 67, 65, 76, 79, 77, 68, 71, 69,112,
    115,113,124,127,125,116,119,117, 80, 83, 81, 92, 95, 93, 84, 87,
      0,  2,  6,233, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58,
     62, 66, 70, 74, 78, 82, 86, 90, 94, 98,102,106,110,114,118,122,
    126,130,134,138,142,146,150,154,158,162,166,249,174,178,182,186,
    190,194,198,202,206,210,214,218,222,226,230,234,238,242,246,250,
    254,  8, 24, 40, 56, 72, 88,104,120,136,152,168,184,200,216,232,
    248, 11, 27, 43, 59, 75, 91,107,123,139,155,171,187,203,219,235,
    251,  9, 25, 41, 57, 73, 89,105,121,137,153,169,185,201,217, 10,
    170, 32, 96,160,224, 35, 99,163,227, 33, 97,161,225, 44,108,172,
    236, 47,111,175,239, 45,109,173,237, 36,100,164,228, 39,103,167,
    231, 37,101,165,229,128,131,129,140,143,141,132,135,133,176,179,
    177,188,191,189,180,183,181,144,147,145,156,159,157,148,151,149 };


//Für SPI Ausgabe wird hier nur 0 und 1 als 0 und 1 Bytes kodiert.
static char LO = 0;
static char HI = 1;

char getMaerklinLO() {
  return LO;
}
char getMaerklinHI() {
  return HI;
}

int comp_maerklin_1(bus_t busnumber, int address, int direction,
                    		int speed, int funcs)         //, char cacheddirection)
{
    char packet[18];
    int i;

    if (address < 0 || address > 80 || speed < 0 || speed > 15) return 1;

    /* compute address part */
    for (i = 0; i < 8; i++)
        packet[i] = (mmadr_cod[address] >> i) & 1;

    /* compute func part */
    packet[8] = packet[9] = funcs & 1;

    /* compute speed part */
    for (i = 5; i < 9; i++) {
        packet[2 * i] = packet[2 * i + 1] = speed & 1;
        speed >>= 1;
    }

    update_MaerklinPacketPool(busnumber, address, packet, packet, packet,
                              packet, packet);
    send_packet(busnumber, packet, 18, QM1LOCOPKT, 2);
    return 0;
}

int comp_maerklin_2(bus_t busnumber, int address, int direction,
                    int speed, int func, int f1, int f2, int f3, int f4)
{

    char trits[9];
    char packet[18];
    char f_packets[4][18];
    int fx = 0, fx_changed = 0;
    char *fx_packet;

    char mask[5];
    int i, j;
    int mspeed;

    if (direction == 0)
        direction = -1;
    else
        direction = 1;
    speed = direction * speed;

    /* no special error handling, it's job of the clients */
    if (address < 0 || address > 255 || func < 0 || func > 1 || speed < -15
        || speed > 15 || f1 < 0 || f1 > 1 || f2 < 0 || f2 > 1 || f3 < 0
        || f3 > 1 || f4 < 0 || f4 > 1)
        return 1;

    /* compute address part */
    for (i = 0; i < 8; i++)
        packet[i] = (mmadr_cod[address] >> i) & 1;

    /* compute func part */
    packet[8] = packet[9] = func & 1;

    /* so far the same procedure as by Maerklin type 1, but now ... */
    /* compute speed trits   */
    if (speed < -7)
        strcpy(mask, "HLHL");
    if (speed <= 0 && speed >= -7 && direction == -1)
        strcpy(mask, "HLHH");
    if (speed >= 0 && speed <= 7 && direction == 1)
        strcpy(mask, "LHLH");
    if (speed > 7)
        strcpy(mask, "LHLL");
    speed = abs(speed);
    mspeed = speed;

    for (i = 5; i < 9; i++) {
        j = speed % 2;
        speed = speed / 2;
        switch (j) {
            case 0:
                trits[i] = 'L';
                break;
            case 1:
                trits[i] = 'H';
                break;
        }
        if (trits[i] == 'H' && mask[i - 5] == 'L')
            trits[i] = 'O';
        if (trits[i] == 'L' && mask[i - 5] == 'H')
            trits[i] = 'U';     /* Oops, whats */
    }                           /* this? :-)    */

    for (i = 5; i < 9; i++) {
        switch (trits[i]) {
            case 'L':
                packet[2 * i] = LO;
                packet[2 * i + 1] = LO;
                break;
            case 'H':
                packet[2 * i] = HI;
                packet[2 * i + 1] = HI;
                break;
            case 'O':
                packet[2 * i] = HI;
                packet[2 * i + 1] = LO;
                break;
            case 'U':
                packet[2 * i] = LO;
                packet[2 * i + 1] = HI;
                break;
        }
    }
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 18; i++) {
            f_packets[j][i] = packet[i];
        }
    }
    f_packets[0][11] = HI;
    f_packets[0][13] = HI;
    f_packets[0][15] = LO;
    if (f1)
        f_packets[0][17] = HI;
    else
        f_packets[0][17] = LO;
    f_packets[1][11] = LO;
    f_packets[1][13] = LO;
    f_packets[1][15] = HI;
    if (f2)
        f_packets[1][17] = HI;
    else
        f_packets[1][17] = LO;
    f_packets[2][11] = LO;
    f_packets[2][13] = HI;
    f_packets[2][15] = HI;
    if (f3)
        f_packets[2][17] = HI;
    else
        f_packets[2][17] = LO;
    f_packets[3][11] = HI;
    f_packets[3][13] = HI;
    f_packets[3][15] = HI;
    if (f4)
        f_packets[3][17] = HI;
    else
        f_packets[3][17] = LO;

    /* thanks to Dieter Schaefer for the following code */
    for (j = 0; j < 4; j++) {
        if (((j == 0) && (mspeed == 3) && (!f1))
            || ((j == 1) && (mspeed == 4) && (!f2))
            || ((j == 2) && (mspeed == 6) && (!f3))
            || ((j == 3) && (mspeed == 7) && (!f4))) {
            f_packets[j][11] = HI;
            f_packets[j][13] = LO;
            f_packets[j][15] = HI;
        }
        if (((j == 0) && (mspeed == 11) && (f1))
            || ((j == 1) && (mspeed == 12) && (f2))
            || ((j == 2) && (mspeed == 14) && (f3))
            || ((j == 3) && (mspeed == 15) && (f4))) {
            f_packets[j][11] = LO;
            f_packets[j][13] = HI;
            f_packets[j][15] = LO;
        }
    }

    /* lets have a look, what has changed ... */
    for (i = 0; i < 4; i++) {
        fx_packet = get_maerklin_packet(busnumber, address, i);
        if (fx_packet[17] != f_packets[i][17]) {
            fx_changed = 1;
            fx = i;
            break;
        }
    }

    update_MaerklinPacketPool(busnumber, address, packet, f_packets[0],
                              f_packets[1], f_packets[2], f_packets[3]);

    //Ausgabe Geschwindigkeitsbefehl erfolgt immer
    send_packet(busnumber, packet, 18, QM2LOCOPKT, 2);
    if (fx_changed) {
    	send_packet(busnumber, f_packets[fx], 18, QM2FXPKT, 1);
    }
    return 0;
}

int comp_maerklin_28(bus_t busnumber, int address, int direction,
                    int speed, int func, int f1, int f2, int f3, int f4)
{

    char trits[9];
    char packet[18];
    char f_packets[4][18];
    int fx = 0, fx_changed = 0;
    char *fx_packet;

    char mask[5];
    int i, j;
    int adr = 0;
    int speed_halfstep = 0;
    int mspeed;

    adr = address;
    if (direction == 0)
        direction = -1;
    else
        direction = 1;
    speed = direction * speed;

    /* no special error handling, it's job of the clients */
    if (address < 0 || address > 255 || func < 0 || func > 1 || speed < -28
        || speed > 28 || f1 < 0 || f1 > 1 || f2 < 0 || f2 > 1 || f3 < 0
        || f3 > 1 || f4 < 0 || f4 > 1)
        return 1;

    /* compute address part */
    for (i = 0; i < 8; i++)
        packet[i] = (mmadr_cod[address] >> i) & 1;

    /* so far the same procedure as by Maerklin type 1, but now ... */

    speed_halfstep = !(abs(speed) % 2);
    if (speed > 0)
        speed = ((speed + 1) / 2) + 1;
    else
        speed = ((speed - 1) / 2) - 1;

    /* compute func trit and speed half step */
    if (!speed_halfstep)
        if (func)
            trits[4] = 'H';
        else
            trits[4] = 'L';
    else if (func)
        trits[4] = 'O';
    else
        trits[4] = 'U';

    /* compute speed trits   */
    if (speed < -7)
        strcpy(mask, "HLHL");
    if (speed <= 0 && speed >= -7 && direction == -1)
        strcpy(mask, "HLHH");
    if (speed >= 0 && speed <= 7 && direction == 1)
        strcpy(mask, "LHLH");
    if (speed > 7)
        strcpy(mask, "LHLL");
    speed = abs(speed);
    mspeed = speed;

    for (i = 5; i < 9; i++) {
        j = speed % 2;
        speed = speed / 2;
        switch (j) {
            case 0:
                trits[i] = 'L';
                break;
            case 1:
                trits[i] = 'H';
                break;
        }
        if (trits[i] == 'H' && mask[i - 5] == 'L')
            trits[i] = 'O';
        if (trits[i] == 'L' && mask[i - 5] == 'H')
            trits[i] = 'U';     /* Oops, whats */
    }                           /* this? :-)    */

    for (i = 4; i < 9; i++) {
        switch (trits[i]) {
            case 'L':
                packet[2 * i] = LO;
                packet[2 * i + 1] = LO;
                break;
            case 'H':
                packet[2 * i] = HI;
                packet[2 * i + 1] = HI;
                break;
            case 'O':
                packet[2 * i] = HI;
                packet[2 * i + 1] = LO;
                break;
            case 'U':
                packet[2 * i] = LO;
                packet[2 * i + 1] = HI;
                break;
        }
    }
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 18; i++) {
            f_packets[j][i] = packet[i];
        }
    }
    f_packets[0][11] = HI;
    f_packets[0][13] = HI;
    f_packets[0][15] = LO;
    if (f1)
        f_packets[0][17] = HI;
    else
        f_packets[0][17] = LO;
    f_packets[1][11] = LO;
    f_packets[1][13] = LO;
    f_packets[1][15] = HI;
    if (f2)
        f_packets[1][17] = HI;
    else
        f_packets[1][17] = LO;
    f_packets[2][11] = LO;
    f_packets[2][13] = HI;
    f_packets[2][15] = HI;
    if (f3)
        f_packets[2][17] = HI;
    else
        f_packets[2][17] = LO;
    f_packets[3][11] = HI;
    f_packets[3][13] = HI;
    f_packets[3][15] = HI;
    if (f4)
        f_packets[3][17] = HI;
    else
        f_packets[3][17] = LO;

    /* thanks to Dieter Schaefer for the following code */
    for (j = 0; j < 4; j++) {
        if (((j == 0) && (mspeed == 3) && (!f1))
            || ((j == 1) && (mspeed == 4) && (!f2))
            || ((j == 2) && (mspeed == 6) && (!f3))
            || ((j == 3) && (mspeed == 7) && (!f4))) {
            f_packets[j][11] = HI;
            f_packets[j][13] = LO;
            f_packets[j][15] = HI;
        }
        if (((j == 0) && (mspeed == 11) && (f1))
            || ((j == 1) && (mspeed == 12) && (f2))
            || ((j == 2) && (mspeed == 14) && (f3))
            || ((j == 3) && (mspeed == 15) && (f4))) {
            f_packets[j][11] = LO;
            f_packets[j][13] = HI;
            f_packets[j][15] = LO;
        }
    }

    /* lets have a look, what has changed ... */
    for (i = 0; i < 4; i++) {
        fx_packet = get_maerklin_packet(busnumber, adr, i);
        if (fx_packet[17] != f_packets[i][17]) {
            fx_changed = 1;
            fx = i;
            break;
        }
    }

    update_MaerklinPacketPool(busnumber, adr, packet, f_packets[0],
                              f_packets[1], f_packets[2], f_packets[3]);

    //Ausgabe Geschwindigkeitsbefehl erfolgt immer
    send_packet(busnumber, packet, 18, QM2LOCOPKT, 2);
    if (fx_changed) {
    	send_packet(busnumber, f_packets[fx], 18, QM2FXPKT, 1);
    }
    return 0;
}

int comp_maerklin_27(bus_t busnumber, int address, int direction,
                    int speed, int func, int f1, int f2, int f3, int f4, int speed_old)
{

    int sFS1, sFS2;
    int rtc;
    int two_commands = false;
    int acceleration = false;

    /* no special error handling, it's job of the clients */
    if (speed < 0 || speed > 28)
        return 1;

    /* xFS    rFS   sFS1 -> (500ms) -> sFS2

       0      0      0                  %

       2      2      0                  2
       3    2.5      3                  2
       4      3      2                  3
       5    3.5      4                  3
       6      4      3                  4
       7    4.5      5                  4
       8      5      4                  5
       9    5.5      6                  5
       10      6      5                  6
       11    6.5      7                  6
       12      7      6                  7
       13    7.5      8                  7
       14      8      7                  8
       15    8.5      9                  8
       16      9      8                  9
       17    9.5     10                  9
       18     10      9                 10
       19   10.5     11                 10
       20     11     10                 11
       21   11.5     12                 11
       22     12     11                 12
       23   12.5     13                 12
       24     13     12                 13
       25   13.5     14                 13
       26     14     13                 14
       27   14.5     15                 14
       28     15     14                 15
     */

    acceleration = (speed_old < speed);

    if ((acceleration && (speed % 2)) || (!acceleration && !(speed % 2)))
        two_commands = true;

    if (speed > 0) {
        if (two_commands) {     /* two commands necessary */
            if (speed % 2) {
                sFS2 = speed / 2 + 1;
                sFS1 = sFS2 + 1;
            }
            else {
                sFS2 = speed / 2 + 1;
                sFS1 = sFS2 - 1;
                if (sFS1 == 1)
                    sFS1 = 0;
            }
        }
        else {                  /* one command necessary */
            sFS1 = speed / 2 + 1;
            sFS2 = 0;
        }
    }
    else {                      /* one command necessary */
        sFS1 = 0;
        sFS2 = 0;
    }

    rtc = comp_maerklin_2(busnumber, address, direction, sFS1, func, f1, f2, f3, f4);
    if ((sFS2 > 0) && (rtc == 0)) {
        if (usleep(50000) == -1) {
            syslog_bus(busnumber, DBG_ERROR,
                       "usleep() failed in Märklin line %d: %s (errno = %d)",
                       __LINE__, strerror(errno), errno);
        }
        rtc = comp_maerklin_2(busnumber, address, direction, sFS2, func,
                              f1, f2, f3, f4);
    }
    return rtc;
}

int comp_maerklin_ms(bus_t busnumber, int address, int port, int action)
{
    char packet[18];
    int i, j;
    int id, subid;

    syslog_bus(busnumber, DBG_DEBUG,
               "command for MM solenoid received addr:%d port:%d action:%d",
			   address, port, action);

    /* no special error handling, it's job of the clients */
    if (address < 1 || address > 324 || action < 0 || action > 1 || port < 0 || port > 1) {
        return 1;
    }
    id = ((address - 1) >> 2);
    subid = (((address - 1) & 3) << 1) + port;
    if (action)
    	packet[16] = packet[17] = 1;
    else
    	packet[16] = packet[17] = 0;

    /* compute address part */
    if (id == 0) id = 80;		// special translation
    for (i = 0; i < 8; i++)
        packet[i] = (mmadr_cod[id] >> i) & 1;

    /* compute dummy func part */
    packet[8] = packet[9] = 0;

    /* compute port trits   */
    for (i = 5; i < 8; i++) {
        j = subid % 2;
        subid = subid / 2;
        switch (j) {
            case 0:
                packet[i*2] = LO;
                packet[i*2+1] = LO;
                break;
            case 1:
                packet[i*2] = HI;
                packet[i*2+1] = HI;
                break;
        }
    }

    send_packet(busnumber, packet, 18, QM1SOLEPKT, 2);
    return 0;
}

int comp_maerklin_mf(bus_t busnumber, int address, int f1, int f2,
                     int f3, int f4)
{
    char trits[9];
    char packet[18];
    int i;

    syslog_bus(busnumber, DBG_DEBUG,
               "Command for func decoder (Maerklin) (MF) addr %d received", address);

    /* no special error handling, it's job of the clients */
    if (address < 0 || address > 80 || f1 < 0 || f1 > 1 ||
        f2 < 0 || f2 > 1 || f3 < 0 || f3 > 1 || f4 < 0 || f4 > 1)
        return 1;

    /* compute address part */
    for (i = 0; i < 8; i++)
        packet[i] = (mmadr_cod[address] >> i) & 1;

    /* compute func trit (dummy) */
    trits[4] = 'H';
    /* compute function trits   */
    if (f1)
        trits[5] = 'H';
    else
        trits[5] = 'L';
    if (f2)
        trits[6] = 'H';
    else
        trits[6] = 'L';
    if (f3)
        trits[7] = 'H';
    else
        trits[7] = 'L';
    if (f4)
        trits[8] = 'H';
    else
        trits[8] = 'L';

    for (i = 4; i < 9; i++) {
        switch (trits[i]) {
            case 'L':
                packet[i*2] = LO;
                packet[i*2+1] = LO;
                break;
            case 'H':
                packet[i*2] = HI;
                packet[i*2+1] = HI;
                break;
            case 'O':
                packet[i*2] = HI;
                packet[i*2+1] = LO;
                break;
        }
    }

    send_packet(busnumber, packet, 18, QM1FUNCPKT, 2);
    return 0;
}

void comp_maerklin_loco(bus_t bus, gl_data_t *glp)
{
	int pv = glp->protocolversion;
    int addr = glp->id;
    int speed = glp->speed;
    int direction = glp->direction;

    if (glp->speedchange & SCEMERG) {   // Emergency Stop
        speed = 0;
        direction = glp->cacheddirection;
        glp->speedchange &= ~SCEMERG;
    }
    else if (glp->speedchange & SCDIREC) {
        speed = 1;                      // change direction
        glp->speedchange &= ~SCDIREC;
    }
    else if (speed) speed++;        	// Never send FS1

    syslog_bus(bus, DBG_DEBUG,
    	"command for M%d protocol received addr:%d dir:%d (%d) speed:%d of %d chg %d funcs:%x",
        pv, addr, direction, glp->cacheddirection, speed, glp->n_fs, glp->speedchange, glp->funcs);

	if (pv == 1) {	comp_maerklin_1(bus, addr, direction, speed, glp->funcs);
                	if (glp->n_func > 1)
                		comp_maerklin_mf(bus, addr, ((glp->funcs >> 1) & 0x01),
                                ((glp->funcs >> 2) & 0x01), ((glp->funcs >> 3) & 0x01),
                                ((glp->funcs >> 4) & 0x01));
    }
    else switch (glp->n_fs) {
        case 14:	comp_maerklin_2(bus, addr, direction, speed,
                                glp->funcs & 0x01, ((glp->funcs >> 1) & 0x01),
                                ((glp->funcs >> 2) & 0x01), ((glp->funcs >> 3) & 0x01),
                                ((glp->funcs >> 4) & 0x01));
                    break;
        case 27:	comp_maerklin_27(bus, addr, direction, speed,
                                glp->funcs & 0x01, ((glp->funcs >> 1) & 0x01),
                                ((glp->funcs >> 2) & 0x01), ((glp->funcs >> 3) & 0x01),
                                ((glp->funcs >> 4) & 0x01), glp->cachedspeed);
                    break;
    	case 28:	comp_maerklin_28(bus, addr, direction, speed,
                                glp->funcs & 0x01, ((glp->funcs >> 1) & 0x01),
                                ((glp->funcs >> 2) & 0x01), ((glp->funcs >> 3) & 0x01),
                                ((glp->funcs >> 4) & 0x01));
                    break;
    }
    glp->speedchange &= ~SCSPEED;       // handled now
}
