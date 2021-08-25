/*  reads CS2 pcap file
 */

#include <ctype.h>
#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#define  __FAVOR_BSD
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#define IPHDR_LEN	(20)
/* defines for the packet type code in an ETHERNET header */
#define ETHER_TYPE_IP (0x0800)
#define ETHER_TYPE_8021Q (0x8100)

#define CS2_PORT1	15730
#define CS2_PORT2	15731

void print_content(unsigned char *ptr, int length) {
    int i, j;
    for (i = 1; i < length + 1; i++) {
	printf("%02x ", ptr[i - 1]);
	if ((i % 13) == 0) {
	    printf("  |");
	    for (j = i - 13; j <= i; j++) {
		if (isprint(ptr[j]))
		    printf("%c", ptr[j]);
		else
		    putchar(46);
	    }
	    printf("|\n");
	}
    }
    if ((length % 13) != 0)
	printf("\n");
}

/*-------------------------------------------------------------------*/
int main(int argc, char **argv) {
    unsigned int pkt_counter = 1;	/* packet counter */
    /* unsigned long byte_counter=0; */	/* total bytes seen in entire trace */
    /* unsigned long cur_counter=0;  */	/* counter for current 1-second interval */
    /* unsigned long current_ts=0;   */	/* current timestamp */
    struct tcphdr *mytcp;
    struct udphdr *myudp;
    int fnum;

    /* temporary packet buffers */
    struct pcap_pkthdr header;	/* The header that pcap gives us */
    const u_char *packet;	/* The actual packet */

    /* check command line arguments */
    if (argc < 2) {
	fprintf(stderr, "Usage: %s [input pcaps]\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    /* -------- Begin Main Packet Processing Loop ------------------- */
    /* loop through each pcap file in command line args */
    for (fnum = 1; fnum < argc; fnum++) {

	/* ------------------ */
	/* open the pcap file */
	pcap_t *handle;
	char errbuf[PCAP_ERRBUF_SIZE];	/* not sure what to do with this, oh well */
	handle = pcap_open_offline(argv[fnum], errbuf);	/* call pcap library function */

	if (handle == NULL) {
	    fprintf(stderr, "Couldn't open pcap file %s: %s\n", argv[fnum], errbuf);
	    return (2);
	}

	/* ----------------- */
	/* begin processing the packets in this particular file, one at a time */

	while ((packet = pcap_next(handle, &header)) != NULL) {
	    /* header contains information about the packet (e.g. timestamp) */
	    u_char *pkt_ptr = (u_char *) packet;	/* cast a pointer to the packet data */

	    /* parse the first (ethernet) header, grabbing the type field */
	    int ether_type = ((int)(pkt_ptr[12]) << 8) | (int)pkt_ptr[13];
	    int ether_offset = 0;

	    if (ether_type == ETHER_TYPE_IP)	/* most common */
		ether_offset = 14;
	    else if (ether_type == ETHER_TYPE_8021Q)	/* dot1q tag ? */
		ether_offset = 18;
	    else
		fprintf(stderr, "Unknown ethernet type, %04X, skipping...\n", ether_type);

	    /* parse the IP header */
	    pkt_ptr += ether_offset;	/* skip past the Ethernet II header */
	    struct ip *ip_hdr = (struct ip *)pkt_ptr;	/* point to an IP header structure  */

	    int packet_length = ntohs(ip_hdr->ip_len);

	    if (ip_hdr->ip_p == IPPROTO_UDP) {
		myudp = (struct udphdr *)(pkt_ptr + sizeof(struct ip));
		int size_payload = packet_length - (IPHDR_LEN+ sizeof(struct udphdr));
		printf("%04u UDP %s -> ", pkt_counter, inet_ntoa(ip_hdr->ip_src));
		printf("%s port %d -> %d", inet_ntoa(ip_hdr->ip_dst), ntohs(myudp->uh_sport), ntohs(myudp->uh_dport));
		printf("  packet_length %d\n", size_payload);
		print_content((unsigned char *)pkt_ptr + IPHDR_LEN+ sizeof(struct udphdr), size_payload);
		printf("\n");
	    }

	    if (ip_hdr->ip_p == IPPROTO_TCP) {
		mytcp = (struct tcphdr *)(pkt_ptr + sizeof(struct ip));

		int tcp_offset = mytcp->th_off * 4;
		int size_payload = packet_length - (IPHDR_LEN + tcp_offset);

		if (size_payload > 0) {
		    printf("%04u TCP %s -> ", pkt_counter, inet_ntoa(ip_hdr->ip_src));
		    printf("%s port %d -> %d", inet_ntoa(ip_hdr->ip_dst), ntohs(mytcp->th_sport),
			   ntohs(mytcp->th_dport));
		    unsigned char *dump = (unsigned char *)pkt_ptr + IPHDR_LEN + tcp_offset;
		    printf("  packet_length %d\n", size_payload);
		    print_content(dump, size_payload);
		    printf("\n");
		}
	    }

	    /* check to see if the next second has started, for statistics purposes */
#if 0
	    if (current_ts == 0) {	/* this takes care of the very first packet seen */
		current_ts = header.ts.tv_sec;
	    } else if (header.ts.tv_sec > current_ts) {
		printf("%ld KBps\n", cur_counter / 1000);
		cur_counter = 0;	/* reset counters  */
		current_ts = header.ts.tv_sec;	/* update time interval */
	    }

	    cur_counter += packet_length;
	    byte_counter += packet_length;	/* byte counter update */
#endif
	    pkt_counter++;	/* increment number of packets seen */
	}
	pcap_close(handle);	/* close the pcap file */
    }
    return 0;
}
