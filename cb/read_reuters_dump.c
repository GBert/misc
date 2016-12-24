/*  reads Reuters pcap file
 */

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
#include "uthash.h"

#define IPHDR_LEN		(20)
/* defines for the packet type code in an ETHERNET header */
#define ETHER_TYPE_IP		(0x0800)
#define ETHER_TYPE_8021Q	(0x8100)

#define REUTERS_PORT1		32787
#define REUTERS_PORT2		32771

#define LENGTH			16

#define timersub_nano(s,t,a) ( (a)->tv_sec = (s)->tv_sec - (t)->tv_sec, \
	((a)->tv_nsec = (s)->tv_nsec - (t)->tv_nsec) < 0 && \
	((a)->tv_nsec += 1000000000L, (a)->tv_sec++) )

struct rt_struct {
    char *data;
    int id;                     /* key */
    int reuters_id;
    struct timespec ts;
    UT_hash_handle hh;          /* makes this structure hashable */
};

int print_timestamp(struct timespec *ts) {
    /* char *timestamp = (char *)malloc(sizeof(char) * 16); */
    struct tm *tm;

    tm = localtime(&ts->tv_sec);

    printf("%02d:%02d:%02d.%09ld", tm->tm_hour, tm->tm_min, tm->tm_sec, ts->tv_nsec);
    return 0;
}


void print_content(unsigned char *ptr, int length) {
    int i, j;
    for (i = 1; i < length + 1; i++) {
	printf("%02x ", ptr[i - 1]);
	if ((i % LENGTH) == 0) {
	    printf("  |");
	    for (j = i - LENGTH; j < i; j++) {
		if (isprint(ptr[j]))
		    printf("%c", ptr[j]);
		else
		    putchar(46);
	    }
	    printf("|\n");
	}
    }
    if ((length % LENGTH) != 0)
	printf("\n");
}

void print_hex(unsigned char *ptr, int length) {
    int i;
    unsigned char *my_ptr;

    my_ptr = ptr;
    printf(" ");
    for (i = 0; i < length - 1; i++) {
	printf("%02x:", *my_ptr++);
    }
    printf("%02x", *my_ptr);
}

/*-------------------------------------------------------------------*/
int main(int argc, char **argv) {
    unsigned int all_pkt_counter = 0;	/* packet counter */
    unsigned int pkt_counter = 0;	/* packet counter */
    /* unsigned long byte_counter=0; *//* total bytes seen in entire trace */
    /* unsigned long cur_counter=0;  *//* counter for current 1-second interval */
    /* unsigned long current_ts=0;   *//* current timestamp */
    struct udphdr *myudp;
    int opt;
    struct timespec ts1, ts2, ts_diff;
    uint16_t search_id, reuters_id, reuters_id_new, payload_offset, vlan;
    unsigned char src_mac[6];
    unsigned char dst_mac[6];

    /* temporary packet buffers */
    struct pcap_pkthdr header;	/* The header that pcap gives us */
    const u_char *packet;	/* The actual packet */

    search_id = 0;
    reuters_id = 0;
    reuters_id_new = 0;

    while ((opt = getopt(argc, argv, "i:h?")) != -1) {
	switch (opt) {
	case 'i':
	    search_id = atoi(optarg);
	    break;
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
	fprintf(stderr, "Usage: %s [input pcaps]\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    /* -------- Begin Main Packet Processing Loop ------------------- */

    /* ------------------ */
    /* open the pcap file */
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];	/* not sure what to do with this, oh well */
    handle = pcap_open_offline_with_tstamp_precision(argv[optind], PCAP_TSTAMP_PRECISION_NANO, errbuf);	/* call pcap library function */

    if (handle == NULL) {
	fprintf(stderr, "Couldn't open pcap file %s: %s\n", argv[optind], errbuf);
	return (2);
    }

    /* ----------------- */
    /* begin processing the packets in this particular file, one at a time */

    while ((packet = pcap_next(handle, &header)) != NULL) {
	/* header contains information about the packet (e.g. timestamp) */
	u_char *pkt_ptr = (u_char *) packet;	/* cast a pointer to the packet data */

	memcpy(dst_mac, pkt_ptr, 6);
	memcpy(src_mac, &pkt_ptr[6], 6);
	/* parse the first (ethernet) header, grabbing the type field */
	int ether_type = ((int)(pkt_ptr[12]) << 8) | (int)pkt_ptr[13];
	int ether_offset = 0;

	if (ether_type == ETHER_TYPE_IP) {	/* most common */
	    ether_offset = 14;
	    vlan = 0;
	} else if (ether_type == ETHER_TYPE_8021Q) {	/* dot1q tag ? */
	    memcpy(&vlan, &pkt_ptr[16], 2);
	    vlan = ntohs(vlan);
	    ether_offset = 18;
	} else {
	    /* fprintf(stderr, "Unknown ethernet type, %04X, skipping...\n", ether_type); */
	    continue;
	}

	/* parse the IP header */
	pkt_ptr += ether_offset;	/* skip past the Ethernet II header */
	struct ip *ip_hdr = (struct ip *)pkt_ptr;	/* point to an IP header structure  */

	/* int packet_length = ntohs(ip_hdr->ip_len); */
	all_pkt_counter++;
	if (ip_hdr->ip_p == IPPROTO_UDP) {
	    /* blame pcap: timeval -> timespec */
	    memcpy(&ts1, &header.ts, sizeof(ts1));
	    if (ts2.tv_sec || ts2.tv_nsec) {
		timersub_nano(&ts1, &ts2, &ts_diff);
	    }
	    payload_offset = IPHDR_LEN + sizeof(struct udphdr);
	    reuters_id = pkt_ptr[payload_offset + 4] * 256 + pkt_ptr[payload_offset + 5];
	    if ((search_id ^ reuters_id) && search_id)
		continue;
	    pkt_counter++;	/* increment number of packets seen */
	    ts2 = ts1;

	    print_timestamp(&ts1);
	    /* printf("%ld.%09ld ", ts1.tv_sec, ts1.tv_nsec); */

	    /* printf("(%ld.%09ld) - (%ld.%09ld) ->  (%ld.%09ld)\n",
	       ts1.tv_sec, ts1.tv_nsec, ts2.tv_sec, ts2.tv_nsec, ts_diff.tv_sec, ts_diff.tv_nsec); */

	    myudp = (struct udphdr *)(pkt_ptr + sizeof(struct ip));
	    /* int size_payload = packet_length - (IPHDR_LEN + sizeof(struct udphdr)); */
	    printf("%8d %8d %ld.%09ld %4d UDP %s -> ", pkt_counter, all_pkt_counter, (long)ts_diff.tv_sec,
		   (long)ts_diff.tv_nsec, vlan, inet_ntoa(ip_hdr->ip_src));
	    printf("%s port %d -> %d", inet_ntoa(ip_hdr->ip_dst), ntohs(myudp->uh_sport), ntohs(myudp->uh_dport));
	    print_hex(src_mac, 6);
	    printf("  length %4d %4d Reuters ID: %5d", header.len, header.caplen, reuters_id);
#if 0
	    print_content((unsigned char *)packet + ether_offset + 12, packet_length - 12 );
	    printf("\n");
#endif
	    if (reuters_id_new) {
		if ((reuters_id - 1) != reuters_id_new)
		    printf(" *\n");
		else
		    printf("\n");
	    }
	    reuters_id_new = reuters_id;
	}
    }
    pcap_close(handle);		/* close the pcap file */
    return 0;
}
