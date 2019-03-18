/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* timing defines */

/* DCC DC     58us            */
/* MM1 M1     26us            */
/* MM2 M2     52us            */
/* mfx M4     50us            */
/* slx SX     40us + 10us off */

const far unsigned char dcc_lo[] @ 0x1000 = {  1, 3,  58,  58 };
const far unsigned char dcc_hi[] @ 0x1004 = {  1, 3, 116, 116 };
const far unsigned char mm1_lo[] @ 0x1008 = {  1, 1,  26, 182 };
const far unsigned char mm1_hi[] @ 0x100c = {  1, 2, 182,  26 };
const far unsigned char mm2_lo[] @ 0x1010 = {  1, 1,  13,  91 };
const far unsigned char mm2_hi[] @ 0x1014 = {  1, 2,  91,  13 };
const far unsigned char mfx_lo[] @ 0x1018 = {  1, 3,  50,  50 };
const far unsigned char mfx_hi[] @ 0x101c = {  1, 3, 100, 100 };
const far unsigned char slx_xx[] @ 0x1020 = {  3, 3,  40,  10 };
