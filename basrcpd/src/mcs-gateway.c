// mcs-gateway.c : gateway between Märklin CS CAN commands and srcpd
//
// C 2018 Rainer Müller 
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).

//#include <errno.h>
//#include <sys/socket.h>
//#include <sys/ioctl.h>

#include <net/if.h>
#include <netinet/in.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>
//#include <unistd.h>

#include "ddl.h"
//#include "config-srcpd.h"
//#include "canbus.h"
//#include "toolbox.h"
//#include "srcp-fb.h"
//#include "srcp-ga.h"
//#include "srcp-gl.h"
//#include "srcp-sm.h"
#include "srcp-power.h"
//#include "srcp-server.h"
#include "srcp-info.h"
//#include "srcp-error.h"
#include "syslogmessage.h"

#define INVALID_SOCKET -1
#define __DDL ((DDL_DATA*)buses[busnumber].driverdata)

static bus_t mcs_bus = 0;
static int fd = INVALID_SOCKET;
static pthread_t mcsGatewayThread;

static unsigned char M_SOFTVERS[]	= {0x00, 0x01, 0x9B, 0x32, 0x02, 0x05, 0x00, 0x00};

static inline uint16_t be16(uint8_t *u) {
    return (u[0] << 8) | u[1];
}

static inline uint32_t be32(uint8_t *u) {
    return (u[0] << 24) | (u[1] << 16) | (u[2] << 8) | u[3];
}

/* thread cleanup routine */
static void end_handleCAN(void *arg)
{
    close(fd);
    fd = INVALID_SOCKET;
	syslog_bus(mcs_bus, DBG_INFO, "MCS gateway Thread ended");
	mcs_bus = 0;
}

/* main thread routine */
void *thr_handleCAN(void *vp)
{
    fd_set rfds;
	struct timeval tv;
  	struct can_frame frame;
  	int nready, v;
  	char msg[64];
  	uint32_t uid;

	sleep(1);
	syslog_bus(mcs_bus, DBG_INFO, "MCS gateway Thread started");
    pthread_cleanup_push((void *) end_handleCAN, NULL);	
    FD_ZERO(&rfds);

    while (true) {
    	FD_SET(fd, &rfds);
    	tv.tv_sec = 1;
    	tv.tv_usec = 0;
    	nready = select(fd+1, &rfds, NULL, NULL, &tv); 

		if(nready < 0) 
	    	syslog_bus(mcs_bus, DBG_ERROR, "select exception: [%d] %s",
					nready, strerror(errno));
		/* send PING via CAN interface */
		else if (nready == 0) {
			frame.can_id = 0x00300300 | CAN_EFF_FLAG;
			frame.can_dlc = 0;
			if (write(fd, &frame, sizeof(struct can_frame)) < 0)
					syslog_bus(mcs_bus, DBG_ERROR, "sending PING frame error: %s", 
									strerror(errno));
		}
		/* receive a CAN frame */
		if(FD_ISSET(fd, &rfds)) {
	    	/* reading via SockatCAN */
	    	if (read(fd, &frame, sizeof(struct can_frame)) < 0) {
				syslog_bus(mcs_bus, DBG_ERROR, "reading CAN frame error: %s", 
					strerror(errno));
	    	}				
	   		else if(frame.can_id & CAN_EFF_FLAG) {
//				printf("0x%08X [%d]\n", frame.can_id & CAN_EFF_MASK, frame.can_dlc);
				uid = be32(frame.data);
	    		switch ((frame.can_id & 0x00FF0000UL) >> 16) {
				/* System Befehle */
				case 0x00:	switch (frame.data[4]) {
						// Stopp
    					case 0x00:	setPower(mcs_bus, 0, "BY MCS");	
									continue;		// reply later
    					// Go
    					case 0x01:  setPower(mcs_bus, 1, "BY MCS");
									continue;		// reply later
    					// Emergency Stopp
    					case 0x03:  handle_mcs_dir(mcs_bus, uid, 4);
									break;
						// Protokoll  
    					case 0x05:  if (handle_mcs_prot(mcs_bus, uid, frame.data[5])
											< 0) continue;
									break;
						// Neuanmeldezähler setzen
						case 0x09:	handle_regcnt(mcs_bus, be16(&frame.data[5]));
									break;
						/* discarded without notice */
						case 0x20:	// System Time
									continue;
    					// unhandled codes
			    		default:	goto defmsg;
						}
						break;
				/* mfx bind / verify */
				case 0x04:  handle_mfx_bind_verify(mcs_bus, 0, uid,
													be16(&frame.data[4]));
							continue;		// reply later
				case 0x06:	handle_mfx_bind_verify(mcs_bus, 1, uid,
													be16(&frame.data[4]));
							continue;		// reply later
				/* Lok Geschwindigkeit */
				case 0x08:  v = be16(&frame.data[4]);
							v = handle_mcs_speed(mcs_bus, uid, 
									(frame.can_dlc == 6) ? v : -1);
							break;
				/* Lok Richtung */
				case 0x0A:  v = handle_mcs_dir(mcs_bus, uid, 
									(frame.can_dlc == 5) ? frame.data[4] : -1);
							frame.data[4] = v;
							if (v >= 0) frame.can_dlc = 5;
							break;
				/* Lok Funktion */
				case 0x0C:  v = handle_mcs_func(mcs_bus, uid, frame.data[4],
				 					(frame.can_dlc > 5) ? frame.data[5] : -1);
							frame.data[5] = v;
							if (v >= 0) frame.can_dlc = 6;
							break;
				/* read / write config */
				case 0x0E:
				case 0x10:	syslog_bus(mcs_bus, DBG_INFO,
									"***** read / write config fehlt noch");
							continue;
				/* Ping */
				case 0x30:  frame.can_dlc = 8; 
				            memcpy(&frame.data, M_SOFTVERS, 8);
				            break;
				/* discarded without notice */
				case 0x31:  // Ping response
				case 0x36:	// Bootloader CAN
							continue;
    			/* unhandled codes */
				default:
    			defmsg:		sprintf(msg, "0x%08X [%d]", 
									frame.can_id & CAN_EFF_MASK, frame.can_dlc);
    						for (int i = 0; i < frame.can_dlc; i++) 
								sprintf(msg+14+3*i, " %02X", frame.data[i]);
							syslog_bus(mcs_bus, DBG_INFO, "unexpected %s", msg);
							continue;
				}
				// send response
				frame.can_id |= 0x00010000UL;		// set response bit
				if (write(fd, &frame, sizeof(struct can_frame)) < 0)
					syslog_bus(mcs_bus, DBG_ERROR,
								"sending CAN frame error in MCS line %d: %s",
								__LINE__, strerror(errno));
			}
    	}
	}
    pthread_cleanup_pop(1);
    return NULL;
}

// send synchronisation info via CAN 
int info_mcs(bus_t bus, uint16_t infoid, uint32_t itemid, char * info)
{
  	struct can_frame frame;
  	
	if (bus != mcs_bus) return -1;
	frame.can_id = ((infoid << 16) + 0x300) | CAN_EFF_FLAG;
	frame.can_dlc = info[0] + 4;
	uint32_t nid = htonl(itemid);
    memcpy(&frame.data[0], &nid, 4);	
    memcpy(&frame.data[4], &info[1], info[0]);
	// send info frame
	if (write(fd, &frame, sizeof(struct can_frame)) < 0)
		syslog_bus(mcs_bus, DBG_ERROR, "sending CAN frame error in MCS line %d: %s",
						__LINE__, strerror(errno));
	return 0;
}

void init_mcs_gateway(bus_t busnumber)
{
    struct sockaddr_can addr;
    struct ifreq ifr;
    int ret;
    
	syslog_bus(busnumber, DBG_INFO, "MCS Gateway will use device %s.",
               	__DDL->MCS_DEVNAME);
    
    fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	strcpy(ifr.ifr_name, __DDL->MCS_DEVNAME);
    ret = ioctl(fd, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if(ret == 0) ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    
    if(ret) {
        close(fd);
		fd = INVALID_SOCKET;
		syslog_bus(busnumber, DBG_ERROR, "error binding CAN socket: %s",
				strerror(errno));
        return;
    }

	mcs_bus = busnumber;	
	ret = pthread_create(&mcsGatewayThread, NULL, thr_handleCAN, NULL);
    if (ret) syslog_bus(busnumber, DBG_ERROR,
               	"pthread_create mcsGatewayThread fail");
}

void term_mcs_gateway(void)
{
    int ret;
	ret = pthread_cancel(mcsGatewayThread);
    if (ret) syslog_bus(mcs_bus, DBG_ERROR,
               	"pthread_cancel mcsGatewayThread fail");
}

// Da gibts noch einiges zu erledigen:
// TODO:	Lok-Antworten in Responsepaket einarbeiten
