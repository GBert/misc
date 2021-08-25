#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <net/if.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    int s, sb;
    struct sockaddr_in baddr;

    int destination_port = 15730;
    const char dest_address[] = "127.0.0.1";
    char udpframe[256];
    
    /* prepare udp destination struct with defaults */
    memset(&baddr, 0, sizeof(baddr));
    baddr.sin_family = AF_INET;
    baddr.sin_port = htons(destination_port);
    s = inet_pton(AF_INET, dest_address, &baddr.sin_addr);
    if (s <= 0) {
        if (s == 0)
            fprintf(stderr, "UDP IP invalid\n");
        else
            fprintf(stderr, "invalid address family\n");
        exit(EXIT_FAILURE);
    }

    /* prepare UDP sending socket */
    sb = socket(AF_INET, SOCK_DGRAM, 0);
    if (sb < 0) {
        fprintf(stderr, "Send UDP socket error %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* for broadcast 
    if (setsockopt(sb, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
        fprintf(stderr, "UDP set socket option error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    */

    /* endless and useless loop */
    while(1) {
	sleep(10);
	/* send UDP packet */
	if (sendto(sb, udpframe, 13, 0, (struct sockaddr *)&baddr, sizeof(baddr)) != 13) {
	    fprintf(stderr, "UDP write error: %s\n", strerror(errno));
	}
    }
}

