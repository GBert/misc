#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define FRAME_SIZE	13
unsigned char GETCONFIG[] = {0x00,0x40,0x03,0x00,0x08};

int netframe_to_net(int net_socket, unsigned char *netframe, int length) {
    int s;
    s = send(net_socket, netframe, length, 0);
    if (s != length) {
        printf("%s: error sending TCP/UDP data\n", __func__);
        return -1;
    }
    return 0;
}

int main(int argc, char**argv) {
    int sockfd, i, n=1;
    struct sockaddr_in servaddr;
    unsigned char netframe[FRAME_SIZE];
    unsigned char recvline[1000];
    
    if (argc != 3)
    {
       printf("usage:  %s <config> <IP address>\n", argv[0]);
       exit(1);
    }

    if (strlen(argv[1])>7) {
        printf("config name to long\n");
        exit(1);
    }

    if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0) {
        printf("can't crate TCP socket\n");
        exit(1);
    }

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if (inet_aton((argv[2]), (struct in_addr *) &servaddr.sin_addr.s_addr) == 0) {
        printf("invalid address\n");
        exit(1);
    }
    
    servaddr.sin_port=htons(15731);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
        printf("can't connect to socket - terminating\n");
        exit(1);
    }

    bzero(netframe, sizeof(netframe));
    memcpy(netframe, GETCONFIG,5);
    memcpy(&netframe[5], argv[1], strlen(argv[1]));
    netframe_to_net(sockfd, netframe, FRAME_SIZE);

    while(n) {
        n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
        for ( i=0; i<n; i++) {
            if (( i % 13 ) == 0) {
                printf("\n");
            }
            printf("%02x ", recvline[i]);
        }
        printf("\n");
    }
    return 1;
}
