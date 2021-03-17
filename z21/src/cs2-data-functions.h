/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _CS2_DATA_FUNCTIONS_H
#define _CS2_DATA_FUNCTIONS_H

#include "cs2-config.h"

uint8_t loco_get_direction(unsigned int uid);
int loco_save_direction(unsigned int uid, uint8_t direction);
unsigned int loco_get_func_summary(unsigned int uid);
uint8_t loco_get_function(unsigned int uid, uint8_t function);
int loco_save_function(unsigned int uid, uint8_t function, uint8_t value);
int loco_toggle_function(unsigned int uid, uint8_t function);
unsigned int get_magnet_type(unsigned int id);
void print_locos_by_uid(void);
unsigned int loco_get_speed(unsigned int uid);
int loco_save_speed(unsigned int uid, unsigned int speed);

#endif /* _CS2_DATA_FUNCTIONS_H */
