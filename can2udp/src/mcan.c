/*
 *  Created by Maximilian Goldschmidt <maxigoldschmidt@gmail.com>
 *  If you need Help, feel free to contact me any time!
 *  Do with this whatever you want, but keep thes Header and tell
 *  the others what you changed!
 *
 *  Last changed: 2017-02-13
 * 
 *  Joerg Dorchain <joerg@dorchain.net> 26.12.2017
 *  Ported to C and linux socketcan
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <arpa/inet.h>
#include "mcan.h"

#define BIT(i) (1<<i)
#define bitWrite(a, b, c) ((c)?(a|=BIT(b)):(a&=~BIT(b)))
#define bitRead(a, b) ((a&BIT(b))?1:0)

uint16_t be16(uint8_t * u) {
    return (u[0] << 8) | u[1];
}

uint32_t be32(uint8_t * u) {
    return (u[0] << 24) | (u[1] << 16) | (u[2] << 8) | u[3];
}

void be16_to_pu8(uint32_t x, uint8_t * u) {
    u[0] = x >> 8;
    u[1] = x;
}

void set_uid(uint8_t * u, uint32_t x) {
    u[0] = x >> 24;
    u[1] = x >> 16;
    u[2] = x >> 8;
    u[3] = x;
}

void print_net_frm(struct can_frame *frm) {
    int i;

    printf("      CANID 0x%08x  dlc [%u] (", frm->can_id, frm->can_dlc);
    for (i = 0; i < frm->can_dlc; i++) {
	printf(" 0x%02x", frm->data[i]);
    }
    printf(")\n");
}

void print_canmsg(MCANMSG * msg) {
    int i;
    printf("  cmd 0x%02x  hash 0x%04x  resp %d  dlc %u (", msg->cmd, msg->hash, msg->resp_bit, msg->dlc);
    for (i = 0; i < msg->dlc; i++) {
	printf("0x%02x ", msg->data[i]);
    }
    printf(")\n");
}

/*
  CS2 CAN hash generation

  xxxxxxx11 0xxxxxxxx
  ^^^^^^^    ^^^^^^^^
   \\\\\\\   ||||||||
    \\\\\\-- ||||||||
     \\\\\\ \||||||||
  000xxxxxx xxxxxxxxx

 */
uint16_t generateHash(uint32_t uid) {
    uint16_t hash, highword, lowword;

    highword = uid >> 16;
    lowword = uid & 0xFFFF;
    hash = highword ^ lowword;
    hash = (((hash << 3) & 0xFF00) | 0x0300) | (hash & 0x7F);
    return hash;
}

uint16_t generateLocId(uint16_t prot, uint16_t adrs) {

    if (prot == 0)
	prot = DCC_ACC;
    if (prot == 1)
	prot = MM_ACC;
    return (prot + adrs - 1);
}

uint16_t getadrs(uint16_t prot, uint16_t locid) {
    return (locid + 1 - prot);
}

void sendCanFrame(int canfd, MCANMSG can_frame) {
    struct can_frame frm;
    uint32_t canid;
    int e;

    memset(&frm, 0, sizeof(frm));

    canid = (can_frame.cmd << 17) | can_frame.hash;
    bitWrite(canid, 16, can_frame.resp_bit);
    canid &= CAN_EFF_MASK;
    canid |= CAN_EFF_FLAG;
    frm.can_id = canid;

    frm.can_dlc = can_frame.dlc;
    memcpy(frm.data, can_frame.data, 8);

    /* TODO error handling */
    while (write(canfd, &frm, sizeof(frm)) != sizeof(frm)) {
	e = errno;
	fprintf(stderr, "%s: error writing CAN frame: %s\n", __func__, strerror(errno));
	if (e != ENOBUFS) {
	    return;
	}
	fprintf(stderr, "Working around short txqueuelen by trying to sleep\n");
	sched_yield();
    }
}

/*
 *  Sendet die Basisinformationen über das Gerät an die GUI der CS2.
 */
void sendDeviceInfo(int canfd, CanDevice device, int configNum) {
    MCANMSG can_frame;
    int frameCounter = 0;

    can_frame.cmd = CONFIG;
    can_frame.resp_bit = 1;
    can_frame.dlc = 8;
    memset(can_frame.data, 0, 8);

    can_frame.data[1] = configNum;
    can_frame.data[7] = device.boardNum;
    can_frame.hash = 0x0301;

    sendCanFrame(canfd, can_frame);
    frameCounter++;

    memset(can_frame.data, 0, 8);
    if (strnlen(device.artNum, 8) == 8)
	memcpy(can_frame.data, device.artNum, 8);
    else
	strncpy((char *)can_frame.data, device.artNum, 8 - 1);

    can_frame.hash++;
    sendCanFrame(canfd, can_frame);
    frameCounter++;

    int nameLen = strlen(device.name);
    int neededFrames;

    if (nameLen % 8)
	neededFrames = (nameLen / 8) + 1;
    else
	neededFrames = nameLen / 8;

    for (int i = 0; i < neededFrames; i++) {
	for (int j = 0; j < 8; j++) {
	    if ((8 * i) + j < nameLen)
		can_frame.data[j] = device.name[(8 * i) + j];
	    else
		can_frame.data[j] = 0;
	}
	can_frame.hash++;
	sendCanFrame(canfd, can_frame);
	frameCounter++;
    }

    can_frame.hash = device.hash;
    can_frame.dlc = 6;
    set_uid(can_frame.data, device.uid);
    can_frame.data[4] = 0;
    can_frame.data[5] = frameCounter;

    sendCanFrame(canfd, can_frame);
}

void sendConfigInfoDropdown(int canfd, CanDevice device, uint8_t configChanel, uint8_t numberOfOptions,
			    uint8_t defaultSetting, char *settings) {
    MCANMSG can_frame;
    int frameCounter = 0;

    can_frame.cmd = CONFIG;
    can_frame.resp_bit = 1;
    can_frame.hash = 0x0301;
    can_frame.dlc = 8;

    //Erster Frame mit Grundinformationen:

    can_frame.data[0] = configChanel;
    can_frame.data[1] = 1;
    can_frame.data[2] = numberOfOptions;
    can_frame.data[3] = defaultSetting;
    memset(&can_frame.data[4], 0, 4);

    sendCanFrame(canfd, can_frame);
    can_frame.hash++;
    frameCounter++;

    //Frames, die Strings enthalten:

    int length = strlen(settings);
    int neededFrames;

    if (length % 8)
	neededFrames = (length / 8) + 1;
    else
	neededFrames = length / 8;

    for (int i = 0; i < neededFrames; i++) {
	for (int j = 0; j < 8; j++) {
	    if (((8 * i) + j < length) && (settings[(8 * i) + j] != '_'))
		can_frame.data[j] = settings[(8 * i) + j];
	    else
		can_frame.data[j] = 0;
	}

	sendCanFrame(canfd, can_frame);
	can_frame.hash++;
	frameCounter++;
    }

    //Abschließender Bestätigungsframe:

    can_frame.hash = device.hash;
    can_frame.dlc = 6;
    set_uid(can_frame.data, device.uid);
    can_frame.data[4] = configChanel;
    can_frame.data[5] = frameCounter;

    sendCanFrame(canfd, can_frame);
}

void sendConfigInfoSlider(int canfd, CanDevice device, uint8_t configChanel, uint16_t lowerValue, uint16_t upperValue,
			  uint16_t defaultValue, char *settings) {
    MCANMSG can_frame;
    int frameCounter = 0;

    can_frame.cmd = CONFIG;
    can_frame.resp_bit = 1;
    can_frame.hash = 0x0301;
    can_frame.dlc = 8;

    //Erster Frame mit Grundinformationen:

    can_frame.data[0] = configChanel;
    can_frame.data[1] = 2;
    can_frame.data[2] = lowerValue >> 8;
    can_frame.data[3] = lowerValue;
    can_frame.data[4] = upperValue >> 8;
    can_frame.data[5] = upperValue;
    can_frame.data[6] = defaultValue >> 8;
    can_frame.data[7] = defaultValue;

    sendCanFrame(canfd, can_frame);
    can_frame.hash++;
    frameCounter++;

    //Frames, die Strings enthalten:

    int length = strlen(settings);
    int neededFrames;

    if (length % 8)
	neededFrames = (length / 8) + 1;
    else
	neededFrames = length / 8;

    for (int i = 0; i < neededFrames; i++) {
	for (int j = 0; j < 8; j++) {
	    if (((8 * i) + j < length) && (settings[(8 * i) + j] != '_'))
		can_frame.data[j] = settings[(8 * i) + j];
	    else
		can_frame.data[j] = 0;
	}

	sendCanFrame(canfd, can_frame);
	can_frame.hash++;
	frameCounter++;
    }

    //Abschließender bestätigungsframe:

    can_frame.hash = device.hash;
    can_frame.dlc = 6;
    set_uid(can_frame.data, device.uid);
    can_frame.data[4] = configChanel;
    can_frame.data[5] = frameCounter;

    sendCanFrame(canfd, can_frame);
}

void sendPingFrame(int canfd, CanDevice device, bool response) {
    MCANMSG can_frame;
    can_frame.cmd = PING;
    can_frame.hash = device.hash;
    can_frame.resp_bit = response;
    can_frame.dlc = 8;
    set_uid(can_frame.data, device.uid);
    can_frame.data[4] = device.versHigh;
    can_frame.data[5] = device.versLow;
    can_frame.data[6] = device.type >> 8;
    can_frame.data[7] = device.type;

    sendCanFrame(canfd, can_frame);
}

void switchAccResponse(int canfd, CanDevice device, uint32_t locId, bool state) {

    MCANMSG can_frame;

    can_frame.cmd = SWITCH_ACC;
    can_frame.resp_bit = 1;
    can_frame.hash = device.hash;
    can_frame.dlc = 6;
    can_frame.data[0] = 0;
    can_frame.data[1] = 0;
    can_frame.data[2] = locId >> 8;
    can_frame.data[3] = locId;
    can_frame.data[4] = state;	/* Meldung der Lage für Märklin-Geräte. */
    can_frame.data[5] = 0;
    sendCanFrame(canfd, can_frame);

    can_frame.data[4] = 0xfe - state;	/* Meldung für CdB-Module und Rocrail Feldereignisse. */
    sendCanFrame(canfd, can_frame);

    //Serial.println("S88-Weichenevent");

    can_frame.cmd = S88_EVENT;
    can_frame.dlc = 8;
    can_frame.data[2] = locId >> 8;
    can_frame.data[3] = locId - (state - 1);
    can_frame.data[4] = 1;
    can_frame.data[5] = 0;
    can_frame.data[6] = 0;
    can_frame.data[7] = 0;
    sendCanFrame(canfd, can_frame);
    can_frame.data[3] = locId + state;
    can_frame.data[4] = 0;
    can_frame.data[5] = 1;
    sendCanFrame(canfd, can_frame);
}

void sendAccessoryFrame(int canfd, CanDevice device, uint32_t locId, bool state, bool response, bool power) {

    MCANMSG can_frame;

    can_frame.cmd = SWITCH_ACC;
    can_frame.resp_bit = response;
    can_frame.hash = device.hash;
    can_frame.dlc = 6;
    can_frame.data[0] = 0;
    can_frame.data[1] = 0;
    can_frame.data[2] = locId >> 8;
    can_frame.data[3] = locId;
    can_frame.data[4] = state;
    can_frame.data[5] = power;
    sendCanFrame(canfd, can_frame);
}

void checkS88StateFrame(int canfd, CanDevice device, uint16_t dev_id, uint16_t contact_id) {
    MCANMSG can_frame;

    can_frame.cmd = S88_EVENT;
    can_frame.resp_bit = 0;
    can_frame.hash = device.hash;
    can_frame.dlc = 4;
    can_frame.data[0] = dev_id >> 8;
    can_frame.data[1] = dev_id;
    can_frame.data[2] = contact_id >> 8;
    can_frame.data[3] = contact_id;
    sendCanFrame(canfd, can_frame);
}

void sendS88Event(int canfd, CanDevice device, uint16_t dev_id, uint16_t contact_id, uint8_t old, uint8_t new) {
    MCANMSG can_frame;

    can_frame.cmd = S88_EVENT;
    can_frame.resp_bit = 1;
    can_frame.hash = device.hash;
    can_frame.dlc = 8;
    can_frame.data[0] = dev_id >> 8;
    can_frame.data[1] = dev_id;
    can_frame.data[2] = contact_id >> 8;
    can_frame.data[3] = contact_id;
    can_frame.data[4] = old;
    can_frame.data[5] = new;
    can_frame.data[6] = 0;	/* time not implemented */
    can_frame.data[7] = 0;	/* time not implemented */
    sendCanFrame(canfd, can_frame);
}

MCANMSG getCanFrame(int canfd) {
    struct can_frame frm;
    uint32_t rxId;
    MCANMSG can_frame;

    /* TODO: error handling */
    memset(&frm, 0, sizeof(frm));
    if (read(canfd, &frm, sizeof(frm)) < 0 ) {
	fprintf(stderr, "error reading CAN frame: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    memset(&can_frame, 0, sizeof(can_frame));
    memcpy(can_frame.data, frm.data, 8);
    can_frame.dlc = frm.can_dlc;
    rxId = frm.can_id;
    /* & CAN_EFF_FLAG check skipped */
    if (rxId & CAN_EFF_FLAG) {
	can_frame.cmd = (rxId >> 17) ;
	can_frame.hash = rxId & 0xffff;
	can_frame.resp_bit = bitRead(rxId, 16);
    }

    can_frame.cmd = (rxId >> 17);
    can_frame.hash = rxId & 0xffff;
    can_frame.resp_bit = bitRead(rxId, 16);

//      Serial.println(canFrameToString(can_frame, false));

    return can_frame;
}

int checkAccessoryFrame(MCANMSG can_frame, uint16_t locIds[], int accNum, bool response) {

    uint16_t currentLocId = (can_frame.data[2] << 8) | can_frame.data[3];

    if ((can_frame.cmd == SWITCH_ACC) && (can_frame.resp_bit == response)) {
	for (int i = 0; i < accNum; i++) {
	    if (currentLocId == locIds[i])
		return i;
	}
    }

    return -1;
}

void statusResponse(int canfd, CanDevice device, int chanel, int success) {
    MCANMSG can_frame_out;

    can_frame_out.cmd = SYS_CMD;
    can_frame_out.hash = device.hash;
    can_frame_out.resp_bit = true;
    can_frame_out.dlc = 7;
    set_uid(can_frame_out.data, device.uid);
    can_frame_out.data[4] = SYS_STAT;
    can_frame_out.data[5] = chanel;
    can_frame_out.data[6] = success;
    can_frame_out.data[7] = 0;

    sendCanFrame(canfd, can_frame_out);
}
