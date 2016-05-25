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
#define CAN_BAUDRATE	250
#endif

struct speed_s {
    unsigned int kbps;
    unsigned int CxCFG1;
    unsigned int CxCFG2;
};

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
 */

const static struct speed_s __attribute__ ((space(auto_psv))) speed_tab_64[] = {
    {
    .kbps = 1000, .CxCFG1 = 0x0001, .CxCFG2 = 0x03AC,}, {
    .kbps = 800,  .CxCFG1 = 0x0001, .CxCFG2 = 0x03BE,}, {
    .kbps = 500,  .CxCFG1 = 0x0003, .CxCFG2 = 0x01B5,}, {
    .kbps = 250,  .CxCFG1 = 0x0007, .CxCFG2 = 0x01B5,}, {
    .kbps = 125,  .CxCFG1 = 0x000F, .CxCFG2 = 0x01B5,}, {
    .kbps = 100,  .CxCFG1 = 0x0013, .CxCFG2 = 0x01B5,}, {
    .kbps = 50,   .CxCFG1 = 0x0027, .CxCFG2 = 0x01B5,}, {
    .kbps = 20,   .CxCFG1 = 0x003F, .CxCFG2 = 0x07BF,}, {
    .kbps = 0,}
};

#if 0
/* speed test: slow down CPU */
/*
 * Bit timing parameters for PIC24 (cmd-line) with 16.000000 MHz ref clock
 * nominal                                 real Bitrt   nom  real SampP
 * Bitrate TQ[ns] PrS PhS1 PhS2 SJW BRP Bitrate Error SampP SampP Error C1CFG1 C1CFG2
 * 1000000    125   2    3    2   1   2 1000000  0.0% 75.0% 75.0%  0.0% 0x0000 0x0191
 *  800000    125   3    4    2   1   2  800000  0.0% 80.0% 80.0%  0.0% 0x0000 0x019a
 *  500000    125   6    7    2   1   2  500000  0.0% 87.5% 87.5%  0.0% 0x0000 0x01b5
 *  250000    250   6    7    2   1   4  250000  0.0% 87.5% 87.5%  0.0% 0x0001 0x01b5
 *  125000    500   6    7    2   1   8  125000  0.0% 87.5% 87.5%  0.0% 0x0003 0x01b5
 *  100000    625   6    7    2   1  10  100000  0.0% 87.5% 87.5%  0.0% 0x0004 0x01b5
 *   50000   1250   6    7    2   1  20   50000  0.0% 87.5% 87.5%  0.0% 0x0009 0x01b5
 *   20000   3125   6    7    2   1  50   20000  0.0% 87.5% 87.5%  0.0% 0x0018 0x01b5
 *   10000   6250   6    7    2   1 100   10000  0.0% 87.5% 87.5%  0.0% 0x0031 0x01b5
 */

const static struct speed_s __attribute__ ((space(auto_psv))) speed_tab_16[] = {
    {
    .kbps = 1000, .CxCFG1 = 0x0000, .CxCFG2 = 0x0191,}, {
    .kbps = 800,  .CxCFG1 = 0x0000, .CxCFG2 = 0x019A,}, {
    .kbps = 500,  .CxCFG1 = 0x0000, .CxCFG2 = 0x01B5,}, {
    .kbps = 250,  .CxCFG1 = 0x0001, .CxCFG2 = 0x01B5,}, {
    .kbps = 125,  .CxCFG1 = 0x0003, .CxCFG2 = 0x01B5,}, {
    .kbps = 100,  .CxCFG1 = 0x0004, .CxCFG2 = 0x01B5,}, {
    .kbps = 50,   .CxCFG1 = 0x0009, .CxCFG2 = 0x01B5,}, {
    .kbps = 20,   .CxCFG1 = 0x0018, .CxCFG2 = 0x01B5,}, {
    .kbps = 10,   .CxCFG1 = 0x0031, .CxCFG2 = 0x01B5,}, {
    .kbps = 0,}
};

/*
 * Bit timing parameters for PIC24 (cmd-line) with 8.000000 MHz ref clock
 * nominal                                 real Bitrt   nom  real SampP
 * Bitrate TQ[ns] PrS PhS1 PhS2 SJW BRP Bitrate Error SampP SampP Error C1CFG1 C1CFG2
 * 1000000    250   1    1    1   1   2 1000000  0.0% 75.0% 75.0%  0.0% 0x0000 0x0080
 *  800000    250   1    2    1   1   2  800000  0.0% 80.0% 80.0%  0.0% 0x0000 0x0088
 *  500000    250   3    3    1   1   2  500000  0.0% 87.5% 87.5%  0.0% 0x0000 0x0092
 *  250000    250   6    7    2   1   2  250000  0.0% 87.5% 87.5%  0.0% 0x0000 0x01b5
 *  125000    500   6    7    2   1   4  125000  0.0% 87.5% 87.5%  0.0% 0x0001 0x01b5
 *  100000   1250   3    3    1   1  10  100000  0.0% 87.5% 87.5%  0.0% 0x0004 0x0092
 *   50000   1250   6    7    2   1  10   50000  0.0% 87.5% 87.5%  0.0% 0x0004 0x01b5
 *   20000   6250   3    3    1   1  50   20000  0.0% 87.5% 87.5%  0.0% 0x0018 0x0092
 *   10000   6250   6    7    2   1  50   10000  0.0% 87.5% 87.5%  0.0% 0x0018 0x01b5
 */

const static struct speed_s __attribute__ ((space(auto_psv))) speed_tab_8[] = {
    {
    .kbps = 1000, .CxCFG1 = 0x0000, .CxCFG2 = 0x0080,}, {
    .kbps = 800,  .CxCFG1 = 0x0000, .CxCFG2 = 0x0088,}, {
    .kbps = 500,  .CxCFG1 = 0x0000, .CxCFG2 = 0x0092,}, {
    .kbps = 250,  .CxCFG1 = 0x0000, .CxCFG2 = 0x01B5,}, {
    .kbps = 125,  .CxCFG1 = 0x0001, .CxCFG2 = 0x01B5,}, {
    .kbps = 100,  .CxCFG1 = 0x0004, .CxCFG2 = 0x0092,}, {
    .kbps = 50,   .CxCFG1 = 0x0004, .CxCFG2 = 0x01B5,}, {
    .kbps = 20,   .CxCFG1 = 0x0018, .CxCFG2 = 0x0092,}, {
    .kbps = 10,   .CxCFG1 = 0x0018, .CxCFG2 = 0x01B5,}, {
    .kbps = 0,}
};
#endif

#define CB_C1CFG1 0x0007
#define CB_C1CFG2 0x01b5

#define CAN_OPMODE_CONFIG	0x4
#define CAN_OPMODE_LOOPBACK	0x2
#define CAN_OPMODE_NORMAL	0x0
#define NUM_OF_ECAN_BUFFERS	32

int can_test_receive(void);
void can_test_send(void);
void init_can(void);

#endif /* _CAN_H_ */
