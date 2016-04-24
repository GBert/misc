/*------------------------------------------------------------------------------
;
;   Take it Easy Loader for The Wellington Boot Loader for PIC18
;
;   Copyright (c) 2014 Gerhard Bertelsmann
;   Copyright (c) 2014 - 2015 Darron M Broad
;
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;   This file is part of The Wellington Boot Loader.
;
;   The Wellington Boot Loader is free software: you can redistribute it and/or
;   modify it under the terms of the GNU General Public License as published
;   by the Free Software Foundation.
;
;   The Wellington Boot Loader is distributed in the hope that it will be
;   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License along
;   with The Wellington Boot Loader. If not, see http://www.gnu.org/licenses/
;-----------------------------------------------------------------------------*/

/*
 * DISCLAIMER:
 *
 * This program has no relation to any 1970s popular music of a similar
 * name. This is purely co-incidental. You are not discouraged, however,
 * from humming along to it's tune when using this program.
 */

/*
 * Various UNIX and Linux header files
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
 * Generic buffer
 *
 * Must be as large as the largest PIC18 write row size
 */
#define BUFLEN (1536)
#define BUFMAX (BUFLEN - 1)

/*
 * Easy loader commands
 */
#define COMMAND_HELLO       (0x80)
#define COMMAND_FLASH_ERASE (0x81)
#define COMMAND_FLASH_WRITE (0x82)
#define COMMAND_FLASH_READ  (0x83)
#define COMMAND_EE_WRITE    (0x84)
#define COMMAND_EE_READ     (0x85)
#define COMMAND_RUN         (0x86)

/*
 * Wellington boot loader command_hello packet
 */
struct hello_packet {
	uint8_t row;
	uint8_t erah, eral;
	uint8_t startu, starth, startl;
	uint8_t eeh, eel;
	uint8_t response;
} __attribute__((packed));

/*
 * Easy loader command_[flash|ee] packet
 */
struct command_packet {
	uint8_t command;
	uint8_t addru, addrh, addrl;
	uint8_t datasize;
	uint8_t data[BUFLEN];
} __attribute__((packed));

/*
 * Wellington boot loader responses
 */
#define RESPONSE_OK     'O'
#define RESPONSE_ERRCMD 'E'
#define RESPONSE_ERRSUM 'S'

/*
 * I/O time out in seconds
 */
#define TIMEOUT (2)

/*
 * HELLO retries
 */
#define RETRY (10)

/*
 * I/O in or out
 */
#define IN  (1)
#define OUT (0)

/*
 * CAN Bus
 */
#define CAN_ID        (0x666)
#define CAN_MSG_DELAY (100)
#define CAN_CMD_DELAY (100)

/*
 * Baud rate lookup
 */
typedef struct {
	uint32_t baud;
	speed_t speed;
} baudrate_t;

/*
 * Easy loader session
 */
typedef struct {
	FILE *info;	/* Information stream     */
	int fd;		/* I/O descriptor         */
	int fdtyp;	/* I/O type 0=uart, 1=CAN */
	int cid;	/* CAN bus id             */
} session_t;

/*******************************************************************************
 *
 * Open serial device
 *
 *  Hang up RTS/DTR If Required
 *
 ******************************************************************************/
int
openDevice(const char *dev, speed_t speed, int tim, int rts, int dtr)
{
	int fd, status;
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

	/*
	 * Raw mode 8N2
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

	/*
	 * Hang up RTS/DTR If Required
	 *
	 *  If the serial port is opened with a rate > B0 then
	 *  DTR and RTS are asserted. This means they are set to
	 *  logic 0 which is RS232 +VE.
	 *
	 *  Here we may de-assert these lines.
	 */

	if (rts) {
		ioctl(fd, TIOCMGET, &status);
		status |= TIOCM_RTS;
		ioctl(fd, TIOCMSET, &status);
		usleep(200000);
		status &= ~TIOCM_RTS;
		ioctl(fd, TIOCMSET, &status);
		usleep(200000);
	}
	if (dtr) {
		ioctl(fd, TIOCMGET, &status);
		status |= TIOCM_DTR;
		ioctl(fd, TIOCMSET, &status);
		usleep(200000);
		status &= ~TIOCM_DTR;
		ioctl(fd, TIOCMSET, &status);
		usleep(200000);
	}

	/*
	 * Delay after open?
	 */

	if (tim) {
		sleep(tim);
	}

	return fd;
}

/*******************************************************************************
 *
 * Return `speed_t' for Given Baud Rate
 *
 ******************************************************************************/
speed_t
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
int
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
 * Write 1 byte message to Linux CAN socket
 *
 *  If this fails with ENOBUFS, then increase the TX queue with:
 *
 *	ip link set can0 txqueuelen 1024
 *
 ******************************************************************************/
int
canWrite(session_t *e, uint8_t *buffer, int buflen)
{
#ifdef __linux
	struct can_frame frame;
	int rc;

	if (buflen == 0)
		return 0;

	bzero(&frame, sizeof(frame));

	frame.can_id = e->cid;
	frame.can_dlc = 1;
	frame.data[0] = buffer[0];

	rc = write(e->fd, &frame, sizeof(frame));
	if (rc <= 0)
		return rc;
	if (rc != sizeof(frame))
		return -1;

	usleep(CAN_MSG_DELAY);

	return 1;
#else
	errno = EBADF;

	return -1;
#endif
}

/*******************************************************************************
 *
 * Read 1 byte message from Linux CAN socket
 *
 ******************************************************************************/
int
canRead(session_t *e, uint8_t *buffer, int buflen)
{
#ifdef __linux
	struct can_frame frame;
	int rc;

	if (buflen == 0)
		return 0;

	bzero(&frame, sizeof(frame));

	rc = read(e->fd, &frame, sizeof(frame));
	if (rc <= 0)
		return rc;
	if (rc != sizeof(frame))
		return -1;
	
	if (frame.can_id != e->cid) {
		errno = EAGAIN; /* Filter message */
		return -1;
	}

	if (frame.can_dlc != 1) {
		errno = EAGAIN; /* Filter message */
		return -1;
	}

	memcpy(buffer, frame.data, frame.can_dlc);

	return frame.can_dlc;
#else
	errno = EBADF;

	return -1;
#endif
}

/*******************************************************************************
 *
 * Malloc with panic on error
 *
 ******************************************************************************/
static inline void *
xmalloc(size_t n, int c)
{
	void *s;

	if (!n)
		return NULL;
	s = malloc(n);
	if (s == NULL)
		exit(EX_OSERR);
	return memset(s, c, n);
}

/*******************************************************************************
 *
 * Free with sanity check
 *
 ******************************************************************************/
static inline void *
xfree(void *s)
{
	if (s)
		free(s);
	return NULL;
}

/*******************************************************************************
 *
 * Select in or out
 *
 ******************************************************************************/
static inline int
fdselect(session_t *e, long timeout, int io)
{
	int rc;
	struct timeval tv;
	fd_set fdset;

	tv.tv_sec = timeout;
	tv.tv_usec = 0;

	FD_ZERO(&fdset);
	FD_SET(e->fd, &fdset);

	if (io == IN)
		rc = select(e->fd + 1, &fdset, NULL, NULL, &tv);
	else	/* OUT */
		rc = select(e->fd + 1, NULL, &fdset, NULL, &tv);

	if (rc < 0)
		return rc;

	if (!FD_ISSET(e->fd, &fdset))
		return 0; /* Timed-out */

	if (rc != 1)
		return 0; /* Timed-out */

	return 1;
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
		if (s[i] > ' ' && s[i] < 127)
			printf("0x%02X[%c] ", s[i], s[i]);
		else
			printf("0x%02X ", s[i]);
		i++;
	}
	printf("\n");
}

/*******************************************************************************
 *
 * Read or write
 *
 ******************************************************************************/
static inline int
fdreadwrite(session_t *e, uint8_t *buffer, int buflen, int io)
{
	int rc;

	if (io == IN) {
		if (e->fdtyp) {
			rc = canRead(e, buffer, buflen);
		} else {
			rc = read(e->fd, buffer, buflen);
		}
	} else {	/* OUT */
		if (e->fdtyp) {
			rc = canWrite(e, buffer, buflen);
		} else {
			rc = write(e->fd, buffer, buflen);
		}
	}

	return rc;
}

/*******************************************************************************
 *
 * I/O
 *
 ******************************************************************************/
int
fdio(session_t *e, uint8_t *buffer, int buflen, long timeout, int io)
{
	int rc;
	int nb = 0;

	while (nb < buflen) {
		rc = fdselect(e, timeout, io);
		if (rc < 0) {
			if (errno == EINTR)
				continue;
			fprintf(stderr, "Select failed [%d/%d].\n", nb, buflen);
			return rc;
		}
		if (rc == 0) {
			fprintf(stderr, "Select timed-out [%d/%d]\n", nb, buflen);
			return 0;
		}
		rc = fdreadwrite(e, &buffer[nb], buflen - nb, io);
		if (rc < 0) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
			fprintf(stderr, "R/W failed [%d/%d].\n", nb, buflen);
			return rc;
		}
		if (rc == 0) {
			fprintf(stderr, "EOF [%d/%d].\n", nb, buflen);
			break;
		}
		nb += rc;
	}

	return nb;
}

/*******************************************************************************
 *
 * Read Hex Nibble
 *
 ******************************************************************************/
uint8_t
hex2nib(char c)
{
	uint8_t n = 0;

	if (c >= 'a' && c <= 'f') {
		n = c - 'a' + 10;
	}
	else if (c >= 'A' && c <= 'F') {
		n = c - 'A' + 10;
	}
	else if (c >= '0' && c <= '9') {
		n = c - '0';
	}
	return n;
}

/*******************************************************************************
 *
 * Read Hex Byte
 *
 ******************************************************************************/
static inline uint8_t
hex2byt(char *s)
{
	return hex2nib(s[0]) << 4 | hex2nib(s[1]);
}

/*******************************************************************************
 *
 * Read Hex Line
 *
 ******************************************************************************/
int
gethex(FILE *fp, uint16_t *addr, uint8_t *typ, uint8_t *bin, uint8_t *binlen)
{
	char line[BUFLEN];
	int i;
	uint8_t sum, len;

	if (fgets(line, BUFLEN, fp) == NULL)
		return 0;

	if (line[0] != ':')
		return -1;

	line[BUFMAX] = '\0';

	i = strlen(line) - 1;
	while (i >= 0 && (line[i] == '\r' || line[i] == '\n'))
		line[i--] = '\0';
		
	if (i & 1 || i < 10)
		return -1;

	sum = 0;
	*binlen = 0;
	for (i = 1; line[i]; i += 2) {
		bin[*binlen] = hex2byt(&line[i]);
		sum += bin[(*binlen)++];
	}
	if (sum)
		return -1;

	len = bin[0];
	if (*binlen != (len + 5))
		return -1;

	*addr = bin[1] << 8 | bin[2];
	*typ = bin[3];
	
	return len;
}

/*******************************************************************************
 *
 * Read Hex File
 *
 *  The hex file data is read into flash memory image.
 *
 *  Only data below the GOTO app vector is accepted.
 *
 ******************************************************************************/
int
readFile(const char *file, uint8_t *flash, uint16_t *eeprom, uint32_t startaddr, uint16_t eesize)
{
	FILE *fp;
	uint16_t addr, ext_addr = 0;
	uint8_t typ = 0, bin[BUFLEN], binlen, *data = &bin[4];
	uint32_t i, j, address, nbytes = 0;

	fp = fopen(file, "rb");
	if (!fp) {
		return -1;
	}
	while (typ != 1 && (gethex(fp, &addr, &typ, bin, &binlen) > 0)) {
		switch (typ) {
		case 0:	/* data */
			binlen -= 5;
			address = ext_addr << 16 | addr;
			for (i = 0; i < binlen; ++i) {
				j = address + i;
				if (j < (startaddr - 4)) {
					flash[j] = data[i];
					nbytes++;
				} else if (j >= 0xF00000 && j < (0xF00000 + eesize)) {
					eeprom[j - 0xF00000] = data[i];
					nbytes++;
				}
			}
			break;
		case 4: /* address */
			ext_addr = bin[4] << 8 | bin[5];
			break;
		}
	}
	fclose(fp);

	return nbytes;
}

/*******************************************************************************
 *
 * Fix GOTO
 *
 *  Preserve boot loader reset vector and store application reset vector in
 *  bootloader GOTO application vector.
 *
 ******************************************************************************/
void
fixGOTO(uint8_t *flash, uint32_t startaddr)
{
	// GOTO application
	flash[startaddr - 4] = flash[0];
	flash[startaddr - 3] = flash[1];
	flash[startaddr - 2] = flash[2];
	flash[startaddr - 1] = flash[3];

	// GOTO bootloader
	flash[0] = ((startaddr & 0x00001FE) >> 1);
	flash[1] = 0xEF;
	flash[2] = ((startaddr & 0x001FE00) >> 9);
	flash[3] = ((startaddr & 0x01E0000) >> 17) | 0xF0;
}

/*******************************************************************************
 *
 * Calculate Checksum
 *
 ******************************************************************************/
uint8_t
getChecksum(uint8_t *buffer, int blen)
{
	int i;
	uint8_t csum = 0;

	for (i = 0; i < blen; ++i)
		csum -= buffer[i];

	return csum;
}

/*******************************************************************************
 *
 * Send command to boot loader
 *
 ******************************************************************************/
int
doCommand(session_t *e, struct command_packet *cmd, int blen, int rlen)
{
	uint8_t *bcmd = (uint8_t *)cmd;
	int rc;

	while (1) {
#if 0
		printf("%s() address  = 0x%02X%02X%02X\n", __func__, cmd->addru, cmd->addrh, cmd->addrl);
		printf("%s() command  = 0x%02X\n", __func__, cmd->command);
		printf("%s() datasize = 0x%02X\n", __func__, cmd->datasize);
		printf("%s() data[0]  = 0x%02X\n", __func__, cmd->data[0]);
#endif
		assert(blen > 0);

		rc = fdio(e, bcmd, blen, TIMEOUT, OUT);
		if (rc != blen) {
			if (rc < 0) {
				fprintf(stderr, "I/O error in write [%s].\n", strerror(errno));
				return -1;
			}
			fprintf(stderr, "I/O timed-out in write [cmd = 0x%02X].\n", cmd->command);
			return -1;
		}
		if (e->fdtyp)
			usleep(CAN_CMD_DELAY);
	
		assert(rlen > 0 && rlen < BUFLEN);

		rc = fdio(e, bcmd, rlen, TIMEOUT, IN);
		if (rc != rlen) {
			if (rc < 0) {
				fprintf(stderr, "I/O error in read [%s].\n", strerror(errno));
				return -1;
			}
			fprintf(stderr, "I/O timed-out in read [cmd = 0x%02X].\n", cmd->command);
			return -1;
		}
		
		break;
	}

	if (bcmd[rlen - 1] != RESPONSE_OK) {
		fprintf(stderr, "I/O comms error in COMMAND [0x%02X != '%c'].\n", bcmd[rlen - 1], RESPONSE_OK);
		return -1;
	}
	return 0;
}

/*******************************************************************************
 *
 * Test for a blank flash row
 *
 ******************************************************************************/
static inline int
isBlank(uint8_t *flash, int size)
{
	int i;

	for (i = 0; i < size; ++i)
		if (flash[i] != 0xFF)
			return 0;
	return 1;
}

/*******************************************************************************
 *
 * Upload to flash
 *
 *  Erase and Write rows. For some devices the erase row is larger than
 *  the write row.
 *
 ******************************************************************************/
int
uploadFlash(session_t *e, int simulate, uint8_t *flash, uint32_t startaddr, uint16_t erasesize, uint8_t rowsize, int verify)
{
	struct command_packet cmd;
	uint8_t *bcmd = (uint8_t *)&cmd;
	int eaddress, raddress, i;

	/* For each erase row */
	for (eaddress = 0; eaddress < startaddr; eaddress += erasesize) {
		if (isBlank(&flash[eaddress], erasesize))
			continue;
		/* Erase then write first row */
		cmd.command = COMMAND_FLASH_ERASE;
		cmd.addru = (eaddress & 0xFF0000) >> 16;
		cmd.addrh = (eaddress & 0x00FF00) >> 8;
		cmd.addrl = (eaddress & 0x0000FF);
		if (isBlank(&flash[eaddress], rowsize)) {
			cmd.datasize = 1;
			cmd.data[0] = getChecksum(bcmd, 5);
			if (!simulate)
				if (doCommand(e, &cmd, 6, 1) < 0)
					return -1;
			if (e->info) fprintf(e->info, "ERASE FLASH ROW 0x%06X %4d BYTES PAYLOAD\n",
				eaddress, cmd.datasize - 1);
		} else { /* Erase then write */
			cmd.datasize = 1 + rowsize;
			memcpy(cmd.data, &flash[eaddress], rowsize);
			cmd.data[rowsize] = getChecksum(bcmd, 5 + rowsize);
			if (!simulate)
				if (doCommand(e, &cmd, 6 + rowsize, 1) < 0)
					return -1;
			if (e->info) fprintf(e->info, "ERASE FLASH ROW 0x%06X %4d BYTES PAYLOAD\n",
				eaddress, cmd.datasize - 1);
			if (verify) {
				cmd.command = COMMAND_FLASH_READ;
				cmd.addru = (eaddress & 0xFF0000) >> 16;
				cmd.addrh = (eaddress & 0x00FF00) >> 8;
				cmd.addrl = (eaddress & 0x0000FF);
				cmd.datasize = 1;
				cmd.data[0] = getChecksum(bcmd, 5);
				if (doCommand(e, &cmd, 6, 1 + rowsize) < 0)
					return -1;
				if (memcmp(&flash[eaddress], bcmd, rowsize) == 0) {
					if (e->info) fprintf(e->info, " VERIFY OK\n");
				} else {
					fprintf(stderr, " VERIFY ERROR\n");
				}
			}
		}
		/* For each other write row */
		for (i = rowsize; i < erasesize; i += rowsize) {
			raddress = eaddress + i;
			if (isBlank(&flash[raddress], rowsize))
				continue;
			/* Write other rows */
			cmd.command = COMMAND_FLASH_WRITE;
			cmd.addru = (raddress & 0xFF0000) >> 16;
			cmd.addrh = (raddress & 0x00FF00) >> 8;
			cmd.addrl = (raddress & 0x0000FF);
			cmd.datasize = 1 + rowsize;
			memcpy(cmd.data, &flash[raddress], rowsize);
			cmd.data[rowsize] = getChecksum(bcmd, 5 + rowsize);
			if (!simulate)
				if (doCommand(e, &cmd, 6 + rowsize, 1) < 0)
					return -1;
			if (e->info) fprintf(e->info, "WRITE FLASH ROW 0x%06X %4d BYTES PAYLOAD\n",
				raddress, cmd.datasize - 1);
			if (verify) {
				cmd.command = COMMAND_FLASH_READ;
				cmd.addru = (raddress & 0xFF0000) >> 16;
				cmd.addrh = (raddress & 0x00FF00) >> 8;
				cmd.addrl = (raddress & 0x0000FF);
				cmd.datasize = 1;
				cmd.data[0] = getChecksum(bcmd, 5);
				if (doCommand(e, &cmd, 6, 1 + rowsize) < 0)
					return -1;
				if (memcmp(&flash[raddress], bcmd, rowsize) == 0) {
					if (e->info) fprintf(e->info, " VERIFY OK\n");
				} else {
					fprintf(stderr, " VERIFY ERROR\n");
				}
			}
		}
	}
	return 0;
}

/*******************************************************************************
 *
 * Upload to EEPROM
 *
 ******************************************************************************/
int
uploadEEPROM(session_t *e, int simulate, uint16_t *eeprom, uint16_t eesize, int verify)
{
	struct command_packet cmd;
	uint8_t *bcmd = (uint8_t *)&cmd;
	int i;

	for (i = 0; i < eesize; ++i) {
		if (eeprom[i] == 0xFFFF)
			continue;
		cmd.command = COMMAND_EE_WRITE;
		cmd.addru = 0;
		cmd.addrh = (i & 0x00FF00) >> 8;
		cmd.addrl = (i & 0x0000FF);
		cmd.datasize = 2;
		cmd.data[0] = eeprom[i];
		cmd.data[1] = getChecksum(bcmd, 6);
		if (!simulate)
			if (doCommand(e, &cmd, 7, 1) < 0)
				return -1;
		if (e->info) fprintf(e->info, "WRITE EEPROM 0x%04X = 0x%02X\n", i, eeprom[i]);
		if (!verify)
			continue;
		cmd.command = COMMAND_EE_READ;
		cmd.addru = 0;
		cmd.addrh = (i & 0x00FF00) >> 8;
		cmd.addrl = (i & 0x0000FF);
		cmd.datasize = 1;
		cmd.data[0] = getChecksum(bcmd, 5);
		if (doCommand(e, &cmd, 6, 2) < 0)
			return -1;
		if (bcmd[0] == eeprom[i]) {
			if (e->info) fprintf(e->info, " VERIFY OK\n");
		} else {
			fprintf(stderr, " VERIFY ERROR\n");
		}
	}
	return 0;
}

/*******************************************************************************
 *
 * Dump device EEPROM
 *
 ******************************************************************************/
int
dumpEEPROM(session_t *e, uint16_t *eeprom, uint16_t eesize)
{
	struct command_packet cmd;
	uint8_t *bcmd = (uint8_t *)&cmd;
	int i, j;

	for (i = 0; i < eesize; ++i) {
		cmd.command = COMMAND_EE_READ;
		cmd.addru = 0;
		cmd.addrh = (i & 0x00FF00) >> 8;
		cmd.addrl = (i & 0x0000FF);
		cmd.datasize = 1;
		cmd.data[0] = getChecksum(bcmd, 5);
		if (doCommand(e, &cmd, 6, 2) < 0)
			return -1;
		eeprom[i] = bcmd[0];
	}
	for (i = 0; i < eesize; i += 16) {
		if (e->info) fprintf(e->info, "[%04X] ", i);
		for (j = 0; j < 16; ++j)
			if (e->info) fprintf(e->info, "%02X ", eeprom[i + j]);
		if (e->info) fputc('\n', e->info);
	}
	return 0;
}

/*******************************************************************************
 *
 * Dump device flash
 *
 ******************************************************************************/
int
dumpFlash(session_t *e, uint8_t *flash, uint32_t startaddr, uint8_t rowsize)
{
	struct command_packet cmd;
	uint8_t *bcmd = (uint8_t *)&cmd;
	int i, j;

	for (i = 0; i < startaddr; i += rowsize) {
		cmd.command = COMMAND_FLASH_READ;
		cmd.addru = (i & 0xFF0000) >> 16;
		cmd.addrh = (i & 0x00FF00) >> 8;
		cmd.addrl = (i & 0x0000FF);
		cmd.datasize = 1;
		cmd.data[0] = getChecksum(bcmd, 5);
		if (doCommand(e, &cmd, 6, 1 + rowsize) < 0)
			return -1;
		for (j = 0; j < rowsize; ++j)
			flash[i + j] = bcmd[j];
	}
	for (i = 0; i < startaddr; i += 16) {
		if (e->info) fprintf(e->info, "[%06X] ", i);
		for (j = 0; j < 16; ++j)
			if (e->info) fprintf(e->info, "%02X ", flash[i + j]);
		if (e->info) fputc('\n', e->info);
	}
	return 0;
}

/*******************************************************************************
 *
 * HOWTO
 *
 ******************************************************************************/
void
usage(const char *msg, int err)
{
	fprintf(stderr, "USAGE: easy-loader [OPTIONS] DEVICE [HEX-FILE]\n\n");

	if (msg)
		fprintf(stderr, "%s\n\n", msg);

	fprintf(stderr, "Options:\n"
		" -b N use TTY baud rate N\n"
		" -d   hang up DTR after serial port open\n"
		" -e   read EEPROM\n"
		" -f   read flash\n"
		" -h   HELLO only\n"
		" -i N use CAN bus message id N\n"
		" -n   do not run firmware on exit\n"
		" -q   quiet\n"
		" -r   hang up RTS after serial port open\n"
		" -s   simulate erase/write\n"
		" -t   time to wait after serial port open\n"
		" -v   verify\n"
		
		"\n");

	exit(err);
}

/*******************************************************************************
 *
 * Take it easy, take it easy... are there any other lyrics?
 *
 ******************************************************************************/
int
main(int argc, char **argv)
{
	int opt, rc, i;
	int nargs = 2, eeprom_read = 0, flash_read = 0, hello_only = 0, simulate = 0, verify = 0;
	int rts = 0, dtr = 0, tim = 0, autorun = 1;
	char *dev, *file;
	uint32_t baudrate = 115200;
	uint8_t *flash = NULL;
	uint16_t *eeprom = NULL;

	struct hello_packet hello;
	uint8_t *bhello = (uint8_t *)&hello, run;
	uint8_t rowsize;
	uint16_t erasesize, eesize;
	uint32_t startaddr;

	uint8_t discard[BUFLEN];

	session_t e;
	e.info = stdout;
	e.cid = CAN_ID;

	opterr = 0;
	while ((opt = getopt(argc, argv, "b:defhi:nrqst:v")) != -1) {
		switch (opt) {
		case 'b':
			baudrate = strtoul(optarg, NULL, 0);
			break;
		case 'd':
			dtr = 1;
			break;
		case 'e':
			eeprom_read = 1;
			nargs = 1;
			break;
		case 'f':
			flash_read = 1;
			nargs = 1;
			break;
		case 'h':
			hello_only = 1;
			nargs = 1;
			break;
		case 'i':
			e.cid = strtol(optarg, NULL, 0);
			break;	
		case 'n':
			autorun = 0;
			break;
		case 'r':
			rts = 1;
			break;
		case 'q':
			e.info = NULL;
			break;
		case 's':
			simulate = 1;
			break;
		case 't':
			tim = strtol(optarg, NULL, 0);
			break;
		case 'v':
			verify = 1;
			break;
		default:
			usage("Unknown option", EX_USAGE);
		}
	}
	argc -= optind;
	argv += optind;
	if (argc != nargs) {
		usage("Invalid args.", EX_USAGE);
	}

	dev = argv[0];
	if (dev[0] == '/') {
		e.fd = openDevice(dev, serial_speed(baudrate), tim, rts, dtr);
		e.fdtyp = 0;
	} else {
		e.fd = openCanSock(dev);
		e.fdtyp = 1;
	}
	if (e.fd < 0) {
		fprintf(stderr, "Failed to open I/O device [%s].\n", dev);
		exit(EX_OSERR);
	}

	for (i = 0;; ++i) {
		/* Discard */
		fdio(&e, discard, BUFLEN, 0, IN);

		/* Hello? */
		bhello[0] = COMMAND_HELLO;
		rc = fdio(&e, bhello, 1, TIMEOUT, OUT);

		if (rc == 1) {
			rc = fdio(&e, bhello, sizeof(struct hello_packet), TIMEOUT, IN);
			if (rc == sizeof(struct hello_packet)) {
				break;
			}
		}
		if (i == RETRY) {
			fprintf(stderr, "I/O failed [HELLO].\n");
			exit(EX_SOFTWARE);
		}
		sleep(1);
	}

	if (hello.response != RESPONSE_OK) {
		fprintf(stderr, "I/O comms error in HELLO [0x%02X != '%c'].\n", hello.response, RESPONSE_OK);
		exit(EX_SOFTWARE);
	}

	rowsize = hello.row;
	erasesize = hello.erah << 8 | hello.eral;
	startaddr = hello.startu << 16 | hello.starth << 8 | hello.startl;
	eesize = hello.eeh << 8 | hello.eel;

	if (e.info) fprintf(e.info, "PIC18 BOOT LOADER START ADDRESS = 0x%06X\n", startaddr);
	if (e.info) fprintf(e.info, "PIC18 ERASE SIZE  = %d\n", erasesize);
	if (e.info) fprintf(e.info, "PIC18 ROW SIZE    = %d\n", rowsize);
	if (e.info) fprintf(e.info, "PIC18 EEPROM SIZE = %d\n", eesize);

	if (hello_only) {
		close(e.fd);
		exit(EX_OK);
	}

	eeprom = xmalloc(eesize * sizeof(uint16_t), -1);
	flash = xmalloc(startaddr * sizeof(uint8_t), -1);

	if (eeprom_read) {
		dumpEEPROM(&e, eeprom, eesize);
	} else if (flash_read) {
		dumpFlash(&e, flash, startaddr, rowsize);
	} else {
		file = argv[1];
		rc = readFile(file, flash, eeprom, startaddr, eesize);
		if (rc < 0) {
			fprintf(stderr, "Failed to open hex file [%s].\n", file);
			exit(EX_SOFTWARE);
		}
		if (rc == 0) {
			fprintf(stderr, "No data in hex file [%s].\n", file);
			exit(EX_SOFTWARE);
		}

		fixGOTO(flash, startaddr);

		if (uploadFlash(&e, simulate, flash, startaddr, erasesize, rowsize, verify) == 0) {
			if (uploadEEPROM(&e, simulate, eeprom, eesize, verify) == 0) {
				if (autorun) {
					run = COMMAND_RUN;
					fdio(&e, &run, 1, TIMEOUT, OUT);
				}
			}
		}
	}

	xfree(flash);
	xfree(eeprom);
	close(e.fd);
	exit(EX_OK);
}
