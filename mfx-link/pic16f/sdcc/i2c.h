/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _I2C_H_
#define _I2C_H_

void i2c_start(void);
void i2c_restart(void);
void i2c_stop(void);
void i2c_sendACK(void);
int8_t i2c_write(volatile uint8_t);

#endif	/* _I2C_H_ */
