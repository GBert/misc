/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <ixam97@ixam97.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * Maximilian Goldschmidt
 * ----------------------------------------------------------------------------
 * https://github.com/Ixam97
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <net/if.h>

#include <linux/can.h>

#include <libwebsockets.h>

#include "maecan_common.h"
#include "color_defs.h"

#include "version.h"

char version[16];

uint8_t first_ping_loop = 0;

uint8_t master = 0;

uint8_t power = 0;		/* Track power on (1) or off (0) */

struct device_t *devices;		/* Array of CAN-bus devices */
uint8_t n_devices;		/* Number of CAN-Bus devices */

/* Socket stuff */
struct ifreq ifr;
struct sockaddr_can addr;
int socketcan;

fd_set readfds;

uint8_t can_monitor, lws_monitor, print_timestamps;	/* Logging arguments */

uint8_t busy_request = 0;	/* Currely requesting something */
uint8_t devices_changed;
uint32_t *request_que = 0;	/* Array of qued UIDs */
uint8_t *request_que_index = 0;	/* Array of qued channel indexes */
uint8_t que_len = 0;		/* Lenght of the current request queue */

uint32_t last_que;
uint8_t last_que_index;
uint8_t last_que_tries;
uintmax_t last_que_us;

/* Websocket stuff */
struct lws_context *context;
uint8_t num_clients = 0;	/* number of connected websocket clients */
uint8_t callback_request;	/* 1 if data needs to be sent, don't interrupt if possible */
char *lws_tx_buffer;		/* Buffer to send to websocket */

void printUsage(char *progname) {
    printf("\nUsage: %s [can] [lws] [stamp]\n  Version: %s\n\n", progname, version);
    printf("         can    Print CAN-Bus trace.\n");
    printf("         lws    Print websocket trace.\n");
    printf("         stamp  Print timestamps.\n\n");
}

uint16_t be16(uint8_t *u) {
    return (u[0] << 8) | u[1];
}

uint32_t be32(uint8_t *u) {
    return (u[0] << 24) | (u[1] << 16) | (u[2] << 8) | u[3];
}

void logTime() {
    if (print_timestamps == 1) {
        /* Printing timestamp if argument is set */
        struct timeval tp;
        gettimeofday(&tp, 0);
        time_t timer;
        char buffer[26];
        struct tm *tm_info;
        time(&timer);
        tm_info = localtime(&timer);
        strftime(buffer, 26, "%Y/%m/%d %H:%M:%S", tm_info);
        printf("[%s:%04ld] ", buffer, tp.tv_usec / 100);
    }
}

void logTimeDummy() {
    /* print spaces to align with timestamp */
    if (print_timestamps == 1)
    printf("                           ");
}

void printCanFrame(struct can_frame *frame, uint8_t dir) {
    /* print raw CAN-frame to console */
    logTime();
    if (dir) {
        printf("CAN <-  ");
    } else {
        printf("CAN  -> ");
    }
    printf("%07x [%d] ", frame->can_id & 0xfffffff, frame->can_dlc);
    for (int i = 0; i < frame->can_dlc; i++) {
        printf("%02x ", frame->data[i]);
    }
    for (int i = frame->can_dlc; i < 8; i++) {
        printf("-- ");
    }

    /* print charackters if printable */
    printf("| ");
    for (uint8_t i = 0; i < frame->can_dlc; i++) {
        if (isprint(frame->data[i]))
            printf("%c", frame->data[i]);
        else
            printf(".");
    }
    for (uint8_t i = frame->can_dlc; i < 8; i++) {
        printf(".");
    }
    printf("\n");
}

void sendCanFrame(uint16_t cmd, uint8_t resp, uint16_t hash, uint8_t dlc, uint8_t * data) {
    /* send CAN-frame over SocketCAN */
    struct can_frame frame;
    memset(&frame, 0, sizeof(frame));

    frame.can_id = ((uint32_t) cmd << 17) | ((uint32_t) resp << 16) | (hash);
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
    if (can_monitor == 1)
    printCanFrame(&frame, 1);
    write(socketcan, &frame, sizeof(frame));
    usleep(10000);
}

void fillLwsTxBuffer(char *tx_buffer) {
    /* prepare websocket buffer to send data */
    size_t len = strlen(tx_buffer) + 1;
    lws_tx_buffer = realloc(lws_tx_buffer, len);
    memset(lws_tx_buffer, '\0', len);
    strcpy(lws_tx_buffer, tx_buffer);
    /* callback_request = 1; */
}

void go(void) {
    /* Send CAN go frame */
    uint8_t data[] = { 0, 0, 0, 0, 1, 0, 0, 0 };
    sendCanFrame(SYS_CMD, 0, 0x300, 5, data);
}

void stop(void) {
    /* Send CAN stop frame */
    uint8_t data[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    sendCanFrame(SYS_CMD, 0, 0x300, 5, data);
}

void sendPing(void) {
    /* Send broadcast Ping */
    logTime();
    printf(YEL "[Request]  " RESET "Ping broadcast.\n");
    uint8_t data[] = { 0, 0, 0, 0, 0, 0, 0xff, 0xff };
    sendCanFrame(PING, 0, 0x0300, 0, NULL);
    sendCanFrame(PING, 1, 0x300, 8, data);
}

void createDeviceFromPing(uint8_t * data) {
    /* Add device to devices array */
    n_devices++;
    devices = realloc(devices, n_devices * sizeof(struct device_t));
    devices[n_devices - 1].uid = be32((uint8_t *)&data[0]);
    devices[n_devices - 1].v_low = data[4];
    devices[n_devices - 1].v_high = data[5];
    devices[n_devices - 1].type = be16(&data[6]);
    logTimeDummy();
    printf("           └─ Added device 0x%08x.\n", devices[n_devices - 1].uid);
    logTimeDummy();
    printf("              └─ Need to get basic device info...\n");
}

void readingColor(uint8_t value, char **color) {
    /* generate "rgb(r,g,b)" string for readings channels */
    uint8_t rgb[3];
    rgb[0] = 85 * (value >> 6);
    rgb[1] = 85 * ((value & 0x30) >> 4);
    rgb[2] = 85 * ((value & 0x0C) >> 2);

    sprintf(*color, "rgb(%d,%d,%d)", rgb[0], rgb[1], rgb[2]);
}

void updateDeviceFromBuffer(uint32_t uid, uint8_t index, char *buffer, uint8_t buffer_len) {
    /* update device with new information */
    for (uint8_t i = 0; i < n_devices; i++) {
        if (devices[i].uid == uid) {

            if (index == 0) {
                /* Add basic information to device: */

                devices[i].n_reads = buffer[0];
                devices[i].n_configs = buffer[1];
                devices[i].serial_nbr = be32((uint8_t *)&buffer[4]);

                memcpy(devices[i].product_nbr, buffer + 8, 8);

                uint8_t name_len = strlen(&buffer[16]);

                /* for (uint8_t j = 16; j < ((buffer_len * 8)); j++) {
                    if (buffer[j] == 0) {
                    break;
                    } else {
                    name_len++;
                    }
                } */

                devices[i].name = (char *)malloc((name_len + 1) * sizeof(char));

                memcpy(devices[i].name, buffer + 16, name_len + 1);

                logTimeDummy();
                printf("           └─ Updated device 0x%08x with basic info.\n", uid);

                if (devices[i].n_reads > 0) {
                    devices[i].readings = malloc(0);
                    logTimeDummy();
                    printf("              └─ Need to get %d channels for readings...\n", devices[i].n_reads);

                    for (uint8_t j = 0; j < devices[i].n_reads; j++) {
                        /* que in requests for all readings channels of device i */
                        request_que = realloc(request_que, (que_len + 1) * sizeof(uint32_t));
                        request_que_index = realloc(request_que_index, (que_len + 1) * sizeof(uint8_t));
                        request_que[que_len] = uid;
                        request_que_index[que_len] = j + 1;
                        que_len++;
                    }
                }

                if (devices[i].n_configs > 0) {
                    devices[i].config_types = malloc(0);
                    devices[i].configs = malloc(0);
                    logTimeDummy();
                    printf("              └─ Need to get %d channels for config...\n", devices[i].n_configs);

                    for (uint8_t j = 0; j < devices[i].n_configs; j++) {
                        /* queue in requests for all config channels of device i */
                        request_que = realloc(request_que, (que_len + 1) * sizeof(uint32_t));
                        request_que_index = realloc(request_que_index, (que_len + 1) * sizeof(uint8_t));
                        request_que[que_len] = uid;
                        request_que_index[que_len] = j + 1 + devices[i].n_reads;
                        que_len++;
                    }
                }

                devices_changed = 1;

            } else if (index > 0 && index <= devices[i].n_reads) {
                /* Add reading channel: */

                devices[i].readings = realloc(devices[i].readings, index * sizeof(struct reading_t));
                devices[i].readings[index - 1].index = buffer[0];
                devices[i].readings[index - 1].power = buffer[1];

                devices[i].readings[index - 1].origin = be16((uint8_t *)&buffer[6]);

                char *tmp_color = malloc(17);

                for (uint8_t j = 0; j < 4; j++) {
                    memset(tmp_color, 0, 17);
                    readingColor(buffer[j + 2], &tmp_color);
                    strcpy(devices[i].readings[index - 1].colors[j], tmp_color);
                    devices[i].readings[index - 1].ranges[j] = be16((uint8_t *)&buffer[8 + (j * 2)]);

                }

                free(tmp_color);

                uint8_t limiter = 16;

                devices[i].readings[index - 1].name = malloc(strlen(&buffer[limiter]) + 1);
                memset(devices[i].readings[index - 1].name, 0, strlen(&buffer[limiter]) + 1);
                memcpy(devices[i].readings[index - 1].name, &buffer[limiter], strlen(&buffer[limiter]));
                limiter += (strlen(&buffer[limiter]) + 1);

                devices[i].readings[index - 1].start = malloc(strlen(&buffer[limiter]) + 1);
                memset(devices[i].readings[index - 1].start, 0, strlen(&buffer[limiter]) + 1);
                memcpy(devices[i].readings[index - 1].start, &buffer[limiter], strlen(&buffer[limiter]));
                limiter += (strlen(&buffer[limiter]) + 1);

                devices[i].readings[index - 1].end = malloc(strlen(&buffer[limiter]) + 1);
                memset(devices[i].readings[index - 1].end, 0, strlen(&buffer[limiter]) + 1);
                memcpy(devices[i].readings[index - 1].end, &buffer[limiter], strlen(&buffer[limiter]));
                limiter += (strlen(&buffer[limiter]) + 1);

                devices[i].readings[index - 1].unit = malloc(strlen(&buffer[limiter]) + 1);
                memset(devices[i].readings[index - 1].unit, 0, strlen(&buffer[limiter]) + 1);
                memcpy(devices[i].readings[index - 1].unit, &buffer[limiter], strlen(&buffer[limiter]));
                limiter += (strlen(&buffer[limiter]) + 1);

                logTimeDummy();
                printf("           └─ Updated device 0x%08x with reading index %d.\n", uid, index);
                devices_changed = 1;

            } else if (index > devices[i].n_reads && index <= (devices[i].n_reads + devices[i].n_configs)) {
                /* Add config channel: */

                uint8_t i_config = index - devices[i].n_reads - 1;

                if (buffer[1] == 1) {
                    /* Dropdown */
                    devices[i].config_types = realloc(devices[i].config_types, sizeof(uint8_t) * (index - devices[i].n_reads));
                    devices[i].configs = realloc(devices[i].configs, sizeof(union config_t) * (index - devices[i].n_reads));
                    devices[i].config_types[i_config] = 1;

                    devices[i].configs[i_config].dropdown.index = buffer[0];
                    devices[i].configs[i_config].dropdown.n_options = buffer[2];
                    devices[i].configs[i_config].dropdown.def_option = buffer[3];

                    uint16_t limiter[devices[i].configs[i_config].dropdown.n_options + 1];
                    uint8_t limiter_index = 0;

                    /* get delimiters for substrings */
                    for (uint8_t j = 8; j < buffer_len * 8; j++) {
                        buffer[j - 8] = buffer[j];
                        if (buffer[j - 8] == 0) {
                            limiter[limiter_index] = j - 8;
                            //printf("Delimiter %d: %d\n", limiter_index, limiter[limiter_index]);
                            limiter_index++;
                            if (limiter_index == devices[i].configs[i_config].dropdown.n_options + 1)
                            break;
                        }
                    }

                    devices[i].configs[i_config].dropdown.description = malloc(limiter[0]);
                    memset(devices[i].configs[i_config].dropdown.description, 0, limiter[0]);
                    memcpy(devices[i].configs[i_config].dropdown.description, buffer, limiter[0] + 1);

                    devices[i].configs[i_config].dropdown.options =
                    malloc(devices[i].configs[i_config].dropdown.n_options * sizeof(char *));

                    for (uint8_t j = 0; j < devices[i].configs[i_config].dropdown.n_options; j++) {
                        devices[i].configs[i_config].dropdown.options[j] = malloc(limiter[j + 1] - limiter[j]);
                        memset(devices[i].configs[i_config].dropdown.options[j], 0, limiter[j + 1] - limiter[j]);
                        memcpy(devices[i].configs[i_config].dropdown.options[j], &buffer[limiter[j] + 1],
                            limiter[j + 1] - limiter[j]);
                        printf("Option %d: %s\n", j + 1, devices[i].configs[i_config].dropdown.options[j]);
                    }
                } else if (buffer[1] == 2) {
                    /* Slider */
                    devices[i].config_types = realloc(devices[i].config_types, sizeof(uint8_t) * (index - devices[i].n_reads));
                    devices[i].configs = realloc(devices[i].configs, sizeof(union config_t) * (index - devices[i].n_reads));
                    devices[i].config_types[i_config] = 2;

                    devices[i].configs[i_config].slider.index = buffer[0];
                    devices[i].configs[i_config].slider.min = be16((uint8_t *)&buffer[2]);
                    devices[i].configs[i_config].slider.max = be16((uint8_t *)&buffer[4]);
                    devices[i].configs[i_config].slider.def_value = be16((uint8_t *)&buffer[6]);

                    uint8_t limiter = 8;

                    devices[i].configs[i_config].slider.description = malloc(strlen(&buffer[limiter]) + 1);
                    memset(devices[i].configs[i_config].slider.description, 0, strlen(&buffer[limiter]) + 1);
                    memcpy(devices[i].configs[i_config].slider.description, &buffer[limiter], strlen(&buffer[limiter]));
                    limiter += (strlen(&buffer[limiter]) + 1);

                    devices[i].configs[i_config].slider.start = malloc(strlen(&buffer[limiter]) + 1);
                    memset(devices[i].configs[i_config].slider.start, 0, strlen(&buffer[limiter]) + 1);
                    memcpy(devices[i].configs[i_config].slider.start, &buffer[limiter], strlen(&buffer[limiter]));
                    limiter += (strlen(&buffer[limiter]) + 1);

                    devices[i].configs[i_config].slider.end = malloc(strlen(&buffer[limiter]) + 1);
                    memset(devices[i].configs[i_config].slider.end, 0, strlen(&buffer[limiter]) + 1);
                    memcpy(devices[i].configs[i_config].slider.end, &buffer[limiter], strlen(&buffer[limiter]));
                    limiter += (strlen(&buffer[limiter]) + 1);

                    devices[i].configs[i_config].slider.unit = malloc(strlen(&buffer[limiter]) + 1);
                    memset(devices[i].configs[i_config].slider.unit, 0, strlen(&buffer[limiter]) + 1);
                    memcpy(devices[i].configs[i_config].slider.unit, &buffer[limiter], strlen(&buffer[limiter]));
                }
                logTimeDummy();
                printf("           └─ Updated device 0x%08x with config index %d.\n", uid, index);
                devices_changed = 1;
            }
            break;
        }
    }
}

static int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    return 0;
}

static int callback_maecan(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:{
            /* notify about newly connected clients */
            num_clients++;
            logTime();
            printf(MAG "[Notice]   " RESET "Client connetcted to WebSocket, %d clients currently connected.\n", num_clients);

            /* Send power state to newly connected clients */
            if (power == 0) {
                fillLwsTxBuffer("stop");
            } else {
                fillLwsTxBuffer("go");
            }
            callback_request = 1;
            lws_callback_on_writable(wsi);
            break;
        }

        case LWS_CALLBACK_CLOSED:{
            /* notify about disconnected clients */
            num_clients--;
            logTime();
            printf(MAG "[Notice]  " RESET "Client disconnected from WebSocket, %d clients currently connected.\n", num_clients);
            break;
        }

        case LWS_CALLBACK_RECEIVE:{
            /* put incoming websocket data into buffer for further processing */
            char buffer[len + 1];

            memset(buffer, '\0', len + 1);
            strncpy(buffer, (char *)in, len);

            /* extract command from buffer */
            uint8_t cmd_len = 1 + strchr(buffer, ':') - buffer;
            char cmd[cmd_len];

            memset(cmd, 0, cmd_len);
            memcpy(cmd, buffer, cmd_len - 1);

            if (lws_monitor == 1) {
                /* print websocket data */
                logTime();
                printf("LWS  -> Size: %ld, Content: %s\n", (unsigned long)len, buffer);
            }
            /* Do stuff with buffer ... */

            if (strcmp(cmd, "go") == 0) {
                /* Received GO command */
                go();
                logTime();
                printf(YEL "[Request] " RESET " GO\n");

            } else if (strcmp(cmd, "stop") == 0) {
                /* Received STOP command */
                stop();
                logTime();
                printf(YEL "[Request] " RESET " STOP\n");

            } else if (strcmp(cmd, "getVersion") == 0) {
                /* Redeived request for server version */
                char tmp_buf[30];

                memset(tmp_buf, 0, 30);
                sprintf(tmp_buf, "updateVersion:%s", version);
                fillLwsTxBuffer(tmp_buf);
                callback_request = 1;

                lws_callback_on_writable(wsi);

            } else if (strcmp(cmd, "getDevicelist") == 0) {
                /* Received request for device list */
                char *device_buffer = malloc(0);

                generateDevicesJson(&device_buffer, devices, n_devices);
                lws_tx_buffer = realloc(lws_tx_buffer, strlen("updateDevicelist:") + strlen(device_buffer) + 1);
                memset(lws_tx_buffer, 0, strlen("updateDevicelist:") + strlen(device_buffer) + 1);
                sprintf(lws_tx_buffer, "updateDevicelist:%s", device_buffer);

                free(device_buffer);
                callback_request = 1;
                lws_callback_on_writable(wsi);

            } else if (strcmp(cmd, "getStatus") == 0) {
                /* Received request for reading */
                uint32_t uid;
                uint16_t channel;

                sscanf(&buffer[cmd_len], "%ld:%d", &uid, &channel);
                logTime();
                printf(YEL "[Request]  " RESET "statusinfo from UID 0x%08x, channel %d.\n", uid, channel);
                uint8_t data[] = { (uint8_t) (uid >> 24), (uint8_t) (uid >> 16), (uint8_t) (uid >> 8), (uint8_t) (uid), SYS_STATUS, channel };
                sendCanFrame(SYS_CMD, 0, 0x300, 6, data);

            } else if (strcmp(cmd, "setConfigValue") == 0) {
                uint32_t uid;
                uint8_t channel;
                uint16_t value;

                sscanf(&buffer[cmd_len], "%ld:%d:%d", &uid, &channel, &value);
                logTime();
                printf(YEL "[Request] " RESET "Set config value of UID 0x%08x, channel %d, to %d.\n", uid, channel, value);
                uint8_t data[] = { (uint8_t) (uid >> 24), (uint8_t) (uid >> 16), (uint8_t) (uid >> 8), (uint8_t) (uid), SYS_STATUS,
                    channel, (uint8_t) (value >> 8), (uint8_t) (value) };
                sendCanFrame(SYS_CMD, 0, 0x300, 8, data);

            } else if(strcmp(cmd, "delDevice") == 0) {
                /* Received request to delete CAN device from device list */

                uint32_t uid;
                uint8_t index = 0;
                uint8_t found_uid = 0;
                sscanf(&buffer[cmd_len], "%ld", &uid);
                for (uint8_t i = 0; i < n_devices; i++) {
                    if (devices[i].uid == uid) {
                        index = i;
                        found_uid = 1;
                        break;
                    }
                }
                if (found_uid == 1) {
                    for (uint8_t i = index; i < n_devices; i++) {
                        if (i < (n_devices - 1)) devices[i] = devices[i + 1];
                        else {
                            n_devices--;
                            devices = realloc(devices, n_devices * sizeof(struct device_t));
                        }
                    }
                    logTime(); 
                    printf(MAG "[Notice]  " RESET "UID 0x%08x deleted.\n", uid);
                } else {
                    logTime(); 
                    printf(MAG "[Notice]  " RESET "UID 0x%08x is not in device list.\n", uid);
                }
            }
            break;
        }
        case LWS_CALLBACK_SERVER_WRITEABLE:{
            if (lws_tx_buffer != NULL) {
                /* transmit prepared websocket buffer */
                size_t len = strlen(lws_tx_buffer) + 1;
                unsigned char buf[LWS_PRE + len];
                unsigned char *p = &buf[LWS_PRE];

                memset(p, 0, len);
                memcpy(p, lws_tx_buffer, len - 1);

                if (lws_write(wsi, p, len - 1, LWS_WRITE_TEXT) != 0 && lws_monitor == 1) {
                    logTime();
                    printf("LWS <-  Size: %ld, content: %s\n", (unsigned long)len - 1, p);
                }
                
                if (callback_request > 0)
                    callback_request--;
            }

            break;
        }
        default: break;
    }
    return 0;
}

const static struct lws_protocols protocols[] = {
    /* first protocol must always be HTTP handler */
    {
        "http-only",		// name
        callback_http,		// callback
        0				// per_session_data_size
    }, {
        "maecan",
        callback_maecan,
        0
    }, {
        NULL, NULL, 0, 0		/* End of list */
    }
};

void *pinger(void *argument) {

    logTime();
    printf(GRN "Created pinger thread. %s\n", RESET);

    /* int interval = *((int *)argument); */

    /* uintmax_t last_time = (uintmax_t) (clock() / CLOCKS_PER_SEC); */

    sendPing();
    uint8_t bootl_frame[] = { 0, 0, 0, 0, 0x11 };
    sendCanFrame(BOOTLOADER_CAN, 0, 0x300, 0, NULL);
    sendCanFrame(BOOTLOADER_CAN, 0, 0x300, 5, bootl_frame);

    uint8_t loops = 0;

    first_ping_loop = 1;

    while (1) {
        /* Send proadcast ping every 10 seconds */

        usleep(1000000);
        loops++;
        if (loops == 10) {
            sendPing();
            loops = 0;
        }

        if (busy_request == 1) {
            last_que_tries++;
            logTime();
            printf(RED "[RETRY]    " RESET "device info from UID 0x%08x, index %d.\n", last_que, last_que_index);

            uint8_t data[] = { (uint8_t) (last_que >> 24), (uint8_t) (last_que >> 16), (uint8_t) (last_que >> 8),
            (uint8_t) (last_que), last_que_index, 0, 0, 0 };

            sendCanFrame(STATUS_CONFIG, 0, 0x300, 5, data);
            if (last_que_tries >= 20) {
                busy_request = 0;
            }
        }
        if (que_len == 0 && busy_request == 0 && devices_changed == 1) {
            char *device_buffer;

            device_buffer = malloc(0);
            generateDevicesJson(&device_buffer, devices, n_devices);
            FILE *f = fopen("/www/MaeCAN-Server/html/devices.json", "w");

            if (f == NULL) {
                printf(RED "FILE ERROR!\n" RESET);
            } else {
                fwrite(device_buffer, strlen(device_buffer), 1, f);
                fclose(f);
                printf(MAG "[Notice]   " RESET "Updated devices.json file.\n");
            }

            free(device_buffer);
            devices_changed = 0;
        }
    }
}

void *canListener() {

    logTime();
    printf(GRN "Created can listener thread. %s\n", RESET);

    struct can_frame frame;
    memset(&frame, 0x0, sizeof(frame));

    char *status_buffer = 0;
    uint8_t buffer_len = 0;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(socketcan, &readfds);

        /* Reading can frame: */
        if (FD_ISSET(socketcan, &readfds)) {
            if (read(socketcan, &frame, sizeof(struct can_frame))) {

                while (callback_request > 0) {
                }

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
                /* Debug can monitor: */
                if (can_monitor == 1)
                    printCanFrame(&frame, 0);

                /* check for relevant commands from CAN: */
                switch (cmd) {
                    case SYS_CMD:{

                        /* check for relevant SYS_CMDs: */
                        switch (data[4]) {
                            case 0x00:{
                                /* Stop */
                                if (resp == 1) {
                                    power = 0;
                                    logTime();
                                    printf(CYN "[Received] " RED "STOP\n" RESET);
                                    fillLwsTxBuffer("stop");
                                    callback_request = num_clients;
                                    lws_callback_on_writable_all_protocol(context, &protocols[1]);
                                }
                                break;
                            }
                            case 0x01:{
                                /* Go */
                                if (resp == 1) {
                                    power = 1;
                                    logTime();
                                    printf(CYN "[Received] " GRN "GO\n" RESET);
                                    fillLwsTxBuffer("go");
                                    callback_request = num_clients;

                                    lws_callback_on_writable_all_protocol(context, &protocols[1]);
                                }
                                break;
                            }
                            case SYS_STATUS:{
                                if (resp == 1 && dlc == 8) {
                                    uint32_t uid = be32(&data[0]);
                                    logTime();
                                    printf(CYN "[Received] " RESET "statusinfo from UID 0x%08x, channel: %d, value: %d\n", uid, data[5], be16(&data[6]));
                                    
                                    char tx_msg[40];

                                    memset(tx_msg, 0, 40);
                                    sprintf(tx_msg, "updateReading:%ld:%d:%d", uid, data[5], be16(&data[6]));
                                    
                                    fillLwsTxBuffer(tx_msg);
                                    callback_request = num_clients;
                                    lws_callback_on_writable_all_protocol(context, &protocols[1]);

                                } else if (resp == 1 && dlc == 7) {
                                    uint32_t uid = be32(&data[0]);
                                    if (data[6] == 1) {
                                        logTime();
                                        printf(GRN "[Done]     " RESET "Config value of UID 0x%08x, channel %d, successfully set.\n", uid, data[5]);
                                    } else {
                                        logTime();
                                        printf(RED "[Warning]  " RESET "Config value of UID 0x%08x, channel %d, could not be written!", uid, data[5]);
                                    }
                                }
                            }
                        }

                    break;
                    }
                    case LOCO_DISCOVERY:
                        break;
                    case MFX_BIND:
                        break;
                    case MFX_VERIFY:
                        break;
                    case LOCO_SPEED:
                        break;
                    case LOCO_DIR:
                        break;
                    case LOCO_FN:
                        break;
                    case READ_CONFIG:
                        break;
                    case WRITE_CONFIG:
                        break;
                    case SWITCH_ACC:
                        break;
                    case CONFIG_ACC:
                        break;
                    case S88_POLL:
                        break;
                    case S88_EVENT:
                        break;
                    case SX1_EVENT:
                        break;

                    /* Ping-Frame: */
                    case PING:{
                        if (resp == 1) {
                            uint32_t uid = be32(&data[0]);

                            logTime();
                            printf(CYN "[Received] " RESET "ping response from UID 0x%08x.\n", uid);

                            if (first_ping_loop == 1) {
                                uint8_t known_device = 0;
                                for (uint8_t i = 0; i < n_devices; i++) {
                                    if (devices[i].uid == uid) {
                                        known_device = 1;
                                        break;
                                    }
                                }
                                if (known_device == 0) {
                                    /* Add to device que: */
                                    createDeviceFromPing(data);
                                    request_que = realloc(request_que, (que_len + 1) * sizeof(uint32_t));
                                    request_que_index = realloc(request_que_index, (que_len + 1) * sizeof(uint8_t));
                                    request_que[que_len] = uid;
                                    request_que_index[que_len] = 0;
                                    que_len++;
                                }
                            }

                        } else {
                            logTime();
                            printf(RED "[Warning]  " RESET
                            "Received ping request from other device! Make sure only ONE master is connected to the bus!\n");
                        }
                        break;
                        }

                    case UPDATE_OFFER:
                        break;
                    case READ_CONFIG_DATA:
                        break;
                    case BOOTLOADER_CAN:
                        break;
                    case BOOTLOADER_TRACK:
                        break;

                    case STATUS_CONFIG:{
                        if (dlc == 8 && resp == 1) {
                            buffer_len++;
                            status_buffer = realloc(status_buffer, (buffer_len * 8) * sizeof(uint8_t));
                            for (uint8_t i = 0; i < 8; i++) {
                            status_buffer[i + ((buffer_len - 1) * 8)] = data[i];
                            }
                        } else if (resp == 1) {
                            uint32_t uid = be32(&data[0]);
                            /* Work with buffer: */
                            logTime();
                            printf(GRN "[Done]     " RESET "device info from UID 0x%08x, index %d.\n", uid, data[4]);
                            updateDeviceFromBuffer(uid, data[4], status_buffer, buffer_len);
                            buffer_len = 0;
                            status_buffer = realloc(status_buffer, 0);
                            busy_request = 0;
                        }
                        break;
                        }
                    case DATA_QUERY:
                        break;
                    case CONFIG_DATA_STREAM:
                        break;
                    default:{
                        logTime();
                        printf(RED "Received unknown command 0x%02x! %s\n", cmd, RESET);

                        break;
                    }
                }
            }
        }

        if (que_len > 0 && busy_request == 0 && first_ping_loop == 1) {

            struct timeval tp;
            gettimeofday(&tp, 0);

            last_que_us = tp.tv_usec;

            last_que = request_que[0];
            last_que_index = request_que_index[0];
            last_que_tries = 0;

            logTime();
            printf(YEL "[Request]  " RESET "device info from UID 0x%08x, index %d.\n", request_que[0], request_que_index[0]);

            uint8_t data[] = {(uint8_t) (request_que[0] >> 24), (uint8_t) (request_que[0] >> 16), (uint8_t) (request_que[0] >> 8), (uint8_t) (request_que[0]), request_que_index[0], 0, 0, 0 };
            sendCanFrame(STATUS_CONFIG, 0, 0x300, 5, data);

            busy_request = 1;

            for (uint8_t i = 0; i < que_len; i++) {
                request_que[i] = request_que[i + 1];
                request_que_index[i] = request_que_index[i + 1];
            }
            que_len--;
            request_que = realloc(request_que, que_len * sizeof(uint32_t));
            request_que_index = realloc(request_que_index, que_len * sizeof(uint8_t));
        }
    }
    close(socketcan);

}

int main(int argc, char *argv[]) {

    devices = malloc(0);
    n_devices = 0;

    sprintf(version, "%s.%d", DATE, BUILD);

    if (argc > 1) {
        for (uint8_t i = 1; i < argc; i++) {
            if (strcmp(argv[i], "can") == 0) {
                can_monitor = 1;
            } else if (strcmp(argv[i], "lws") == 0) {
                lws_monitor = 1;
            } else if (strcmp(argv[i], "-v") == 0 && argc == 2) {
                printf("%s\n", version);
                exit(EXIT_SUCCESS);
            } else if (strcmp(argv[i], "stamp") == 0) {
                print_timestamps = 1;
            } else if (strcmp(argv[i], "master") == 0) {
                master = 1;
            } else {
                printUsage(argv[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    lws_tx_buffer = malloc(0);

    memset(&ifr, 0x0, sizeof(ifr));
    memset(&addr, 0x0, sizeof(addr));

    strcpy(ifr.ifr_name, "can0");
    socketcan = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    addr.can_family = AF_CAN;
    ioctl(socketcan, SIOCGIFINDEX, &ifr);
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(socketcan, (struct sockaddr *)&addr, sizeof(addr));

    stop();			/* initial STOP */

    pthread_t ping_tread;
    pthread_t can_listener_thread;
    int ping_interval_sec = 10;

    if (!pthread_create(&ping_tread, NULL, pinger, &ping_interval_sec)) {
        /* printf(GRN "Created pinger thread.%s\n", RESET); */
    }

    if (!pthread_create(&can_listener_thread, NULL, canListener, NULL)) {
        /* printf(GRN "Created can listener thread.%S\n", RESET); */
    }

    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));

    info.port = 8080;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;

    context = lws_create_context(&info);

    lws_set_log_level(0, NULL);

    while (1) {
        lws_service(context, 50);
    }

    lws_context_destroy(context);
    exit(EXIT_SUCCESS);
}
