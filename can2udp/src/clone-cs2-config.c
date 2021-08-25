/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <assert.h>

#include <zlib.h>

#define FRAME_SIZE	13
#define MAXSIZE		(1024*FRAME_SIZE)
#define TCP_PORT	15731
#define MAXDIR		256
#define MAXNAME		256
#define MAXGBS		16
#define MAXSTRING	1024

unsigned char GETCONFIG[]          = { 0x00, 0x40, 0x03, 0x00, 0x08 };
unsigned char GETCONFIG_DATA[]     = { 0x00, 0x42, 0x03, 0x00, 0x08 };
unsigned char GETCONFIG_RESPONSE[] = { 0x00, 0x42, 0x03, 0x00, 0x06 };

char *configs[][2] = {
    {"loks", "lokomotive.cs2"},
    {"mags", "magnetartikel.cs2"},
    {"fs", "fahrstrassen.cs2"},
    {"gbs", "gleisbild.cs2"},
/*    {NULL, NULL}, */
    {"lokstat", "lokomotive.sr2"},
    {"magstat", "magnetartikel.sr2"},
    {"gbsstat", "gbsstat.sr2"},
    {"fsstat", "fahrstarssen.sr2"},
    {NULL, NULL},
};

char *gleisbild_dir = { "/gleisbilder" };
char *gleisbild_name = { "gleisbild.cs2" };
char *gbs_site = { "gbs-" };
char *gbs_default = { "gbs-0" };

struct config_data {
    int deflated_stream_size;
    int deflated_size;
    int inflated_size;
    int verbose;
    uint16_t crc;
    char *name;
    char *directory;
    char *filename;
    uint8_t *deflated_data;
    uint8_t *inflated_data;
};

uint16_t CRCCCITT(unsigned char *data, size_t length, unsigned short seed);

int netframe_to_net(int net_socket, unsigned char *netframe, int length) {
    if (send(net_socket, netframe, length, 0) != length)
	return 1;
    return 0;
}

int inflate_data(struct config_data *config_data) {
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

int config_write(struct config_data *config_data) {
    FILE *config_fp;
    uint16_t crc;
    int i;
    char *filename;

    crc = CRCCCITT(config_data->deflated_data, config_data->deflated_stream_size, 0xFFFF);

    printf("writing to %s/%s - size 0x%04x crc 0x%04x calculated crc 0x%04x\n", config_data->directory,
	   config_data->filename, config_data->deflated_stream_size, config_data->crc, crc);

    filename = calloc(MAXSTRING, 1);

    if (filename == NULL) {
	fprintf(stderr, "can't calloc in %s: %s\n", __func__, strerror(errno));
	exit(EXIT_FAILURE);
    }

    strcpy(filename, config_data->directory);
    strcat(filename, "/");
    strcat(filename, config_data->filename);

    config_fp = fopen(filename, "wb");

    if (!config_fp) {
	fprintf(stderr, "\ncan't open file %s for writing - error: %s\n", config_data->filename, strerror(errno));
	exit(EXIT_FAILURE);
    } else {
	if (config_data->verbose) {
	    for (i = 0; i < config_data->deflated_stream_size; i++) {
		if ((i % 8) == 0)
		    printf("\n");
		printf("%02x ", config_data->deflated_data[i]);
	    }
	    printf("\n");
	}
    }
    if (config_data->verbose)
	printf("deflated size: %d inflated size: %d\n", config_data->deflated_size, config_data->inflated_size);
    inflate_data(config_data);
    fwrite(config_data->inflated_data, 1, config_data->inflated_size, config_fp);
    fclose(config_fp);
    free(filename);
    return 1;
}

int get_data(struct config_data *config_data, int sockfd) {
    unsigned char netframe[FRAME_SIZE];
    unsigned char recvline[MAXSIZE];
    int n, tcp_packet_nr, ddi = 0;
    int file_not_done, temp, config_data_start, config_data_stream, deflated_size = 0;
    fd_set rset;

    if (config_data->verbose)
	printf(" try getting %s\n", config_data->name);

    memset(netframe, 0, FRAME_SIZE);
    memcpy(netframe, GETCONFIG, 5);
    memcpy(&netframe[5], config_data->name, strlen(config_data->name));
    if (netframe_to_net(sockfd, netframe, FRAME_SIZE)) {
	fprintf(stderr, "can't send data on TCP socket: %st\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    FD_ZERO(&rset);
    tcp_packet_nr = 0;
    config_data_start = 0;
    config_data_stream = 0;

    file_not_done = 1;

    while (file_not_done) {
	FD_SET(sockfd, &rset);

	if (select(sockfd + 1, &rset, NULL, NULL, NULL) < 0) {
	    fprintf(stderr, "socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	tcp_packet_nr++;
	if (FD_ISSET(sockfd, &rset)) {
	    if ((n = recv(sockfd, recvline, 13, 0)) > 0) {
		if (memcmp(recvline, GETCONFIG_RESPONSE, 5) == 0) {
		    memcpy(&temp, &recvline[5], 4);
		    deflated_size = ntohl(temp);
		    config_data->deflated_size = deflated_size;
		    memcpy(&temp, &recvline[9], 2);
		    config_data->crc = ntohs(temp);
		    if (config_data->verbose)
			printf("\nstart of config - deflated size: 0x%08x crc 0x%04x", deflated_size, config_data->crc);
		    config_data_start = 1;
		    /* we alloc 8 bytes more to be sure that it fits */
		    config_data->deflated_data = malloc(deflated_size + 16);
		    if (config_data->deflated_data == NULL) {
			fprintf(stderr, "can't malloc deflated config data buffer - size 0x%04x\n", deflated_size + 8);
			exit(EXIT_FAILURE);
		    }
		    /* deflated data index */
		    ddi = 0;
		} else if (memcmp(recvline, GETCONFIG_DATA, 5) == 0) {
		    memcpy(&config_data->deflated_data[ddi], &recvline[5], 8);
		    ddi += 8;
		    if (config_data_start) {
			memcpy(&temp, &recvline[5], 4);
			config_data->inflated_size = ntohl(temp);
			if (config_data->verbose)
			    printf("\ninflated size: 0x%08x", config_data->inflated_size);
			config_data->inflated_data = malloc(config_data->inflated_size);
			if (config_data->inflated_data == NULL) {
			    fprintf(stderr, "can't malloc inflated config data buffer - size 0x%04x\n",
				    config_data->inflated_size);
			    exit(EXIT_FAILURE);
			}
			config_data_start = 0;
			config_data_stream = 1;
			deflated_size -= 8;
		    } else if (config_data_stream) {
			if (deflated_size <= 8) {
			    config_data_stream = 0;
			    config_data->deflated_stream_size = ddi;
			    config_write(config_data);
			    if (config_data->inflated_data)
				free(config_data->inflated_data);
			    if (config_data->deflated_data)
				free(config_data->deflated_data);
			    file_not_done = 0;
			} else {
			    deflated_size -= 8;
			}
		    }
		}
	    }
	}
    }
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    int sockfd;
    struct config_data config_data;
    struct sockaddr_in servaddr;
    int config_index, gbs_valid;
    char gbs[MAXGBS];
    char gbs_name[MAXNAME];
    char line[MAXSIZE];
    char *dir;
    char *gleisbild;
    FILE *fp;

    if (argc != 3) {
	fprintf(stderr, "usage: %s <dir_to_write> <IP address>\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    dir = calloc(MAXDIR, 1);
    if (dir == NULL) {
	fprintf(stderr, "can't alloc bufer for directory string: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    strncpy(dir, argv[1], MAXDIR - 1);

    memset(&config_data, 0, sizeof(config_data));
    config_data.directory = dir;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	fprintf(stderr, "can't create TCP socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if (inet_aton((argv[2]), (struct in_addr *)&servaddr.sin_addr.s_addr) == 0) {
	fprintf(stderr, "invalid address: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    servaddr.sin_port = htons(TCP_PORT);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
	fprintf(stderr, "can't connect to TCP socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    config_data.verbose = 0;
    config_index = 0;

#if 1
    while (configs[config_index][0]) {
	config_data.name = configs[config_index][0];
	config_data.filename = configs[config_index++][1];
	get_data(&config_data, sockfd);
    }
#endif

    gleisbild = calloc(strlen(gleisbild_name) + strlen(config_data.directory) + 2, 1);
    strcpy(gleisbild, config_data.directory);
    strcat(gleisbild, "/");
    strcat(gleisbild, gleisbild_name);

    gbs_valid = 0;
    config_data.name = gbs_default;
    strcat(config_data.directory, gleisbild_dir);

    if ((fp = fopen(gleisbild, "r")) != NULL) {
	while (fgets(line, MAXSIZE, fp) != NULL) {
	    if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = 0;
	    if (strstr(line, "seite") == line) {
		gbs_valid = 1;
	    } else if (strstr(line, " .id=") == line) {
		strcpy(gbs, gbs_site);
		strncat(gbs, &line[5], strnlen(&line[5], sizeof(gbs) - strlen(" .id=")));
		config_data.name = gbs;
	    } else if (strstr(line, " .name=") == line) {
		if (gbs_valid) {
		    memset(gbs_name, 0, sizeof(gbs_name));
		    strncpy(gbs_name, &line[7], sizeof(gbs_name) - 1);
		    strcat(gbs_name, ".cs2");
		    config_data.filename = gbs_name;
		    get_data(&config_data, sockfd);
		}
	    }
	}
    } else {
	fprintf(stderr, "can't open gleisbild.cs2: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    free(dir);
    free(gleisbild);
    fclose(fp);
    return EXIT_SUCCESS;
}
