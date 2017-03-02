/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _I2C_LCD_H_
#define _I2C_LCD_H_

#define LCD_01_ADDRESS	0x40

void LCD_init(unsigned char addr);
void LCD_putcmd(unsigned char addr, unsigned char data, unsigned char cmdtype);

#endif	/* _I2C_LCD_H_ */
