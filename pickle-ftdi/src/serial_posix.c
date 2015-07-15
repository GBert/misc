/*
 * Copyright (C) 2005-2015 Darron Broad
 * All rights reserved.
 * 
 * This file is part of Pickle Microchip PIC ICSP.
 * 
 * Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation. 
 * 
 * Pickle Microchip PIC ICSP is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along
 * with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
 */

#include "pickle.h"

/*
 * Open serial port
 *
 * For bit-bang I/O use speed B0.
 *
 *  return file descriptor on success or -1 on error
 */
int
serial_open(const char *device, speed_t speed)
{
	int serial_port;
	struct termios options;

	serial_port = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (serial_port < 0)
		return -1;

	fcntl(serial_port, F_SETFL, O_NONBLOCK | fcntl(serial_port, F_GETFL));

	/* Get options */
	tcgetattr(serial_port, &options);

	/*
	 * Raw mode
	 *
	 *  Linux TERMIOS(3)
	 */

	/* Input options */
	options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP |
		INLCR | IGNCR | ICRNL | IXON);

	/* Output options */
	options.c_oflag &= ~(OPOST);

	/* Control options */
	options.c_cflag &= ~(CSIZE | PARENB);
	options.c_cflag |= (CS8);

	/* Local options */
	options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

	/* Control characters */
	options.c_cc[VMIN] = 0;
	options.c_cc[VTIME] = 0;

	/* Baud rate */
	cfsetispeed(&options, speed);
	cfsetospeed(&options, speed);

	/* Set options */
	tcsetattr(serial_port, TCSANOW, &options);

	/* Discard */
	tcflush(serial_port, TCIOFLUSH);

	return serial_port;
}

/*
 * Close serial port
 */
void
serial_close(int serial_port)
{
	tcdrain(serial_port);
	close(serial_port);
}

/*
 * Get CTS (input set when +ve) (DATA_IN)
 */
int
get_cts(int serial_port)
{
	int status;

	ioctl(serial_port, TIOCMGET, &status);

	return (status & TIOCM_CTS) ? (HIGH) : (LOW);
}

/*
 * Set DTR (output +ve on set) (DATA_OUT)
 */
void
set_dtr(int serial_port, int dtr)
{
	int status;

	ioctl(serial_port, TIOCMGET, &status);

	if (dtr)
		status |= TIOCM_DTR;
	else
		status &= ~TIOCM_DTR;

	ioctl(serial_port, TIOCMSET, &status);
}

/*
 * Set RTS (output +ve on set) (CLOCK)
 */
void
set_rts(int serial_port, int rts)
{
	int status;

	ioctl(serial_port, TIOCMGET, &status);

	if (rts)
		status |= TIOCM_RTS;
	else
		status &= ~TIOCM_RTS;

	ioctl(serial_port, TIOCMSET, &status);
}

/*
 * Set Tx (output +ve on set) (VPP)
 */
void
set_tx(int serial_port, int tx)
{
	if (tx)
		ioctl(serial_port, TIOCSBRK, 0); /* +ve */
	else
		ioctl(serial_port, TIOCCBRK, 0); /* -ve */
}

/*
 * Return `speed_t' for given baud rate
 */
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
