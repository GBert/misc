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

#include "config.h"
#include "i2c_lcd_states.h"

//byte Lock[8] = {
//0b01110,
//0b10001,
//0b10001,
//0b11111,
//0b11011,
//0b11011,
//0b11111,
//0b00000
//};
// Bits for character Trip 
// 11100
// 00100
// 00100
// 10101
// 01110
// 00100
// 00100
// 00111
// Bits for character +
// 00000
// 00100
// 01110
// 00100
// 00000
// 00000
// 00000
// 00000
// Bits for character -
// 00000
// 00000
// 00000
// 00100
// 00000
// 01110
// 00000
// 00000

const uint8_t lcd_userchar[] =  { LCD_SETCGRAMADDR | (LCD_USERCHAR_POSITION << 3),
                                    0x1C, 0x04, 0x04, 0x15, 0x0E, 0x04, 0x04, 0x07, //Char Trip
                                    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,  //Bargraph 1pix
                                    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,  //Bargraph 2pix
                                    0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C,  //Bargraph 3pix   
                                    0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,  //Bargraph 4pix
                                    0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,  //Bargraph 5pix (full)
                                    0x00, 0x04, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00, //small +
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, //small -    
                                  LCD_SETDDRAMADDR          
                                };

const uint8_t lcd_initseq[]=
            {LCD_INIT_BYTE,
             LCD_INIT_BYTE,
             LCD_INIT_BYTE,
             LCD_BUS_WIDTH_4Bit, 
             LCD_4BITS_2LINES_5x8FONT_H,LCD_4BITS_2LINES_5x8FONT_L,
             LCD_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF_H,LCD_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF_L,
             LCD_CLEAR_H,LCD_CLEAR_L,
			 LCD_INCREMENT_NO_SHIFT_H,LCD_INCREMENT_NO_SHIFT_L,
             LCD_DISPLAY_ON_CURSOR_OFF_H,LCD_DISPLAY_ON_CURSOR_OFF_L
            };

I2C_STATUS_T i2c_status;
uint8_t lcd_buff[LCD_ROWS * LCD_LINES];
uint8_t lcd_x;
uint8_t lcd_refreshindex;
uint8_t lcd_nibble;
uint8_t lcd_data;
LCD_STATES_T lcd_state;			 
LCD_STATES_T lcd_nextstate;	
LCD_STATES_T lcd_overnextstate;
uint16_t lcd_delay;
uint8_t lcd_writeindex;

#ifdef USE_BARGRAPH
uint8_t lcd_bargraph_valperpixel_pos;
uint8_t lcd_bargraph_valperpixel_neg;
uint16_t lcd_bargraph_value100percent_pos;
uint16_t lcd_bargraph_value100percent_neg;
#endif

void LCD_gotobufxy(uint8_t x, uint8_t y);
void LCD_putchbuf(uint8_t dat);

// ##############################################################################################
// Custom LCD_I2C functions

//I2C_STATUS_T LCD_GetStatus(void)
//{
//    return i2c_status;
//}

void LCD_Init(void)
{
    uint8_t i;
    for(i = 0; i<sizeof (lcd_buff); i++)
        lcd_buff[i] = ' ';
    lcd_refreshindex = 0; //start init sequence
    lcd_state = LCD_INIT;
    lcd_delay = 0x0400;
}


uint8_t LCD_GetIdle(void)
{
    return (lcd_state == LCD_IDLE);
}

void LCD_putchbuf(uint8_t dat)
{
    if(lcd_writeindex < sizeof (lcd_buff))
    {
        if(lcd_buff[lcd_writeindex] != dat)
        {
            lcd_buff[lcd_writeindex] = dat;
            if(lcd_writeindex < lcd_refreshindex)
            {
                lcd_refreshindex = lcd_writeindex;
            }
            i2c_status.Refresh = 1;
        }
        lcd_writeindex++;
    }
}

void LCD_gotobufxy(uint8_t x, uint8_t y)
{
    lcd_writeindex = (y - 1) * LCD_ROWS;
    lcd_writeindex += x - 1;

}

void LCD_putsbufxy(uint8_t x, uint8_t y, const char *s)
{
    LCD_gotobufxy(x, y);
    while(*s != 0)
    {
        LCD_putchbuf(*s++);
    }
}

void LCD_SetBLOn(void)
{
    i2c_status.BL_FLAG = 1;
}

void LCD_SetBLOff(void)
{
    i2c_status.BL_FLAG = 0;
}

#ifdef USE_BARGRAPH
void LCD_BargraphSetFullScale(uint16_t fullscaleval16pos, uint16_t fullscaleval16neg)
{
    lcd_bargraph_value100percent_pos = fullscaleval16pos;
    lcd_bargraph_valperpixel_pos = (uint8_t)(fullscaleval16pos / (uint16_t)BARGRAPH_PIXELSX);
    lcd_bargraph_value100percent_neg = fullscaleval16neg;
    lcd_bargraph_valperpixel_neg = (uint8_t)(fullscaleval16neg / (uint16_t)BARGRAPH_PIXELSX);
}

void LCD_DrawBargraph(uint16_t val16,uint8_t usepos)
{
    uint16_t ui16;
    uint16_t fullchars;
    uint8_t i;
    
    LCD_gotobufxy(BARGRAPH_X, BARGRAPH_Y);
    if(usepos)
    {
        if(val16>lcd_bargraph_value100percent_pos)
            val16=lcd_bargraph_value100percent_pos;
        ui16 = val16 / lcd_bargraph_valperpixel_pos; //=number of active pixels
    }
    else
    {
        if(val16>lcd_bargraph_value100percent_neg)
            val16=lcd_bargraph_value100percent_neg;
        ui16 = val16 / lcd_bargraph_valperpixel_neg; //=number of active pixels
    }
    if((ui16==0)&&(val16!=0))
        ui16++;
    fullchars = ui16 / 5;
    for(i=0; i<fullchars; i++)
    {
        LCD_putchbuf(CHAR_BG5PIX);
    }
    //calc rest pixel
    fullchars = ui16 - (5u * fullchars);
    if(fullchars)
    {
        LCD_putchbuf((CHAR_BG1PIX-1u) + (uint8_t)fullchars);
        i++;
    }
    while(i<BARGRAPH_CHARS)
    {
        LCD_putchbuf(' ');
        i++;
    }
}
#endif


void LCD_DoTask(void)
{
    switch(lcd_state)
    {
        case LCD_IDLE:
            if(i2c_status.Refresh == 1)
            {
                lcd_state = LCD_GOTO_X_Y;
            }
            return;
        case LCD_SENDCMD:
            lcd_nibble = HI_NIBBLE(lcd_data);
            i2c_status.EN_FLAG = 1;
            lcd_state = LCD_SENDNIBBLE;
            lcd_nextstate = LCD_SENDCMDLOW;
            return;
        case LCD_SENDCMDLOW:
            lcd_nibble = LO_NIBBLE(lcd_data);
            i2c_status.EN_FLAG = 1;
            lcd_state = LCD_SENDNIBBLE;
            lcd_nextstate = lcd_overnextstate;
            return;
        case LCD_PUTCH:
            if(lcd_refreshindex == sizeof (lcd_buff)) // lcd_nrofbytes == 0)
            {
                i2c_status.Initialized = 1;
                lcd_state = LCD_IDLE;
                i2c_status.Refresh = 0;
                return;
            }
            else
            {
                i2c_status.RS_FLAG = 1;
                lcd_state = LCD_SENDCMD;
                lcd_data = lcd_buff[lcd_refreshindex++];
                lcd_overnextstate = LCD_PUTCH;
                lcd_x++;
                if((lcd_x == LCD_ROWS)&&(lcd_refreshindex < sizeof (lcd_buff)))
                {
                    lcd_x = 0;
                    lcd_overnextstate = LCD_GOTO_X_Y;
                }
            }
            return;
        case LCD_SENDNIBBLE:
            if(lcd_delay)
            {
                lcd_delay--;
                return;
            }
            SEN = 1;
            lcd_state = LCD_WFSTART;
            return;
        case LCD_WFSTART:
            if(SEN == 0)
            {
                // send I2C address (write bit = 0)
                SSPBUF = LCD_01_ADDRESS << 1;
                lcd_state = LCD_WFADR;
            }
            return;

        case LCD_WFADR:
            /* wait for R/W and Buffer Full clearing */
            if((SSP1STAT & 0x05) == 0)
            { //send data
                if(i2c_status.EN_FLAG == 1)
                    lcd_nibble |= LCD_EN;
                else
                    lcd_nibble &= ~LCD_EN;
                if(i2c_status.RS_FLAG == 1)
                    lcd_nibble |= LCD_RS;
                if(i2c_status.BL_FLAG == 1)
                    lcd_nibble |= LCD_BL;
                SSPBUF = lcd_nibble;
                lcd_state = LCD_WFDAT;
            }
            return;
        case LCD_WFDAT:
            /* wait for R/W and Buffer Full clearing */
            if((SSP1STAT & 0x05) == 0)
            {
                PEN = 1; //send stop
                lcd_state = LCD_WFSTOP;
            }
            return;
        case LCD_WFSTOP:
            if(PEN == 0)
            {
                if(i2c_status.EN_FLAG == 1)
                {
                    i2c_status.EN_FLAG = 0;
                    lcd_state = LCD_SENDNIBBLE;
                    lcd_delay = 2;
                }
                else
                {
                    lcd_state = lcd_nextstate;
                    if(i2c_status.RS_FLAG == 0)
                        lcd_delay = LCD_WAIT_DELAY;
                    else
                        lcd_delay = 1;
                }
            }
            return;
        case LCD_GOTO_X_Y:
            i2c_status.RS_FLAG = 0;
            lcd_state = LCD_SENDCMD;
            lcd_overnextstate = LCD_PUTCH;
            if(lcd_refreshindex < LCD_ROWS)
            {
                lcd_data = LCD_LINE1;
                lcd_x = lcd_refreshindex;
                lcd_data += lcd_x;
                return;
            }
            else if(lcd_refreshindex < 2 * LCD_ROWS)
            {
                lcd_data = LCD_LINE2;
                lcd_x = (lcd_refreshindex - LCD_ROWS);
                lcd_data += lcd_x;
                return;
            }
            else if(lcd_refreshindex < 3 * LCD_ROWS)
            {
                lcd_data = LCD_LINE3;
                lcd_x = (lcd_refreshindex - 2 * LCD_ROWS);
                lcd_data += lcd_x;
                return;
            }
            else
            {
                lcd_data = LCD_LINE4;
                lcd_x = (lcd_refreshindex - 3 * LCD_ROWS);
                lcd_data += lcd_x;
            }
            return;
        case LCD_INIT:
            if(lcd_refreshindex == sizeof (lcd_initseq))
            {
                lcd_refreshindex = 0;
                lcd_state = LCD_CREATE_CHARS;
                return;
            }
            i2c_status.RS_FLAG = 0;
            lcd_nibble = lcd_initseq[lcd_refreshindex++];
            if(lcd_refreshindex == 2)
                lcd_delay = LCD_WAIT_INITIAL;
            lcd_nextstate = LCD_INIT;
            lcd_state = LCD_SENDNIBBLE;
            i2c_status.EN_FLAG = 1;
            return;
        case LCD_CREATE_CHARS:
            //be sure lcd_readindex is 0 before activate this state
            if(lcd_refreshindex == sizeof (lcd_userchar))
            { //all commands were sent
                lcd_state = LCD_GOTO_X_Y;
                lcd_refreshindex = 0;
                return;
            }
            lcd_data = lcd_userchar[lcd_refreshindex++];
            i2c_status.RS_FLAG = 1; //default
            lcd_overnextstate = LCD_CREATE_CHARS;
            lcd_state = LCD_SENDCMD;
            if((lcd_data & 0xC0) != 0x00)
            { //send SET CG RAM or SET DD RAM
                i2c_status.RS_FLAG = 0;
            }
            return;
    }
}
