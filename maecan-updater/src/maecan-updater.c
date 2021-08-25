/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* Maximilian Goldschmidt wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return.
* 
* ----------------------------------------------------------------------------
* MäCAN-Updater:
* https://github.com/Ixam97/SRSEII/maecan-updater/
* ----------------------------------------------------------------------------
*
* This updater was made for the MäCAN-Bootloader only: https://github.com/Ixam97/MaeCAN-Bootloader/
*
* Last changed: [2020-01-16.1]
*/
#define VERSION "[2020-01-16.1]"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <ctype.h>
#include <libgen.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <net/if.h>

#include <linux/can.h>

#define PAGE_SIZE 256

extern char *optarg;

/* Socket stuff */
struct ifreq ifr;
struct sockaddr_can addr;
int socketcan;

uint32_t can_uid;

uint32_t remote_uid;

fd_set readfds;

uint8_t page_buffer[PAGE_SIZE];
uint8_t page_buffer_index;
uint16_t page_count;
uint8_t page_index;

uint8_t remote_type;
uint16_t remote_pagesize;
uint16_t remote_pagecount;

uint8_t updating;
uint8_t listening;
uint8_t trys;
uint8_t retry;

uint8_t complete_buffer[0xfffff];
uint32_t complete_buffer_index;
uint32_t adress_offset = 0;

uint8_t slow_mode = 0;

void sendCanFrame(uint32_t id, uint8_t dlc, uint8_t * data) {
    /* send CAN-frame over SocketCAN */
    struct can_frame frame;
    memset(&frame, 0, sizeof(frame));

    frame.can_id = id;
    frame.can_id &= CAN_EFF_MASK;
    frame.can_id |= CAN_EFF_FLAG;
    frame.can_dlc = dlc;
    for (uint8_t i = 0; i < 8; i++) {
	if (data) {
	    frame.data[i] = data[i];
	} else {
	    frame.data[i] = 0;
	}
    }
    write(socketcan, &frame, sizeof(frame));
    usleep(500);
}

void sendPage(uint8_t page) {
    memcpy(page_buffer, &complete_buffer[remote_pagesize * page], remote_pagesize);
    uint8_t begin_frame_data[] = {(can_uid >> 24) & 0xff, (can_uid >> 16) & 0xff, (can_uid >> 8) & 0xff, (can_uid) & 0xff, 0x04, page, 0, 0};
    uint8_t finished_frame_data[] = {(can_uid >> 24) & 0xff, (can_uid >> 16) & 0xff, (can_uid >> 8) & 0xff, (can_uid) & 0xff, 0x05, page, 0, 0};
    uint8_t data_frame[8];
    uint8_t needed_frames = ((remote_pagesize - 1) / 8) + 1;

    printf("Sending page");

    sendCanFrame(0x00800300, 6, begin_frame_data);

    for (uint8_t i = 0; i < needed_frames; i++) {
	memcpy(data_frame, &page_buffer[i * 8], 8);
	sendCanFrame(0x00800300 + i + 1, 8, data_frame);
	printf(".");
	if (slow_mode == 1) {
	    usleep(20000);
	}
    }

    sendCanFrame(0x00800300, 6, finished_frame_data);
    printf("%d done.\n", page);

}

void printUsage(char *prg) {
    fprintf(stderr, "\nUsage: %s -f <update file> -t <target uid>\n", prg);
    fprintf(stderr, "   Version: %s\n\n", VERSION);
    fprintf(stderr, "         -f <update file> path to the update file in intel hex format\n");
    fprintf(stderr, "         -t <target uid>  CAN UID of the device to be updated\n\n");
    fprintf(stderr, "         -s               use slow mode in case of falure or to look at the can-monitor\n\n");
    fprintf(stderr, "         -h               show this help\n\n");
}

int main(int argc, char *argv[]) {

    int opt;
    char uidstring[256];
    char filepath[256];

    while ((opt = getopt(argc, argv, "f:t:sh?")) != -1) {
	switch (opt) {
	case 's':
	    slow_mode = 1;
	    printf("Slow mode\n");
	    break;
	case 'f':
	    strncpy(filepath, optarg, sizeof(filepath) - 1);
	    break;
	case 't':
	    strncpy(uidstring, optarg, sizeof(uidstring) - 1);
	    break;
	case 'h':
	case '?':
	    printUsage(basename(argv[0]));
	    exit(EXIT_SUCCESS);
	    break;
	default:
	    printUsage(basename(argv[0]));
	    exit(EXIT_FAILURE);
	    break;
	}
    }

    if (filepath[0] == 0 || uidstring[0] == 0) {
	printUsage(basename(argv[0]));
	exit(EXIT_FAILURE);
    }

    can_uid = (uint32_t) strtol(uidstring, NULL, 16);

    memset(&ifr, 0x0, sizeof(ifr));
    memset(&addr, 0x0, sizeof(addr));

    strcpy(ifr.ifr_name, "can0");
    socketcan = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    addr.can_family = AF_CAN;
    ioctl(socketcan, SIOCGIFINDEX, &ifr);
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(socketcan, (struct sockaddr *)&addr, sizeof(addr));

    struct can_frame frame;
    memset(&frame, 0x0, sizeof(frame));

    uint8_t out_data[8] = { 0 };

    /* pinging UID for Bootloader */
    out_data[0] = (can_uid >> 24) & 0xff;
    out_data[1] = (can_uid >> 16) & 0xff;
    out_data[2] = (can_uid >> 8) & 0xff;
    out_data[3] = (can_uid) & 0xff;

    /* File test */

    FILE *update_file;

    update_file = fopen(filepath, "r");

    char ch, s_byte_count[3], s_data_address[5], s_line_type[3], s_data_byte[3];
    uint8_t byte_count, line_type, data_byte;
    uint16_t data_adress;

    memset(complete_buffer, 0xff, 0xfffff);
    memset(page_buffer, 0xff, PAGE_SIZE);
    memset(s_byte_count, 0, 3);
    memset(s_data_address, 0, 5);
    memset(s_line_type, 0, 3);
    memset(s_data_byte, 0, 3);

    while ((ch = fgetc(update_file)) != EOF) {
	if (ch == ':') {
	    // New File Line
	    s_byte_count[0] = fgetc(update_file);
	    s_byte_count[1] = fgetc(update_file);
	    byte_count = strtol(s_byte_count, NULL, 16);

	    s_data_address[0] = fgetc(update_file);
	    s_data_address[1] = fgetc(update_file);
	    s_data_address[2] = fgetc(update_file);
	    s_data_address[3] = fgetc(update_file);
	    data_adress = strtol(s_data_address, NULL, 16);

	    s_line_type[0] = fgetc(update_file);
	    s_line_type[1] = fgetc(update_file);
	    line_type = strtol(s_line_type, NULL, 16);

	    if (line_type == 0x00) {
		for (uint8_t i = 0; i < byte_count; i++) {
		    // Read data bytes 
		    s_data_byte[0] = fgetc(update_file);
		    s_data_byte[1] = fgetc(update_file);
		    data_byte = strtol(s_data_byte, NULL, 16);
		    //complete_buffer[complete_buffer_index] = data_byte;
		    //complete_buffer_index++;
		    complete_buffer[adress_offset + data_adress + i] = data_byte;
		    complete_buffer_index = adress_offset + data_adress + i;
		}
	    } else if (line_type == 0x02) {
		s_data_byte[0] = 0;
		s_data_byte[1] = fgetc(update_file);
		data_byte = strtol(s_data_byte, NULL, 16);
		adress_offset = (uint32_t) data_byte << 0x16;
		data_byte = 0xff;

	    }
	}
    }

    fclose(update_file);

    sendCanFrame(0x00800300, 4, out_data);

    printf("Looking for Device with UID 0x%8x...\n", can_uid);

    listening = 1;

    while (listening) {
	uint8_t received_frame = 0;

	FD_ZERO(&readfds);
	FD_SET(socketcan, &readfds);
	/* Reading can frame: */
	struct timeval timeout = { 1, 0 };

	if (select((socketcan + 1), &readfds, NULL, NULL, &timeout) >= 0) {
	    if (FD_ISSET(socketcan, &readfds)) {
		if (read(socketcan, &frame, sizeof(struct can_frame))) {

		    received_frame = 1;

		    uint8_t cmd = (uint8_t) (frame.can_id >> 17);
		    uint8_t resp = (uint8_t) ((frame.can_id >> 16) & 0x01);
		    /* uint16_t hash = (uint16_t) (frame.can_id); */
		    uint8_t dlc = frame.can_dlc;
		    uint8_t data[8];
		    for (uint8_t i = 0; i < dlc; i++) {
			data[i] = frame.data[i];
		    }
		    for (uint8_t i = dlc; i < 8; i++) {
			data[i] = 0;
		    }

		    /* check for relevant commands from CAN: */
		    if (cmd == 0x40 && (dlc == 6 || dlc == 7)) {
			/* check for UID */
			if ((data[0] == (uint8_t) (can_uid >> 24)) && (data[1] == (uint8_t) (can_uid >> 16))
			    && (data[2] == (uint8_t) (can_uid >> 8)) && (data[3] == (uint8_t) can_uid)) {

			    switch (data[4]) {
			    case 0x01:{
				    // Type:
				    remote_type = data[5];
				    break;
				}
			    case 0x02:{
				    // Pagesize:
				    remote_pagesize = (data[5] << 8) + data[6];
				    break;
				}
			    case 0x03:{
				    // Pagecount:
				    remote_pagecount = (data[5] << 8) + data[6];
				    break;
				}
			    case 0x05:{
				    if (dlc == 7 && data[5] == (page_index - 1)) {
					if (data[6] == 0) {
					    // Transmisson error, new attempt:
					    printf("Page error, new attempt...\n");
					    sendPage(page_index - 1);
					} else if (page_index < page_count) {
					    if (data[6] == 1) {
						// Transfer successfull, next page:
						sendPage(page_index);
						page_index++;
					    }
					} else {
					    // Update complete
					    out_data[4] = 0x07;
					    sendCanFrame(0x00800300, 6, out_data);
					    printf("Update completed.\n");
					    close(socketcan);
					    exit(EXIT_SUCCESS);
					}

				    }
				}
			    default:{
				    //break;
				}
			    }
			}

			if (remote_type != 0 && remote_pagecount != 0 && remote_pagesize != 0 && updating == 0) {
			    // Check for compatibility:
			    updating = 1;
			    out_data[4] = 0x01;
			    out_data[5] = remote_type;
			    page_count = (complete_buffer_index / remote_pagesize) + 1;
			    if (retry == 0) {
				printf("Device found! \nByte count: %d, Page count: %d\n", complete_buffer_index,
				       page_count);
			    }
			    if (page_count > remote_pagecount) {
				// Update too large:
				printf("New update too large, aborting.\n");
				out_data[6] = 0;
				sendCanFrame(0x00800300, 7, out_data);
				exit(EXIT_FAILURE);

			    } else {

				if (retry == 0) {
				    //usleep(1000000);
				    printf("Beginning update...\n");
				    page_index = 0;
				    out_data[6] = 1;
				    sendCanFrame(0x00810300, 7, out_data);
				    sendPage(page_index);
				    page_index++;

				} else {

				    out_data[6] = 1;
				    sendCanFrame(0x00810300, 7, out_data);
				    sendPage(page_index);
				    page_index++;
				}
			    }
			}
		    }
		}
	    }
	}

	if (updating == 0 && trys < 10 && received_frame == 0) {
	    trys++;
	    printf("Try %d ... \n", trys);

	    out_data[0] = (can_uid >> 24) & 0xff;
	    out_data[1] = (can_uid >> 16) & 0xff;
	    out_data[2] = (can_uid >> 8) & 0xff;
	    out_data[3] = (can_uid) & 0xff;

	    sendCanFrame(0x00800300, 4, out_data);

	} else if (trys >= 10) {
	    printf("Device unreachable!\n");
	    listening = 0;

	    close(socketcan);
	    //break;
	} else if (updating == 1 && page_index == 1 && received_frame == 0) {
	    if (retry >= 1) {
		printf("Transmission error! \n");
		trys++;
	    }
	    page_index = 0;
	    updating = 0;
	    retry = 1;

	    out_data[0] = (can_uid >> 24) & 0xff;
	    out_data[1] = (can_uid >> 16) & 0xff;
	    out_data[2] = (can_uid >> 8) & 0xff;
	    out_data[3] = (can_uid) & 0xff;

	    sendCanFrame(0x00800300, 4, out_data);
	}
    }
    close(socketcan);

}
