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
	us = -1;
    } else {
	timersub_nano(a, b, res);
	us = 1;
    }
    us = us * (res->tv_sec * 1000000 + res->tv_nsec / 1000);
    return (us);
}

int main(int argc, char **argv) {
    int opt;
    unsigned int us;
    struct timespec ts1, ts2, ts_diff;
    pcap_t *handle_pcap;
    uint16_t ixia_signature;

    /* temporary packet buffers */
    struct pcap_pkthdr header1;	/* The header that pcap gives us */
    const u_char *packet1;	/* The actual packet */

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
    if (argc < 2) {
	fprintf(stderr, "Usage: %s pcap1 pcap2\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    char errbuf[PCAP_ERRBUF_SIZE];	/* not sure what to do with this, oh well */

    handle_pcap = pcap_open_offline_with_tstamp_precision(argv[optind], PCAP_TSTAMP_PRECISION_NANO, errbuf);	/* call pcap library function */
    if (handle_pcap == NULL) {
	fprintf(stderr, "Couldn't open pcap1 file %s: %s\n", argv[optind], errbuf);
	return (EXIT_FAILURE);
    }

    while ((packet1 = pcap_next(handle_pcap, &header1)) != NULL) {
	memcpy(&ts1, &header1.ts, sizeof(ts1));
	u_char *pkt_ptr = (u_char *) packet1;
	printf("%ld.%09ld us %d octets length", ts1.tv_sec, ts1.tv_nsec, header1.caplen);
	ixia_signature = ((pkt_ptr[header1.caplen - 4] << 8) & 0xff00) | (pkt_ptr[header1.caplen - 3] & 0x00ff);
	printf("  0x%04x  ", ixia_signature);
	if (ixia_signature == 0xaf12) {
	    memcpy(&ts2.tv_sec, &pkt_ptr[header1.caplen - 13], 4);
	    memcpy(&ts2.tv_nsec, &pkt_ptr[header1.caplen - 9], 4);
	    ts2.tv_sec = ntohl(ts2.tv_sec);
	    ts2.tv_nsec = ntohl(ts2.tv_nsec);
	    us = timespec_sub(&ts1, &ts2, &ts_diff);
	    printf("(%ld.%09ld) - (%ld.%09ld) ->  %6d us\n", ts1.tv_sec, ts1.tv_nsec, ts2.tv_sec, ts2.tv_nsec, us);
	} else {
	    printf("\n");
	}
    }
}
