/*
 * base on:
 * Z21type.h
 * Created on: 16.04.2015
 *
 * Copyright (c) by Philipp Gahtow, year 2015
*/

/* Z21 LAN protocol specification */

#ifndef _Z21_H_
#define _Z21_H_

#define MAX(a,b)		((a) > (b) ? (a) : (b))
#define MAXDG			1500	/* maximum datagram size */
#define MAXIPLEN		40	/* maximum IP string length */
#define PRIMARY_UDP_PORT	21105
#define SECONDARY_UDP_PORT	21106
#define MAERKLIN_PORT		15731
#define MAXSIZE			16384

#define v_printf(verbose, ...) \
	do { if ((verbose)) { printf(__VA_ARGS__);} } while (0)

#define vas_printf(verbose, ...) \
	do { if ((verbose)) { asprintf(__VA_ARGS__);} } while (0)

struct z21data_t {
    int type;
    uint16_t length;
    uint16_t header;
    uint8_t *data;
};

struct z21_data_t {
    struct sockaddr_in spaddr;
    struct sockaddr_in ssaddr;
    struct sockaddr_in scaddr;
    struct sockaddr_in staddr;
    uint32_t ip;
    uint32_t bcf;
    int sp;
    int ss;
    int sc;
    int st;
    int foreground;
    int turnout_enable;
    int power;
    uint8_t loco_number;
    char *format;
    unsigned char udpframe[MAXDG];
};

#define LAN_GET_SERIAL_NUMBER		0x10
#define LAN_GET_CODE			0x18
#define LAN_GET_HWINFO			0x1A
#define LAN_LOGOFF			0x30
#define LAN_X_HEADER			0x40
#define LAN_X_GET_VERSION		0x21
#define LAN_X_GET_STATUS		0x24
#define LAN_X_SET_TRACK_POWER_OFF	0x80
#define LAN_X_SET_TRACK_POWER_ON	0x81
#define LAN_X_BC_TRACK_POWER		0x61
#define LAN_X_UNKNOWN_COMMAND		0x61
#define LAN_X_STATUS_CHANGED		0x62
#define LAN_X_SET_STOP			0x80  //AW: LAN_X_BC_STOPPED
#define LAN_X_BC_STOPPED		0x81
#define LAN_X_GET_FIRMWARE_VERSION	0xF1  //AW: 0xF3
#define LAN_SET_BROADCASTFLAGS		0x50
#define LAN_GET_BROADCASTFLAGS		0x51
#define LAN_SYSTEMSTATE_DATACHANGED	0x84
#define LAN_SYSTEMSTATE_GETDATA		0x85  //AW: LAN_SYSTEMSTATE_DATACHANGED
#define LAN_GET_LOCOMODE		0x60
#define LAN_SET_LOCOMODE		0x61
#define LAN_GET_TURNOUTMODE		0x70
#define LAN_SET_TURNOUTMODE		0x71
#define LAN_X_GET_LOCO_INFO		0xE3
#define LAN_X_SET_LOCO			0xE4  //X-Header
#define LAN_X_SET_LOCO_FUNCTION		0xF8  //DB0
#define LAN_X_LOCO_INFO			0xEF
#define LAN_X_GET_TURNOUT_INFO		0x43 
#define LAN_X_SET_TURNOUT		0x53
#define LAN_X_TURNOUT_INFO		0x43 
#define LAN_X_CV_READ			0x23
#define LAN_X_CV_WRITE			0x24
#define LAN_X_CV_NACK_SC		0x61
#define LAN_X_CV_NACK			0x61
#define LAN_X_CV_RESULT			0x64
#define LAN_RMBUS_DATACHANGED		0x80
#define LAN_RMBUS_GETDATA		0x81
#define LAN_RMBUS_PROGRAMMODULE		0x82

#define LAN_RAILCOM_DATACHANGED		0x88
#define LAN_RAILCOM_GETDATA		0x89

#define LAN_LOCONET_Z21_RX		0xA0
#define LAN_LOCONET_Z21_TX		0xA1
#define LAN_LOCONET_FROM_LAN		0xA2
#define LAN_LOCONET_DISPATCH_ADDR	0xA3
#define LAN_LOCONET_DETECTOR		0xA4

#define LAN_X_CV_POM			0xE6 	/* X-Header */

/* since Z21 FW Version 1.23	*/
#define LAN_X_MM_WRITE_BYTE		0x24

/* ab Z21 FW Version 1.25	*/
#define LAN_X_DCC_READ_REGISTER		0x22
#define LAN_X_DCC_WRITE_REGISTER	0x23

/**************************************************************/
/* Z21 BC Flags */
#define BIT(x)			(1<<x)
#define Z21bcNone		0
#define Z21bcAll		BIT(1)
#define Z21bcRBus		BIT(2)
/* #define Z21bcRailcom         0x00000004 */	/* (looka at Z21 Protokoll V1.05) */
#define Z21bcSystemInfo		BIT(3)
#define Z21bcNetAll		BIT(4)	/* Alles, auch alle Loks ohne vorher die Lokadresse abonnieren zu muesen (fuer PC Steuerung) */

#define Z21bcLocoNet		BIT(5)	/* LocoNet Meldungen an LAN Client weiterleiten (ohne Loks und Weichen) */
#define Z21bcLocoNetLocos	BIT(6)	/* Lok-spezifische LocoNet Meldungen an LAN Client weiterleiten */
#define Z21bcLocoNetSwitches	BIT(7)	/* Weichen-spezifische LocoNet Meldungen an LAN Client weiterleiten */
#define Z21bcLocoNetGBM		BIT(8)	/* Status-Meldungen von Gleisbesetztmeldern am LocoNet-Bus */

//-----------------------------------------

#endif /* _Z21_H_ */
