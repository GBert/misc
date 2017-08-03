/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 *
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#include <ftdi.h>

#define DEFAULT_SPEED	62500

struct rs485_data_t {
    unsigned int speed;
    struct ftdi_context *ftdi;
};

struct rs485_data_t rs485_data;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -s <speed>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -s <speed>          speed - default %d\n", DEFAULT_SPEED);
}

int time_stamp(char *timestamp) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
    return 0;
}

int countBits(uint8_t value) {
    int ret = 0;

    while (value > 0) {
	ret++;
	value >>= 1;
    }
    return ret;
}

int init_rs485(struct rs485_data_t *rs485_data) {
    int ret;

    rs485_data->ftdi = ftdi_new();

    if (!rs485_data->ftdi) {
	fprintf(stderr, "ftdi_new failed\n");
	return -1;
    }

    /* check for FT232RL device */
    if ((ret = ftdi_usb_open(rs485_data->ftdi, 0x0403, 0x6001)) < 0) {
	fprintf(stderr, "unable to open FTDI device: %d (%s)\n", ret, ftdi_get_error_string(rs485_data->ftdi));
	ftdi_free(rs485_data->ftdi);
	return EXIT_FAILURE;
    }

    if ((ret = ftdi_set_baudrate(rs485_data->ftdi, rs485_data->speed)) < 0) {
	fprintf(stderr, "unable to set baudrate: %d (%s)\n", ret, ftdi_get_error_string(rs485_data->ftdi));
	ftdi_free(rs485_data->ftdi);
	return EXIT_FAILURE;
    }

    if ((ret = ftdi_setflowctrl(rs485_data->ftdi, SIO_DISABLE_FLOW_CTRL)) < 0) {
        fprintf(stderr, "unable to set flow control: %d (%s)\n", ret, ftdi_get_error_string(rs485_data->ftdi));
        return EXIT_FAILURE;
    }
    return 0;
}


int close_rs485(struct rs485_data_t *rs485_data) {
    int ret;

    if ((ret = ftdi_usb_close(rs485_data->ftdi)) < 0) {
	fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(rs485_data->ftdi));
	ftdi_free(rs485_data->ftdi);
	return EXIT_FAILURE;
    }
    ftdi_free(rs485_data->ftdi);
    return 0;
}

int send_rs485_data(struct rs485_data_t *rs485_data, unsigned char *data, int length) {
    int ret;

    if ((ret = ftdi_set_line_property(rs485_data->ftdi, BITS_8, STOP_BIT_1, MARK)) < 0) {
        fprintf(stderr, "unable to set flow control: %d (%s)\n", ret, ftdi_get_error_string(rs485_data->ftdi));
        return EXIT_FAILURE;
    }
    if ((ret = ftdi_write_data(rs485_data->ftdi, data, 1)) != 1) {
        fprintf(stderr, "unable to sent data (address): %d (%s)\n", ret, ftdi_get_error_string(rs485_data->ftdi));
        return EXIT_FAILURE;
    }

    if ((ret = ftdi_set_line_property(rs485_data->ftdi, BITS_8, STOP_BIT_1, SPACE)) < 0) {
        fprintf(stderr, "unable to set flow control: %d (%s)\n", ret, ftdi_get_error_string(rs485_data->ftdi));
        return EXIT_FAILURE;
    }
    if ((ret = ftdi_write_data(rs485_data->ftdi, &data[1], length - 1)) != length -1) {
        fprintf(stderr, "unable to sent data (address): %d (%s)\n", ret, ftdi_get_error_string(rs485_data->ftdi));
        return EXIT_FAILURE;
    }
    return 0;
}

int main(int argc, char **argv) {
    int ret, opt;

    memset(&rs485_data, 0, sizeof(rs485_data));

    rs485_data.speed = DEFAULT_SPEED;

    while ((opt = getopt(argc, argv, "i:s:h?")) != -1) {
	switch (opt) {
	case 's':
	    rs485_data.speed = atoi(optarg);
	    break;
	case 'h':
	case '?':
	    print_usage(basename(argv[0]));
	    exit(EXIT_SUCCESS);
	    break;
	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(EXIT_FAILURE);
	}
    }

    ret = init_rs485(&rs485_data);
    if (ret > 0)
	exit(EXIT_FAILURE);

    while (1) {
    }
    ret = close_rs485(&rs485_data);
    if (ret > 0)
	exit(EXIT_FAILURE);
    return 0;
}
