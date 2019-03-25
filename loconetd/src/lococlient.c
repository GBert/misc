/*
$Id: lococlient.c,v 1.4 2002/06/28 10:38:39 glenn Exp $

 lococlient - UDP multicast/broadcast demo client for Digitrax Loconet
 author: Glenn G. Butcher
 date: 6/9/2002

ChangeLog:

	0.1: Initial release.
	
	0.2: Changed port numbers.
	
	0.3: Nothing.
	
	0.3.1: Added: Automatically picks mcast/bcast based on IP.
	       Added: Command-line parameters.
	       Fixed: broadcast, multicast behavior.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>		/* close */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define VERSION 0.3.1
#define DEFAULT_PORT 4501
#define MAX_MSG 100

int main(int argc, char *argv[]) {
  int one = 1;
  int sd, rc, n, i;
  unsigned int cliLen;
  struct ip_mreq mreq;
  struct sockaddr_in cliAddr, servAddr;
  struct in_addr mcastAddr;
  int port = 0;
  char mcastString[80];
  struct hostent *h;
  unsigned char msg[MAX_MSG];
  int c;

  /* Command line option processing */
  mcastString[0] = 0;
  opterr = 0;
  while ((c = getopt(argc, argv, "m:p:")) != -1) {
    switch (c) {
    case 'm':			/* multicast address */
      strcpy(mcastString, optarg);
      break;
    case 'p':			/* port to listen to */
      n = sscanf(optarg, "%d", &port);
      if (n != 1) {
	printf("Invalid port: %s. Exiting...\n", optarg);
	exit(1);
      }
      break;
    case '?':
      if (isprint(optopt))
	fprintf(stderr, "Unknown option '-%c'.\n", optopt);
      else
	fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
      return 1;
    default:
      abort();
    }
  }

  /* create socket */
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    printf("%s : cannot create socket\n", argv[0]);
    exit(1);
  }

  rc = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
  if (rc < 0)
    printf("Reuse port (SO_REUSEADDR) puked...\n");

  rc = setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &one, sizeof(one));
  if (rc < 0)
    printf("Receive broadcast (SO_BROADCAST) puked...\n");

  /* bind port */
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (port != 0) {
    servAddr.sin_port = htons(port);
  } else {
    servAddr.sin_port = htons(DEFAULT_PORT);
    port = DEFAULT_PORT;
  }
  if (bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
    printf("%s : cannot bind port %d \n", argv[0], port);
    exit(1);
  }

  printf("listening to ");

  /* check given address is multicast */
  if (strlen(mcastString) != 0) {
    h = gethostbyname(mcastString);
    memcpy(&mcastAddr, h->h_addr_list[0], h->h_length);
    if (IN_MULTICAST(ntohl(mcastAddr.s_addr))) {
      /* join multicast group */
      mreq.imr_multiaddr.s_addr = mcastAddr.s_addr;
      mreq.imr_interface.s_addr = htonl(INADDR_ANY);
      rc = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&mreq, sizeof(mreq));
      printf("multicast group %s:", mcastString);
    } else {
      printf("%s : unknown group '%s'\n", argv[0], mcastString);
      exit(1);
    }
  }

  printf("%d\n", port);

  /* infinite server loop */
  while (1) {
    cliLen = sizeof(cliAddr);
    n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *)&cliAddr, &cliLen);
    if (n < 0) {
      printf("%s : receive error\n", argv[0]);
      continue;
    }

    /* Prints a loconet packet in hex */
    for (i = 0; i < n; i++) {
      printf("%02x ", msg[i]);
    }
    printf("\n");
  }				/* end of infinite server loop */
  return 0;
}
