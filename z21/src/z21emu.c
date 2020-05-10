/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return.
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/*
 * Z21 Emulation for Roco WiFi Mouse
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <linux/can.h>

#include "subscriber.h"
#include "utils.h"
#include "z21.h"
#include "cs2-data-functions.h"
#include "read-cs2-config.h"

#define v_printf(verbose, ...) \
	do { if ((verbose)) { printf(__VA_ARGS__);} } while (0)

#define vas_printf(verbose, ...) \
	do { if ((verbose)) { asprintf(__VA_ARGS__);} } while (0)

#define check_free(a) \
            do { if ( a ) free(a); } while (0)

struct sockaddr_in *bsa;
pthread_mutex_t lock;

struct z21_data_t z21_data;
extern struct loco_data_t *loco_data, *loco_data_by_uid;
extern struct subscriber_t *subscriber;

static char *UDP_SRC_STRG	= "->UDP    len 0x%04x ID 0x%04x";
static char *UDP_DST_STRG	= "  UDP->  len 0x%04x ID 0x%04x";
static char *TCP_FORMAT_STRG_S	= "  TCP->  CANID 0x%06X   [%d]";
static char *TCP_FORMAT_STRG	= "->TCP    CANID 0x%06X   [%d]";
static char *TCP_FORMATS_STRG	= "->TCP*   CANID 0x%06X   [%d]";

#define MAXLINE		256
#define WLM_DELAY	180000

char cs2addr[32] = "127.0.0.1";
char config_dir[MAXLINE] = "/www/config/";

static unsigned char MS_POWER_ON[]		= { 0x00, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };
static unsigned char MS_POWER_OFF[]		= { 0x00, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char MS_LOCO_DRIVE[]		= { 0x00, 0x08, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char MS_LOCO_DIRECTION[]	= { 0x00, 0x0A, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char MS_LOCO_FUNCTION[]		= { 0x00, 0x0C, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char MS_TURNOUT[]		= { 0x00, 0x16, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char XPN_GET_CODE_RESPONSE[]      = { 0x05, 0x00, 0x18, 0x00, 0x00 };
static unsigned char XPN_SERIAL_NUMBER_RESPONSE[] = { 0x08, 0x00, 0x10, 0x00, 0x4D, 0xC1, 0x02, 0x00 };
static unsigned char XPN_HWINFO_RESPONSE[]        = { 0x0C, 0x00, 0x1A, 0x00, 0x01, 0x02, 0x00, 0x00, 0x32, 0x01, 0x00, 0x00 };
static unsigned char XPN_X_STATUS_CHANGED[]       = { 0x08, 0x00, 0x40, 0x00, 0x62, 0x22, 0x00, 0x40 };
static unsigned char XPN_X_BC_TRACK_POWER_OFF[]   = { 0x07, 0x00, 0x40, 0x00, 0x61, 0x00, 0x61 };
static unsigned char XPN_X_BC_TRACK_POWER_ON[]    = { 0x07, 0x00, 0x40, 0x00, 0x61, 0x01, 0x60 };
static unsigned char XPN_X_LOCO_INFO[]            = { 0x0E, 0x00, 0x40, 0x00, 0xEF, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char XPN_X_TURNOUT_INFO[]         = { 0x09, 0x00, 0x40, 0x00, 0x43, 0x00, 0x00, 0x00, 0x00 };
static unsigned char XPN_X_Z21_FIRMWARE_VERSION[] = { 0x09, 0x00, 0x40, 0x00, 0xF3, 0x0A, 0x01, 0x32, 0xCA };
static unsigned char XPN_X_STORE1[]               = { 0x0E, 0x00, 0x12, 0x00, 0x01, 0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char XPN_X_STORE2[]               = { 0x14, 0x00, 0x16, 0x00, 0x19, 0x06, 0x07, 0x01, 0x05, 0x14,
						      0x88, 0x13, 0x10, 0x27, 0x32, 0x00, 0x50, 0x46, 0x20, 0x4e };

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -c config_dir -p <port> -s <port>\n", prg);
    fprintf(stderr, "   Version 0.99\n\n");
    fprintf(stderr, "         -c <config_dir>     set the config directory - default %s\n", config_dir);
    fprintf(stderr, "         -p <port>           primary UDP port for the server - default %d\n", PRIMARY_UDP_PORT);
    fprintf(stderr, "         -s <port>           secondary UDP port for the server - default %d\n", SECONDARY_UDP_PORT);
    fprintf(stderr, "         -g <gateway addr>   gateway IP\n");
#ifndef NO_CAN
    fprintf(stderr, "         -i <CAN interface>  CAN interface\n");
#endif
    fprintf(stderr, "         -x                  enable turnout switching\n");
    fprintf(stderr, "         -f                  running in foreground\n\n");
}

unsigned int m_id(uint16_t loco_id) {
    if (loco_id > 0x1fff)
	return (loco_id + 0xa000);
    else if (loco_id > 0xff)
	return (loco_id + 0x3f00);
    return((unsigned int)loco_id);
}

uint16_t xpn_id(unsigned int m_id) {

    if (m_id < 0x3FFF) {
	return(m_id);
    } else if (m_id < 0xC000) {
	return(m_id - 0x3F00);
    } else {
	return(m_id - 0xA000);
    }
}

int send_z21_clients(unsigned char *udpframe, char *format, char *vchar) {
    int s;
    uint16_t length;
    struct subscriber_t *z21client, *tmp;

    length = le16(&udpframe[0]);

    /* check if BROACAST FLAGS indicates only directed answer to one client */
    if (z21_data.bcf == 0) {
	if (vchar) {
	    print_udp_frame(format, length, udpframe);
	    printf("%s", vchar);
	}
	HASH_FIND_INT(subscriber, &z21_data.ip, z21client);
	if (z21client) {
	    s = sendto(z21_data.sp, udpframe, length, 0, (struct sockaddr *)&z21client->client_addr, sizeof(z21client->client_addr));
	    if (s < 0) {
		fprintf(stderr, "UDP write error: %s\n", strerror(errno));
		return (EXIT_FAILURE);
	    }
	}
	return(EXIT_SUCCESS);
    }

    HASH_ITER(hh, subscriber, z21client, tmp) {
	if (z21_data.bcf & z21client->broadcast_flags) {
	    if (vchar) {
		print_udp_frame(format, length, udpframe);
		printf("%s", vchar);
	    }
	    s = sendto(z21_data.sp, udpframe, length, 0, (struct sockaddr *)&z21client->client_addr, sizeof(z21client->client_addr));
	    if (s < 0) {
		fprintf(stderr, "UDP write error: %s\n", strerror(errno));
		return (EXIT_FAILURE);
	    }
	}
    }
    return (EXIT_SUCCESS);
}

int send_xpn(unsigned char *data, char *vchar) {
    unsigned char udpxpn[64];
    char *format;
    int length;

    length = le16(data);
    if (length > 64) {
	fprintf(stderr, "Xpn length to long: %d\n", length);
	return (EXIT_FAILURE);
    }
    memcpy(udpxpn, data, length);
    format = UDP_DST_STRG;
    send_z21_clients(udpxpn, format, vchar);
    return (EXIT_SUCCESS);
}

int send_tcp_frame(unsigned char *frame, char *format, int verbose) {
    int on, s;
    uint16_t length = 13;

    s = sendto(z21_data.st, frame, length, 0, (struct sockaddr *)&z21_data.staddr, sizeof(z21_data.staddr));
    if (s < 0) {
	fprintf(stderr, "TCP write error: %s\n", strerror(errno));
	return (EXIT_FAILURE);
    }

    /* disable Nagle - force PUSH */
    if (setsockopt(z21_data.st, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on)) < 0) {
	fprintf(stderr, "error disabling Nagle - TCP_NODELAY on: %s\n", strerror(errno));
	return (EXIT_FAILURE);
    }

    if (s == 13)
	print_net_frame(format, frame, verbose);
    return (EXIT_SUCCESS);
}

int send_can(unsigned char *data, int verbose) {
    unsigned char frame[16];

    memcpy(frame, data, 13);
    send_tcp_frame(frame, TCP_FORMAT_STRG_S, verbose);
    return (EXIT_SUCCESS);
}

int send_xpn_loco_info(uint16_t uid, int verbose) {
    uint8_t direction;
    unsigned char xpnframe[32];
    unsigned int comp_func;
    uint16_t loco_id, speed;
    char *vchar;

    vchar = NULL;
    memcpy(xpnframe, XPN_X_LOCO_INFO, sizeof(XPN_X_LOCO_INFO));
    loco_id = xpn_id(uid);
    comp_func = loco_get_func_summary(uid);
    speed = loco_get_speed(uid);

    xpnframe[5] = loco_id >> 8;
    xpnframe[6] = loco_id & 0xff;
    xpnframe[7] = 4;		/* TODO */
    if ((speed > 0) && (speed < 16))
	xpnframe[8] = 2;
    else
	xpnframe[8] = (loco_get_speed(uid) >> 3) & 0xff;	/*TODO */
    direction = loco_get_direction(uid);
    if (direction != 1)
	direction = 0;
    xpnframe[8] |= direction << 7;
    xpnframe[9]  = ((comp_func & 1) << 4) | ((comp_func >> 1) & 0x0F);
    xpnframe[10] = (comp_func >>  5) & 0xFF;
    xpnframe[11] = (comp_func >> 13) & 0xFF;
    xpnframe[12] = (comp_func >> 21) & 0xFF;
    xpnframe[13] = xor(&xpnframe[4], 8);
    z21_data.bcf = 0x00000001;
    vas_printf(verbose, &vchar, "LAN_X_LOCO_INFO LOC ID 0x%04X\n", loco_id);
    send_xpn(xpnframe, vchar);
    check_free(vchar);

    return (EXIT_SUCCESS);
}

int send_xpn_loco_name(uint16_t loco_id, char *loco_name, uint8_t index, uint8_t n, int verbose) {
    unsigned char xpnframe[32];
    size_t length;
    char *vchar;

    vchar = NULL;
    memset(xpnframe, 0, sizeof(xpnframe));
    length = strnlen(loco_name, 10);

    xpnframe[0] = (length + 11) & 0xff;
    xpnframe[2] = 0x40;
    xpnframe[4] = (length + 0xe5) & 0xff;
    xpnframe[5] = 0xf1;
    if (loco_id > 0xc000)
	loco_id -= 0xa000;
    else if (loco_id > 0x4000)
	loco_id -= 0x3f00;
    xpnframe[6] = loco_id >> 8;
    xpnframe[7] = loco_id & 0xff;
    xpnframe[8] = index;
    xpnframe[9] = n;
    memcpy(&xpnframe[10], loco_name, length);
    xpnframe[length + 10] = xor(&xpnframe[4], length + 6);
    z21_data.bcf = 0x00000000;
    vas_printf(verbose, &vchar, " LAN_LOCO 0x%04X %s", loco_id, loco_name);
    send_xpn(xpnframe, vchar);
    check_free(vchar);
    return (EXIT_SUCCESS);
}

int send_xpn_locos(struct z21_data_t *z21_data, struct loco_data_t *loco_data, int verbose) {
    struct loco_data_t *l;
    int i = 0;

    for (l = loco_data; l != NULL; l = l->hh.next) {
	send_xpn_loco_name(l->uid, l->name, i, z21_data->loco_number, verbose);
	v_printf(verbose, "\n");
	usec_sleep(WLM_DELAY);
	send_xpn_loco_name(l->uid, l->name, i, z21_data->loco_number, verbose);
	v_printf(verbose, "\n");
	usec_sleep(WLM_DELAY);
	i++;
    }
    return (EXIT_SUCCESS);
}

int send_xpn_turnout_info(uint16_t FAdr, uint8_t zz, char *vchar) {
    unsigned char xpnframe[32];

    memcpy(xpnframe, XPN_X_TURNOUT_INFO, sizeof(XPN_X_TURNOUT_INFO));
    xpnframe[5] = FAdr >> 8;
    xpnframe[6] = FAdr & 0xff;
    xpnframe[7] = zz;

    xpnframe[8] = xor(&xpnframe[4], 4);
    z21_data.bcf = 0x00000001;
    send_xpn(xpnframe, vchar);

    return (EXIT_SUCCESS);
}

int send_xpn_lcn_detector(uint16_t id, uint8_t state, char *vchar) {
    unsigned char xpnframe[16];

    memset(xpnframe, 0, sizeof(xpnframe));
    xpnframe[0] = 0x08;
    xpnframe[2] = 0xA4;
    xpnframe[4] = id & 0xFF;
    xpnframe[5] = (id >> 8);
    xpnframe[6] = 0x01;	/* Type */
    xpnframe[7] = state;

    z21_data.bcf = 0x00000008;
    send_xpn(xpnframe, vchar);
    return (EXIT_SUCCESS);
}

int send_xpn_system_info(char *vchar) {
    /* TODO */
    unsigned char xpnframe[32];

    memset(xpnframe, 0, sizeof(xpnframe));
    xpnframe[0] = 0x14;
    xpnframe[2] = 0x84;
    /* mA */
    xpnframe[4] = 200;
    xpnframe[6] = 10;
    xpnframe[8] = 150;
    /* degree Celsius */
    xpnframe[10] = 20;
    /* Voltage */
    xpnframe[12] = 0x50;
    xpnframe[13] = 0x46;

    xpnframe[14] = 0x80;
    xpnframe[15] = 0x3E;

    z21_data.bcf = 0x00000000;
    send_xpn(xpnframe, vchar);
    return (EXIT_SUCCESS);
}

void set_loco_id(unsigned char *data, uint16_t loco_id) {
    /*
       0x0000 - 0x007F mm2_prg   Adresse + 0x0000
       0x0080 - 0x00FF mm2_dil   Adresse + 0x0080
       0x0100 - 0x1FFF mfx       Adresse + 0x0100
       0x2000 - 0x3FFF dcc       Adresse + 0x2000
     */
    if (loco_id < 0x0100) {
	data[2] = 0x00;
	data[3] = loco_id;
    } else if (loco_id < 0x2000) {
	data[2] |= 0x40;
	data[3] = loco_id & 0xff;
    } else if (loco_id < 0x3FFF) {
	data[2] |= 0xC0;
	data[3] = loco_id & 0xff;
    }
}

int send_can_loco_function(uint16_t loco_id, uint8_t function, uint8_t value, int verbose) {
    unsigned char udpframe[13];

    memcpy(udpframe, MS_LOCO_FUNCTION, 13);
    set_loco_id(&udpframe[5], loco_id);
    udpframe[9] = function;
    udpframe[10] = value;
    send_can(udpframe, verbose);

    return (EXIT_SUCCESS);
}

int send_can_loco_drive(uint16_t loco_id, uint8_t direction, uint8_t step, uint8_t speed, int verbose) {
    unsigned char netframe[13];
    uint16_t mspeed;

    memcpy(netframe, MS_LOCO_DIRECTION, 13);
    set_loco_id(&netframe[5], loco_id);
    if (direction == 0)
	netframe[9] = 2;
    else if (direction == 1)
	netframe[9] = 1;
    if (netframe[9] != loco_get_direction(m_id(loco_id))) {
	send_can(netframe, verbose);
	v_printf(verbose, "\n");
    }

    memcpy(netframe, MS_LOCO_DRIVE, 13);
    set_loco_id(&netframe[5], loco_id);
    /* TODO */
    mspeed = speed << 3;
    if (mspeed > 1023)
	mspeed = 1023;
    netframe[9] = (mspeed >> 8) & 0x03;
    netframe[10] = mspeed & 0xFF;
    send_can(netframe, verbose);

    return (EXIT_SUCCESS);
}

int send_can_turnout(uint16_t id, uint8_t port, int verbose) {
    unsigned char udpframe[13];

    memcpy(udpframe, MS_TURNOUT, 13);
    /* TODO */
    id += 0x3000;
    udpframe[7] = id >> 8;
    udpframe[8] = id & 0xFF;
    udpframe[9] = port;
    send_can(udpframe, verbose);

    return (EXIT_SUCCESS);
}

int check_data_lan_x_header(struct z21_data_t *z21_data, unsigned char *udpframe, int verbose) {
    uint8_t db0, tport, turnout, value, xheader, zz;
    uint16_t length, loco_id, FAdr;
    unsigned char xpnframe[32];
    char *vchar;

    vchar = NULL;
    length = le16(udpframe);
    xheader = udpframe[4];
    switch (xheader) {
    case 0x21:
	db0 = udpframe[5];
	switch (db0) {
	case LAN_X_GET_STATUS:
	    v_printf(verbose, "LAN_X_GET_STATUS\n");
	    memcpy(xpnframe, XPN_X_STATUS_CHANGED, sizeof(XPN_X_STATUS_CHANGED));
	    xpnframe[6] = z21_data->power ? 0x00 : 0x02;
	    xpnframe[7] = xor(&xpnframe[4], 3);
	    z21_data->bcf = 0x00000000;
	    vas_printf(verbose, &vchar, "LAN_X_STATUS_CHANGED\n");
	    send_xpn(xpnframe, vchar);
	    break;
	case LAN_X_SET_TRACK_POWER_ON:
	    v_printf(verbose, "LAN_X_SET_TRACK_POWER_ON\n");
	    z21_data->power = 1;
	    send_can(MS_POWER_ON, verbose);
	    v_printf(verbose, "\n");
	    break;
	case LAN_X_SET_TRACK_POWER_OFF:
	    v_printf(verbose, "LAN_X_SET_TRACK_POWER_OFF\n");
	    z21_data->power = 0;
	    send_can(MS_POWER_OFF, verbose);
	    v_printf(verbose, "\n");
	    break;
	default:
	    v_printf(verbose, "LAN_X_HEADER type ?\n");
	    break;
	}
	break;
    case LAN_X_GET_LOCO_INFO:
	v_printf(verbose, "LAN_X_GET_LOCO_INFO\n");
	if (length == 9) {
	    loco_id = be16(&udpframe[6]) & 0x3FFF;
	    /* TODO */
	    vas_printf(verbose, &vchar, " LOC ID 0x%04X\n", loco_id);
	    /* TODO */
	    send_xpn_loco_info(m_id(loco_id), verbose);
	}
	break;
    case LAN_X_SET_LOCO:
	if (length == 0x0A) {
	    loco_id = be16(&udpframe[6]) & 0x3FFF;
	    if (udpframe[5] == LAN_X_SET_LOCO_FUNCTION) {
		v_printf(verbose, "LAN_X_SET_LOCO_FUNCTION 0x%04X 0x%02X\n", loco_id, udpframe[8]);
		uint8_t function = (udpframe[8]) & 0x1F;
		uint8_t switchtype = (udpframe[8] >> 6) & 0x03;
		if (switchtype == 2)
		    value = loco_get_function(m_id(loco_id), function) ^ 1;
		else
		    value = switchtype;
		send_can_loco_function(loco_id, function, value, z21_data->foreground);
	    } else if ((udpframe[5] & 0xF0) == 0x10) {
		/* LAN_X_SET_LOCO_DRIVE */
		v_printf(verbose, "LAN_X_SET_LOCO_DRIVE 0x%04X 0x%02X\n", loco_id, udpframe[8]);
		uint8_t step = udpframe[5] & 0x03;
		uint8_t direction = udpframe[8] >> 7;
		uint8_t speed = udpframe[8] & 0x7F;
		send_can_loco_drive(loco_id, direction, step, speed, z21_data->foreground);
		v_printf(z21_data->foreground, "\n");
	    }
	}
	/* LAN_X_SET_LOCO */
	break;
    case LAN_X_GET_TURNOUT_INFO:
	FAdr = be16(&udpframe[5]);
	if (length == 0x08) {
	    v_printf(verbose, "LAN_X_GET_TURNOUT_INFO 0x%04X\n", FAdr);
	} else if (length == 0x09) {
	    v_printf(verbose, "LAN_X_GET_TURNOUT_INFO 0x%04X\n", FAdr);
	    zz = udpframe[7];
	    vas_printf(verbose, &vchar, "LAN_X_TURNOUT_INFO 0x%04X 0x%02X\n", FAdr, zz);
	    /* TODO */
	    if (!zz) {
		zz = 0x01;
		z21_data->bcf = 0x00000001;
		send_xpn_turnout_info(FAdr, zz, vchar);
	    }
	}
	break;
    case LAN_X_SET_TURNOUT:
	FAdr = be16(&udpframe[5]);
	turnout = udpframe[7];
	tport = turnout & 0x1;
	v_printf(verbose, "LAN_X_SET_TURNOUT 0x%04X\n", FAdr);
	if (z21_data->turnout_enable) {
	    send_can_turnout(FAdr, tport, verbose);
	    v_printf(verbose, "\n");
	}
	break;
    case LAN_X_CV_READ:
	v_printf(verbose, "LAN_X_CV_READ CV %u *TODO*\n", be16(&udpframe[6]));
	break;
    case LAN_X_GET_FIRMWARE_VERSION:
	v_printf(verbose, "LAN_X_GET_FIRMWARE_VERSION\n");
	z21_data->bcf = 0x00000000;
	vas_printf(verbose, &vchar, "LAN_X_FIRMWARE_VERSION %u.%u%u\n", XPN_X_Z21_FIRMWARE_VERSION[6],
		   XPN_X_Z21_FIRMWARE_VERSION[7] >> 4, XPN_X_Z21_FIRMWARE_VERSION[7] & 0xF);
	send_xpn(XPN_X_Z21_FIRMWARE_VERSION, vchar);
	break;
    default:
	break;
    }
    check_free(vchar);
    return (EXIT_SUCCESS);
}

int check_data_xpn(struct z21_data_t *z21_data, int udplength, int verbose) {
    uint32_t flags, i;
    uint16_t length, header;
    char *vchar;

    vchar = NULL;
    i = 0;
    length = le16(z21_data->udpframe);

    while (i + length <= udplength) {
	length = le16(&z21_data->udpframe[i]);
	header = le16(&z21_data->udpframe[i + 2]);
	if (verbose)
	    print_udp_frame(UDP_SRC_STRG, length, &z21_data->udpframe[i]);

	switch (header) {
	case LAN_GET_SERIAL_NUMBER:
	    if (length == 4) {
		v_printf(verbose, "LAN_GET_SERIAL_NUMBER\n");
		z21_data->bcf = 0x00000000;
		vas_printf(verbose, &vchar, "LAN_SERIAL_NUMBER 0x%08X\n", le32(&XPN_SERIAL_NUMBER_RESPONSE[i + 4]));
		send_xpn(XPN_SERIAL_NUMBER_RESPONSE, vchar);
	    }
	    break;
	case LAN_GET_CODE:
	    z21_data->bcf = 0x00000000;
	    v_printf(verbose, "LAN_GET_CODE\n");
	    vas_printf(verbose, &vchar, "LAN_GET_CODE_RESPONSE\n");
	    send_xpn(XPN_GET_CODE_RESPONSE, vchar);
	    break;
	case LAN_GET_HWINFO:
	    if (length == 4) {
		v_printf(verbose, "LAN_GET_HWINFO\n");
		z21_data->bcf = 0x00000000;
		vas_printf(verbose, &vchar, "LAN HWINFO 0x%04X %u.%u%u\n", le32(&XPN_HWINFO_RESPONSE[i + 4]),
			   XPN_HWINFO_RESPONSE[i + 9], XPN_HWINFO_RESPONSE[i + 8] >> 4, XPN_HWINFO_RESPONSE[i + 8] & 0xF);
		send_xpn(XPN_HWINFO_RESPONSE, vchar);
	    }
	    break;
	case LAN_LOGOFF:
	    del_z21c_ip(z21_data->ip);
	    v_printf(verbose, "LAN_LOGOFF *");
	    break;
	case LAN_GET_LOCOMODE:
	    v_printf(verbose, "LAN_GET_LOCO_MODE 0x%04X *TODO*\n", be16(&z21_data->udpframe[i + 4]));
	    break;
	case LAN_SET_BROADCASTFLAGS:
	    flags = be32(&z21_data->udpframe[i + 4]);
	    set_z21c_bcf(z21_data->ip, flags);
	    v_printf(verbose, "LAN_SET_BROADCASTFLAGS 0x%08X\n", flags);
	    break;
	case LAN_X_HEADER:
	    check_data_lan_x_header(z21_data, &z21_data->udpframe[i], verbose);
	    break;
	case LAN_SYSTEMSTATE_GETDATA:
	    vas_printf(verbose, &vchar, "LAN_SYSTEMSTATE_GETDATA\n");
	    send_xpn_system_info(vchar);
	    break;
	case 0x12:
	    if (length == 0x04) {
		vas_printf(verbose, &vchar, "LAN_GET_STORE1?\n");
		z21_data->bcf = 0x00000000;
		send_xpn(XPN_X_STORE1, vchar);
	    }
	    break;
	case 0x16:
	    if (length == 0x04) {
		vas_printf(verbose, &vchar, "LAN_GET_STORE2?\n");
		z21_data->bcf = 0x00000000;
		send_xpn(XPN_X_STORE2, vchar);
	    }
	    break;
	case 0x84:
	    /* ignore self sent data */
	    break;
	default:
	    v_printf(verbose, "XPN unknown\n");
	    break;
	}
	i += length;
    }
    check_free(vchar);
    return (EXIT_SUCCESS);
}

int check_data_can(struct z21_data_t *z21_data, uint8_t * data, int verbose) {
    uint32_t uid;
    uint16_t id;
    uint8_t function, tport, tpower, value, state;
    char *vchar;

    vchar = NULL;
    uid = be32(&data[5]);
    switch ((be32(data) & 0x00FF0000UL) >> 16) {
    case 0x01:
	switch (data[9]) {
	case 0x00:
	    if (uid)
		v_printf(verbose, "System: UID 0x%08X Stop\n", uid);
	    else
		v_printf(verbose, "System: alle Stop\n");
	    z21_data->bcf = 0x00000001;
	    vas_printf(verbose, &vchar, "XPN_X_BC_TRACK_POWER_OFF\n");
	    send_xpn(XPN_X_BC_TRACK_POWER_OFF, vchar);
	    z21_data->power = 0;
	    break;
	case 0x01:
	    if (uid)
		v_printf(verbose, "System: UID 0x%08X Go\n", uid);
	    else
		v_printf(verbose, "System: alle Go\n");
	    z21_data->bcf = 0x00000001;
	    vas_printf(verbose, &vchar, "XPN_X_BC_TRACK_POWER_ON\n");
	    send_xpn(XPN_X_BC_TRACK_POWER_ON, vchar);
	    z21_data->power = 1;
	    break;
	/* emergency stop */
	case 0x03:
	    if (uid) {
		loco_save_speed(uid, 0);
		v_printf(verbose, "\n");
		send_xpn_loco_info(uid, verbose);
	    }
	    break;
	default:
	    break;
	}
	break;
    case 0x09:
	loco_save_speed(uid, be16(&data[9]));
	v_printf(verbose, "\n");
	send_xpn_loco_info(uid, verbose);
	break;
    /* Lokinfo (Loco ID 0x0001 & F2 Trigger */
    case 0x0C:
	if ((uid == 0x01) && (data[9] == 2)) {
	    v_printf(verbose, "Send Loco names\n");
	    send_xpn_locos(z21_data, loco_data, verbose);
	} else {
	    v_printf(verbose, "\n");
	}
	break;
    /* loc functions */
    case 0x0D:
	function = data[9];
	value = data[10];
	loco_save_function(uid, function, value);
	v_printf(verbose, "\n");
	send_xpn_loco_info(uid, verbose);
	break;
    case 0x0B:
	/* v_printf(verbose, "loco uid 0x%08X : actual direction %u - saved direction %u", uid, data[9], loco_get_direction(uid)); */
	if (data[9] != loco_get_direction(uid)) {
	    loco_save_direction(uid, data[9]);
	    v_printf(verbose, "\n");
	    send_xpn_loco_info(uid, verbose);
	}
	break;
    /* turnout */
    case 0x17:
	/* TODO */
	uid = be16(&data[7]) & 0xCFFF;
	tport = 1 << (data[9] & 0x01);
	tpower = data[10];
	v_printf(verbose, "\n");
	vas_printf(verbose, &vchar, "LAN_X_TURNOUT_INFO 0x%04X 0x%02X\n", uid, tport);
	if (!tpower)
	    send_xpn_turnout_info(uid, tport, vchar);
	break;
    /* S88 Event */
    case 0x23:
	v_printf(verbose, "\n");
	id = be16(&data[7]);
	state = data[10];
	/* we use the simplest GBM */
	vas_printf(verbose, &vchar, "LAN_LOCONET_DETECTOR %d state %d\n", id, state);
	send_xpn_lcn_detector(id, state, vchar);
	break;
    default:
	v_printf(verbose, "\n");
	break;
    }
    check_free(vchar);
    return (EXIT_SUCCESS);
}

void *z21_periodic_tasks(void *ptr) {
    /* struct z21_data_t *z21_data = (struct z21_data_t *)ptr; */

    while (1) {
	usec_sleep(1E6);
    }
}

int main(int argc, char **argv) {
    pid_t pid;
    pthread_t pth;
    int ret, opt, max_fds;
    struct ifreq ifr;
#ifndef NO_CAN
    struct sockaddr_can caddr;
#endif
    struct sockaddr_in src_addr;
    fd_set readfds;
    int primary_port = PRIMARY_UDP_PORT;
    int secondary_port = SECONDARY_UDP_PORT;
    unsigned char recvline[MAXSIZE];
    char timestamp[16];
    char *loco_file;

#ifndef NO_CAN
    socklen_t caddrlen = sizeof(caddr);
#endif
    socklen_t slen = sizeof(src_addr);

    memset(&z21_data, 0, sizeof(z21_data));

    while ((opt = getopt(argc, argv, "c:p:s:b:g:i:xhf?")) != -1) {
	switch (opt) {
	case 'c':
	    if (strnlen(optarg, MAXLINE) < MAXLINE) {
		strncpy(config_dir, optarg, sizeof(config_dir) - 1);
	    } else {
		fprintf(stderr, "config file dir to long\n");
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'p':
	    primary_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 's':
	    secondary_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 'g':
	    strncpy(cs2addr, optarg, sizeof(cs2addr) - 1);
	    break;
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    break;
	case 'f':
	    z21_data.foreground = 1;
	    break;
	case 'x':
	    z21_data.turnout_enable = 1;
	    break;
	case 'h':
	case '?':
	    print_usage(basename(argv[0]));
	    exit(EXIT_SUCCESS);
	    break;
	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(EXIT_FAILURE);
	}
    }

    /* prepare primary UDP socket */
    z21_data.sp = socket(PF_INET, SOCK_DGRAM, 0);
    if (z21_data.sp < 0) {
	fprintf(stderr, "primary UDP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    z21_data.spaddr.sin_family = AF_INET;
    z21_data.spaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    z21_data.spaddr.sin_port = htons(primary_port);

    if (bind(z21_data.sp, (struct sockaddr *)&z21_data.spaddr, sizeof(z21_data.spaddr)) < 0) {
	fprintf(stderr, "primary UDP bind error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    /* prepare secondary UDP socket */
    z21_data.ss = socket(PF_INET, SOCK_DGRAM, 0);
    if (z21_data.ss < 0) {
	fprintf(stderr, "secondary UDP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    z21_data.ssaddr.sin_family = AF_INET;
    z21_data.ssaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    z21_data.ssaddr.sin_port = htons(secondary_port);

    if (bind(z21_data.ss, (struct sockaddr *)&z21_data.ssaddr, sizeof(z21_data.ssaddr)) < 0) {
	fprintf(stderr, "scondary UDP bind error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    if (strlen(ifr.ifr_name)) {
#ifndef NO_CAN
	/* prepare CAN socket */
	printf("ifr.ifr_name: >%s<\n", ifr.ifr_name);
	if ((z21_data.sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	    fprintf(stderr, "error creating CAN socket: %s on >%s<\n", strerror(errno), ifr.ifr_name);
	    exit(EXIT_FAILURE);
	}
	caddr.can_family = AF_CAN;
	if (ioctl(z21_data.sc, SIOCGIFINDEX, &ifr) < 0) {
	    fprintf(stderr, "setup CAN socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	caddr.can_ifindex = ifr.ifr_ifindex;

	if (bind(z21_data.sc, (struct sockaddr *)&caddr, caddrlen) < 0) {
	    fprintf(stderr, "error binding CAN socket: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
#endif
    } else {
	/* prepare TCP client socket */
	if ((z21_data.st = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	    fprintf(stderr, "can't create TCP socket: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}

	memset(&z21_data.staddr, 0, sizeof(z21_data.staddr));
	z21_data.staddr.sin_family = AF_INET;

	if (inet_aton(cs2addr, (struct in_addr *)&z21_data.staddr.sin_addr.s_addr) == 0) {
	    fprintf(stderr, "invalid address : %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}

	z21_data.staddr.sin_port = htons(MAERKLIN_PORT);

	if (connect(z21_data.st, (struct sockaddr *)&z21_data.staddr, sizeof(z21_data.staddr))) {
	    fprintf(stderr, "can't connect to TCP socket : %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    if (pthread_mutex_init(&lock, NULL)) {
	fprintf(stderr, "can't init mutex %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    if (pthread_create(&pth, NULL, z21_periodic_tasks, &z21_data)) {
	fprintf(stderr, "can't create pthread %s\n", strerror(errno));
	exit(EXIT_FAILURE);

    }
    if (z21_data.foreground) {
	printf("created periodic z21 thread\n");
    }

    if (asprintf(&loco_file, "%s/%s", config_dir, loco_name) < 0) {
	fprintf(stderr, "can't alloc buffer for loco_name: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    read_loco_data(loco_file, CONFIG_FILE);
    z21_data.loco_number = HASH_COUNT(loco_data);
    v_printf(z21_data.foreground, "loco data: %u\n", z21_data.loco_number);
    if (z21_data.foreground) {
	print_locos_by_uid();
	printf("\n");
    }
    /* send_xpn_locos(&z21_data, loco_data,z21_data.foreground); */

    if (!z21_data.foreground) {
	/* Fork off the parent process */
	pid = fork();
	if (pid < 0)
	    exit(EXIT_FAILURE);
	/* If we got a good PID, then we can exit the parent process */

	if (pid > 0) {
	    printf("Going into background ...\n");
	    exit(EXIT_SUCCESS);
	}
    }

    while (1) {
	FD_ZERO(&readfds);
	FD_SET(z21_data.sp, &readfds);
	FD_SET(z21_data.ss, &readfds);
	if (z21_data.st) {
	    FD_SET(z21_data.st, &readfds);
	    max_fds = MAX(MAX(z21_data.sp, z21_data.ss), z21_data.st);
	} else {
	    FD_SET(z21_data.sc, &readfds);
	    max_fds = MAX(MAX(z21_data.sp, z21_data.ss), z21_data.sc);
	}

	if (select(max_fds + 1, &readfds, NULL, NULL, NULL) < 0) {
	    fprintf(stderr, "select error: %s\n", strerror(errno));
	    break;
	}
#ifndef NO_CAN
	if (FD_ISSET(z21_data.sc, &readfds)) {
	}
#endif

	/* received a UDP packet on primary */
	if (FD_ISSET(z21_data.sp, &readfds)) {
	    memset(&src_addr, 0, sizeof(src_addr));
	    ret = recvfrom(z21_data.sp, z21_data.udpframe, MAXDG, 0, (struct sockaddr *)&src_addr, &slen);
	    /* v_printf(verbose, "FD_ISSET sp, ret %d\n", ret); */
	    if (ret < 0) {
		fprintf(stderr, "UDP read error: %s\n", strerror(errno));
		break;
	    } else if (ret) {
		// printf("Client %s ", inet_ntoa(src_addr.sin_addr));
		z21_data.ip = src_addr.sin_addr.s_addr;
		add_z21c_ip(z21_data.ip);
		check_data_xpn(&z21_data, ret, z21_data.foreground);
	    }
	}
	/* received a UDP packet on secondary */
	if (FD_ISSET(z21_data.ss, &readfds)) {
	    memset(&src_addr, 0, sizeof(src_addr));
	    ret = recvfrom(z21_data.ss, z21_data.udpframe, MAXDG, 0, (struct sockaddr *)&src_addr, &slen);
	    /* v_printf(verbose, "FD_ISSET ss, ret %d\n", ret); */
	    if (ret < 0) {
		fprintf(stderr, "UDP read error: %s\n", strerror(errno));
		break;
	    } else {
		z21_data.ip = src_addr.sin_addr.s_addr;
		add_z21c_ip(z21_data.ip);
		check_data_xpn(&z21_data, ret, z21_data.foreground);
	    }
	}

	if (FD_ISSET(z21_data.st, &readfds)) {
	    int i, n;
	    n = recv(z21_data.st, recvline, MAXSIZE, 0);
	    /* v_printf(verbose, "FD_ISSET st, n %d\n", n); */
	    if (n > 0) {
		/* check the whole TCP packet, if there are more than one CAN frame included */
		/* TCP packets with size modulo 13 !=0 are ignored though */
		if (n % 13) {
		    time_stamp(timestamp);
		    fprintf(stderr, "%s received packet %% 13 : length %d\n", timestamp, n);
		} else {
		    for (i = 0; i < n; i += 13) {
			if (i >= 13)
			    print_net_frame(TCP_FORMATS_STRG, &recvline[i], z21_data.foreground);
			else
			    print_net_frame(TCP_FORMAT_STRG, &recvline[i], z21_data.foreground);
			check_data_can(&z21_data, &recvline[i], z21_data.foreground);
		    }
		}
	    }
	}
    }
    close(z21_data.sp);
    close(z21_data.ss);
    pthread_join(pth, (void *)&z21_data);
    pthread_mutex_unlock(&lock);
    /* if we reach this point, there was an error */
    return EXIT_FAILURE;
}
