/*
 * libopencm3 test code
 */

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <stdio.h>
#include <errno.h>

int _write(int file, char *ptr, int len);

static void clock_setup(void) {
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable GPIOC clock (for LED GPIOs). */
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_USART1);
}

static void usart_setup(void) {

	/* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port A for transmit. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
	/* Setup UART parameters. */
	usart_set_baudrate(USART1, 57600);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_TX);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

static void gpio_setup(void) {
	gpio_set(GPIOC, GPIO13);
	/* Setup GPIO6 and 7 (in GPIO port A) for LED use. */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

static void tim_setup(void) {
	/* Enable TIM2 clock. */
	rcc_periph_clock_enable(RCC_TIM2);
	/* Enable TIM2 interrupt. */
	nvic_enable_irq(NVIC_TIM2_IRQ);
	/* Reset TIM2 peripheral. */
	timer_reset(TIM2);
	/* Timer global mode:
	* - No divider
	* - Alignment edge
	* - Direction up
	*/
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT,
	TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	/* Reset prescaler value. */
	timer_set_prescaler(TIM2, 36000);
	/* Enable preload. */
	timer_disable_preload(TIM2);
	/* Continous mode. */
	timer_continuous_mode(TIM2);
	/* Period (36kHz). */
	timer_set_period(TIM2, 65535);
	/* Disable outputs. */
	timer_disable_oc_output(TIM2, TIM_OC1);
	timer_disable_oc_output(TIM2, TIM_OC2);
	timer_disable_oc_output(TIM2, TIM_OC3);
	timer_disable_oc_output(TIM2, TIM_OC4);
	/* -- OC1 configuration -- */
	/* Configure global mode of line 1. */
	timer_disable_oc_clear(TIM2, TIM_OC1);
	timer_disable_oc_preload(TIM2, TIM_OC1);
	timer_set_oc_slow_mode(TIM2, TIM_OC1);
	timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_FROZEN);
	/* Set the capture compare value for OC1. */
	timer_set_oc_value(TIM2, TIM_OC1, 1000);
	/* ---- */
	/* ARR reload enable. */
	timer_disable_preload(TIM2);
	/* Counter enable. */
	timer_enable_counter(TIM2);
	/* Enable commutation interrupt. */
	timer_enable_irq(TIM2, TIM_DIER_CC1IE);
}


int _write(int file, char *ptr, int len) {
	int i;
	if (file == 1) {
		for (i = 0; i < len; i++)
		usart_send_blocking(USART1, ptr[i]);
		return i;
	}
	errno = EIO;
	return -1;
}

int main(void) {
	clock_setup();
	gpio_setup();
	usart_setup();
	// tim_setup();

	/* Blink the LED (PC13) on the board. */
	while (1) {
		gpio_toggle(GPIOC, GPIO13);
		for (int i = 0; i < 0x100000; ++i) {
			__asm__("nop");
		}
		printf("UUUUUUUUUUUUUU\n");
	}
	return 0;
}
