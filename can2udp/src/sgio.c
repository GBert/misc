/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "scan2lan.h"
#include <zlib.h>

/* CHUNK is the size of the memory chunk used by the zlib routines. */

#define CHUNK 0x8000
#define windowBits 15
#define GZIP_ENCODING 16

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

int time_stamp(char *timestamp) {
    /* char *timestamp = (char *)malloc(sizeof(char) * 16); */
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
    return 0;
}

char **read_track_file(char *filename, char **page_name) {
    FILE *fp;
    int id = 0;
    int page = 0;
    char line[MAXLINE];

    if ((fp = fopen(filename, "r")) != NULL) {
	while (fgets(line, MAXLINE, fp) != NULL) {
	    if (strstr(line, "seite") == line) {
		page = 1;
	    } else if (strstr(line, " .id=") == line) {
		id = strtoul(&line[5], NULL, 0);
	    } else if (strstr(line, " .name=") == line) {
		rmcrlf(line, MAXLINE);
		if (page) {
		    page_name[id] = calloc(strlen(&line[7]) + 1, 1);
		    if (page_name[id] == NULL) {
			fprintf(stderr, "%s: error calloc failed creating config buffer for %s\n", __func__, filename);
			return NULL;
		    }
		    strcpy(page_name[id], &line[7]);
		}
	    }
	}
	/* fgets returned null */
	if (errno != 0) {
	    fprintf(stderr, "error reading line\n");
	    return NULL;
	}
	fclose(fp);
	/* EOF found, normal exit */
	return page_name;
    } else {
	fprintf(stderr, "error reading file %s\n", filename);
	return NULL;
    }
}

void print_can_frame(char *format_string, unsigned char *netframe) {
    uint32_t canid;
    int i, dlc;
    char timestamp[16];

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
	for (i = 6 + dlc; i < 13; i++) {
	    printf(" %02x", netframe[i]);
	}
	printf(")");
    } else {
	printf(" ");
    }
    printf("  ");
    for (i = 5; i < 13; i++) {
	if (isprint(netframe[i]))
	    printf("%c", netframe[i]);
	else
	    putchar(46);
    }

    printf("\n");
}

int net_to_net(int net_socket, struct sockaddr *net_addr, unsigned char *netframe, int length) {
    int s;
    s = sendto(net_socket, netframe, length, 0, net_addr, sizeof(*net_addr));
    if (s != length) {
	fprintf(stderr, "%s: error sending TCP/UDP data; %s\n", __func__, strerror(errno));
	return -1;
    }
    return 0;
}

int frame_to_net(int net_socket, struct sockaddr *net_addr, struct can_frame *frame) {
    int s;
    uint32_t canid;

    memset(netframe, 0, 13);
    frame->can_id &= CAN_EFF_MASK;
    canid = htonl(frame->can_id);
    memcpy(netframe, &canid, 4);
    netframe[4] = frame->can_dlc;
    memcpy(&netframe[5], &frame->data, frame->can_dlc);

    /* send TCP/UDP frame */
    s = sendto(net_socket, netframe, 13, 0, net_addr, sizeof(*net_addr));
    if (s != 13) {
	fprintf(stderr, "%s: error sending TCP/UDP data %s\n", __func__, strerror(errno));
	return -1;
    }
    return 0;
}

int frame_to_can(int can_socket, unsigned char *netframe) {
    struct timespec to_wait;
    /* Maerklin TCP/UDP Format: always 13 bytes
     *   byte 0 - 3  CAN ID
     *   byte 4      DLC
     *   byte 5 - 12 CAN data
     */
    if (write(can_socket, netframe, 13) != 13) {
	fprintf(stderr, "%s: error sending CAN frame: %s\n", __func__, strerror(errno));
	return -1;
    }
    /* TODO : it seems Gleisbox needs a short break after every CAN message -> 20ms*/
    to_wait.tv_sec = 0;
    to_wait.tv_nsec = 20*1000000;
    nanosleep(&to_wait, NULL);
    return 0;
}

uint8_t *read_config_file(char *filename, char *config_dir, uint32_t * nbytes) {
    int rc;
    struct stat st;
    FILE *fp;
    /* char *s; */
    uint8_t *config;
    char *file_name;

    file_name = calloc(MAXLINE, 1);

    strcat(file_name, config_dir);
    strcat(file_name, filename);

    printf("%s: try reading file %s\n", __func__, file_name);

    rc = stat(file_name, &st);
    if (rc < 0) {
	fprintf(stderr, "%s: error stat failed for file %s\n", __func__, filename);
	return NULL;
    }
    fp = fopen(file_name, "rb");
    if (fp == NULL) {
	fprintf(stderr, "%s: error fopen failed for file %s\n", __func__, filename);
	return NULL;
    }
    *nbytes = st.st_size;
    config = (uint8_t *) calloc(*nbytes, 1);
    if (config == NULL) {
	fprintf(stderr, "%s: error calloc failed creating config buffer for %s\n", __func__, filename);
	fclose(fp);
	return NULL;
    }
    rc = fread((void *)config, 1, *nbytes, fp);
    if (((unsigned int)rc != *nbytes)) {
	fprintf(stderr, "%s: error fread failed reading %s\n", __func__, filename);
	fclose(fp);
	free(config);
	return NULL;
    }
    free(file_name);
    fclose(fp);
    return config;
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

    out = NULL;

    config = read_config_file(filename, config_dir, &nbytes);
    if (config) {
	printf("%s read config file %s\n", __func__, filename);
    } else {
	printf("%s: error reading config %s\n", __func__, filename);
	return -1;
    }

    if (flags & COMPRESSED) {
	/* we need some more bytes to prepare send data (includes inflated file size and padding)    */
	/* assuming that out[CHUNK] is large enough to compress the whole file, otherwise don't send */
	out = (uint8_t *) calloc(CHUNK + 12, 1);
	if (out == NULL) {
	    printf("%s: error calloc failed creating deflation buffer\n", __func__);
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
	    printf("%s: compressed file to large : %u filesize %d strm.avail_out\n", __func__, nbytes, strm.avail_out);
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
	printf("%s: canid 0x%08x filesize %u deflated size: %d crc 0x%04x\n", __func__, canid, nbytes, deflated_size, crc);
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

	if (net_to_net(tcp_socket, NULL, netframe, 13)) {
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
	    } while ((src_i < padded_nbytes) && n_packets < MAX_PACKETS);
	    /* don't use frame_to_net because we have more then 13 bytes to send */
	    if (net_to_net(tcp_socket, NULL, netframe, i)) {
		perror("error sending TCP data\n");
		deflateEnd(&strm);
		free(config);
		free(out);
		return -1;
	    }
	} while (src_i < padded_nbytes);
#if 0
	/* print compressed data */
	temp32 = i;
	for (i = 0; i < temp32; i++) {
	    if ((i % 13) == 0) {
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
