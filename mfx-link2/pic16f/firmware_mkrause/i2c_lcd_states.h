/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _I2C_LCD_STATES_H_
#define _I2C_LCD_STATES_H_

#include "config.h"

#define LCD_ROWS 16
#define LCD_LINES 2

/* I2C LCD defines */


#define LCD_BL 0x08
#define LCD_EN 0x04
#define LCD_RW 0x02
#define LCD_RS 0x01

/* LCD Commands */
#define LCD_INIT_BYTE		0x30
#define LCD_BUS_WIDTH_4Bit	0x20
#define LCD_BUS_WIDTH_8Bit	0x30

#define LCD_CLEAR		0x01
#define LCD_CLEAR_H		0x00
#define LCD_CLEAR_L		0x10

#define LCD_HOME		0x02

#define LCD_ON			0x0C
#define LCD_OFF			0x08
#define LCD_LINE1		0x80
#define LCD_LINE2		0xC0
#define LCD_LINE3		0x94
#define LCD_LINE4		0xD4
#define LCD_CURSOR_OFF		0x0C
#define LCD_UNDERLINE_ON	0x0E
#define LCD_BLINK_CURSOR_ON	0x0F
#define LCD_MOVE_CURSOR_LEFT	0x10
#define LCD_MOVE_CURSOR_RIGHT	0x14
#define LCD_SHIFT_LEFT		0x18
#define LCD_SHIFT_RIGHT		0x1E
#define LCD_DISPLAY_ON_CURSOR_OFF 0x0c
#define LCD_DISPLAY_ON_CURSOR_OFF_H 0x00
#define LCD_DISPLAY_ON_CURSOR_OFF_L 0xc0

#define LCD_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF 0x08
#define LCD_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF_H 0x00
#define LCD_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF_L 0x80

#define LCD_4BITS_2LINES_5x8FONT 0x28
#define LCD_4BITS_2LINES_5x8FONT_H 0x20
#define LCD_4BITS_2LINES_5x8FONT_L 0x80

#define LCD_INCREMENT_NO_SHIFT	0x06
#define LCD_INCREMENT_NO_SHIFT_H	0x00
#define LCD_INCREMENT_NO_SHIFT_L	0x60

#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

#define LO_NIBBLE(b) (((b) << 4) & 0xF0)
#define HI_NIBBLE(b) ((b) & 0xF0)

#define LCD_USERCHAR_POSITION 0
#define CHAR_TRIP  LCD_USERCHAR_POSITION
#define CHAR_BG1PIX LCD_USERCHAR_POSITION + 1
#define CHAR_BG5PIX CHAR_BG1PIX + 4
#define BARGRAPH_PIXELSX (BARGRAPH_CHARS * 5)
#define CHAR_POS LCD_USERCHAR_POSITION + 6
#define CHAR_NEG LCD_USERCHAR_POSITION + 7
typedef struct
{
	unsigned EN_FLAG:1;
	unsigned RS_FLAG:1;
    unsigned BL_FLAG:1; //back light LED on off
	unsigned Initialized:1;
	unsigned Refresh:1;
} I2C_STATUS_T;

typedef enum {LCD_IDLE=0,  LCD_SENDCMD,
        LCD_SENDCMDLOW, LCD_PUTCH, LCD_SENDNIBBLE, LCD_WFSTART,
        LCD_WFADR, LCD_WFDAT, LCD_WFSTOP, LCD_GOTO_X_Y, LCD_INIT, LCD_CREATE_CHARS} LCD_STATES_T;

void LCD_Init(void);
I2C_STATUS_T LCD_GetStatus(void);
uint8_t LCD_GetIdle(void);
void LCD_DoTask(void);
void LCD_putchbuf(uint8_t dat);
void LCD_gotobufxy(uint8_t x, uint8_t y);
void LCD_putsbufxy(uint8_t x, uint8_t y, const char *s);
void LCD_SetBLOn(void);
void LCD_SetBLOff(void);
void LCD_BargraphSetFullScale(uint16_t fullscaleval16pos,uint16_t fullscaleval16neg);
void LCD_DrawBargraph(uint16_t val16,uint8_t usepos);
#endif	/* _I2C_LCD_H_ */
