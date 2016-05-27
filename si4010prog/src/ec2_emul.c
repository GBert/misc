/*
 * ec2_emul.c - Silicon Laboritories EC2 Serial debugger emulator
 *
 * NOTE: This program is only written for testing purpose... Don't expect too
 * much from it.
 *
 * Copyright (c) 2014, David Imhoff <dimhoff.devel@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors may
 *       be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <getopt.h>

#include "ec2_cksum.h"
#include "si4010.h"

uint8_t pages[512 * 256] = { 0 };
uint8_t active_page = 0;

uint8_t code_mem[0x1200] = { 0 };
uint8_t iram_mem[0xFF] = { 0 };

bool mcu_running = false;;

void usage(const char *name)
{
	fprintf(stderr,
		"Usage: %s [-d <uri] <socket file>\n"
		"Options:\n"
		"  -d <uri>       Programmer device to use. Use 'help' for help.\n"
		"  -h             Print this help message\n"
		"\n"
		"Normaly the Socket file argument will point to a socket file created by a \n"
		"VMWare virtual COM port.\n"
		, name);
}


#define CRC16_CCITT_POLY 0x1021
uint16_t update_crc16_ccitt(uint16_t crc, uint8_t val)
{
        int i;

	crc ^= ((uint16_t) val) << 8;

	for (i = 0; i < 8; i++) {
		if (crc & 0x8000) {
			crc <<= 1;
			crc ^= CRC16_CCITT_POLY;
		} else {
			crc <<= 1;
		}
        }
 
        return crc;
}

void dump_flashpage_to_file()
{
	FILE *ofp;
	char fname[1024];

	snprintf(fname, sizeof(fname), "/tmp/ec2_fp.%d.dat", active_page);
	if ((ofp = fopen(fname, "wb")) == NULL) {
		return;
	}
	fwrite(&pages[active_page * 512], 512, 1, ofp);
	fclose(ofp);
}

void dump_code_to_file()
{
	FILE *ofp;
	char *fname = "/tmp/c2_code_mem.dat";

	if ((ofp = fopen(fname, "wb")) == NULL) {
		return;
	}
	fwrite(code_mem, sizeof(code_mem), 1, ofp);
	fclose(ofp);
}

int read_byte(int ifd)
{
	uint8_t b;
	ssize_t len;
	len = read(ifd, &b, 1);
	if (len != 1) {
		return EOF;
	}
	return b;
}

int write_byte(uint8_t b, int ifd)
{
	ssize_t len;
	len = write(ifd, &b, 1);
	if (len != 1) {
		return EOF;
	}
	return b;
}

int main(int argc, char *argv[])
{
	int c;
	uint8_t buf[256];
	uint16_t addr;
	int len;
	int r;
	int i;
	int bytes_read;
	int x = 2;
	int ifd = STDIN_FILENO;
	int ofd = STDOUT_FILENO;
	int retval = EXIT_SUCCESS;
	int opt;
	char *c2_bus_type = "fx2";
	char *c2_bus_path = "";
	struct c2_bus c2_bus_handle;

	while ((opt = getopt(argc, argv, "d:h")) != -1) {
		switch (opt) {
		case 'd':
			c2_bus_type = optarg;
			if (strcmp(c2_bus_type, "help") == 0) {
				//usage_dev_uri();
//TODO:
				fprintf(stderr, "TODO:\n");
				exit(EXIT_SUCCESS);
			} else {
				char *sep;
				sep = strstr(c2_bus_type, "://");
				if (sep == NULL) {
					fprintf(stderr, "C2 bus device uri "
							"incorrect format\n");
					exit(EXIT_FAILURE);
				}
				*sep = '\0';
				c2_bus_path = sep + 3;
			}
			break;
		case 'h':
			usage(argv[0]);
			exit(EXIT_SUCCESS);
			break;
		default: /* '?' */
			usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (argc - optind == 1) {
		if (strcmp(argv[optind], "-") != 0) {
			struct sockaddr_un saddr;
			int sockfd;

			sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
			if (sockfd == -1) {
				perror("socket()");
				exit(EXIT_FAILURE);
			}

			saddr.sun_family = AF_UNIX;
			strncpy(saddr.sun_path, argv[optind], UNIX_PATH_MAX);
			saddr.sun_path[UNIX_PATH_MAX - 1] = '\0';

			if (connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) != 0) {
				perror("Failed to bind to socket");
				close(sockfd);
				exit(EXIT_FAILURE);
			}

			ifd = ofd = sockfd;
		}
	} else {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	if (c2_bus_open(&c2_bus_handle, c2_bus_type, c2_bus_path) != 0) {
		fprintf(stderr, "Failed to open C2 bus: %s\n",
					c2_bus_get_error(&c2_bus_handle));
		goto bad;
	}

	if (si4010_init(&c2_bus_handle) != 0) {
		fprintf(stderr, "Failed to init si4010\n");
		retval = EXIT_FAILURE;
		goto bad;
	}

	while (true) {
		c = read_byte(ifd);
		switch (c) {
		case 'U':
			write_byte(0x5A, ofd);
			break;
		case 0x00:
			// EC2 connect
			fprintf(stderr, "EC2 Connect\n");
			c = read_byte(ifd);
			if (c != 0) {
				fprintf(stderr, "Invalid EC2 Connect byte\n");
				break;
			}
			c = read_byte(ifd);
			if (c != 0) {
				fprintf(stderr, "Invalid EC2 Connect byte\n");
				break;
			}
			write_byte(0x01, ofd);
			break;
		case 0x01:
			// EC2 select flash page
			fprintf(stderr, "EC2 Select Flash Page\n");
			c = read_byte(ifd); // page number
			if (c == EOF) {
				fprintf(stderr, "Invalid EC2 Select Flash page byte\n");
				break;
			}
			active_page = c;
			c = read_byte(ifd);
			if (c != 0) {
				fprintf(stderr, "Invalid EC2 Select Flash page byte\n");
				break;
			}
			write_byte(0x00, ofd);
			break;
		case 0x02:
			// EC2 erase flash page
			fprintf(stderr, "EC2 Erase Flash Page\n");
			c = read_byte(ifd);
			if (c != 0) {
				fprintf(stderr, "Invalid EC2 Erase Flash page byte\n");
				break;
			}
			c = read_byte(ifd);
			if (c != 0) {
				fprintf(stderr, "Invalid EC2 Erase Flash page byte\n");
				break;
			}
			write_byte(0x00, ofd);
			break;
		case 0x03:
			// EC2 Write Flash page
			fprintf(stderr, "EC2 Write Flash Page\n");
			c = read_byte(ifd);
			if (c != 0x02) {
				fprintf(stderr, "Invalid EC2 Write Flash page byte\n");
				break;
			}
			c = read_byte(ifd);
			if (c != 0) {
				fprintf(stderr, "Invalid EC2 Write Flash page byte\n");
				break;
			}
			write_byte(0x00, ofd);
			bytes_read = 0;
			while (bytes_read < 512) {
				r = read(ifd, &pages[active_page * 512 + bytes_read], 512 - bytes_read);
				if (r < 0) {
					perror("Couldn't read flash page in EC2 Write Flash Page");
					break;
				} else if (r == 0) {
					fprintf(stderr, "Couldn't read flash page in EC2 Write Flash Page: socket closed");
					break;
				}
				bytes_read += r;
			}
			for (i=0; i < 512; i+=1) { //TODO: xor per int
				pages[active_page * 512 + i] ^= 0x55;
			}
//dump_flashpage_to_file();
			write_byte(0x00, ofd);
			break;
		case 0x04: {
			uint16_t chksum;
			// EC2 calc page cksum page
			fprintf(stderr, "EC2 Calc Page Checksum\n");
			c = read_byte(ifd);
			if (c != 0) {
				// start address?
				fprintf(stderr, "Invalid EC2 Calc Page Checksum byte\n");
				break;
			}
			c = read_byte(ifd);
			if (c != 0) {
				// start address?
				fprintf(stderr, "Invalid EC2 Calc Page Checksum byte\n");
				break;
			}
			chksum = boot_local_calc_page_cksum(&pages[active_page * 512]);
			write_byte((chksum >> 8) & 0xff, ofd);
			write_byte(chksum & 0xff, ofd);
			break;
		}
		case 0x05:
			// EC2 read byte
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid EC2 Calc Page Checksum byte\n");
				break;
			}
			addr = c << 8;
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid EC2 Calc Page Checksum byte\n");
				break;
			}
			addr |= (c & 0xff);
			fprintf(stderr, "EC2 Read Byte @ 0x%.4x\n", addr);

			write_byte(pages[addr], ofd);
			break;
		case 0x06:
			// EC2 Run App
			fprintf(stderr, "EC2 Run App\n");
			c = read_byte(ifd);
			if (c != 0) {
				fprintf(stderr, "Invalid EC2 Run App byte\n");
				break;
			}
			c = read_byte(ifd);
			if (c != 0) {
				fprintf(stderr, "Invalid EC2 Run App byte\n");
				break;
			}
			write_byte(0x17, ofd);
			break;




		/* C2 commands */
		case 0x20:
			// C2 Connect Target
			fprintf(stderr, "C2 Connect Target\n");
			if (si4010_init(&c2_bus_handle) == 0 && si4010_reset() == 0) {
				write_byte(0x0d, ofd);
			} else {
				write_byte(0x00, ofd);
			}
			break;
		case 0x21:
			// C2 Disconnect Target
			fprintf(stderr, "C2 Disconnect Target\n");
			write_byte(0x0d, ofd);
			break;
		case 0x22:
			// Get Device ID
			fprintf(stderr, "Get Device ID\n");
			addr = c2_get_chip_version();
			write_byte(addr >> 8, ofd);
			write_byte(addr & 0xff, ofd);
			write_byte(0x00, ofd);
			break;
		case 0x23:
			// Get Uniq Device ID???
			fprintf(stderr, "Get Uniq Device ID %.2x\n", x);
			write_byte(0x00, ofd);
			write_byte(0xd0, ofd);
//TODO:
			write_byte(0x00, ofd);
			break;
		case 0x24:
			// Run
			fprintf(stderr, "Run\n");
			si4010_resume();
			mcu_running = true;
			write_byte(0x0d, ofd);
			break;
		case 0x25:
			// Halt
			fprintf(stderr, "Halt\n");
			si4010_halt();
			mcu_running = false;
			write_byte(0x0d, ofd);
			break;
		case 0x26:
			// Step
			fprintf(stderr, "Step\n");
//TODO:
			write_byte(0x0d, ofd);
			break;
		case 0x27:
			// Check Running
			fprintf(stderr, "Check Running\n");
			// return status: 0=running, 1=halted
//TODO: actually check the C2 bus master to see if device is still running
			if (mcu_running) {
				write_byte(0x00, ofd);
			} else {
				write_byte(0x01, ofd);
			}
			break;
		case 0x28:
			// C2 Read SFR
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Read SFR byte 1 0x%.2x\n", c);
				break;
			}
			r = c;
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Read SFR byte 2 0x%.2x\n", c);
				break;
			}
			fprintf(stderr, "C2 Read SFR %d @ 0x%.2x\n", c, r);

			if (si4010_sfr_read(r, c, buf))
			{
				fprintf(stderr, "Failed to read from SFR\n");
				// return some error?
			}

			for (i = 0; i < c; i++) {
				write_byte(buf[i], ofd);
			}
			write_byte(0x00, ofd);
			break;
		case 0x29:
			// C2 Write SFR
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Write SFR byte 1 0x%.2x\n", c);
				break;
			}
			addr = c;
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Write SFR byte 2 0x%.2x\n", c);
				break;
			}
			len = c;
			fprintf(stderr, "C2 Write SFR %d @ 0x%.2x: ", len, addr);

			for (i=0; i < len; i++) {
				c = read_byte(ifd);
				if (c == EOF) {
					fprintf(stderr, " -- EOF!!\n");
					break;
				}
				fprintf(stderr, "%.2x ", c);
				buf[i] = c;
			}
			fprintf(stderr, "\n");

			if (si4010_sfr_write(addr, len, buf) != 0) {
				fprintf(stderr, "Failed to write SFR bytes\n");
				write_byte(0x00, ofd);
			} else {
				write_byte(0x0d, ofd);
			}
			break;
		case 0x2A:
			// C2 Read Int. Ram
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Read IRAM byte 1 0x%.2x\n", c);
				break;
			}
			addr = c;
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Read IRAM byte 2 0x%.2x\n", c);
				break;
			}
			fprintf(stderr, "C2 Read IRAM %d @ 0x%.2x\n", c, addr);

			if (si4010_ram_read(addr, c, buf) != 0) {
				fprintf(stderr, "Failed to read from IRAM\n");
			}

			for (i=0; i < c; i++) {
				write_byte(buf[i], ofd);
			}
			write_byte(0x00, ofd);
			break;
		case 0x2B: {
			uint16_t addr;
			// C2 Write Int. Ram
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Write IRAM byte 1 0x%.2x\n", c);
				break;
			}
			addr = c;
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Write IRAM byte 3 0x%.2x\n", c);
				break;
			}
			len = c;

			fprintf(stderr, "Write IRAM %d @ %.4x: ", len, addr);

			for (i=0; i < len; i++) {
				c = read_byte(ifd);
				if (c == EOF) {
					fprintf(stderr, " -- EOF!!\n");
					break;
				}
				fprintf(stderr, "%.2x ", c);
				buf[i] = c;
			}
			fprintf(stderr, "\n");

			if (si4010_ram_write(addr, len, buf) != 0) {
				fprintf(stderr, "Failed to write IRAM bytes\n");
				write_byte(0x00, ofd);
			} else {
				write_byte(0x0d, ofd);
			}
			break;
		}
		case 0x2e: {
			uint16_t addr;
			// C2 Read CODE
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Read CODE byte 1 0x%.2x\n", c);
				break;
			}
			addr = c;
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Read CODE byte 2 0x%.2x\n", c);
				break;
			}
			addr |= (c << 8);
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Read CODE byte 3 0x%.2x\n", c);
				break;
			}
			len = c;

			fprintf(stderr, "C2 Read CODE %d @ %.4x\n", len, addr);
			if (addr + len > 0x1200) {
				fprintf(stderr, "C2 Read CODE address out of range 0x%.4x\n", addr);
				break;
			}

			if (si4010_xram_read(addr, len, buf) != 0) {
				fprintf(stderr, "Failed to read from XRAM\n");
			}

			for (i=0; i < len; i++) {
				write_byte(buf[i], ofd);
			}
			write_byte(0x00, ofd);
			break;
		}
		case 0x2f: {
			uint16_t addr;
			// C2 Write CODE
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Write CODE byte 1 0x%.2x\n", c);
				break;
			}
			addr = c;
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Write CODE byte 2 0x%.2x\n", c);
				break;
			}
			addr |= (c << 8);
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Write CODE byte 3 0x%.2x\n", c);
				break;
			}
			len = c;

			fprintf(stderr, "C2 Write CODE %d @ %.4x: ", len, addr);
			if (addr + len > 0x1200) {
				fprintf(stderr, "C2 Write CODE address out of range 0x%.4x\n", addr);
				break;
			}

			for (i=0; i < len; i++) {
				c = read_byte(ifd);
				if (c == EOF) {
					fprintf(stderr, " -- EOF!!\n");
					break;
				}
				fprintf(stderr, "%.2x ", c);
				buf[i] = c;
			}
			fprintf(stderr, "\n");

			if (si4010_xram_write(addr, len, buf) != 0) {
				fprintf(stderr, "Failed to write XRAM bytes\n");
				write_byte(0x00, ofd);
			} else {
				write_byte(0x0d, ofd);
			}
			break;
		}

		case 0x31: {
			uint16_t chksum;
			// C2 calc CODE cksum
			c = read_byte(ifd);
			if (c < 0 || c > 0x11) {
				// page nr?
				fprintf(stderr, "Invalid C2 Calc CODE Checksum byte %.2x\n", c);
				break;
			}
			addr = c;
			addr <<= 8;
			fprintf(stderr, "C2 Calc CODE Checksum @ %.4x\n", addr);

			if (si4010_xram_read(addr, 0x100, buf) != 0) {
				fprintf(stderr, "Failed to read from XRAM\n");
			}

			chksum = 0;
			for (i=0; i<0x100; i++) {
				chksum = update_crc16_ccitt(chksum, buf[i]);
			}
			write_byte((chksum >> 8) & 0xff, ofd);
			write_byte(chksum & 0xff, ofd);
			write_byte(0x00, ofd);
			break;
		}



/*
		case 0x36:
			// C2 Special Read
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Special Read byte\n");
				break;
			}
			fprintf(stderr, "C2 Special Read 0x%.2x \n", c);
			c = read_byte(ifd);
			if (c != 1) {
				fprintf(stderr, "Invalid C2 Special Read byte\n");
				break;
			}

			write_byte(0x00, ofd);
			write_byte(0x0d, ofd);
			break;
		case 0x37:
			// C2 Special Write
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Special Write byte 1 0x%.2x\n", c);
				break;
			}
			r = c;
			c = read_byte(ifd);
			if (c != 1) {
				fprintf(stderr, "Invalid C2 Special Write byte 2 0x%.2x\n", c);
				break;
			}
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Special Write byte 3 0x%.2x\n", c);
				break;
			}
			fprintf(stderr, "C2 Special Write 0x%.2x = 0x%.2x\n", r, c);

			write_byte(0x0d, ofd);
			break;
*/
		case 0x3e:
			// C2 Read XDATA EMIF
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Read XDATA EMIF byte 1 0x%.2x\n", c);
				break;
			}
			addr = c;
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Read XDATA EMIF byte 2 0x%.2x\n", c);
				break;
			}
			addr |= (c << 8);
			c = read_byte(ifd);
			if (c == EOF) {
				fprintf(stderr, "Invalid C2 Read XDATA EMIF byte 3 0x%.2x\n", c);
				break;
			}
			len = c;

			fprintf(stderr, "C2 Read XDATA EMIF %d @ 0x%.4x\n", len, addr);
			if (addr > 0xFFFF - len) {
				fprintf(stderr, "C2 Read XDATA EMIF address out of range 0x%.4x\n", addr);
				break;
			}

			if (si4010_xram_read(addr, len, buf) != 0) {
				fprintf(stderr, "Failed to read from XRAM\n");
			}

			for (i=0; i < len; i++) {
				write_byte(buf[i], ofd);
			}
			write_byte(0x00, ofd);

			break;


			
		default:
			while (c != EOF) {
				fprintf(stderr, "read byte: %.2x\n", c);
				c = read_byte(ifd);
			}
		}
	}

bad:
	c2_bus_destroy(&c2_bus_handle);
	if (ifd != STDIN_FILENO) {
		close(ifd);
	}
	if (ofd != STDOUT_FILENO) {
		close(ofd);
	}

	return retval;
}
