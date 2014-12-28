#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define FRAME_SIZE	13
#define MAXSIZE		16384
unsigned char GETCONFIG[]          = {0x00,0x40,0x03,0x00,0x08};
unsigned char GETCONFIG_DATA[]     = {0x00,0x42,0x03,0x00,0x08};
unsigned char GETCONFIG_RESPONSE[] = {0x00,0x42,0x03,0x00,0x06};
unsigned char GZIP_HEADER[]        = {0x1f,0x8b,0x08,0x00,0x00,0x00,0x00,0x00};

int netframe_to_net(int net_socket, unsigned char *netframe, int length) {
    int s;
    s = send(net_socket, netframe, length, 0);
    if (s != length) {
        return 1;
    }
    return 0;
}

int main(int argc, char**argv) {
    int sockfd, i, tcp_packet_nr, n=1;
    FILE *config_fp;
    int temp, config_data_start, config_data_stream, inflated_size, deflated_size;
    struct sockaddr_in servaddr;
    fd_set rset;
    unsigned char netframe[FRAME_SIZE];
    unsigned char recvline[MAXSIZE];
    char *config_file;
    
    if (argc != 3)
    {
       printf("usage:  %s <config> <IP address>\n", argv[0]);
       exit(1);
    }

    if (strlen(argv[1])>7) {
        printf("config name to long\n");
        exit(1);
    } else {
        if (( config_file = malloc(strlen(argv[1]+2)))) {
            config_file[0]= '\0';
            strcat(config_file, argv[1]);
            strcat(config_file, ".z");
        } else {
            printf("can't malloc config %s.z file name\n", argv[1]);
            exit(1);
        }
    }

    if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0) {
        printf("can't create TCP socket\n");
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
        printf("can't connect to TCP socket\n");
        exit(1);
    }

    bzero(netframe, sizeof(netframe));
    memcpy(netframe, GETCONFIG,5);
    memcpy(&netframe[5], argv[1], strlen(argv[1]));
    if (netframe_to_net(sockfd, netframe, FRAME_SIZE)) {
        printf("can't send data on TCP socket\n");
        exit(1);
    }

    FD_ZERO(&rset);
    tcp_packet_nr=0;
    config_data_start=0;
    config_data_stream=0;

    for(;;) {
        FD_SET(sockfd,&rset);

        if(select(sockfd+1,&rset,NULL,NULL,NULL)<0) {
            printf("connection lost\n");
            exit(0);
        }
        tcp_packet_nr++;
        if (FD_ISSET(sockfd,&rset)) {
            if ((n=recv(sockfd,recvline,MAXSIZE,0)) > 0) {
                if (memcmp(recvline,GETCONFIG_RESPONSE,5)==0) {
                    memcpy(&temp,&recvline[5],4);
                    deflated_size=ntohl(temp);
                    printf("\nstart of config - deflated size: 0x%08x", deflated_size);
                    config_data_start=1;
                    config_fp = fopen(config_file,"wb");
                    if (!config_fp) {
                        printf("\ncan't open file %s for writing\n", config_file);
                        exit(1);
                    } else {
                        fwrite(GZIP_HEADER, 1, 8, config_fp);
                    }
                }
                for ( i=0; i<n; i++) {
                    if (( i % FRAME_SIZE ) == 0) {
                        if (config_data_start) {
                            if (memcmp(&recvline[i],GETCONFIG_DATA,5)==0) {
                                memcpy(&temp,&recvline[i+5],4);
                                inflated_size=ntohl(temp);
                                printf("\ninflated size: 0x%08x", inflated_size);
                                config_data_start=0;
                                config_data_stream=1;
                                deflated_size -= 4;
                                fwrite(&recvline[i+9], 1, 4, config_fp);
                            } 
                        } else {
                            if (config_data_stream) {
                                fwrite(&recvline[i+5], 1, 8, config_fp);
                                deflated_size -= 8;
                                if (deflated_size <=0) {
                                    fclose(config_fp);
                                    config_data_stream=0;
                                } 
                            }
                        }
                        printf("\n %04d: ", tcp_packet_nr);
                    }
                    printf("%02x ", recvline[i]);
                }
                printf("\n");
            }
        }
    }
    return 1;
}
