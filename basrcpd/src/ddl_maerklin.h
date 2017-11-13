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
/* file: maerklin.h                                            */
/* job : exports the functions from maerklin.c                 */
/*                                                             */
/* Torsten Vogt, january 1999                                  */
/*                                                             */
/* last changes: Torsten Vogt, march 2000                      */
/*               Torsten Vogt, january 2001                    */
/*                                                             */
/***************************************************************/

#ifndef __MAERKLIN_H__
#define __MAERKLIN_H__

#define WAIT_TRASH           1000
#define WAIT_BETWEEN_19K     3750        /* 3750 */
#define WAIT_END_19K         3750        /* 3750 */
#define WAIT_BETWEEN_38K     2570        /* 1875 */
#define WAIT_END_38K         2570        /* 1875 */

/* signal generating functions for maerklin */
int comp_maerklin_1(bus_t bus, int address, int direction, int speed, int func, bool prio);
int comp_maerklin_2(bus_t bus, int address, int direction, int speed, int func,
                    int f1, int f2, int f3, int f4, bool prio);
int comp_maerklin_3(bus_t bus, int address, int direction, int speed, int func,
                    int f1, int f2, int f3, int f4, bool prio);
int comp_maerklin_4(bus_t bus, int address, int direction, int speed, int func,
                    int f1, int f2, int f3, int f4, bool prio);
int comp_maerklin_5(bus_t bus, int address, int direction, int speed, int func,
                    int f1, int f2, int f3, int f4, bool prio);
int comp_maerklin_ms(bus_t bus, int address, int port, int action);
int comp_maerklin_mf(bus_t bus, int address,
                     int f1, int f2, int f3, int f4);

int protocol_maerklin_sm_write_phregister (bus_t bus, int addr, int speed);

/**
  * Setzen SPI Mode zur Berechnung der zu sendenen Datenpakete.
  * Da diese bei SPI anders sein müssen (siehe Kommentar oben).
  * @param newSpiMode true=SPI, false=RS232 (default).
  */
void setSPIModeMaerklin(bool newSpiMode);

#endif
