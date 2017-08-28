#include "allwinner.h"
#include <libgen.h>

#define AW_IO_BASE_ADDR		(0x01C20000)
#define AW_PWM_CTRL_REG		(0x00000E00)
#define AW_PWM_CH0_PERIOD	(0x00000E04)
#define AW_PWM_CH1_PERIOD	(0x00000E08)

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s \n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -p period\n");
    fprintf(stderr, "         -d duty cycle\n");
}

int main(int argc, char **argv) {
    int opt, pwm_period, pwm_duty_cycle;

    if (io_aw_open("/dev/mem") < 0) {
	fprintf(stderr, "Can't open IO mem: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "p:d:h?")) != -1) {
	switch (opt) {
	case 'p':
	    pwm_period = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 'd':
	    pwm_duty_cycle = strtoul(optarg, (char **)NULL, 10);
	    break;
        case 'h':
        case '?':
	    print_usage(basename(argv[0]));
	    exit(EXIT_SUCCESS);
	}
    }

    io_aw_close();
    return 0;
}
