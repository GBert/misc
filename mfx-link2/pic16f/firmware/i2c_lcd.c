/*
 * File: i2c_lcd.c
 * Author: Alain Boudreault - ve2cuy
 *
 * Created on 6 avril 2015, 15:57
 * Note: Certains morceaux de code inspiréde http://paulfjujo.free.fr/_18FxxKxx/datas/18F26k22_LCD_PCF8574_I2C_Hardw_master_10Mhz.c
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

#include "main.h"
#include "i2c.h"
#include "lcd.h"

/// ##############################################################################################
/// Custom LCD_I2C functions

int8_t I2C_PCF8574_Write(uint8_t addr, uint8_t value) {
    int8_t s, dummy;
    i2c_start();
    s = i2c_write(addr << 1);
    if (s < 0) {		//bus collision ?
	dummy = SSPBUF;		// clear the buffer,
	SSP1CON1bits.WCOL = 0;	// clear collision status bit
    }
    s = i2c_write(value);
    i2c_stop();
    return s;
}

void LCD_putcmd(uint8_t addr, uint8_t data, uint8_t cmdtype) {
    uint8_t lcddata;

    // Write high nibble
    lcddata = HI_NIBBLE(data) | LCD_BL;
    I2C_PCF8574_Write(addr, lcddata | LCD_EN);
    I2C_PCF8574_Write(addr, lcddata & ~LCD_EN);	// Reset LCD bus

    // cmdtype = 0; One cycle write, cmdtype = 1; Two cycle writes
    if (cmdtype) {
	// Write low nibble
	lcddata = LO_NIBBLE(data) | LCD_BL;
	I2C_PCF8574_Write(addr, lcddata | LCD_EN);
	I2C_PCF8574_Write(addr, lcddata & ~LCD_EN);	// Reset LCD bus
    }
    delay_ms(2);		// For most command, Wait > 100 us is ok.
}

// Extract data high and low nible and send it to I2C LCD
void LCD_putch(uint8_t addr, uint8_t data) {
    uint8_t lcddata;
    lcddata = HI_NIBBLE(data) | LCD_BL | LCD_RS;	// Get high nibble
    I2C_PCF8574_Write(addr, lcddata | LCD_EN);	// Send it!
    I2C_PCF8574_Write(addr, lcddata & ~LCD_EN);	// Reset LCD bus
    lcddata = LO_NIBBLE(data) | LCD_BL | LCD_RS;	// Get low nibble
    I2C_PCF8574_Write(addr, lcddata | LCD_EN);	// Send it!
    I2C_PCF8574_Write(addr, lcddata & ~LCD_EN);	// Reset LCD bus
}

// Init the LCD: DATA bus 4 bits, cursor off, auto increment, no shift.
void LCD_init(uint8_t addr) {
    delay_ms(20);		// Wait > 15 ms after power ON

    LCD_putcmd(addr, LCD_INIT_BYTE, 0);
    delay_ms(5);		// Wait > 4.1 ms
    LCD_putcmd(addr, LCD_INIT_BYTE, 0);
    LCD_putcmd(addr, LCD_INIT_BYTE, 0);
    LCD_putcmd(addr, LCD_BUS_WIDTH_4Bit, 0);
    LCD_putcmd(addr, LCD_4BITS_2LINES_5x8FONT, 1);
    LCD_putcmd(addr, LCD_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF, 1);
    LCD_putcmd(addr, LCD_CLEAR, 1);
    LCD_putcmd(addr, LCD_INCREMENT_NO_SHIFT, 1);
    LCD_putcmd(addr, LCD_DISPLAY_ON_CURSOR_OFF, 1);
}

// Goto line number. On line err, goto line 1.
void LCD_goto(uint8_t addr, uint8_t row, uint8_t column) {
    switch (row) {
    case 1:
	LCD_putcmd(addr, LCD_LINE1 + (column - 1), 1);
	break;
    case 2:
	LCD_putcmd(addr, LCD_LINE2 + (column - 1), 1);
	break;
    case 3:
	LCD_putcmd(addr, LCD_LINE3 + (column - 1), 1);
	break;
    case 4:
	LCD_putcmd(addr, LCD_LINE4 + (column - 1), 1);
	break;
    default:
	LCD_putcmd(addr, LCD_LINE1 + (column - 1), 1);
	break;
    }
}

// Note: The string must be zero terminated!
// Example: char callSign[] = "ve2cuy\0";
void LCD_puts(uint8_t addr, const char *s) {
    int i = 0;
    while (*s != 0)
	LCD_putch(addr, *s++);
}
