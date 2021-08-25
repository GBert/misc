// spitest.c : Konsolenanwendung zum Testen der SPI-Schnittstelle auf einem Pi.
// C 2017 Rainer Müller 
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).

//#define OSZI

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

//SPI Baudrate für Märklin / Motorola Protokoll.
#define SPI_BAUDRATE_MAERKLIN_LOCO	38461
#define SPI_BAUDRATE_MAERKLIN_LOCO_2	(2*SPI_BAUDRATE_MAERKLIN_LOCO)

//SPI Baudrate für DCC/NMRA.
#define SPI_BAUDRATE_NMRA		68966
#define SPI_BAUDRATE_NMRA_2		(SPI_BAUDRATE_NMRA * 2)

//SPI Baudrate für MFX.
#define SPI_BAUDRATE_MFX		80000
#define SPI_BAUDRATE_MFX_2		(SPI_BAUDRATE_MFX * 2)

int baudrates[] = { SPI_BAUDRATE_MAERKLIN_LOCO_2, SPI_BAUDRATE_NMRA_2, SPI_BAUDRATE_MFX_2 };

unsigned char puffer[2000];
unsigned char rpuffer[2000];
time_t t_now = 0;
struct timeval tv = { 0, 0 };

int dev;

void test(int speed, int pktsize) {
    long int soll = ((1000000L * pktsize) / speed) * 8 + 100;
    struct spi_ioc_transfer spi_tx;

    memset(&spi_tx, 0, sizeof(spi_tx));
    spi_tx.tx_buf = (unsigned long)puffer;
    spi_tx.rx_buf = (unsigned long)rpuffer;
    spi_tx.len = pktsize;
    spi_tx.bits_per_word = 8;
    spi_tx.speed_hz = speed;

    gettimeofday(&tv, NULL);
    t_now = tv.tv_sec * 1000000 + tv.tv_usec;

    if (ioctl(dev, SPI_IOC_MESSAGE(1), &spi_tx) < 0) {
	printf("Error SPI Transfer ioctl: %s (errno = %d).\n", strerror(errno), errno);
	return;
    }

    gettimeofday(&tv, NULL);
    t_now = tv.tv_sec * 1000000 + tv.tv_usec - t_now;
    long int diff = ((signed)t_now - soll) * 1000 / soll;
#ifndef OSZI
    printf("%6ld => %6ldµs (%4ld‰)", soll, t_now, diff);
#endif
}

int main(void) {
    int anzahl, rate;
    unsigned int n;

    printf("spitest vom %s\n", __DATE__);
    for (n = 0; n < sizeof(puffer); n++)
	puffer[n] = 0x43;
    for (rate = 0; rate < 3; rate++)
	printf("        %8d Baud     ", baudrates[rate]);
    printf("\n");

    /* open port */
    dev = open("/dev/spidev0.0", O_WRONLY);
    if (dev < 0) {
	printf("Device open failed: %s (errno = %d).\n", strerror(errno), errno);
	return 1;
    }
    int mode = 1;
    if (ioctl(dev, SPI_IOC_WR_MODE, &mode) < 0) {
	printf("SPI Mode Change failure: %s.\n", strerror(errno));
	return 1;
    }
    unsigned char spiBPW = 8;
    if (ioctl(dev, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0) {
	printf("SPI BPW Change failure: %s.\n", strerror(errno));
	return 1;
    }
    int speed = SPI_BAUDRATE_MFX_2;
    if (ioctl(dev, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
	printf("SPI Speed Change failure: %s.\n", strerror(errno));
	return 1;
    }
#ifdef OSZI
    while (1) {
	test(baudrates[2], 2);
    }
#endif

    for (anzahl = 60; anzahl <= 70; anzahl += 1) {
	printf("%4d: ", anzahl);
	for (rate = 0; rate < 3; rate++) {
	    test(baudrates[rate], anzahl);
	}
	printf("\n");
    }

    return 0;
}
