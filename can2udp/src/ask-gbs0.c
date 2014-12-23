#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

unsigned char GBS0[]   = {0x00,0x40,0x03,0x00,0x08,0x67,0x62,0x73,0x2d,0x30,0x00,0x00,0x00};
unsigned char GBSSTAT[]= {0x00,0x40,0x03,0x00,0x08,0x67,0x62,0x73,0x73,0x74,0x61,0x74,0x00};

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
    unsigned char recvline[1000];

    if (argc != 2)
    {
       printf("usage:  client <IP address>\n");
       exit(1);
    }

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(argv[1]);
    servaddr.sin_port=htons(15731);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
        printf("can't connect to socket - terminating\n");
        exit(1);
    }
    /* netframe_to_net(sockfd, GBS0, 13); */
    netframe_to_net(sockfd, GBSSTAT, 13);
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
