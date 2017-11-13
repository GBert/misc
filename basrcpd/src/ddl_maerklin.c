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

#define LO_RS232 (char)63
#define HI_RS232 (char)0
//Für SPI Ausgabe wird hier nur 0 und 1 als 0 und 1 Bytes kodiert.
#define LO_SPI (char)0x00
#define HI_SPI (char)0x01

//Die schnelle Ausgabe wird mit SPI mit doppelter Baudrate realisiert. Deshalb keine extra Konstanten für RaspberryPI.
#define LO_38K (char)55
#define HI_38K (char)4
#define OP_38K (char)52


typedef struct _tMotorolaCodes {
    int rAdr;
    int bAdr;
    char Code[5];
} tMotorolaCodes;

static tMotorolaCodes MotorolaCodes[256] = {
    {0, 85, "OOOO"},
    {1, 3, "HLLL"},
    {2, 1, "OLLL"},
    {3, 12, "LHLL"},
    {4, 15, "HHLL"},
    {5, 13, "OHLL"},
    {6, 4, "LOLL"},
    {7, 7, "HOLL"},
    {8, 5, "OOLL"},
    {9, 48, "LLHL"},
    {10, 51, "HLHL"},
    {11, 49, "OLHL"},
    {12, 60, "LHHL"},
    {13, 63, "HHHL"},
    {14, 61, "OHHL"},
    {15, 52, "LOHL"},
    {16, 55, "HOHL"},
    {17, 53, "OOHL"},
    {18, 16, "LLOL"},
    {19, 19, "HLOL"},
    {20, 17, "OLOL"},
    {21, 28, "LHOL"},
    {22, 31, "HHOL"},
    {23, 29, "OHOL"},
    {24, 20, "LOOL"},
    {25, 23, "HOOL"},
    {26, 21, "OOOL"},
    {27, 192, "LLLH"},
    {28, 195, "HLLH"},
    {29, 193, "OLLH"},
    {30, 204, "LHLH"},
    {31, 207, "HHLH"},
    {32, 205, "OHLH"},
    {33, 196, "LOLH"},
    {34, 199, "HOLH"},
    {35, 197, "OOLH"},
    {36, 240, "LLHH"},
    {37, 243, "HLHH"},
    {38, 241, "OLHH"},
    {39, 252, "LHHH"},
    {40, 255, "HHHH"},
    {41, 253, "OHHH"},
    {42, 244, "LOHH"},
    {43, 247, "HOHH"},
    {44, 245, "OOHH"},
    {45, 208, "LLOH"},
    {46, 211, "HLOH"},
    {47, 209, "OLOH"},
    {48, 220, "LHOH"},
    {49, 223, "HHOH"},
    {50, 221, "OHOH"},
    {51, 212, "LOOH"},
    {52, 215, "HOOH"},
    {53, 213, "OOOH"},
    {54, 64, "LLLO"},
    {55, 67, "HLLO"},
    {56, 65, "OLLO"},
    {57, 76, "LHLO"},
    {58, 79, "HHLO"},
    {59, 77, "OHLO"},
    {60, 68, "LOLO"},
    {61, 71, "HOLO"},
    {62, 69, "OOLO"},
    {63, 112, "LLHO"},
    {64, 115, "HLHO"},
    {65, 113, "OLHO"},
    {66, 124, "LHHO"},
    {67, 127, "HHHO"},
    {68, 125, "OHHO"},
    {69, 116, "LOHO"},
    {70, 119, "HOHO"},
    {71, 117, "OOHO"},
    {72, 80, "LLOO"},
    {73, 83, "HLOO"},
    {74, 81, "OLOO"},
    {75, 92, "LHOO"},
    {76, 95, "HHOO"},
    {77, 93, "OHOO"},
    {78, 84, "LOOO"},
    {79, 87, "HOOO"},
    {80, 0, "LLLL"},
    {81, 2, "ULLL"},
    {82, 6, "UOLL"},
    {83, 233, "OUUH"},
    {84, 14, "UHLL"},
    {85, 18, "ULOL"},
    {86, 22, "UOOL"},
    {87, 26, "UUOL"},
    {88, 30, "UHOL"},
    {89, 34, "ULUL"},
    {90, 38, "UOUL"},
    {91, 42, "UUUL"},
    {92, 46, "UHUL"},
    {93, 50, "ULHL"},
    {94, 54, "UOHL"},
    {95, 58, "UUHL"},
    {96, 62, "UHHL"},
    {97, 66, "ULLO"},
    {98, 70, "UOLO"},
    {99, 74, "UULO"},
    {100, 78, "UHLO"},
    {101, 82, "ULOO"},
    {102, 86, "UOOO"},
    {103, 90, "UUOO"},
    {104, 94, "UHOO"},
    {105, 98, "ULUO"},
    {106, 102, "UOUO"},
    {107, 106, "UUUO"},
    {108, 110, "UHUO"},
    {109, 114, "ULHO"},
    {110, 118, "UOHO"},
    {111, 122, "UUHO"},
    {112, 126, "UHHO"},
    {113, 130, "ULLU"},
    {114, 134, "UOLU"},
    {115, 138, "UULU"},
    {116, 142, "UHLU"},
    {117, 146, "ULOU"},
    {118, 150, "UOOU"},
    {119, 154, "UUOU"},
    {120, 158, "UHOU"},
    {121, 162, "ULUU"},
    {122, 166, "UOUU"},
    {123, 249, "OUHH"},
    {124, 174, "UHUU"},
    {125, 178, "ULHU"},
    {126, 182, "UOHU"},
    {127, 186, "UUHU"},
    {128, 190, "UHHU"},
    {129, 194, "ULLH"},
    {130, 198, "UOLH"},
    {131, 202, "UULH"},
    {132, 206, "UHLH"},
    {133, 210, "ULOH"},
    {134, 214, "UOOH"},
    {135, 218, "UUOH"},
    {136, 222, "UHOH"},
    {137, 226, "ULUH"},
    {138, 230, "UOUH"},
    {139, 234, "UUUH"},
    {140, 238, "UHUH"},
    {141, 242, "ULHH"},
    {142, 246, "UOHH"},
    {143, 250, "UUHH"},
    {144, 254, "UHHH"},
    {145, 8, "LULL"},
    {146, 24, "LUOL"},
    {147, 40, "LUUL"},
    {148, 56, "LUHL"},
    {149, 72, "LULO"},
    {150, 88, "LUOO"},
    {151, 104, "LUUO"},
    {152, 120, "LUHO"},
    {153, 136, "LULU"},
    {154, 152, "LUOU"},
    {155, 168, "LUUU"},
    {156, 184, "LUHU"},
    {157, 200, "LULH"},
    {158, 216, "LUOH"},
    {159, 232, "LUUH"},
    {160, 248, "LUHH"},
    {161, 11, "HULL"},
    {162, 27, "HUOL"},
    {163, 43, "HUUL"},
    {164, 59, "HUHL"},
    {165, 75, "HULO"},
    {166, 91, "HUOO"},
    {167, 107, "HUUO"},
    {168, 123, "HUHO"},
    {169, 139, "HULU"},
    {170, 155, "HUOU"},
    {171, 171, "HUUU"},
    {172, 187, "HUHU"},
    {173, 203, "HULH"},
    {174, 219, "HUOH"},
    {175, 235, "HUUH"},
    {176, 251, "HUHH"},
    {177, 9, "OULL"},
    {178, 25, "OUOL"},
    {179, 41, "OUUL"},
    {180, 57, "OUHL"},
    {181, 73, "OULO"},
    {182, 89, "OUOO"},
    {183, 105, "OUUO"},
    {184, 121, "OUHO"},
    {185, 137, "OULU"},
    {186, 153, "OUOU"},
    {187, 169, "OUUU"},
    {188, 185, "OUHU"},
    {189, 201, "OULH"},
    {190, 217, "OUOH"},
    {191, 10, "UULL"},
    {192, 170, "UUUU"},
    {193, 32, "LLUL"},
    {194, 96, "LLUO"},
    {195, 160, "LLUU"},
    {196, 224, "LLUH"},
    {197, 35, "HLUL"},
    {198, 99, "HLUO"},
    {199, 163, "HLUU"},
    {200, 227, "HLUH"},
    {201, 33, "OLUL"},
    {202, 97, "OLUO"},
    {203, 161, "OLUU"},
    {204, 225, "OLUH"},
    {205, 44, "LHUL"},
    {206, 108, "LHUO"},
    {207, 172, "LHUU"},
    {208, 236, "LHUH"},
    {209, 47, "HHUL"},
    {210, 111, "HHUO"},
    {211, 175, "HHUU"},
    {212, 239, "HHUH"},
    {213, 45, "OHUL"},
    {214, 109, "OHUO"},
    {215, 173, "OHUU"},
    {216, 237, "OHUH"},
    {217, 36, "LOUL"},
    {218, 100, "LOUO"},
    {219, 164, "LOUU"},
    {220, 228, "LOUH"},
    {221, 39, "HOUL"},
    {222, 103, "HOUO"},
    {223, 167, "HOUU"},
    {224, 231, "HOUH"},
    {225, 37, "OOUL"},
    {226, 101, "OOUO"},
    {227, 165, "OOUU"},
    {228, 229, "OOUH"},
    {229, 128, "LLLU"},
    {230, 131, "HLLU"},
    {231, 129, "OLLU"},
    {232, 140, "LHLU"},
    {233, 143, "HHLU"},
    {234, 141, "OHLU"},
    {235, 132, "LOLU"},
    {236, 135, "HOLU"},
    {237, 133, "OOLU"},
    {238, 176, "LLHU"},
    {239, 179, "HLHU"},
    {240, 177, "OLHU"},
    {241, 188, "LHHU"},
    {242, 191, "HHHU"},
    {243, 189, "OHHU"},
    {244, 180, "LOHU"},
    {245, 183, "HOHU"},
    {246, 181, "OOHU"},
    {247, 144, "LLOU"},
    {248, 147, "HLOU"},
    {249, 145, "OLOU"},
    {250, 156, "LHOU"},
    {251, 159, "HHOU"},
    {252, 157, "OHOU"},
    {253, 148, "LOOU"},
    {254, 151, "HOOU"},
    {255, 149, "OOOU"}
};

static bool spiMode = false;
/*Die zur HI und LO Erzeugung zu sendenden Bytes */
static char LO = LO_RS232;
static char HI = HI_RS232;

/**
  * Setzen SPI Mode zur Berechnung der zu sendenen Datenpakete.
  * Da diese bei SPI anders sein müssen (siehe Kommentar oben).
  * @param newSpiMode true=SPI, false=RS232 (default).
  */
void setSPIModeMaerklin(bool newSpiMode) {
  spiMode = newSpiMode;
  if (spiMode) {
    LO = LO_SPI;
    HI = HI_SPI;
  }
  else {
    LO = LO_RS232;
    HI = HI_RS232;
  }
}
char getMaerklinLO() {
  return LO;
}
char getMaerklinHI() {
  return HI;
}

int comp_maerklin_1(bus_t busnumber, int address, int direction,
                    int speed, int func, bool prio)
{

    char trits[9];
    char packet[18];
    int i, j;
    gl_data_t loco;
    syslog_bus(busnumber, DBG_DEBUG,
               "Command for Maerklin protocol type 1: %d received",
               address);

    /* no special error handling, it's job of the clients */
    if (address < 0 || address > 80 || func < 0 || func > 1 || speed < 0
        || speed > 15 || direction < 0 || direction > 1)
        return 1;
    cacheGetGL(busnumber, address, &loco);
    if (direction != loco.direction) {
        speed = 1;
    }

    /* compute address trits */
    for (i = 0; i < 4; i++)
        trits[i] = MotorolaCodes[address].Code[i];

    /* compute func trit     */
    if (func)
        trits[4] = 'H';
    else
        trits[4] = 'L';
    /* compute speed trits   */
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
    }

    for (i = 0; i < 9; i++) {
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
        }
    }

    update_MaerklinPacketPool(busnumber, address, packet, packet, packet,
                              packet, packet);
    queue_add(busnumber, address, packet, QM1LOCOPKT, 18, prio); //Lok stoppen hat Priorität

    return 0;
}

int comp_maerklin_2(bus_t busnumber, int address, int direction,
                    int speed, int func, int f1, int f2, int f3, int f4, bool prio)
{

    char trits[9];
    char packet[18];
    char f_packets[4][18];
    int fx = 0, fx_changed = 0;
    char *fx_packet;

    char mask[5];
    int i, j;
    int adr = 0;
    int mspeed;

    syslog_bus(busnumber, DBG_DEBUG,
               "Command for Maerklin protocol type 2 (M2) received");

    adr = address;
    if (direction == 0)
        direction = -1;
    else
        direction = 1;
    speed = direction * speed;

    /* no special error handling, it's job of the clients */
    if (address < 0 || address > 80 || func < 0 || func > 1 || speed < -15
        || speed > 15 || f1 < 0 || f1 > 1 || f2 < 0 || f2 > 1 || f3 < 0
        || f3 > 1 || f4 < 0 || f4 > 1)
        return 1;

    /* compute address trits */
    for (i = 0; i < 4; i++)
        trits[i] = MotorolaCodes[address].Code[i];

    /* compute func trit     */
    if (func)
        trits[4] = 'H';
    else
        trits[4] = 'L';

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

    if (speed == 1)
        speed = 0;              /* speed  1 is obsolete */
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

    for (i = 0; i < 9; i++) {
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

    syslog_bus(busnumber, DBG_DEBUG, "   sending packages ... ");

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
    queue_add(busnumber, adr, packet, QM2LOCOPKT, 18, prio); //Lok stoppen hat Priorität
    if (fx_changed) {
      //Wir berücksichtigen die Priotität auch hier weil es vorkommen kann, dass z.B. Anfahr-Bremsverzögerung ausgeschaltet werden soll um eine Lok sofort anzuhalten
      queue_add(busnumber, adr, f_packets[fx], QM2FXPKT, 18, prio); 
    }

    return 0;
}

int comp_maerklin_3(bus_t busnumber, int address, int direction,
                    int speed, int func, int f1, int f2, int f3, int f4, bool prio)
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

    syslog_bus(busnumber, DBG_DEBUG,
               "Command for Maerklin protocol type 3 (28 speed steps) (M3) received");

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

    /* compute address trits */
    for (i = 0; i < 4; i++)
        trits[i] = MotorolaCodes[address].Code[i];

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

    if (speed == 1)
        speed = 0;              /* speed  1 is obsolete */
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

    for (i = 0; i < 9; i++) {
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
    queue_add(busnumber, adr, packet, QM2LOCOPKT, 18, prio);  //Lok stoppen hat Priorität
    if (fx_changed) {
      //Wir berücksichtigen die Priotität auch hier weil es vorkommen kann, dass z.B. Anfahr-Bremsverzögerung ausgeschaltet werden soll um eine Lok sofort anzuhalten
      queue_add(busnumber, adr, f_packets[fx], QM2FXPKT, 18, prio);
    }

    return 0;
}

int comp_maerklin_4(bus_t busnumber, int address, int direction,
                    int speed, int func, int f1, int f2, int f3, int f4, bool prio)
{

    char trits[9];
    char packet[18];
    char f_packets[4][18];
    int fx = 0, fx_changed = 0;
    char *fx_packet;

    char mask[5];
    int i, j;
    int adr = 0;
    int mspeed;

    syslog_bus(busnumber, DBG_DEBUG,
               "Command for Maerklin protocol type 4 "
               "(M4, Uhlenbrock Intellibox) received");

    adr = address;
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

    /* compute address trits */
    for (i = 0; i < 4; i++)
        trits[i] = MotorolaCodes[address].Code[i];

    /* compute func trit     */
    if (func)
        trits[4] = 'H';
    else
        trits[4] = 'L';

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

    //if (speed==1) speed=0;                 /* speed  1 is obsolete */
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

    for (i = 0; i < 9; i++) {
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
    queue_add(busnumber, adr, packet, QM2LOCOPKT, 18, prio);  //Lok stoppen hat Priorität
    if (fx_changed) {
      //Wir berücksichtigen die Priotität auch hier weil es vorkommen kann, dass z.B. Anfahr-Bremsverzögerung ausgeschaltet werden soll um eine Lok sofort anzuhalten
      queue_add(busnumber, adr, f_packets[fx], QM2FXPKT, 18, prio);
    }

    return 0;
}

int comp_maerklin_5(bus_t busnumber, int address, int direction,
                    int speed, int func, int f1, int f2, int f3, int f4, bool prio)
{

    int sFS1, sFS2;
    int rtc;
    int speed_old;
    int two_commands = false;
    int acceleration = false;
    gl_data_t loco;

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

    cacheGetGL(busnumber, address, &loco);
    speed_old = loco.speed;

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

    rtc = comp_maerklin_2(busnumber, address, direction, sFS1, func, f1,
                          f2, f3, f4, prio);
    if ((sFS2 > 0) && (rtc == 0)) {
        if (usleep(50000) == -1) {
            syslog_bus(busnumber, DBG_ERROR,
                       "usleep() failed: %s (errno = %d)\n",
                       strerror(errno), errno);
        }
        rtc = comp_maerklin_2(busnumber, address, direction, sFS2, func,
                              f1, f2, f3, f4, prio);
    }
    return rtc;
}

int comp_maerklin_ms(bus_t busnumber, int address, int port, int action)
{

    char trits[9];
    char packet[2*9]; //Doppelte Grösse wegen SPI Mode
    char *p_packet;
    int i, j;
    int id, subid;

    syslog_bus(busnumber, DBG_DEBUG,
               "command for solenoid (Maerklin) (MS) received");

    /* no special error handling, it's job of the clients */
    if (address < 1 || address > 324 || action < 0 || action > 1 || port < 0 || port > 1) {
        return 1;
    }
    p_packet = packet;
    id = ((address - 1) >> 2);
    subid = (((address - 1) & 3) << 1) + port;
    if (action)
        trits[8] = 'H';
    else
        trits[8] = 'L';

    /* compute address trits */
    for (i = 0; i < 4; i++) {
        j = id % 3;
        id = id / 3;
        switch (j) {
            case 0:
                trits[i] = 'L';
                break;
            case 1:
                trits[i] = 'H';
                break;
            case 2:
                trits[i] = 'O';
                break;
        }
    }
    /* compute func trit (dummy) */
    trits[4] = 'L';
    /* compute port trits   */
    for (i = 5; i < 8; i++) {
        j = subid % 2;
        subid = subid / 2;
        switch (j) {
            case 0:
                trits[i] = 'L';
                break;
            case 1:
                trits[i] = 'H';
                break;
        }
    }

    for (i = 0; i < 9; i++) {
      if (spiMode) {
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
      else {
        switch (trits[i]) {
            case 'L':
                packet[i] = LO_38K;
                break;
            case 'H':
                packet[i] = HI_38K;
                break;
            case 'O':
                packet[i] = OP_38K;
                break;
        }
      }
    }
    queue_add(busnumber, address, p_packet, QM1SOLEPKT, spiMode ? 2*9 : 9, false);
    return 0;
}

int comp_maerklin_mf(bus_t busnumber, int address, int f1, int f2,
                     int f3, int f4)
{

    char trits[9];
    char packet[2*9]; //Doppelte Grösse wegen möglichem SPI Mode
    int i;

    syslog_bus(busnumber, DBG_DEBUG,
               "Command for func decoder (Maerklin) (MF) received");

    /* no special error handling, it's job of the clients */
    if (address < 0 || address > 80 || f1 < 0 || f1 > 1 ||
        f2 < 0 || f2 > 1 || f3 < 0 || f3 > 1 || f4 < 0 || f4 > 1)
        return 1;

    /* compute address trits */
    for (i = 0; i < 4; i++)
        trits[i] = MotorolaCodes[address].Code[i];

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

    for (i = 0; i < 9; i++) {
      if (spiMode) {
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
      else {
        switch (trits[i]) {
            case 'L':
                packet[i] = LO_38K;
                break;
            case 'H':
                packet[i] = HI_38K;
                break;
            case 'O':
                packet[i] = OP_38K;
                break;
        }
      }
    }

    queue_add(busnumber, address, packet, QM1FUNCPKT, spiMode ? 2*9 : 9, false);

    return 0;
}
