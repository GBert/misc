// ddl-nmra.h - adapted for basrcpd project 2018 by Rainer Müller 

/* +----------------------------------------------------------------------+ *
 * | DDL - Digital Direct for Linux                                       | *
 * +----------------------------------------------------------------------+ *
 * | Copyright (c) 2002 - 2003 Vogt IT                                    | *
 * +----------------------------------------------------------------------+ *
 * | This source file is subject of the GNU general public license 2,     | *
 * | that is bundled with this package in the file COPYING, and is        | *
 * | available at through the world-wide-web at                           | *
 * | http://www.gnu.org/licenses/gpl.txt                                  | *
 * | If you did not receive a copy of the PHP license and are unable to   | *
 * | obtain it through the world-wide-web, please send a note to          | *
 * | gpl-license@vogt-it.com so we can mail you a copy immediately.       | *
 * +----------------------------------------------------------------------+ *
 * | Authors: Torsten Vogt <vogt@vogt-it.com>                             | *
 * |                                                                      | *
 * +----------------------------------------------------------------------+ */

/***************************************************************
 * erddcd - Electric Railroad Direct Digital Command Daemon    *
 *    generates without any other hardware digital commands    *
 *    to control electric model railroads                      *
 *                                                             *
 * file: maerklin.h                                            *
 * job : exports the functions from nmra.c                     *
 *                                                             *
 * Torsten Vogt, june 1999                                     *
 *                                                             *
 * last changes: Torsten Vogt, march 2000                      *
 *                                                             *
 ***************************************************************/

#ifndef DDL_NMRA_H
#define DDL_NMRA_H

int translateBitstream2Packetstream(bus_t busnumber, char *Bitstream,
                                    char *Packetstream);

/* signal generating functions for nmra dcc */

/* NMRA multi function decoder */
void comp_nmra_multi_func(bus_t busnumber, gl_data_t *glp);

/* NMRA accessory decoder     */
int comp_nmra_accessory(bus_t busnumber, int nr, int output, int activate,
                        int offset);

/* service mode functions */
int protocol_nmra_sm_write_cvbyte(bus_t busnumber, int cv, int value);
int protocol_nmra_sm_verify_cvbyte(bus_t busnumber, int cv, int value);
int protocol_nmra_sm_get_cvbyte(bus_t busnumber, int cv);
int protocol_nmra_sm_write_cvbit(bus_t busnumber, int cv, int bit,
                                 int value);
int protocol_nmra_sm_verify_cvbit(bus_t busnumber, int cv, int bit,
                                  int value);
int protocol_nmra_sm_write_phregister(bus_t busnumber, int reg, int value);
int protocol_nmra_sm_verify_phregister(bus_t busnumber, int reg,
                                       int value);
int protocol_nmra_sm_get_phregister(bus_t busnumber, int reg);
int protocol_nmra_sm_write_cvbyte_pom(bus_t busnumber, int addr, int cv,
                                      int value, int mode);
int protocol_nmra_sm_write_cvbit_pom(bus_t busnumber, int addr, int cv,
                                     int bit, int value, int mode);
int protocol_nmra_sm_write_page(bus_t busnumber, int cv, int value);
int protocol_nmra_sm_verify_page(bus_t busnumber, int cv, int value);
int protocol_nmra_sm_get_page(bus_t busnumber, int cv);

#endif
