/*
 * Linux GPIO Trigger Test based on
 *    http://www.blaess.fr/christophe/2013/03/13/mesure-de-latences-dinterruptions-gpio-rtdm/
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f1/nvic.h>
#include <libopencm3/stm32/f1/timer.h>

/* Trigger Port GPIOC */
#define GPIO_TRIGGER	10
#define GPIO_LED	13

int _write(int file, char *ptr, int len);
static int systick_counter_ms = 0;

volatile int latency = 0;
volatile int latency_max = 0;

void tim1_cc_isr(void) {
    static int last_value = 0;
    int value;
    if (timer_get_flag(TIM1, TIM_SR_CC1IF) != 0) {
	// Timer channel 1 interrupt -> First edge (outcoming signal).
	timer_clear_flag(TIM1, TIM_SR_CC1IF);
	last_value = timer_get_counter(TIM1);
    }
    if (timer_get_flag(TIM1, TIM_SR_CC2IF)) {
	// Timer channel 2 interrupt -> response (incoming signal).
	timer_clear_flag(TIM1, TIM_SR_CC2IF);
	value = timer_get_counter(TIM1) - last_value;
	if (value < 0)
	    value = value + 0xffff;
	latency = value;
	if (latency > latency_max)
	    latency_max = latency;
    }
}

static void systick_setup(void) {
    /* 72 MHz with DIV 8 -> systick clock source of 9MHz */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    /* Systick interrupt period 1ms -> 9MHz / 1kHz = 9000 */
    systick_set_reload(9000 - 1);
    systick_interrupt_enable();
    systick_counter_enable();
}

void sys_tick_handler(void) {
    systick_counter_ms++;
}

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
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO_LED | GPIO_TRIGGER);
    gpio_set(GPIOC, GPIO_LED);
    gpio_clear(GPIOC, GPIO_TRIGGER);
}

static void tim_setup(void) {
    /* Enable TIM1 clock. */
    rcc_periph_clock_enable(RCC_TIM1);
    /* Configure TIM1_CH1 and TIM1_CH2 as inputs */
    gpio_set_mode(GPIO_BANK_TIM1_CH1, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_TIM1_CH1);
    gpio_set_mode(GPIO_BANK_TIM1_CH2, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_TIM1_CH2);
    /* Enable TIM1 interrupt. */
    nvic_enable_irq(NVIC_TIM1_CC_IRQ);
    /* Reset TIM1 peripheral. */
    /* timer_reset(TIM1); */

    timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    /* set prescaler value -> 1us */
    timer_set_prescaler(TIM1, 72 - 1);
    timer_set_period(TIM1, 0xFFFF);
    timer_set_repetition_counter(TIM1, 0);
    /* Enable preload */
    /* timer_disable_preload(TIM1); */
    /* Continous mode */
    timer_continuous_mode(TIM1);

    /* configure Channel 1 */
    timer_ic_set_input(TIM1, TIM_IC1, TIM_IC_IN_TI1);
    timer_ic_set_filter(TIM1, TIM_IC1, TIM_IC_OFF);
    timer_ic_set_polarity(TIM1, TIM_IC1, TIM_IC_RISING);
    timer_ic_set_prescaler(TIM1, TIM_IC1, TIM_IC_PSC_OFF);
    timer_ic_enable(TIM1, TIM_IC1);
    timer_clear_flag(TIM1, TIM_SR_CC1IF);
    timer_enable_irq(TIM1, TIM_DIER_CC1IE);

    /* configure Channel 2 */
    timer_ic_set_input(TIM1, TIM_IC2, TIM_IC_IN_TI2);
    timer_ic_set_filter(TIM1, TIM_IC2, TIM_IC_OFF);
    timer_ic_set_polarity(TIM1, TIM_IC2, TIM_IC_RISING);
    timer_ic_set_prescaler(TIM1, TIM_IC2, TIM_IC_PSC_OFF);
    timer_ic_enable(TIM1, TIM_IC2);
    timer_clear_flag(TIM1, TIM_SR_CC2IF);
    timer_enable_irq(TIM1, TIM_DIER_CC2IE);

    timer_enable_counter(TIM1);

#if 0
    /* Disable outputs. */
    timer_disable_oc_output(TIM1, TIM_OC1);
    timer_disable_oc_output(TIM1, TIM_OC2);
    timer_disable_oc_output(TIM1, TIM_OC3);
    timer_disable_oc_output(TIM1, TIM_OC4);
    /* -- OC1 configuration -- */
    /* Configure global mode of line 1. */
    timer_disable_oc_clear(TIM1, TIM_OC1);
    timer_disable_oc_preload(TIM1, TIM_OC1);
    timer_set_oc_slow_mode(TIM1, TIM_OC1);
    timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FROZEN);
    /* Set the capture compare value for OC1. */
    timer_set_oc_value(TIM1, TIM_OC1, 1000);
    /* ---- */
    /* ARR reload enable. */
    timer_disable_preload(TIM1);
    /* Counter enable. */
    timer_enable_counter(TIM1);
    /* Enable commutation interrupt. */
    timer_enable_irq(TIM1, TIM_DIER_CC1IE);
#endif
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
    systick_setup();
    tim_setup();

    /* wait 3 seconds to allow Linux detecting /dev/ttyUSB */
    while (systick_counter_ms < 3000) ;

    /* output signal is on GPIO C 10 (EXT1-8) */
    /* gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO_TRIGGER); */

    while (1) {
	/* If the input signal is high */
	if (gpio_get(GPIO_BANK_TIM1_CH2, GPIO_TIM1_CH2) != 0) {
	    /* Output a raising edge */
	    gpio_set(GPIOC, GPIO_TRIGGER);
	    /* Wait for the input signal going down, max 2 ms */
	    for (systick_counter_ms = 0; systick_counter_ms < 2;)
		if (gpio_get(GPIO_BANK_TIM1_CH2, GPIO_TIM1_CH2) == 0)
		    break;
	    /* Output a falling edge */
	    gpio_clear(GPIOC, GPIO_TRIGGER);
	} else {
	    latency = 0;
	    /* Output a raising edge (trigger signal) */
	    gpio_set(GPIOC, GPIO_TRIGGER);
	    /* Wait for the latency to be available */
	    for (systick_counter_ms = 0; systick_counter_ms < 2;)
		if (latency != 0)
		    break;
	    /* Output a falling edge */
	    gpio_clear(GPIOC, GPIO_TRIGGER);
	    if (latency != 0)
		printf("%3d - %3d\n", latency, latency_max);
	}

	/* Wait 1 ms before next trigger */
	for (systick_counter_ms = 0; systick_counter_ms < 1;) ;
    }
}
