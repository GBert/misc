
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>

void options(int fd, struct termios *old, struct termios *new, int speed) {
	fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL));

	tcgetattr(fd, old);
	tcgetattr(fd, new);

        new->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
        new->c_oflag &= ~(OPOST);
        new->c_cflag &= ~(CSIZE | PARENB);
        new->c_cflag |= (CS8);
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
	int fdcons = STDIN_FILENO, fduart;
	int speed = 10000;
	struct termios oldfdcons, newfdcons, oldfduart, newfduart;
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

	options(fdcons, &oldfdcons, &newfdcons, speed);
	options(fduart, &oldfduart, &newfduart, speed);

	memset(buffer, 0x55, 64);
/*	for (int i = 0; i<= 10; i++) */
	    fdwrite(fduart, buffer, 64);

	tcflush(fduart, TCIFLUSH);
	tcsetattr(fduart, TCSANOW, &oldfduart);
	close(fduart);

	tcflush(fdcons, TCIFLUSH);
	tcsetattr(fdcons, TCSANOW, &oldfdcons);
	close(fdcons);

	return 0;
}
