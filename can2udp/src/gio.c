/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "can2lan.h"
#include <zlib.h>

/* it seems Gleisbox needs a short break after every CAN message */
/* use 10ms delay between two consequences CAN frames */
#define TIME_WAIT_US	10 * 1000
/* a shorter break for updates */
#define TIME_WAIT_US_SH	2 * 1000

/* CHUNK is the size of the memory chunk used by the zlib routines. */
#define CHUNK		0x8000
#define windowBits	15
#define GZIP_ENCODING	16

unsigned char GETCONFIG_RESPONSE[]    = { 0x00, 0x42, 0x03, 0x00, 0x06 };
unsigned char GETCONFIG_RESPONSE_II[] = { 0x00, 0x42, 0x03, 0x00, 0x07 };

extern unsigned char GETCONFIG_DATA[];
extern unsigned char GETCONFIG[];
extern struct timeval last_sent;
extern char *cs2_configs[][2];
extern char **page_name;

/* The following macro calls a zlib routine and checks the return
   value. If the return value ("status") is not OK, it prints an error
   message and exits the program. Zlib's error statuses are all less
   than zero. */

#define CALL_ZLIB(x) {                                                  \
        int status;                                                     \
        status = x;                                                     \
        if (status < 0) {                                               \
            fprintf (stderr,                                            \
                     "%s:%d: %s returned a bad status of %d.\n",        \
                     __FILE__, __LINE__, #x, status);                   \
            exit (EXIT_FAILURE);                                        \
        }                                                               \
    }

/*
 * return string with CRLF removed from end
 */
char *rmcrlf(char *s, int slen) {
    int l;

    s[slen - 1] = '\0';
    l = strlen(s) - 1;

    while (l >= 0 && (s[l] == '\r' || s[l] == '\n'))
	s[l--] = '\0';

    return s;
}

void print_pages(char **page_name) {
    int i = 0;
    printf("track pages:\n");
    while (page_name[i]) {
	printf("track %2d -> %s\n", i, page_name[i]);
	i++;
    }
}

void usec_sleep(int usec) {
    struct timespec to_wait;

    to_wait.tv_sec = 0;
    to_wait.tv_nsec = usec * 1000;
    nanosleep(&to_wait, NULL);
}

int time_stamp(char *timestamp) {
    /* char *timestamp = (char *)malloc(sizeof(char) * 16); */
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
    return 0;
}

/*
  CS2 CAN hash generation

  hash' = highword ^ lowword

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

char **read_track_file(char *filename, char **page_name) {
    FILE *fp;
    int id = 0;
    int page = 0;
    char line[MAXLINE];

    syslog(LOG_NOTICE, "%s: read %s", __func__, filename);

    if ((fp = fopen(filename, "r")) != NULL) {
	while (fgets(line, MAXLINE, fp) != NULL) {
	    if (strstr(line, "seite") == line) {
		page = 1;
	    } else if (strstr(line, " .id=") == line) {
		id = strtoul(&line[5], NULL, 0);
	    } else if (strstr(line, " .name=") == line) {
		rmcrlf(line, MAXLINE);
		if (page) {
		    page_name[id] = calloc(strlen(&line[7]) + 5, 1);
		    if (page_name[id] == NULL) {
			fprint_syslog_wc(stderr, LOG_ERR, "error calloc failed creating config buffer for", filename);
			return NULL;
		    }
		    strcpy(page_name[id], &line[7]);
		    strcat(page_name[id], ".cs2");
		}
	    }
	}
	fclose(fp);
	/* EOF found, normal exit */
	return page_name;
    } else {
	fprintf(stderr, "%s: error reading file %s: %s\n", __func__, filename, strerror(errno));
	syslog(LOG_ERR, "%s: error reading file %s: %s\n", __func__, filename, strerror(errno));
	return NULL;
    }
}

void free_track_file(char **page_name) {
    int i = 0;

    while (page_name[i]) {
	free(page_name[i]);
	i++;
    }
}

void print_can_frame(char *format_string, unsigned char *netframe, int verbose) {
    uint32_t canid;
    int i, dlc;
    char timestamp[16];

    if (!verbose)
	return;

    memcpy(&canid, netframe, 4);
    dlc = netframe[4];
    time_stamp(timestamp);
    printf("%s   ", timestamp);
    printf(format_string, ntohl(canid) & CAN_EFF_MASK, netframe[4]);
    for (i = 5; i < 5 + dlc; i++) {
	printf(" %02x", netframe[i]);
    }
    if (dlc < 8) {
	printf("(%02x", netframe[i]);
	for (i = 6 + dlc; i < CAN_ENCAP_SIZE; i++) {
	    printf(" %02x", netframe[i]);
	}
	printf(")");
    } else {
	printf(" ");
    }
    printf("  ");
    for (i = 5; i < CAN_ENCAP_SIZE; i++) {
	if (isprint(netframe[i]))
	    printf("%c", netframe[i]);
	else
	    putchar('.');
    }

    printf("\n");
}

int net_to_net(int net_socket, struct sockaddr *net_addr, unsigned char *netframe, int length) {
    int s;

    s = sendto(net_socket, netframe, length, 0, net_addr, sizeof(*net_addr));
    if (s != length) {
	fprint_syslog_wc(stderr, LOG_ERR, "error sending TCP/UDP data:", strerror(errno));
	return -1;
    }
    return 0;
}

int frame_to_net(int net_socket, struct sockaddr *net_addr, struct can_frame *frame) {
    int s;
    uint32_t canid;

    memset(netframe, 0, CAN_ENCAP_SIZE);
    frame->can_id &= CAN_EFF_MASK;
    canid = htonl(frame->can_id);
    memcpy(netframe, &canid, 4);
    netframe[4] = frame->can_dlc;
    memcpy(&netframe[5], &frame->data, frame->can_dlc);

    /* send TCP/UDP frame */
    s = sendto(net_socket, netframe, CAN_ENCAP_SIZE, 0, net_addr, sizeof(*net_addr));
    if (s != CAN_ENCAP_SIZE) {
	fprint_syslog_wc(stderr, LOG_ERR, "error sending TCP/UDP data:", strerror(errno));
	return -1;
    }
    return 0;
}

int frame_to_can(int can_socket, unsigned char *netframe) {
    uint32_t canid;
    struct can_frame frame;
    struct timespec to_wait;
    struct timeval actual_time;
    long usec, wait_usec;

    /* Maerklin TCP/UDP Format: always 13 (CAN_ENCAP_SIZE) bytes
     *   byte 0 - 3  CAN ID
     *   byte 4      DLC
     *   byte 5 - 12 CAN data
     */
    memset(&frame, 0, sizeof(frame));
    memcpy(&canid, netframe, 4);
    /* CAN uses (network) big endian format */
    frame.can_id = ntohl(canid);
    frame.can_id &= CAN_EFF_MASK;
    frame.can_id |= CAN_EFF_FLAG;
    frame.can_dlc = netframe[4];
    memcpy(&frame.data, &netframe[5], 8);

    /* we calculate the difference between the actual time and the time the last command was sent */
    /* probably we don't need to wait anymore before putting next CAN frame on the wire */
    if (((frame.can_id & 0x00FE0000) == 0x00420000) ||
	((frame.can_id & 0x00FE0000) == 0x00360000))
	wait_usec = TIME_WAIT_US_SH;
    else
	wait_usec = TIME_WAIT_US;

    gettimeofday(&actual_time, NULL);
    usec = (actual_time.tv_sec - last_sent.tv_sec) * 1000000;
    usec += (actual_time.tv_usec - last_sent.tv_usec);
    if (usec < wait_usec) {
	to_wait.tv_sec = 0;
	to_wait.tv_nsec = (wait_usec - usec) * 1000;
	nanosleep(&to_wait, NULL);
    }

    /* send CAN frame */
    if (write(can_socket, &frame, sizeof(frame)) != sizeof(frame)) {
	fprint_syslog_wc(stderr, LOG_ERR, "error CAN frame:", strerror(errno));
	return -1;
    }

    gettimeofday(&last_sent, NULL);
    return 0;
}

int inflate_data(struct cs2_config_data_t *config_data) {
    int ret;
    z_stream strm;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
	return ret;
    strm.avail_in = config_data->deflated_size;
    strm.avail_out = config_data->inflated_size;
    strm.next_in = config_data->deflated_data + 4;
    strm.next_out = config_data->inflated_data;
    ret = inflate(&strm, Z_NO_FLUSH);

    assert(ret != Z_STREAM_ERROR);	/* state not clobbered */
    switch (ret) {
    case Z_NEED_DICT:
	ret = Z_DATA_ERROR;
    /* falls through */
    case Z_DATA_ERROR:
    case Z_MEM_ERROR:
	(void)inflateEnd(&strm);
	return ret;
    }
    (void)inflateEnd(&strm);
    return 0;
}

int config_write(struct cs2_config_data_t *config_data) {
    FILE *config_fp;
    uint16_t crc;
    int i;
    char *filename;

    if (config_data->name == NULL) {
	fprintf(stderr, "\nunexpected config data stream received\n");
	return 0;
    }

    filename = calloc(MAXLINE, 1);
    if (filename == NULL) {
	fprint_syslog_wc(stderr, LOG_ERR, "can't calloc:", strerror(errno));
	exit(EXIT_FAILURE);
    }
    strcpy(filename, config_data->dir);
    strcat(filename, config_data->name);

    crc = CRCCCITT(config_data->deflated_data, config_data->deflated_stream_size, 0xFFFF);

    if (config_data->verbose)
	printf("\n  writing to %s - size 0x%04x crc 0x%04x 0x%04x\n", config_data->name,
	       config_data->deflated_stream_size, config_data->crc, crc);

    config_fp = fopen(filename, "wb");
    if (!config_fp) {
	fprintf(stderr, "\ncan't open file %s for writing - error: %s\n", filename, strerror(errno));
	syslog(LOG_ERR, "%s: can't open file %s for writing - error: %s\n", __func__, filename, strerror(errno));
	free(filename);
	return 0;
    } else if (config_data->verbose) {
	for (i = 0; i < config_data->deflated_stream_size; i++) {
	    if ((i % 8) == 0)
		printf("\n");
	    printf("%02x ", config_data->deflated_data[i]);
	}
	printf("\n");
    }
    inflate_data(config_data);
    fwrite(config_data->inflated_data, 1, config_data->inflated_size, config_fp);
    fclose(config_fp);
    free(filename);
    free(config_data->deflated_data);
    free(config_data->inflated_data);
    return 1;
}

int reassemble_data(struct cs2_config_data_t *config_data, unsigned char *netframe) {
    unsigned int state, temp;
    unsigned char newframe[CAN_ENCAP_SIZE];
    char *filename;
    char *ptr;

    if ((memcmp(netframe, GETCONFIG_RESPONSE, 5) == 0) ||
	(memcmp(netframe, GETCONFIG_RESPONSE_II, 5) == 0)) {
	memcpy(&temp, &netframe[5], 4);
	config_data->deflated_size = ntohl(temp);
	config_data->deflated_size_counter = config_data->deflated_size;

	memcpy(&temp, &netframe[9], 2);
	config_data->crc = ntohs(temp);
	if (config_data->verbose)
	    printf("\nstart of config - deflated size: 0x%08x crc 0x%04x\n", config_data->deflated_size, config_data->crc);

	config_data->start = 1;
	/* we alloc 8 bytes more to be sure that it fits */
	config_data->deflated_data = malloc(config_data->deflated_size + 16);
	if (config_data->deflated_data == NULL) {
	    fprintf(stderr, "can't malloc deflated config data buffer - size 0x%04x\n", config_data->deflated_size + 8);
	    exit(EXIT_FAILURE);
	}
	/* deflated data index */
	config_data->ddi = 0;

    } else if (memcmp(netframe, GETCONFIG_DATA, 5) == 0) {
	memcpy(&config_data->deflated_data[config_data->ddi], &netframe[5], 8);
	config_data->ddi += 8;
	if (config_data->start) {
	    memcpy(&temp, &netframe[5], 4);
	    config_data->inflated_size = ntohl(temp);
	    config_data->inflated_data = malloc(config_data->inflated_size);
	    if (config_data->inflated_data == NULL) {
		fprintf(stderr, "can't malloc inflated config data buffer - size 0x%04x\n", config_data->inflated_size);
		exit(EXIT_FAILURE);
	    }
	    config_data->start = 0;
	    config_data->stream = 1;
	    config_data->deflated_size_counter -= 8;
	} else if (config_data->stream) {
	    if (config_data->deflated_size_counter <= 8) {
		config_data->stream = 0;
		config_data->deflated_stream_size = config_data->ddi;
		config_write(config_data);
		/* if (config_data->inflated_data)
		    free(config_data->inflated_data);
		if (config_data->deflated_data)
		    free(config_data->deflated_data);
		*/
		/* TODO next */
		state = config_data->state;
		switch (state) {
		case CS2_STATE_INACTIVE:
		    break;
		case CS2_STATE_NORMAL_CONFIG:
		    if (cs2_configs[config_data->next][0]) {
			memset(newframe, 0, CAN_ENCAP_SIZE);
			memcpy(newframe, GETCONFIG, 5);
			if (config_data->verbose)
			    printf("getting %s filename %s\n", cs2_configs[config_data->next][0], cs2_configs[config_data->next][1]);
			config_data->name = cs2_configs[config_data->next][1];
			syslog(LOG_NOTICE, "%s: getting %s filename %s\n", __func__, cs2_configs[config_data->next][0], config_data->name);
			memcpy(&newframe[5], cs2_configs[config_data->next][0], strlen(cs2_configs[config_data->next][0]));
			/* print_can_frame(NET_TCP_FORMAT_STRG, newframe, 1); */
			net_to_net(config_data->cs2_tcp_socket, NULL, newframe, CAN_ENCAP_SIZE);
			config_data->next++;
			break;
		    } else {
			/* list is done if no entry is left */
			config_data->state = CS2_STATE_TRACK_SUM;
		    }
		    /* falls through */
		case CS2_STATE_TRACK_SUM:
		    filename = calloc(MAXLINE, 1);
		    if (filename == NULL) {
			fprintf(stderr, "can't calloc in %s: %s\n", __func__, strerror(errno));
			config_data->state = CS2_STATE_INACTIVE;
			return (EXIT_FAILURE);
		    }
		    strcpy(filename, config_data->dir);
		    strcat(filename, "gleisbild.cs2");
		    if (config_data->verbose)
			printf("read track file %s dir %s\n", filename, config_data->dir);
		    syslog(LOG_NOTICE, "%s: read track file %s dir %s\n", __func__, filename, config_data->dir);
		    config_data->page_name = read_track_file(filename, config_data->page_name);
		    if (config_data->page_name == NULL) {
			fprint_syslog(stderr, LOG_ERR, "can't finish CAN member copy config request");
			config_data->state = CS2_STATE_INACTIVE;
			return (EXIT_FAILURE);
		    }
		    if (config_data->verbose)
			printf("copy track file %s dir %s\n", filename, config_data->dir);
		    syslog(LOG_NOTICE, "%s: track track file %s dir %s (global)\n", __func__, filename, config_data->dir);
		    page_name = read_track_file(filename, config_data->page_name);

		    strcat(config_data->dir, "gleisbilder/");
		    if (config_data->verbose)
			print_pages(config_data->page_name);
		    free(filename);
		    config_data->track_index = 0;
		    config_data->state = CS2_STATE_GET_TRACKS;
		    /* falls through */
		case CS2_STATE_GET_TRACKS:
		    if (config_data->page_name[config_data->track_index]) {
			memset(newframe, 0, CAN_ENCAP_SIZE);
			memcpy(newframe, GETCONFIG, 5);
			config_data->name = config_data->page_name[config_data->track_index];
			sprintf((char *)&newframe[5], "gbs-%d", config_data->track_index);
			if (config_data->verbose)
			    printf("getting track %s filename %s\n", &newframe[5], config_data->name);
		        syslog(LOG_NOTICE, "%s: getting track %s filename %s\n", __func__, &newframe[5], config_data->name);
			net_to_net(config_data->cs2_tcp_socket, NULL, newframe, CAN_ENCAP_SIZE);
			config_data->track_index++;
		    } else {
			/* reset dir */
			ptr = strstr(config_data->dir, "gleisbilder/");
			if (ptr)
			    *ptr = 0;
			config_data->state = CS2_STATE_INACTIVE;
		    }
		    break;
		case CS2_STATE_BROADCAST_UPDATE:
		default:
		    config_data->state = CS2_STATE_INACTIVE;
		    break;
		}
	    } else {
		config_data->deflated_size_counter -= 8;
	    }
	}
    }
    return 1;
}

uint8_t *read_config_file(char *filename, char *config_dir, uint32_t * nbytes) {
    int rc;
    struct stat st;
    FILE *fp;
    uint8_t *config;
    char *file_name;

    file_name = calloc(MAXLINE, 1);
    if (!file_name)
	return NULL;

    strncat(file_name, config_dir, MAXLINE - 1);
    strcat(file_name, filename);

    /* printf("%s: try reading file %s\n", __func__, file_name); */

    rc = stat(file_name, &st);
    if (rc < 0) {
	fprintf(stderr, "%s: error stat failed for file %s\n", __func__, filename);
	syslog(LOG_ERR, "%s: error stat failed for file %s\n", __func__, filename);
	goto read_error1;
    }
    fp = fopen(file_name, "rb");
    if (fp == NULL) {
	fprintf(stderr, "%s: error fopen failed for file %s\n", __func__, filename);
	syslog(LOG_ERR, "%s: error fopen failed for file %s\n", __func__, filename);
	goto read_error1;
    }
    *nbytes = st.st_size;
    config = (uint8_t *) calloc(*nbytes, 1);
    if (config == NULL) {
	fprintf(stderr, "%s: error calloc failed creating config buffer for %s\n", __func__, filename);
	syslog(LOG_ERR, "%s: error calloc failed creating config buffer for %s\n", __func__, filename);
	goto read_error2;
    }
    rc = fread((void *)config, 1, *nbytes, fp);
    if (((unsigned int)rc != *nbytes)) {
	fprintf(stderr, "%s: error fread failed reading %s\n", __func__, filename);
	syslog(LOG_ERR, "%s: error fread failed reading %s\n", __func__, filename);
	free(config);
	goto read_error2;
    }
    fclose(fp);
    free(file_name);
    return config;

read_error2:
    fclose(fp);
read_error1:
    free(file_name);
    return NULL;
}

static void strm_init(z_stream * strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    CALL_ZLIB(deflateInit2(strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, windowBits, 8, Z_DEFAULT_STRATEGY));
}

int send_tcp_config_data(char *filename, char *config_dir, uint32_t canid, int tcp_socket, int flags) {
    /* uint16_t crc; */
    uint32_t temp32, canid_be, nbytes = 0;
    uint8_t *config;
    uint8_t *out;
    z_stream strm;
    int inflated_size, deflated_size, padded_nbytes, i, src_i, n_packets;
    uint16_t crc, temp16;
    uint8_t netframe[MAXMTU];
    int on = 1;

    out = NULL;

    config = read_config_file(filename, config_dir, &nbytes);
    if (config == NULL) {
	fprintf(stderr, "%s: error reading config %s\n", __func__, filename);
	syslog(LOG_ERR, "%s: error reading config %s\n", __func__, filename);
	return -1;
    }

    if (flags & COMPRESSED) {
	/* we need some more bytes to prepare send data (includes inflated file size and padding)    */
	/* assuming that out[CHUNK] is large enough to compress the whole file, otherwise don't send */
	out = (uint8_t *) calloc(CHUNK + 12, 1);
	if (out == NULL) {
	    fprintf(stderr, "%s: error calloc failed creating deflation buffer\n", __func__);
	    syslog(LOG_ERR, "%s: error calloc failed creating deflation buffer\n", __func__);
	    return -1;
	}
	strm_init(&strm);
	strm.next_in = config;
	strm.avail_in = nbytes;
	strm.avail_out = CHUNK;
	/* store deflated file beginning at byte 5 */
	strm.next_out = &out[4];
	CALL_ZLIB(deflate(&strm, Z_FINISH));
	deflated_size = CHUNK - strm.avail_out;
	if (strm.avail_out == 0) {
	    /* printf("%s: compressed file to large : %d filesize %d strm.avail_out\n", __func__, nbytes, strm.avail_out); */
	    deflateEnd(&strm);
	    free(config);
	    free(out);
	    return -1;
	}

	/* now prepare the send buffer */
	inflated_size = htonl(nbytes);
	memcpy(out, &inflated_size, 4);
	/* prepare padding */
	padded_nbytes = deflated_size + 4;
	if (padded_nbytes % 8) {
	    padded_nbytes += 8 - (padded_nbytes % 8);
	}

	for (i = deflated_size + 4; i < padded_nbytes; i++) {
	    out[i] = 0;
	}

	crc = CRCCCITT(out, padded_nbytes, 0xffff);
	/* printf("%s: canid 0x%08x filesize %d deflated size: %d crc 0x%04x\n", __func__, canid, nbytes, deflated_size, crc); */
	memset(netframe, 0, MAXMTU);
	/* prepare first CAN frame   */
	/* delete response bit and set canid to config data stream */
	canid_be = htonl((canid & 0xFFFEFFFFUL) | 0x00020000UL);
	memcpy(&netframe[0], &canid_be, 4);
	/* CAN DLC is 6 */
	netframe[4] = 0x06;
	temp32 = htonl(deflated_size + 4);
	memcpy(&netframe[5], &temp32, 4);
	temp16 = htons(crc);
	memcpy(&netframe[9], &temp16, 2);
	netframe[11] = 0x00;
	netframe[12] = 0x00;

	if (net_to_net(tcp_socket, NULL, netframe, CAN_ENCAP_SIZE)) {
	    deflateEnd(&strm);
	    free(config);
	    free(out);
	    return -1;
	}

	/* loop until all packets send */
	src_i = 0;
	do {
	    n_packets = 0;
	    i = 0;
	    do {
		memcpy(&netframe[i], &canid_be, 4);
		i += 4;
		/* CAN DLC is always 8 */
		netframe[i] = 0x08;
		i++;
		memcpy(&netframe[i], &out[src_i], 8);
		i += 8;
		src_i += 8;
		n_packets++;
	    } while ((src_i < padded_nbytes) && (n_packets < MAX_PACKETS));

	    /* disable Nagle - force PUSH */
	    if (setsockopt(tcp_socket, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on)) < 0) {
		fprintf(stderr, "error disabling Nagle - TCP_NODELAY on: %s\n", strerror(errno));
		return -1;
	    }
	    /* printf("send %3d bytes by TCP\n", i); */
	    /* don't use frame_to_net because we have more then 13 (CAN_ENCAP_SIZE) bytes to send */
	    if (net_to_net(tcp_socket, NULL, netframe, i)) {
		perror("error sending TCP data\n");
		deflateEnd(&strm);
		free(config);
		free(out);
		return -1;
	    }
	    /* disable Nagle - force PUSH */
	    if (setsockopt(tcp_socket, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on)) < 0) {
		fprintf(stderr, "error disabling Nagle - TCP_NODELAY on: %s\n", strerror(errno));
		return -1;
	    }
	    /* small sleep (reschedule) */
	    usec_sleep(1000);
	} while (src_i < padded_nbytes);
#if 0
	/* print compressed data */
	temp32 = i;
	for (i = 0; i < temp32; i++) {
	    if ((i % CAN_ENCAP_SIZE) == 0) {
		printf("\n");
	    }
	    printf("%02x ", netframe[i]);
	}
	printf("\n");
#endif
	deflateEnd(&strm);
    }
    free(config);
    if (out)
	free(out);
    return 0;
}
