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

#include "config.h"
#include "ddl.h"
//#include "config-srcpd.h"
//#include "canbus.h"
//#include "toolbox.h"
//#include "srcp-fb.h"
//#include "srcp-ga.h"
//#include "srcp-gl.h"
#include "srcp-sm.h"
#include "srcp-power.h"
//#include "srcp-server.h"
#include "srcp-info.h"
//#include "srcp-error.h"
#include "syslogmessage.h"
#include "platform.h"

#define INVALID_SOCKET -1
#define __DDL ((DDL_DATA*)buses[busnumber].driverdata)

static bus_t mcs_bus = 0;
static int pingactive = 0;
static int fd = INVALID_SOCKET;
static uint32_t ownuid, ownhash = 0;
static pthread_t mcsGatewayThread;

// configuration data: basrcpd Device with temperature reporting
static unsigned char softvers[8] = {0x00};
static char gfpstkonf0H[8]  = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static char gfpstkonf0N[8]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static char *gfpstkonf0[] = {gfpstkonf0H, "60473\0\0", "basrcpd", gfpstkonf0N, NULL};

static char gfpstkonf10[8]  = {0x01, 0xFF, 0x31, 0xF2, 0xC3, 0xC3, 0x00, 0x96 };
static char gfpstkonf11[8]  = {0x01, 0xC2, 0x02, 0x58, 0x02, 0xBC, 0x02, 0xEE };
static char *gfpstkonf1[] = {gfpstkonf10, gfpstkonf11, "TEMP\0" "15", "75\0°C\0", NULL};

// type conversion routines
static inline uint16_t be16(uint8_t *u) {
    return (u[0] << 8) | u[1];
}

static inline uint32_t be32(uint8_t *u) {
    return (u[0] << 24) | (u[1] << 16) | (u[2] << 8) | u[3];
}

// send data blocks via CAN
static int send_datablock(char *info[])
{
	int pkts = 0;
  	struct can_frame frame;
	frame.can_dlc = 8;

	while (info[pkts]) {
    	memcpy(frame.data, info[pkts++], 8);
		frame.can_id = (0x003B0300 + pkts) | CAN_EFF_FLAG;
		// send data block frame
		if (write(fd, &frame, sizeof(struct can_frame)) < 0)
			syslog_bus(mcs_bus, DBG_ERROR, "sending CAN frame error in MCS line %d: %s",
						__LINE__, strerror(errno));
	}
	return pkts;
}

/* thread cleanup routine */
static void end_handleCAN(void *arg)
{
    close(fd);
    fd = INVALID_SOCKET;
	syslog_bus(mcs_bus, DBG_INFO, "MCS gateway Thread ended.");
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
    uint16_t sid;
    uint16_t accactive = 0;

	sleep(1);
	syslog_bus(mcs_bus, DBG_INFO, "MCS gateway Thread started.");
    pthread_cleanup_push((void *) end_handleCAN, NULL);
    FD_ZERO(&rfds);

    while (true) {
    	FD_SET(fd, &rfds);
    	tv.tv_sec = 1;
    	tv.tv_usec = 0;
    	nready = select(fd+1, &rfds, NULL, NULL, &tv);

		if(nready < 0)
	    	syslog_bus(mcs_bus, DBG_ERROR, "select exception in MCS line %d: %s",
								__LINE__, strerror(errno));
		/* send PING via CAN interface */
		else if (nready == 0) {
			if (pingactive > 0) pingactive--;
			frame.can_id = 0x00300000 | ownhash | CAN_EFF_FLAG;
			frame.can_dlc = 0;
			if (write(fd, &frame, sizeof(struct can_frame)) < 0)
					syslog_bus(mcs_bus, DBG_ERROR, "sending PING frame error: %s",
									strerror(errno));
		}
		/* receive a CAN frame */
		else if(FD_ISSET(fd, &rfds)) {
	    	/* reading via SockatCAN */
	    	if (read(fd, &frame, sizeof(struct can_frame)) < 0) {
				syslog_bus(mcs_bus, DBG_ERROR, "reading CAN frame error: %s",
					strerror(errno));
	    	}
            else if((frame.can_id & 0xFE000380) == (CAN_EFF_FLAG | 0x300)) {
				uid = be32(frame.data);
				if ((frame.can_id & 0x0001FFFFUL) == ownhash)
					syslog_bus(mcs_bus, DBG_WARN,
							"*** Own hash value received in: 0x%08X.", frame.can_id);
	    		switch ((frame.can_id & 0x01FF0000UL) >> 16) {
				/* System Befehle */
				case 0x00:	if (frame.can_dlc < 5) {	// Query Stopp/Go
								if ((uid != 0) && (uid != ownuid)) continue;
								frame.can_dlc = 5;
								memcpy(frame.data, softvers, 4);	// uid
								frame.data[4] = getPower(mcs_bus);
								break;
							}
					switch (frame.data[4]) {
						// Stopp
    					case 0x00:	if ((uid != 0) && (uid != ownuid)) continue;
									setPower(mcs_bus, 0, "BY MCS");
									continue;		// reply after done
    					// Go
    					case 0x01:  if ((uid != 0) && (uid != ownuid)) continue;
									setPower(mcs_bus, 1, "BY MCS");
									continue;		// reply after done
    					// Emergency Stopp
    					case 0x03:  handle_mcs_dir(mcs_bus, uid, 4);
									break;
						// Protokoll
    					case 0x05:  if (handle_mcs_prot(mcs_bus, uid, frame.data[5])
											< 0) continue;
									break;
						// accessory active time in multiples of 10ms
						case 0x06:	if ((uid != 0) && (uid != ownuid)) continue;
									accactive = be16(&frame.data[5]);
									break;
						// Rail protocol selection
						case 0x08:  syslog_bus(mcs_bus, DBG_INFO,
										"*** Select protocol %x not yet implemented",
										frame.data[5]);
									break;
						// Neuanmeldezähler setzen
						case 0x09:	handle_mfx_bind_verify(mcs_bus, SET,
													be16(&frame.data[5]), 0);
									continue;		// reply after done
						// System State
						case 0x0B:	if (memcmp(frame.data, softvers, 4)) continue;
									if (frame.data[5] == 1) {
										v = getPlatformData(PL_TEMP)/100;
										frame.can_dlc = 8;
										frame.data[6] = v >> 8;
										frame.data[7] = v & 0xFF;
									}
									break;
						/* discarded without notice */
						case 0x20:	// System Time
									continue;
    					// unhandled codes
			    		default:	goto defmsg;
						}
						break;
				/* discovery */
				case 0x02:	switch (frame.can_dlc) {
                                case 1: handle_mcs_discovery(mcs_bus, frame.data[0], 0);
                                        break;
                                case 5: handle_mcs_discovery(mcs_bus, frame.data[4], uid);
                                        break;
                            }
							continue;			// reply after done
				/* mfx bind / verify */
				case 0x04:	sid = be16(&frame.data[4]);
							if ((sid < 1) || (sid > 0x3fff)) goto defmsg;
							handle_mfx_bind_verify(mcs_bus, SET, uid, sid);
							continue;			// reply after done
				case 0x06:	sid = be16(&frame.data[4]);
							if ((sid < 1) || (sid > 0x3fff)) goto defmsg;
							handle_mfx_bind_verify(mcs_bus, VERIFY, uid, sid);
							continue;			// reply after done
				/* Lok Geschwindigkeit */
				case 0x08:  v = be16(&frame.data[4]);
							v = handle_mcs_speed(mcs_bus, uid,
									(frame.can_dlc == 6) ? v : -1);
							if ((frame.can_dlc == 4) && (v >= 0)) {
								frame.can_dlc = 6;
								frame.data[4] = v >> 8;
								frame.data[5] = v & 0xFF;
							}
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
				/* read config */
				case 0x0E:  v = be16(&frame.data[4]) & 0x3FF;
							handle_mcs_config(mcs_bus, GET, uid, v, frame.data[4]>>2,
													frame.data[6], 0);
							continue;			// reply after done
				/* write config */
				case 0x10:	v = be16(&frame.data[4]) & 0x3FF;
							handle_mcs_config(mcs_bus, SET, uid, v, frame.data[4]>>2,
													frame.data[6], frame.data[7]);
							continue;			// reply after done
				/* switch accessories */
				case 0x16:  if ((uid < 0x3000) || (uid > 0x3FFF)) continue;
							if (frame.data[5] == 0) v = 0;	// activetime calculation
							else v = 10 * ((frame.can_dlc == 8) ?
												be16(&frame.data[6]) : accactive);
							if  (uid & 0x800) 		// DCC
								handle_mcs_gacc(
									((DDL_DATA*)buses[mcs_bus].driverdata)->FWD_N_ACCESSORIES,
									'N', (uid & 0x7FF) + 1, frame.data[4], frame.data[5], v);
							else  					// MM
								handle_mcs_gacc(
									((DDL_DATA*)buses[mcs_bus].driverdata)->FWD_M_ACCESSORIES,
									'M', (uid & 0x7FF) + 1, frame.data[4], frame.data[5], v);
							continue;			// reply after done
				/* Ping */
				case 0x30:  frame.can_dlc = 8;
				            memcpy(frame.data, softvers, 8);
				            break;
				case 0x31:  // Ping response
							v = be16(&frame.data[6]);
							// printf("Ping response from type %04X.\n", v);
							switch (v & 0xFFF0) {
							case 0x0030:	// MS2
							case 0xFFF0:	// CS2-GUI
									pingactive = 3;
									break;
							}
							continue;
				/* discarded without notice */
				case 0x01:  if (frame.data[4] != 0x0C) goto defmsg;	// Gerätekennung
				case 0x36:	// Bootloader CAN
							continue;
				/* status data config */
				case 0x3A:	if (memcmp(frame.data, softvers, 4)) continue;
							switch (frame.data[4]) {
							case 0x00:  frame.data[5] = send_datablock(gfpstkonf0);
										frame.can_dlc = 6;
										break;
							case 0x01:  frame.data[5] = send_datablock(gfpstkonf1);
										frame.can_dlc = 6;
										break;
							default:	goto defmsg;
							}
				            break;
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
				frame.can_id = (frame.can_id & ~0xffff) | ownhash | 0x00010000UL;
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
	frame.can_id = (infoid << 16) | ownhash | CAN_EFF_FLAG;
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

  	// store own UID and calculate own hash
  	ownuid = __DDL->uid;
	uint32_t ouid = htonl(ownuid);
	memcpy(softvers, &ouid, 4);
	ownhash = (((ouid >> 16) ^ ouid) & 0xff7f) | 0x300;

	// and add version and shorted serial
    int version = atoi(VERSION);
    softvers[4] = version / 100;
    softvers[5] = version % 100;
   	uint32_t serial = htonl(getPlatformData(PL_SERIAL) % 1000000L);
    memcpy(&gfpstkonf0H[4], &serial, 4);

	syslog_bus(busnumber, DBG_INFO, "MCS Gateway will use device %s and hash %x.",
               	__DDL->MCS_DEVNAME, ownhash);

	if (fd > INVALID_SOCKET) return;		// already done
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

/* get the info about active pings */
int get_pingactive(void)
{
	return pingactive;
}
