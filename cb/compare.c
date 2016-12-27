#include <ctype.h>
#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define  __FAVOR_BSD
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define IPHDR_LEN		(20)
/* defines for the packet type code in an ETHERNET header */
#define ETHER_TYPE_IP		(0x0800)
#define ETHER_TYPE_8021Q	(0x8100)

#define LENGTH			16

#define timersub_nano(s,t,a) ( (a)->tv_sec = (s)->tv_sec - (t)->tv_sec, \
	((a)->tv_nsec = (s)->tv_nsec - (t)->tv_nsec) < 0 && \
	((a)->tv_nsec -= 1000000000L, (a)->tv_sec++) )

#define timespeccmp(tsp, usp, cmp)	\
	(((tsp)->tv_sec == (usp)->tv_sec) ? 	\
	((tsp)->tv_nsec cmp (usp)->tv_nsec) :	\
	((tsp)->tv_sec cmp (usp)->tv_sec))

unsigned int timespec_sub(struct timespec *a, struct timespec *b, struct timespec *res) {
    int us;

    if (timespeccmp(a, b, <)) {
	timersub_nano(b, a, res);
	us = 1;
    } else {
	timersub_nano(a, b, res);
	us = -1;
    }
    us = us * (res->tv_sec * 1000000 + res->tv_nsec / 1000);
    return (us);
}

int main(int argc, char **argv) {
    unsigned int all_pkt_counter = 0;	/* packet counter */
    unsigned int pkt_counter = 0;	/* packet counter */
    int opt;
    unsigned int us;
    struct timespec ts1, ts2, ts_diff;
    uint16_t payload_offset, vlan;
    unsigned char src_mac[6];
    unsigned char dst_mac[6];
    pcap_t *handle1;
    pcap_t *handle2;

    /* temporary packet buffers */
    struct pcap_pkthdr header1, header2;	/* The header that pcap gives us */
    const u_char *packet1, *packet2;	/* The actual packet */

    while ((opt = getopt(argc, argv, "h?")) != -1) {
	switch (opt) {
	case 'h':
	case '?':
	    exit(0);
	default:
	    break;
	}
    }

    memset(&ts1, 0, sizeof(ts1));
    memset(&ts2, 0, sizeof(ts2));
    memset(&ts_diff, 0, sizeof(ts_diff));

    /* check command line arguments */
    if (argc < 3) {
	fprintf(stderr, "Usage: %s pcap1 pcap2\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    char errbuf[PCAP_ERRBUF_SIZE];	/* not sure what to do with this, oh well */

    handle1 = pcap_open_offline_with_tstamp_precision(argv[optind], PCAP_TSTAMP_PRECISION_NANO, errbuf);	/* call pcap library function */
    if (handle1 == NULL) {
	fprintf(stderr, "Couldn't open pcap1 file %s: %s\n", argv[optind], errbuf);
	return (EXIT_FAILURE);
    }

    handle2 = pcap_open_offline_with_tstamp_precision(argv[optind + 1], PCAP_TSTAMP_PRECISION_NANO, errbuf);	/* call pcap library function */
    if (handle2 == NULL) {
	fprintf(stderr, "Couldn't open pcap2 file %s: %s\n", argv[optind + 1], errbuf);
	return (EXIT_FAILURE);
    }

    while ((packet1 = pcap_next(handle1, &header1)) != NULL) {
	if ((packet2 = pcap_next(handle2, &header2)) != NULL) {
	    memcpy(&ts1, &header1.ts, sizeof(ts1));
	    memcpy(&ts2, &header2.ts, sizeof(ts2));
	    us = timespec_sub(&ts1, &ts2, &ts_diff);
	    printf("(%ld.%09ld) - (%ld.%09ld) ->  %6d us\n", ts1.tv_sec, ts1.tv_nsec, ts2.tv_sec, ts2.tv_nsec, us);
	}
    }
}
