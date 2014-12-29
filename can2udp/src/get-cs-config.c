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

struct config_data {
    int deflated_stream_size;
    int deflated_size;
    int inflated_size;
    uint16_t crc;
    char *name;
    uint8_t *deflated_data;
    uint8_t *inflated_data;
};


uint16_t CRCCCITT(unsigned char *data, size_t length, unsigned short seed);

int netframe_to_net(int net_socket, unsigned char *netframe, int length) {
    int s;
    s = send(net_socket, netframe, length, 0);
    if (s != length) {
        return 1;
    }
    return 0;
}

int config_write(struct config_data *config_data) {
    FILE *config_fp;
    int i;

    printf("\n  writing to %s - size 0x%04x crc 0x%04x\n", config_data->name, config_data->deflated_stream_size, config_data->crc);

    config_fp = fopen(config_data->name,"wb");
    if (!config_fp) {
        printf("\ncan't open file %s for writing\n", config_data->name);
        exit(1);
    } else {
        for (i = 0; i <= config_data->deflated_stream_size ; i++) {
            if ((i % 8 ) == 0 ) {
               printf("\n");
            } else {
               printf("%02x ", config_data->deflated_data[i]);
            }
        }
        printf("\n");
    }
    return 1;
}

int main(int argc, char**argv) {
    int sockfd, ddi, i, tcp_packet_nr, n=1;
    FILE *config_fp;
    int temp, config_data_start, config_data_stream, deflated_size;
    struct config_data config_data;
    struct sockaddr_in servaddr;
    fd_set rset;
    unsigned char netframe[FRAME_SIZE];
    unsigned char recvline[MAXSIZE];
    
    if (argc != 3)
    {
       printf("usage:  %s <config> <IP address>\n", argv[0]);
       exit(1);
    }

    if (strlen(argv[1])>7) {
        printf("config name to long\n");
        exit(1);
    } else {
        if (( config_data.name = malloc(strlen(argv[1]+2)))) {
            config_data.name[0]= '\0';
            strcat(config_data.name, argv[1]);
            strcat(config_data.name, ".z");
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
                    config_data.deflated_size=deflated_size;
                    memcpy(&temp,&recvline[9],2);
                    config_data.crc=ntohs(temp);
                    printf("\nstart of config - deflated size: 0x%08x crc 0x%04x", deflated_size, config_data.crc);
                    config_data_start=1;
                    /* we alloc 8 bytes more to be sure that it fits */
                    config_data.deflated_data = malloc(deflated_size + 16);
                    if (config_data.deflated_data == NULL) {
                        printf("can't malloc config data buffer - size 0x%04x\n", deflated_size + 8);
                        exit(1);
                    }
                    /* deflated data index */
                    ddi=0;
                }
                for ( i=0; i<n; i++) {
                    if (( i % FRAME_SIZE ) == 0) {
                        if (memcmp(&recvline[i],GETCONFIG_DATA,5)==0) {
                            printf("\ni 0x%04x ddi 0x%04x", i, ddi);
                            memcpy(&config_data.deflated_data[ddi],&recvline[i+5],8);
                            ddi +=8;
                            if (config_data_start) {
                                memcpy(&temp,&recvline[i+5],4);
                                config_data.inflated_size=ntohl(temp);
                                printf("\ninflated size: 0x%08x", config_data.inflated_size);
                                config_data_start=0;
                                config_data_stream=1;
                                deflated_size -= 8;
                    /*            fwrite(&recvline[i+9], 1, 4, config_fp); */
                            } else {
                                if (config_data_stream) {
                                    if (deflated_size <= 8) {
                                    /*    fwrite(&recvline[i+5], 1, deflated_size, config_fp);
                                        fclose(config_fp); */
                                        config_data_stream=0;
                                        config_data.deflated_stream_size=ddi -1;
                                        config_write(&config_data);
                                    } else {
                                    /*    fwrite(&recvline[i+5], 1, 8, config_fp); */
                                        deflated_size -= 8;
                                    }
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
