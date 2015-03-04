#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>

/* CP2104 IOCTLs */
#define IOCTL_GPIOGET	0x8000
#define IOCTL_GPIOSET	0x8001
/* CP2104 GPIO */
#define GPIO_0_ON	0x00010001
#define GPIO_0_OFF	0x00000001
#define GPIO_1_ON	GPIO_0_ON  << 1
#define GPIO_1_OFF	GPIO_0_OFF << 1
#define GPIO_2_ON	GPIO_0_ON  << 2
#define GPIO_2_OFF	GPIO_0_OFF << 2
#define GPIO_3_ON	GPIO_0_ON  << 3
#define GPIO_3_OFF	GPIO_0_OFF << 3

void options(int fd, struct termios *old, struct termios *new, int speed) {
    fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL));

    tcgetattr(fd, old);
    tcgetattr(fd, new);

    new->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    new->c_oflag &= ~(OPOST);
    new->c_cflag &= ~(CSIZE | PARENB);
    new->c_cflag |= (CS6);
    /* new->c_cflag |= (BOTHER); */
    new->c_cflag &= ~CBAUD;;
    new->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    new->c_cc[VMIN] = 1;
    new->c_cc[VTIME] = 0;

    cfsetispeed(new, speed);
    cfsetospeed(new, speed);

    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, new);
}

void set_dtr(int serial_port, int dtr) {
	int status;

	ioctl(serial_port, TIOCMGET, &status);

	if (dtr)
		status |= TIOCM_DTR;
	else
		status &= ~TIOCM_DTR;

	ioctl(serial_port, TIOCMSET, &status);
}


int fdwrite(int fd, char *buf, int buflen) {
    int rc, nb = 0;

    while (nb < buflen) {
	rc = write(fd, buf, buflen);
	if (rc < 0) {
	    if (errno == EINTR || errno == EAGAIN)
		continue;
	    perror("write()");
	    exit(1);
	}
	if (rc == 0)
	    return nb;
	nb += rc;
    }
    return nb;
}

int main(int argc, char *argv[]) {
    int fduart;
    int speed = 10000;
    struct termios oldfduart, newfduart;
    char buffer[1024];

    if (argc < 2) {
	fprintf(stderr, "Try: %s /dev/ttyUSB0 [10000]\n", argv[0]);
	exit(1);
    }

    fduart = open(argv[1], O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fduart < 0) {
	perror("open()");
	exit(1);
    }

    if (argc > 2) {
	speed = strtoul(argv[1], (char **)NULL, 10);
    }

    options(fduart, &oldfduart, &newfduart, speed);

    set_dtr(fduart, 0);

    memset(buffer, 0x55, 16);
/*	for (int i = 0; i<= 10; i++) */
    fdwrite(fduart, buffer, 16);

/*    set_dtr(fduart, 0);
    set_dtr(fduart, 1);
    set_dtr(fduart, 0);
*/
    memset(buffer, 0x66, 16);
/*	for (int i = 0; i<= 10; i++) */
    fdwrite(fduart, buffer, 16);
    set_dtr(fduart, 1);


    usleep(30000);

    /* tcflush(fduart, TCIFLUSH);
    tcsetattr(fduart, TCSANOW, &oldfduart); */
    close(fduart);

    return 0;
}
