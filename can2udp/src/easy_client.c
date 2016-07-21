#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#define MAX_SIZE 50

unsigned char LOCO_SPEED[] = { 0x00, 0x08, 0x47, 0x11, 0x06, 0x00, 0x00, 0x00, 0x17, 0x00, 0x66, 0x00, 0x00 };

int main() {
    int sock_descriptor;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buff[MAX_SIZE];
    struct timespec to_wait;

    sock_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_descriptor < 0)
	printf("Failed creating socket\n");

    memset((char *)&serv_addr, 0, sizeof(serv_addr));

    server = gethostbyname("127.0.0.1");

    if (server == NULL) {
	printf("Failed finding server name\n");
	return -1;
    }

    serv_addr.sin_family = AF_INET;
    memcpy((char *)&(serv_addr.sin_addr.s_addr), (char *)(server->h_addr), server->h_length);
    serv_addr.sin_port = htons(15731);

    if (connect(sock_descriptor, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
	printf("Failed to connect to server\n");
	return -1;
    }

    else
	printf("Please type enter to start\n");

    fgets(buff, MAX_SIZE - 1, stdin);

    for (;;) {
	memcpy(buff, LOCO_SPEED, 13);
	int count = write(sock_descriptor, buff, 13);

	if (count < 0)
	    printf("Failed writing requested bytes to server\n");
	else
	    printf("send data\n");
	to_wait.tv_sec = 1;
	to_wait.tv_nsec = 0;
	nanosleep(&to_wait);
    };

    close(sock_descriptor);
    return 0;
}
