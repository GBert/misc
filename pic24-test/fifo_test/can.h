/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _CAN_H_
#define _CAN_H_

#include "main.h"

#ifndef CAN_BAUDRATE
#define CAN_BAUDRATE	250000
#endif

/*
 * Bit timing parameters for PIC24 (cmd-line) with 64.000000 MHz ref clock
 * nominal                                 real Bitrt   nom  real SampP
 * Bitrate TQ[ns] PrS PhS1 PhS2 SJW BRP Bitrate Error SampP SampP Error C1CFG1 C1CFG2
 * 1000000     62   5    6    4   1   4 1000000  0.0% 75.0% 75.0%  0.0% 0x0001 0x03ac
 *  800000     62   7    8    4   1   4  800000  0.0% 80.0% 80.0%  0.0% 0x0001 0x03be
 *  500000    125   6    7    2   1   8  500000  0.0% 87.5% 87.5%  0.0% 0x0003 0x01b5
 *  250000    250   6    7    2   1  16  250000  0.0% 87.5% 87.5%  0.0% 0x0007 0x01b5
 *  125000    500   6    7    2   1  32  125000  0.0% 87.5% 87.5%  0.0% 0x000f 0x01b5
 *  100000    625   6    7    2   1  40  100000  0.0% 87.5% 87.5%  0.0% 0x0013 0x01b5
 *   50000   1250   6    7    2   1  80   50000  0.0% 87.5% 87.5%  0.0% 0x0027 0x01b5
 *   20000   2000   8    8    8   1 128   20000  0.0% 87.5% 68.0% 22.3% 0x003f 0x07bf
 *   10000 ***bitrate not possible***
 * 
 * Bit timing parameters for PIC24 (cmd-line) with 32.000000 MHz ref clock
 * nominal                                 real Bitrt   nom  real SampP
 * Bitrate TQ[ns] PrS PhS1 PhS2 SJW BRP Bitrate Error SampP SampP Error C1CFG1 C1CFG2
 * 1000000     62   5    6    4   1   2 1000000  0.0% 75.0% 75.0%  0.0% 0x0000 0x03ac
 *  800000     62   7    8    4   1   2  800000  0.0% 80.0% 80.0%  0.0% 0x0000 0x03be
 *  500000    125   6    7    2   1   4  500000  0.0% 87.5% 87.5%  0.0% 0x0001 0x01b5
 *  250000    250   6    7    2   1   8  250000  0.0% 87.5% 87.5%  0.0% 0x0003 0x01b5
 *  125000    500   6    7    2   1  16  125000  0.0% 87.5% 87.5%  0.0% 0x0007 0x01b5
 *  100000    625   6    7    2   1  20  100000  0.0% 87.5% 87.5%  0.0% 0x0009 0x01b5
 *   50000   1250   6    7    2   1  40   50000  0.0% 87.5% 87.5%  0.0% 0x0013 0x01b5
 *   20000   3125   6    7    2   1 100   20000  0.0% 87.5% 87.5%  0.0% 0x0031 0x01b5
 *   10000   4000   8    8    8   1 128   10000  0.0% 87.5% 68.0% 22.3% 0x003f 0x07bf
 */
#define CB_C1CFG1 0x0007
#define CB_C1CFG2 0x01b5

#define CAN_OPMODE_CONFIG	0x4
#define CAN_OPMODE_LOOPBACK	0x2
#define CAN_OPMODE_NORMAL	0x0
#define NUM_OF_ECAN_BUFFERS	32

bool can_test_receive(void);
void can_test_send(void);
void init_can(void);

#endif /* _CAN_H_ */
