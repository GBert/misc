/*
 * File: lcd.h
 * Author: Alain Boudreault - ve2cuy
 *
 * Created on 6 avril 2015, 15:57
 *
 * -----------------------------------------------------------------------------
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LCD_H
#define LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
/// ###### I2C LCD defines ######
#define LCD_WAIT_DELAY 2
#define LCD_BL 0x08
#define LCD_EN 0x04
#define LCD_RW 0x02
#define LCD_RS 0x01
// LCD Command
#define LCD_INIT_BYTE 0x30
#define LCD_BUS_WIDTH_4Bit 0x20
#define LCD_BUS_WIDTH_8Bit 0x30
#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ON 0x0C
#define LCD_OFF 0x08
#define LCD_LINE1 0x80
#define LCD_LINE2 0xC0
#define LCD_LINE3 0x94
#define LCD_LINE4 0xD4
#define LCD_CURSOR_OFF 0x0C
#define LCD_UNDERLINE_ON 0x0E
#define LCD_BLINK_CURSOR_ON 0x0F
#define LCD_MOVE_CURSOR_LEFT 0x10
#define LCD_MOVE_CURSOR_RIGHT 0x14
#define LCD_SHIFT_LEFT 0x18
#define LCD_SHIFT_RIGHT 0x1E
#define LCD_DISPLAY_ON_CURSOR_OFF 0x0c
#define LCD_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF 0x08
#define LCD_4BITS_2LINES_5x8FONT 0x28
#define LCD_INCREMENT_NO_SHIFT 0x06
#define Byte unsigned char
#define LO_NIBBLE(b) (((b) << 4) & 0xF0)
#define HI_NIBBLE(b) ((b) & 0xF0)
// function prototypes unsigned char I2C_PCF8574_Write(Byte Adr, Byte value);
void LCD_init(unsigned char addr);
void LCD_putcmd(unsigned char addr, unsigned char data, unsigned char cmdtype);
void LCD_putch(unsigned char addr, unsigned char data);
void LCD_puts(unsigned char addr, const char *s);
void LCD_goto(unsigned char addr, char row, char column);

#endif /* LCD_H */
