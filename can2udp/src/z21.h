/* 
 * File:   z21.h
 * Author: johannes
 *
 * Created on 7. Februar 2015, 18:54
 *
 * https://raw.githubusercontent.com/schabauerj/Roco_Z21_Arduino/master/z21.h
 */

#ifndef Z21_H
#define	Z21_H

/*#ifdef	__cplusplus
extern "C" {
#endif*/

#define	Z21_UDP_PORT1			21105
    
#define LAN_GET_SERIAL_NUMBER		0x10
#define LAN_GET_CONFIG			0x12
#define LAN_GET_HWINFO			0x1A
#define LAN_LOGOFF			0x30
    
#define LAN_XPRESS_NET			0x40
    
#define LAN_X_GENERAL			0x21
#define LAN_X_GET_VERSION		0x21
#define LAN_X_GET_STATUS		0x24
#define LAN_X_SET_TRACK_POWER_OFF 	0x80
#define LAN_X_SET_TRACK_POWER_ON  	0x81
    
#define LAN_X_CV_READ_0			0x23
#define LAN_X_CV_READ_1			0x11
    
#define LAN_X_CV_WRITE_0		0x24
#define LAN_X_CV_WRITE_1		0x12
#define LAN_X_GET_TURNOUT_INFO		0x43
#define LAN_X_SET_TURNOUT		0x53
#define LAN_X_SET_STOP			0x80
    
#define LAN_X_GET_LOCO_INFO_0		0xE3
#define LAN_X_GET_LOCO_INFO_1		0xF0
    
#define LAN_X_SET_LOCO_FUNCTION_0 	0xE4
#define LAN_X_SET_LOCO_FUNCTION_1 	0xF8
   
#define LAN_X_CV_POM			0xE6    
#define LAN_X_CV_POM_WRITE		0x30
#define LAN_X_CV_POM_WRITE_BYTE		0xEC
#define LAN_X_CV_POM_WRITE_BIT		0xE8

#define LAN_X_GET_FIRMWARE_VERSION 	0xF1
    
#define LAN_SET_BROADCASTFLAGS		0x50
#define LAN_GET_BROADCASTFLAGS		0x51
#define LAN_GET_LOCOMODE		0x60
#define LAN_SET_LOCOMODE		0x61
#define LAN_GET_TURNOUTMODE		0x70
#define LAN_SET_TURNOUTMODE		0x71
#define LAN_RMBUS_GETDATA		0x81
#define LAN_RMBUS_PROGRAMMODULE		0x82
#define LAN_SYSTEMSTATE_GETDATA		0x85
#define LAN_RAILCOM_GETDATA		0x89

#define LAN_LOCONET_FROM_LAN		0xA2
#define LAN_LOCONET_DISPATCH_ADDR	0xA3
/*#ifdef	__cplusplus
}
#endif*/

#endif	/* Z21_H */

