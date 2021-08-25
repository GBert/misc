// ddl_maerklin.h - adapted for basrcpd project 2018 by Rainer Müller 
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

/* signal generating functions for maerklin */

void comp_maerklin_loco(bus_t bus, gl_data_t *glp);
int comp_maerklin_ms(bus_t bus, int address, int port, int action);
int comp_maerklin_mf(bus_t bus, int address,
                     int f1, int f2, int f3, int f4);

char getMaerklinHI(void);
char getMaerklinLO(void);

#endif
