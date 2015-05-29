/******************************************************************************
 
  Can-Can test application
 
  Copyright (c) 2014 Darron M Broad
 
*******************************************************************************
 
  This file is part of the Can-Can CAN bus interface project.
 
  Can-Can is licensed under the CC BY-NC-SA 4.0.
 
  See file /LICENSE for details.
  
******************************************************************************/

/*
 * DISCLAIMER:
 *
 * This program has no relation to a 19th century French dance-hall dance.
 * This is purely co-incidental. You are not discouraged, however, from dancing
 * along when using this program.
 */

/*
 * Various UNIX and Linux header files
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <assert.h>
#include <sysexits.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <net/if.h>
#ifdef __linux
#include <linux/can.h>
#endif

/*
 * Input buffer
 */
#define BUFLEN (256)

/*
 * I/O time out in seconds
 */
#define TIMEOUT (2)

/*
 * Baud rate lookup
 */
typedef struct {
	uint32_t baud;
	speed_t speed;
} baudrate_t;

/*
 * Can-can session
 */
typedef struct {
	int fdtty;	/* TTY descriptor             */
	int csock;	/* CAN socket                 */
	uint16_t scid;  /* CAN bus id to send         */ 
	uint16_t rcid;  /* CAN bus id received        */ 
	uint32_t delay; /* Rate delay in microseconds */
	char *ttydev;	/* TTY device name            */
	char *candev;	/* CAN device name            */
} session_t;

/*******************************************************************************
 *
 * Open serial device
 *
 ******************************************************************************/
static int
openDevice(const char *dev, speed_t speed)
{
	int fd;
	struct termios options;

	fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd < 0) {
		return fd;
	}
	if (fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL)) < 0) {
		close(fd);
		return -1;
	}
	if (tcgetattr(fd, &options) < 0) {
		close(fd);
		return -1;
	}

#if 1
	/*
	 * Raw Mode 8N2
	 *
	 *  Linux TERMIOS(3)
	 */

	options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	options.c_oflag &= ~(OPOST);
	options.c_cflag &= ~(CSIZE | PARENB);
	options.c_cflag |= (CS8 | CSTOPB);
	options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

	options.c_cc[VMIN] = 0;
	options.c_cc[VTIME] = 0;
#else
	/*
	 * Raw Mode 8N2
	 *
	 *  `slcand'
	 */

 	cfmakeraw(&options);
	options.c_cflag &= ~(CRTSCTS);
	options.c_iflag &= ~(IXOFF);

	options.c_cflag |= (CSTOPB);
#endif
	if (cfsetispeed(&options, speed) < 0) {
		close(fd);
		return -1;
	}
	if (cfsetospeed(&options, speed) < 0) {
		close(fd);
		return -1;
	}
	if (tcsetattr(fd, TCSANOW, &options) < 0) {
		close(fd);
		return -1;
	}
	if (tcflush(fd, TCIOFLUSH) < 0) {
		close(fd);
		return -1;
	}

	return fd;
}

/*******************************************************************************
 *
 * Return `speed_t' for Given Baud Rate
 *
 *  Speed May Not be Supported On Target Hardware.
 *
 *  Eg. PL2303 Supported Rates:
 *
 *	75,     150,     300,     600,     1200,   1800,  2400, 3600,
 *      4800,   7200,    9600,    14400,   19200,  28800, 38400,
 *      57600,  115200,  230400,  460800,  614400,
 *      921600, 1228800, 2457600, 3000000, 6000000
 *
 ******************************************************************************/
static speed_t
serial_speed(uint32_t baudrate)
{
	static baudrate_t rates[] = {
	{0, B0},
	{50, B50},
	{75, B75},
	{110, B110},
	{134, B134},
	{150, B150},
	{200, B200},
	{300, B300},
	{600, B600},
	{1200, B1200},
	{1800, B1800},
	{2400, B2400},
	{4800, B4800},
#ifdef B7200
	{7200, B7200},
#endif
	{9600, B9600},
#ifdef B14400
	{14400, B14400},
#endif
#ifdef B19200
	{19200, B19200},
#else
	{19200, EXTA},
#endif
#ifdef B28800
	{28800, B28800},
#endif
#ifdef B38400
	{38400, B38400},
#else
	{38400, EXTB},
#endif
#ifdef B57600
	{57600, B57600},
#endif
#ifdef B76800
	{76800, B76800},
#endif
#ifdef B115200
	{115200, B115200},
#endif
#ifdef B230400
	{230400, B230400},
#endif
#ifdef B460800
	{460800, B460800},
#endif
#ifdef B500000
	{500000, B500000},
#endif
#ifdef B576000
	{576000, B576000},
#endif
#ifdef B921600
	{921600, B921600},
#endif
#ifdef B1000000
	{1000000, B1000000},
#endif
#ifdef B1152000
	{1152000, B1152000},
#endif
#ifdef B1500000
	{1500000, B1500000},
#endif
#ifdef B2000000
	{2000000, B2000000},
#endif
#ifdef B2500000
	{2500000, B2500000},
#endif
#ifdef B3000000
	{3000000, B3000000},
#endif
#ifdef B3500000
	{3500000, B3500000},
#endif
#ifdef B4000000
	{4000000, B4000000},
#endif
	{UINT32_MAX, B9600},
	};
	int i = 0;

	while (baudrate > rates[i++].baud)
		;

	return rates[--i].speed;
}

/*******************************************************************************
 *
 * Open Linux CAN socket
 *
 ******************************************************************************/
static int
openCanSock(const char *dev)
{
#ifdef __linux
	struct ifreq ifr;
	struct sockaddr_can addr;
	int fsock, rc;

	fsock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (fsock < 0) {
		return fsock;
	}

	bzero(&ifr, sizeof(ifr));
	strncpy(ifr.ifr_name, dev, IFNAMSIZ);

	rc = ioctl(fsock, SIOCGIFINDEX, &ifr);
	if (rc < 0) {
		close(fsock);
		return rc;
	}

	bzero(&addr, sizeof(addr));
	addr.can_family = AF_CAN ;
	addr.can_ifindex = ifr.ifr_ifindex;

	rc = bind(fsock, (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0) {
		close(fsock);
		return rc;
	}

	return fsock;
#else
	return -1;
#endif
}

/*******************************************************************************
 *
 * Write message to Linux CAN socket
 *
 *  If this fails with ENOBUFS, then increase the TX queue with:
 *
 *	ip link set can0 txqueuelen 1024
 *
 ******************************************************************************/
static int
canWrite(session_t *c, uint8_t *inbuf, int buflen)
{
#ifdef __linux
	struct can_frame frame;
	int rc;

	if (buflen == 0)
		return 0;

	bzero(&frame, sizeof(frame));

	frame.can_id = c->scid;
	frame.can_dlc = buflen;
	memcpy(frame.data, inbuf, buflen);

	rc = write(c->csock, &frame, sizeof(frame));
	if (rc <= 0)
		return rc;
	if (rc != sizeof(frame))
		return -1;

	return buflen;
#else
	errno = EBADF;

	return -1;
#endif
}

/*******************************************************************************
 *
 * Read message from Linux CAN socket
 *
 ******************************************************************************/
static int
canRead(session_t *c, uint8_t *inbuf, int buflen)
{
#ifdef __linux
	struct can_frame frame;
	int rc;

	if (buflen == 0)
		return 0;

	bzero(&frame, sizeof(frame));

	rc = read(c->csock, &frame, sizeof(frame));
	if (rc <= 0)
		return rc;
	if (rc != sizeof(frame))
		return -1;

	c->rcid = frame.can_id;

	memcpy(inbuf, frame.data, frame.can_dlc);

	return frame.can_dlc;
#else
	errno = EBADF;

	return -1;
#endif
}

/*******************************************************************************
 *
 * Dump Bytes as Hex
 *
 ******************************************************************************/
static inline void
dump_hex(uint8_t *s, int n)
{
	int i = 0;

	while (i < n) {
		printf("0x%02X ", s[i]);
		i++;
	}
	printf("\n");
}

/*******************************************************************************
 *
 * Count & Display packets per second
 *
 ******************************************************************************/
static void
count_pps(char *instr, uint32_t *count, struct timeval *tv1)
{
	struct timeval tv2, tv3;
	double pps;

	if (*count == 0) {
		gettimeofday(tv1, NULL);
		*count = 1;
	} else {
		gettimeofday(&tv2, NULL);
		timersub(&tv2, tv1, &tv3);
		if (tv3.tv_sec >= 1) {
			pps = (double)(*count) /
				((double)tv3.tv_sec + tv3.tv_usec / 1000000.);
			printf("%s %.2f pps\n", instr, pps);
			*count = 0;
		} else {
			(*count)++;
		}
	}
}

/*******************************************************************************
 *
 * Process UART Input; Eg. t0008B905000000000000
 *
 ******************************************************************************/
static inline void
uartinput_process(session_t *c, uint8_t *inbuf)
{
	static char banner[BUFLEN] = {0};
	static struct timeval tv = {0};
	static uint32_t count = 0;
	static int64_t prev = -1;
	uint64_t this = 0;
	uint32_t id = 0;

	if (!banner[0])
		snprintf(banner, BUFLEN, "[%s <= %s]", c->ttydev, c->candev);

	sscanf((char *)inbuf, "t%03X8%016jX\r", &id, &this);

	if (this != (prev - 1))
		printf("%s 0x%016jX != 0x%016jX (%s)\n", banner, this, prev - 1, inbuf);

	prev = this;

	count_pps(banner, &count, &tv);
}

/*******************************************************************************
 *
 * `strchr' with length limit
 *
 ******************************************************************************/
static uint8_t *
strnchr(uint8_t *s, uint8_t c, int n)
{
	int i = 0;

	while (s[i] && i < n) {
		if (s[i] == c)
			return &s[i];
		i++;
	}
	return NULL;
}

/*******************************************************************************
 *
 * Detect String on UART Input and Process
 *
 ******************************************************************************/
static inline uint8_t *
uartinput_str(session_t *c, uint8_t *inbuf, int *incnt)
{
	uint8_t *cp;
	int i;

	if ((cp = strnchr(inbuf, '\r', *incnt)) != NULL) {
		i = cp - inbuf;
		assert(i < BUFLEN);

		inbuf[i++] = '\0';
		(*incnt) -= i;
		assert(*incnt >= 0 && *incnt < BUFLEN);

		if (inbuf[0] == 't' && i == 22)
			uartinput_process(c, inbuf);

		memmove(inbuf, &inbuf[i], *incnt);
	}
	return cp;
}

/*******************************************************************************
 *
 * Detect Error on UART Input and Remove
 *
 *  Linux will insert a NUL when an error is detected.
 *
 ******************************************************************************/
static inline uint8_t *
uartinput_nul(uint8_t *inbuf, int *incnt)
{
	uint8_t *cp;
	int i;

	if ((cp = memchr(inbuf, '\0', *incnt)) != NULL) {
		i = cp - inbuf;
		assert(i < BUFLEN);

		i++;
		(*incnt) -= i;
		assert(*incnt >= 0 && *incnt < BUFLEN);

		memmove(inbuf, &inbuf[i], *incnt);
	}
	return cp;
}

/*******************************************************************************
 *
 * Inspect Packet on CAN Input and Process
 *
 ******************************************************************************/
static inline void
caninput_str(session_t *c, uint8_t *canbuf, uint8_t cancnt)
{
	static char banner[BUFLEN] = {0};
	static struct timeval tv = {0};
	static uint32_t count = 0;
	static int64_t prev = -1;
	uint64_t this = 0;
	uint8_t i = 0;
	
	if (!banner[0])
		snprintf(banner, BUFLEN, "[%s => %s]", c->ttydev, c->candev);

	while (i < cancnt)
		this = (this << 8) | canbuf[i++];

	if (this != (prev + 1))
		printf("%s 0x%016jX != 0x%016jX\n", banner, this, prev + 1);

	prev = this;

	count_pps(banner, &count, &tv);
}

/*******************************************************************************
 *
 * CAN - CAN
 *
 ******************************************************************************/
void
can2can(session_t *c)
{
	int fd = (c->csock > c->fdtty) ? c->csock : c->fdtty;
	fd_set fdread, fdwrite;
	struct timeval tv;
	int64_t ttyseq = 0, canseq = 0;
	uint64_t this;
	int rc, i;

	uint8_t inbuf[BUFLEN];
	int incnt = 0;

	uint8_t outbuf[BUFLEN];
	int outcnt = 0, outlen = 0;

	uint8_t canbuf[BUFLEN];

	bzero(inbuf, BUFLEN);
	bzero(outbuf, BUFLEN);
	bzero(canbuf, BUFLEN);

	while (1) {
		tv.tv_sec = TIMEOUT;
		tv.tv_usec = 0;

		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);

		/* CAN DEV OUT/UART DEV IN */
		FD_SET(c->csock, &fdwrite);
		FD_SET(c->fdtty, &fdread);

		/* UART DEV OUT/CAN DEV IN */
		FD_SET(c->fdtty, &fdwrite);
		FD_SET(c->csock, &fdread);

		rc = select(fd + 1, &fdread, &fdwrite, NULL, &tv);
		if (rc < 0) {
			if (errno == EINTR)
				continue;
		}

		/* UART disconnected? CAN Bus down? */
		if (rc == 0) {
			fprintf(stderr, "select() TIMED-OUT\n");
			return;
		}

		/* Send on Serial UART */
		if (FD_ISSET(c->fdtty, &fdwrite)) {
			if (outcnt == outlen) {
				snprintf((char *)outbuf, BUFLEN, "t%03X8%016jX\r", c->scid, ttyseq++);
				outcnt = 0;
				outlen = strlen((char *)outbuf);
			}
			rc = write(c->fdtty, &outbuf[outcnt], outlen - outcnt);
			if (rc < 0) {
				if (errno != EINTR && errno != EAGAIN) {
					fprintf(stderr, "write() FAILED [%s]\n",
						strerror(errno));
					return;
				}
			} else if (rc == 0) {
				fprintf(stderr, "EOF in UART write()\n");
				return;
			} else {
				outcnt += rc;
			}
		}

		/* Receive on Serial UART */
		if (FD_ISSET(c->fdtty, &fdread)) {
			rc = read(c->fdtty, &inbuf[incnt], BUFLEN - incnt);
			if (rc < 0) {
				if (errno != EINTR && errno != EAGAIN) {
					fprintf(stderr, "read() FAILED [%s]\n",
						strerror(errno));
					return;
				}
			} else if (rc == 0) {
				fprintf(stderr, "EOF in UART read()\n");
				return;
			} else {
				incnt += rc;
				while (uartinput_str(c, inbuf, &incnt) || uartinput_nul(inbuf, &incnt))
					;
			}
		}

		/* Send on CAN Bus */
		if (FD_ISSET(c->csock, &fdwrite)) {
			this = canseq;
			for (i = 7; i >= 0; i--) {
				canbuf[i] = (uint8_t)(this);
				this >>= 8;
			}
			rc = canWrite(c, canbuf, 8);
			if (rc < 0) {
				if (errno != EINTR && errno != EAGAIN) {
					fprintf(stderr, "canWrite() FAILED [%s]\n",
						strerror(errno));
					return;
				}
			} else if (rc == 0) {
				fprintf(stderr, "EOF in canWrite()\n");
				return;
			} else {
				canseq--;
			}
		}

		/* Receive on Can Bus */
		if (FD_ISSET(c->csock, &fdread)) {
			rc = canRead(c, canbuf, 8);
			if (rc < 0) {
				if (errno != EINTR && errno != EAGAIN) {
					fprintf(stderr, "canRead() FAILED [%s]\n",
						strerror(errno));
					return;
				}
			} else if (rc == 0) {
				fprintf(stderr, "EOF in canRead()\n");
				return;
			} else {
				caninput_str(c, canbuf, rc);
			}
		}

		/* Wait a while? */
		if (c->delay)
			usleep(c->delay);
	}
}

/*******************************************************************************
 *
 * HOWTO
 *
 ******************************************************************************/
void
usage(const char *msg, int err)
{
	fprintf(stderr, "USAGE: can-can tty can\n\n");

	if (msg)
		fprintf(stderr, "%s\n\n", msg);

	fprintf(stderr, "Options:\n"
		" -b N use TTY baud rate N\n"
		" -i N use CAN bus message id N\n"
		" -m N use microsecond rate delay N\n"

		"\n");

	exit(err);
}

/*******************************************************************************
 *
 * Do the Can-Can...
 *
 ******************************************************************************/
int
main(int argc, char **argv)
{
	int opt;
	int nargs = 2;
	uint32_t baudrate = 115200;

	session_t c;
	c.scid = 0;
	c.delay = 0;

	opterr = 0;
	while ((opt = getopt(argc, argv, "b:i:m:")) != -1) {
		switch (opt) {
		case 'b':
			baudrate = strtoul(optarg, NULL, 0);
			break;
		case 'i':
			c.scid = strtoul(optarg, NULL, 0);
			break;
		case 'm':
			c.delay = strtoul(optarg, NULL, 0);
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if (argc != nargs) {
		usage("Invalid args.", EX_USAGE);
	}

	if (argv[0][0] == '/') {
		c.ttydev = argv[0];
		c.candev = argv[1];
	} else {
		c.candev = argv[0];
		c.ttydev = argv[1];
	}

	c.csock = openCanSock(c.candev);
	if (c.csock < 0) {
		fprintf(stderr, "Failed to open can socket [%s].\n", c.candev);
		exit(EX_OSERR);
	}

	c.fdtty = openDevice(c.ttydev, serial_speed(baudrate));
	if (c.fdtty < 0) {
		fprintf(stderr, "Failed to open tty device [%s].\n", c.ttydev);
		exit(EX_OSERR);
	}

	can2can(&c);

	close(c.fdtty);
	close(c.csock);

	exit(EX_OK);
}
