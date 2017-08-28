#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

typedef volatile uint32_t *PWM_ADDR;
static int pwm_mem = -1;
static void *pwm_map = NULL;

#define AW_PWM_BASE_ADDR	(0x01C20000)
#define AW_PWM_CTRL_REG		(0x00000E00)
#define AW_PWM_CH0_PERIOD	(0x00000E04)
#define AW_PWM_CH1_PERIOD	(0x00000E08)
#define AW_MAP_LEN		4096

void usage(char *prg) {
    fprintf(stderr, "\nUsage: %s \n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -p period\n");
    fprintf(stderr, "         -d duty cycle\n");
}

int pwm_open(const char *device) {

    /* open /dev/mem */
    pwm_mem = open(device, O_RDWR | O_SYNC);
    if (pwm_mem < 0) {
	printf("%s: warning: open failed [%s]\n", __func__, strerror(errno));
	pwm_mem = -1;
	return -1;
    }

    /* memory map PWM */
    pwm_map = mmap(NULL, AW_MAP_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, pwm_mem, AW_PWM_BASE_ADDR);
    if (pwm_map == MAP_FAILED) {
	printf("%s: warning: mmap failed [%s]\n", __func__, strerror(errno));
	close(pwm_mem);
	pwm_mem = -1;
	return -1;
    }
    return 0;
}

void pwm_close(void) {
    if (pwm_map) {
	if (munmap(pwm_map, AW_MAP_LEN)) {
	    printf("%s: warning: munmap failed\n", __func__);
	}
	pwm_map = NULL;
    }
    if (pwm_mem >= 0) {
	if (close(pwm_mem)) {
	    printf("%s: warning: close failed\n", __func__);
	}
	pwm_mem = -1;
    }
}

static inline void pwm_read(uint32_t pwm_reg, uint32_t * val) {
    PWM_ADDR reg = (PWM_ADDR) (pwm_map) + pwm_reg;

    *val = *reg;
}

static inline void pwm_write(uint32_t pwm_reg, uint32_t val) {
    PWM_ADDR reg = (PWM_ADDR) (pwm_map) + pwm_reg;

    *reg = val;
}

int main(int argc, char **argv) {

    if (pwm_open("/dev/mem") < 0) {
	fprintf(stderr, "Can't open IO mem: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    pwm_close();
    return 0;
}
