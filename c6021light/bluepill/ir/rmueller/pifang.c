// pifang.c : Konsolenanwendung zur Digitalsignalaufzeichnung auf einem RasPi.
// C 2017 Rainer Müller 
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>  

#define SPI_BAUDRATE 88200;

unsigned char puffer[500000];
long unsigned int n, anzahl;
time_t t_now = 0;
struct timeval tv = { 0, 0 };

int main(int argc, char* argv[])
{
	anzahl = 55125; //60000;

	printf("fang vom %s\n", __DATE__);
    printf("%ld Samples <= ", anzahl * 8);
	for (n=0; n<anzahl; n++) puffer[n] = 0;

    /* open port */
    int dev = open("/dev/spidev0.1", O_WRONLY);
    if (dev < 0) {
        printf("Device open failed: %s (errno = %d).\n",
            strerror(errno), errno);
        return 1;
    }
    int mode = 1; 
    if (ioctl (dev, SPI_IOC_WR_MODE, &mode) < 0) {
        printf("SPI Mode Change failure: %s.\n", strerror(errno));
        return 1;
    }
    unsigned char spiBPW = 8;
    if (ioctl (dev, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0) {
        printf("SPI BPW Change failure: %s.\n", strerror(errno));
        return 1;
    }
    int speed = SPI_BAUDRATE;
    if (ioctl (dev, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        printf("SPI Speed Change failure: %s.\n", strerror(errno));
        return 1;
    }

    struct spi_ioc_transfer spi_rx;
    memset (&spi_rx, 0, sizeof (spi_rx)) ;
    spi_rx.rx_buf =  (unsigned long) &puffer;
    spi_rx.len = anzahl;
    spi_rx.bits_per_word = 8;
    spi_rx.speed_hz = SPI_BAUDRATE;

    gettimeofday(&tv, NULL);
    t_now = tv.tv_sec * 1000000 + tv.tv_usec;

    if (ioctl(dev, SPI_IOC_MESSAGE(1), &spi_rx) < 0) {
        printf("Error SPI Transfer ioctl: %s (errno = %d).\n", 
            strerror(errno), errno);  
        return 1;
    }

    gettimeofday(&tv, NULL);
    t_now = tv.tv_sec * 1000000 + tv.tv_usec - t_now;
	printf("=>  %i ms\n", t_now/1000);

	for (n=0; n<300; n++) {
		printf("%.2x  ", puffer[n]);
	}	

	FILE *aus = fopen((argc > 1) ? argv[1] : "default.dat", "wb");
	fwrite(puffer, 1, anzahl, aus);
	fclose(aus);

	return 0;
}

