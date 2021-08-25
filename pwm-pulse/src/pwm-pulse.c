#include "allwinner.h"
#include <libgen.h>

#define AW_PWM_CTRL_REG		(0x00000E00)
#define AW_PWM_CH0_PERIOD	(0x00000E04)
#define AW_PWM_CH1_PERIOD	(0x00000E08)

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s \n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -p toggle pin [e.g. PH2 -> (7*32+2) 226]\n");
    fprintf(stderr, "         -t period time in us\n");
    fprintf(stderr, "         -d duty cycle in us\n");
}

void print_iomem(unsigned int start, unsigned int len) {
    unsigned int i, val;
    for (i = start; i <= len; i++) {
	if (!(i & 3))
	    printf("\n%08x: ", AW_BASE_ADDR + 4 * i);
	io_aw_read(i, &val);
	printf(" %08x", val);
    }
}

int main(int argc, char **argv) {
    int opt, pwm_period, pwm_duty_cycle, toggle_pin;

    pwm_period = 0;
    pwm_duty_cycle = 0;
    toggle_pin = 0;

    if (io_aw_open("/dev/mem") < 0) {
	fprintf(stderr, "Can't open IO mem: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "p:t:d:h?")) != -1) {
	switch (opt) {
	case 'p':
	    toggle_pin = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 't':
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

    if (pwm_duty_cycle > pwm_period) {
	fprintf(stderr, "pwm_duty_cycle [%d] larger then pwm_period [%d]\n", pwm_duty_cycle, pwm_period);
	io_aw_close();
	exit(EXIT_FAILURE);
    }

    if (toggle_pin)
	gpio_aw_select_output(toggle_pin);
    else
	return 0;

    printf("toggling port %d ...\n", toggle_pin);

#if 0
    while (1) {
	gpio_aw_set(toggle_pin, HIGH);
	gpio_aw_set(toggle_pin, LOW);
    }
#else
    unsigned int val, oldval;
    io_aw_read(AW_PWM_CTRL_REG >> 2, &val);
    printf("read CTRL 0x%08x 0x%08x\n", AW_PWM_CTRL_REG, val);
    io_aw_write(AW_PWM_CTRL_REG >> 2, val & ~PWM_CH1_EN);
    printf("write CTRL 0x%08x\n", val & ~PWM_CH1_EN);
    io_aw_write(AW_PWM_CH1_PERIOD >> 2, (((pwm_period * 24 - 1) << 16) & 0xffff0000) | ((pwm_duty_cycle * 24) & 0xffff));
    io_aw_write(AW_PWM_CTRL_REG >> 2, PWM_CH1_EN | PWM_CH1_CLK_GATING | 0x00078000);
    io_aw_read(AW_PWM_CTRL_REG >> 2, &val);
    printf("write CTRL 0x%08x\n", val);
    /* print_iomem(0, 1024); */

#if 1
    io_aw_read(AW_PWM_CTRL_REG >> 2, &val);
    oldval = 0;
    while (1) {
	if (val ^ oldval) {
    	    printf("CTRL differ 0x%08x\n", val);
	    gpio_aw_set(toggle_pin, HIGH);
	} else {
	    gpio_aw_set(toggle_pin, LOW);
	}
	oldval = val;
    }
#endif
#endif

    io_aw_close();
    return 0;
}
