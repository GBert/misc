/*
$Id: loconetd.c,v 1.11 2002/06/28 10:45:12 glenn Exp $

 loconetd - UDP multicast gateway for Digitrax Loconet
 author: Glenn G. Butcher
 date: 6/9/2002

ChangeLog:

	0.1: Initial release; UDP mulitcast of packet stream only.

	0.2: Added inbound receipt of unicast packets for transmission
	     on the Loconet.
	     Changed socket port numbers. 

	0.3: Added configurable parameters via loconetd.conf.
	     "Daemonized."
	     Added UDP broadcast to a single subnet.
	     Added logfile.
	     Added signal handling.
	     
	0.3.1: Fixed broadcast - set SO_BROADCAST
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <time.h>

#define BAUDRATE B57600
#define _POSIX_SOURCE 1		/* POSIX compliant source */
#define FALSE 0
#define TRUE 1
#define MAX(x,y) ( ((x) > (y)) ? (x) : (y) )
#define CONF_FIELD 50
#define DEBUG FALSE

/* Configuration file types, variables, and functions */

int loconet_fd, multicast_sd, inbound_sd, broadcast_sd;
unsigned int rc = 0;
unsigned int cliLen = 0;
struct termios oldtio, newtio;
int t;
char log_msg[80];

unsigned char msg[80];

FILE *log_fd;

typedef struct {
  char name[CONF_FIELD];
  char val[CONF_FIELD];
} conf_rec;

conf_rec conf[20];
int conf_next = 0;

int is_name(const char *findname) {
  int i;
  for (i = 0; i < conf_next; i++) {
    if (strcmp(findname, (char *)conf[i].name) == 0)
      return (1);
  }
  return (0);
}

int load_conf() {
  FILE *fd;
  char line[120], *line_end;
  char name[CONF_FIELD], val[CONF_FIELD];

  if ((fd = fopen("loconetd.conf", "r")) == NULL) {
    if ((fd = fopen("/etc/loconetd.conf", "r")) == NULL) {
      printf("No configuration file.\n");
      return (0);
    } else {
      printf("From /etc/loconetd.conf:\n");
    }
  } else {
    printf("From ./loconetd.conf:\n");
  }

  while (!feof(fd)) {
    fgets(line, 120, fd);
    line_end = strchr(line, '#');
    if (line_end != NULL)
      *line_end = (char)0;
    if (strlen(line) > 1) {
      sscanf(line, "%s %s\n", name, val);
      stpcpy(conf[conf_next].name, name);
      stpcpy(conf[conf_next++].val, val);
    }
  }

  fclose(fd);
  return (1);
}

void print_conf() {
  int i;
  for (i = 0; i < conf_next; i++) {
    printf("%s = %s\n", conf[i].name, conf[i].val);
  }
  printf("\n");
}

int get_value(const char *findname, char *gotval) {
  int i;
  for (i = 0; i < conf_next; i++) {
    if (strcmp(findname, (char *)conf[i].name) == 0) {
      strcpy(gotval, conf[i].val);
      return (1);
    }
  }
  return (0);
}

void write_log(char *source, char *msg, int msglength) {
  int j;
  time_t t;
  unsigned char chksum = 0;

  t = time(NULL);
  fprintf(log_fd, "%lu", t);
  fprintf(log_fd, " %s ", source);
  fprintf(log_fd, "%02x", msg[0]);
  chksum = chksum ^ msg[0];
  for (j = 1; j < msglength; j++) {
    chksum = chksum ^ msg[j];	/* cksum not used right now */
    fprintf(log_fd, ",%02x", msg[j]);
  }
  fprintf(log_fd, " %02x\n", chksum);
  fflush(log_fd);
}

void write_log_msg(char *msg) {
  time_t t;
  t = time(NULL);
  fprintf(log_fd, "%lu", t);
  fprintf(log_fd, " %s\n", msg);
  fflush(log_fd);
}

void catch_sig(int sig_num) {
  switch (sig_num) {
  case SIGTERM:
    strcpy(log_msg, "SIGTERM: exiting.");
    write_log_msg(log_msg);
    tcsetattr(loconet_fd, TCSANOW, &oldtio);
    close(loconet_fd);
    close(broadcast_sd);
    close(multicast_sd);
    fclose(log_fd);
    exit(0);
    break;
  case SIGHUP:
    strcpy(log_msg, "SIGHUP: no action.");
    write_log_msg(log_msg);
    break;
  case SIGINT:
    strcpy(log_msg, "SIGINT: exiting.");
    write_log_msg(log_msg);
    tcsetattr(loconet_fd, TCSANOW, &oldtio);
    close(loconet_fd);
    close(broadcast_sd);
    close(multicast_sd);
    fclose(log_fd);
    exit(0);
    break;
  }
}

int main() {
  /* Enable flags for each capability */
  int doSerial, doMulticast, doBroadcast, doUnicast, doLog;
  /* Serial port, message variables */
  int readcnt, writecnt, sendcnt, msglen, i;

  unsigned char chksum = 0;

  char readbuf[255];
  char sendbuf[255];
  char configval[CONF_FIELD];
  int configint = 0;
  unsigned char opcode = 0;

  /* select() variables */
  fd_set rfds;
  int max_fd = 0;

  /* UDP socket variables */
  int one = 1;

  unsigned char ttl = 1;
  unsigned char loop = 0;
  struct sockaddr_in multAddr, inboundAddr, clientAddr, bcastAddr;
  struct hostent *h;

  /* Set signals to catch */
  signal(SIGTERM, catch_sig);
  signal(SIGHUP, catch_sig);
  signal(SIGINT, catch_sig);

  printf("\n");
  if (load_conf()) {
    print_conf();
  } else {
    printf("No configuration file.\n");
    exit(0);
  }

  /* "daemonize" loconetd */

  if (is_name("Daemon")) {
    get_value("Daemon", configval);
    if (strcmp(configval, "yes") == 0) {
      if (getuid() == 0) {
	i = fork();
	if (i < 0)
	  exit(1);
	if (i > 0)
	  exit(0);
	for (i = getdtablesize(); i >= 0; i--)
	  close(i);
	i = open("/dev/null", O_RDWR);
	dup(i);
	dup(i);
	setsid();
      } else {
	printf("Cannot daemonize unless executed as root; continuing as a foreground process...\n\n");
      }
    }
  }

  /* Open log file for output */
  if (is_name("LogFile")) {
    get_value("LogFile", configval);
    log_fd = fopen(configval, "w");
    doLog = TRUE;
  } else {
    doLog = FALSE;
  }

  if (doLog) {
    strcpy(log_msg, "loconetd started.");
    write_log_msg(log_msg);
  }

  /* Serial port initialization */
  if (is_name("LoconetDevice")) {
    doSerial = TRUE;
    printf("Initializing serial port...\n");
    get_value("LoconetDevice", configval);
    loconet_fd = open(configval, O_RDWR | O_NDELAY | O_NOCTTY);
    if (loconet_fd < 0) {
      perror(configval);
      exit(-1);
    }
    tcgetattr(loconet_fd, &oldtio);	/* save current port settings */
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
//    newtio.c_oflag &= ~OPOST;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
//    newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    newtio.c_lflag = 0;
    newtio.c_cc[VTIME] = 0;	/* inter-character timer unused */
    newtio.c_cc[VMIN] = 1;
    tcflush(loconet_fd, TCIFLUSH);
    tcsetattr(loconet_fd, TCSANOW, &newtio);
    strcpy(log_msg, "Initialized serial port: ");
    strcat(log_msg, configval);
    if (doLog)
      write_log_msg(log_msg);
  } else {
    doSerial = FALSE;
  }

  /* UDP mulitcast socket initialization */

  if (is_name("MulticastGroup")) {
    doMulticast = TRUE;
    printf("Initializing multicast...\n");

    get_value("MulticastGroup", configval);
    strcpy(log_msg, "Initialized multicast: ");
    strcat(log_msg, configval);
    h = gethostbyname(configval);
    if (h == NULL) {
      strcpy(log_msg, "Unknown multicast host: exiting.");
      if (doLog)
	write_log_msg(log_msg);
      exit(1);
    }

    multAddr.sin_family = h->h_addrtype;
    memcpy((char *)&multAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    if (is_name("MulticastPort")) {
      get_value("MulticastPort", configval);
      strcat(log_msg, ":");
      strcat(log_msg, configval);
      sscanf(configval, "%d", &configint);
      multAddr.sin_port = htons(configint);
    } else {
      strcpy(log_msg, "No multicast port: exiting.");
      if (doLog)
	write_log_msg(log_msg);
      exit(1);
    }

    /* check if dest address is multicast */
    if (!IN_MULTICAST(ntohl(multAddr.sin_addr.s_addr))) {
      strcpy(log_msg, "Address not multicast: exiting.");
      if (doLog)
	write_log_msg(log_msg);
      exit(1);
    }

    /* create socket */
    multicast_sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (multicast_sd < 0) {
      strcpy(log_msg, "Cannot open multicast socket: exiting.");
      if (doLog)
	write_log_msg(log_msg);
      exit(1);
    }

    /* set multicast ttl */
    if (is_name("MulticastTTL")) {
      get_value("MulticastTTL", configval);
      if (strcmp(configval, "no")) {
	loop = 0;
      } else {
	loop = 1;
      }
      if (setsockopt(multicast_sd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
	strcpy(log_msg, "Cannot set ttl: exiting.");
	if (doLog)
	  write_log_msg(log_msg);
	exit(1);
      }

    }

    /* set multicast loopback */
    if (is_name("MulticastLoopback")) {
      get_value("MulticastLoopback", configval);
      sscanf(configval, "%d", &configint);
      if (loop > 0 && loop <= 255) {
	loop = configint;
      }
      if (setsockopt(multicast_sd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0) {
	strcpy(log_msg, "Cannot set loopback: exiting.");
	if (doLog)
	  write_log_msg(log_msg);
	exit(1);
      }
    }
    get_value("MulticastGroup", configval);
    if (doLog)
      write_log_msg(log_msg);
  } else {
    doMulticast = FALSE;
  }

  /* UDP broadcast socket initialization */

  if (is_name("BroadcastAddress")) {
    doBroadcast = TRUE;
    printf("Initializing broadcast...\n");

    get_value("BroadcastAddress", configval);
    strcpy(log_msg, "Initialized broadcast: ");
    strcat(log_msg, configval);
    h = gethostbyname(configval);
    if (h == NULL) {
      printf("unknown host.\n");
      exit(1);
    }
    multAddr.sin_family = h->h_addrtype;
    memcpy((char *)&bcastAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    if (is_name("BroadcastPort")) {
      get_value("BroadcastPort", configval);
      strcat(log_msg, ":");
      strcat(log_msg, configval);
      sscanf(configval, "%d", &configint);
      bcastAddr.sin_port = htons(configint);
    } else {
      strcpy(log_msg, "No broadcast port: exiting.");
      if (doLog)
	write_log_msg(log_msg);
      exit(1);
    }
    broadcast_sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (broadcast_sd < 0) {
      strcpy(log_msg, "Cannot open broadcast socket: exiting.");
      if (doLog)
	write_log_msg(log_msg);
      exit(1);
    }
    if (setsockopt(broadcast_sd, SOL_SOCKET, SO_BROADCAST, &one, sizeof(one)) < 0) {
      strcpy(log_msg, "Cannot set broadcast option (SO_BROADCAST): exiting.");
      if (doLog)
	write_log_msg(log_msg);
      exit(1);
    }
    if (doLog)
      write_log_msg(log_msg);
  } else {
    doBroadcast = FALSE;
  }

  /* UDP inbound socket initialization */

  if (is_name("UnicastPort")) {
    doUnicast = TRUE;
    printf("Initializing unicast...\n");
    strcpy(log_msg, "Initialized inbound unicast: ");
    get_value("UnicastPort", configval);
    strcat(log_msg, configval);
    sscanf(configval, "%d", &configint);

    inboundAddr.sin_family = AF_INET;
    inboundAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    inboundAddr.sin_port = htons(configint);

    /* create socket */
    inbound_sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inbound_sd < 0) {
      strcpy(log_msg, "Cannot open inbound unicast socket: exiting.");
      if (doLog)
	write_log_msg(log_msg);
      exit(1);
    } else {
      rc = bind(inbound_sd, (struct sockaddr *)&inboundAddr, sizeof(inboundAddr));
    }
    if (doLog)
      write_log_msg(log_msg);
  } else {
    doUnicast = FALSE;
  }

  /* Message buffer counter, checksum initialization */
  readcnt = 0;
  sendcnt = 0;
  chksum = 0;
  msglen = 0;
  if (tcflush(loconet_fd, TCIFLUSH) != 0) {
    if (doLog) {
      strcpy(log_msg, "tcflush failed.");
      write_log_msg(log_msg);
    }
  }

  /* loop all day */
  while (1) {
    FD_ZERO(&rfds);
    max_fd = 0;
    if (doSerial) {
      max_fd = MAX(loconet_fd, max_fd);
      FD_SET(loconet_fd, &rfds);
    }
    if (doUnicast) {
      max_fd = MAX(inbound_sd, max_fd);
      FD_SET(inbound_sd, &rfds);
    }
    select(max_fd + 1, &rfds, NULL, NULL, NULL);

    if (FD_ISSET(loconet_fd, &rfds)) {	/*Got a Loconet packet */
      readbuf[0] = 0;
      readcnt = read(loconet_fd, readbuf, 255);
      if (DEBUG)
	printf("In - Loconet: %d bytes\n", readcnt);
      if (readcnt > 0) {

	for (i = 0; i < readcnt; i++) {
	  if (readbuf[i] >= 0x80) {
	    if ((sendcnt != 0) & doLog)
	      write_log("Loconet-DISCARD", sendbuf, sendcnt);
	    opcode = readbuf[i] & 0x60;
	    if (opcode == 0x00) {
	      msglen = 2;
	    }
	    if (opcode == 0x20) {
	      msglen = 4;
	    }
	    if (opcode == 0x40) {
	      msglen = 6;
	    }
	    if (opcode == 0x60) {
	      msglen = readbuf[i + 1];
	    }			//potential problem
	    sendcnt = 0;
	    chksum = readbuf[i];
	    sendbuf[sendcnt++] = readbuf[i];
	  } else {
	    chksum ^= readbuf[i];
	    sendbuf[sendcnt++] = readbuf[i];
	    if (sendcnt == msglen) {
	      if (chksum == 0xff) {
		if (doBroadcast) {
		  rc = sendto(broadcast_sd, &sendbuf, sendcnt, 0, (struct sockaddr *)&bcastAddr, sizeof(bcastAddr));
		  if (DEBUG)
		    printf("  Out - Bcast: %u bytes.\n", rc);
		}
		if (doMulticast) {
		  rc = sendto(multicast_sd, &sendbuf, sendcnt, 0, (struct sockaddr *)&multAddr, sizeof(multAddr));
		  if (DEBUG)
		    printf("  Out - Mcast: %u bytes.\n", rc);
		}
		if (doLog)
		  write_log("Loconet", sendbuf, sendcnt);
		sendcnt = 0;
	      } else {
		if (doLog)
		  write_log("Loconet-BAD-PACKET", sendbuf, sendcnt);
		sendcnt = 0;
	      }
	    }
	  }
	}			/* end for */
      }
    }

    /*end FD_ISSET - Loconet */
    if (FD_ISSET(inbound_sd, &rfds)) {	/*Got a UDP packet */

      cliLen = sizeof(clientAddr);
      readcnt = recvfrom(inbound_sd, readbuf, 255, 0, (struct sockaddr *)&clientAddr, &cliLen);
      if (DEBUG)
	printf("In - UDP: %d bytes.\n", readcnt);
      chksum = 0;
      for (i = 0; i < readcnt; i++) {
	chksum ^= readbuf[i];
      }
      if (chksum == 0xff) {
	if (doSerial) {
	  writecnt = write(loconet_fd, readbuf, readcnt);
	  fdatasync(loconet_fd);
	  if (DEBUG)
	    printf("  Out - loconet: %d bytes.\n", writecnt);
	}

	if (doMulticast) {
	  rc = sendto(multicast_sd, &readbuf, readcnt, 0, (struct sockaddr *)&multAddr, sizeof(multAddr));
	  if (DEBUG)
	    printf("  Out - Mcast: %u bytes.\n", rc);
	}

	if (doBroadcast) {
	  rc = sendto(broadcast_sd, &readbuf, readcnt, 0, (struct sockaddr *)&bcastAddr, sizeof(bcastAddr));
	  if (DEBUG)
	    printf("  Out - Bcast: %u bytes.\n", rc);
	}

	if (doLog)
	  write_log("UDP", readbuf, readcnt);
      } else {
	if (doLog)
	  write_log("UDP-BAD-PACKET", readbuf, readcnt);
      }

    }
    /*end FD_ISSET - UDP inbound */
  }
}
