/****************************************************************************\
 * Mesure de temps de reponse a un signal                                   *
 *                                                                          *
 * Christophe Blaess 2013.                                                  *
\****************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f1/rcc.h>   // Reset and Clock Control.
#include <libopencm3/stm32/f1/gpio.h>

// ---------------------------------------------------------------------------
// 
static void clock_setup(void) {
	// vcc-gnd uses an external quartz at 8MHz
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	// Enable clock for GPIO ports A & C.
	rcc_peripheral_enable_clock(& RCC_APB2ENR, RCC_APB2ENR_IOPAEN);
	rcc_peripheral_enable_clock(& RCC_APB2ENR, RCC_APB2ENR_IOPCEN);

}

// ---------------------------------------------------------------------------
// 
static void usart_setup(void) {
	rcc_peripheral_enable_clock(& RCC_APB1ENR, RCC_APB1ENR_USART2EN);
	rcc_peripheral_enable_clock(& RCC_APB2ENR, RCC_APB2ENR_IOPAEN);
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART2_TX);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART2_RX);
	usart_set_baudrate (USART2, 115200);
	usart_set_databits (USART2, 8);
	usart_set_stopbits (USART2, USART_STOPBITS_1);
	usart_set_parity   (USART2, USART_PARITY_NONE);
	usart_set_flow_control (USART2, USART_FLOWCONTROL_NONE);
	usart_set_mode (USART2, USART_MODE_TX_RX);
	usart_enable (USART2);
}

static int usart_printf(const char * format, ...)
{
	va_list args;
	int r;
	int i;
	char buffer[256];

	if (format == NULL) 
		return 0;

	va_start(args, format);
	r = vsnprintf(buffer, 256, format, args);
	va_end(args);
	if (r <= 0)
		return r;

	for (i = 0; buffer[i] != '\0'; i ++)
		usart_send_blocking(USART2, buffer[i]);
	return i;
}

// ---------------------------------------------------------------------------
//

static int systick_counter_ms = 0;

static void systick_setup(void) {
	// 72 MHz with DIV 8 -> systick clock source of 9MHz
	systick_set_clocksource(STK_CTRL_CLKSOURCE_AHB_DIV8);

	// Systick interrupt period 1ms -> 9MHz / 1kHz = 9000
	systick_set_reload(9000 - 1);
	systick_interrupt_enable();
	systick_counter_enable();
}

static void sys_tick_handler(void) {
	systick_counter_ms ++;
}

// ---------------------------------------------------------------------------
//
static void tim1_setup_input_capture(void) {
	// Enable clock for Timer 1.
	rcc_peripheral_enable_clock(& RCC_APB2ENR,
	                            RCC_APB2ENR_TIM1EN);

	// Configure TIM1_CH1 and TIM1_CH2 as inputs.
	gpio_set_mode (GPIO_BANK_TIM1_CH1, GPIO_MODE_INPUT,
	               GPIO_CNF_INPUT_PULL_UPDOWN,
	               GPIO_TIM1_CH1);

	gpio_set_mode (GPIO_BANK_TIM1_CH2, GPIO_MODE_INPUT,
	               GPIO_CNF_INPUT_PULL_UPDOWN,
	               GPIO_TIM1_CH2);

	// Enable interrupts for TIM1 CC.	
	nvic_enable_irq        (NVIC_TIM1_CC_IRQ);

	timer_set_mode(TIM1,
	               TIM_CR1_CKD_CK_INT, // Internal 72 MHz clock
	               TIM_CR1_CMS_EDGE,   // Edge synchronization
	               TIM_CR1_DIR_UP);    // Upward counter

	timer_set_prescaler(TIM1, 72-1);  // Counter unit = 1 us.
	timer_set_period(TIM1, 0xFFFF);
	timer_set_repetition_counter(TIM1, 0);
	timer_continuous_mode(TIM1);

	// Configure channel 1
	timer_ic_set_input     (TIM1, TIM_IC1, TIM_IC_IN_TI1);
	timer_ic_set_filter    (TIM1, TIM_IC1, TIM_IC_OFF);
	timer_ic_set_polarity  (TIM1, TIM_IC1, TIM_IC_RISING);
	timer_ic_set_prescaler (TIM1, TIM_IC1, TIM_IC_PSC_OFF);
	timer_ic_enable        (TIM1, TIM_IC1);
	timer_clear_flag       (TIM1, TIM_SR_CC1IF);
	timer_enable_irq       (TIM1, TIM_DIER_CC1IE);

	// Configure channel 2
	timer_ic_set_input     (TIM1, TIM_IC2, TIM_IC_IN_TI2);
	timer_ic_set_filter    (TIM1, TIM_IC2, TIM_IC_OFF);
	timer_ic_set_polarity  (TIM1, TIM_IC2, TIM_IC_RISING);
	timer_ic_set_prescaler (TIM1, TIM_IC2, TIM_IC_PSC_OFF);
	timer_ic_enable        (TIM1, TIM_IC2);
	timer_clear_flag       (TIM1, TIM_SR_CC2IF);
	timer_enable_irq       (TIM1, TIM_DIER_CC2IE);

	timer_enable_counter   (TIM1);

}

volatile int latency = 0;
volatile int latency_max = 0;

static void tim1_cc_isr(void) {
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

// ---------------------------------------------------------------------------
//
int main(void)
{
	clock_setup();
	usart_setup();
	systick_setup();
	tim1_setup_input_capture();

	// Wait 3 seconds to allow Linux detecting /dev/ttyUSB
	while (systick_counter_ms < 3000)
		;

	// Output signal is on GPIO C 10 (EXT1-8)
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
	              GPIO_CNF_OUTPUT_PUSHPULL, GPIO10);

	while (1) {
		// If the input signal is high.
		if (gpio_get(GPIO_BANK_TIM1_CH2, GPIO_TIM1_CH2) != 0) {
			// Output a raising edge.
			gpio_set(GPIOC, GPIO10);
			// Wait for the input signal going down, max 2 ms.
			for (systick_counter_ms = 0; systick_counter_ms < 2; )
				if (gpio_get(GPIO_BANK_TIM1_CH2, GPIO_TIM1_CH2) == 0)
					break;
			// Output a falling edge.
			gpio_clear(GPIOC, GPIO10);
		} else {
			latency = 0;
			// Output a raising edge (trigger signal).
			gpio_set(GPIOC, GPIO10);
			// Wait for the latency to be available.
			for (systick_counter_ms = 0; systick_counter_ms < 2; )
				if (latency != 0)
					break;
			// Output a falling edge.
			gpio_clear(GPIOC, GPIO10);
			if (latency != 0)
				usart_printf("%3d - %3d\n", latency, latency_max);
		}

		// Wait 1 ms before next trigger.
		for (systick_counter_ms = 0; systick_counter_ms < 1; )
			;
	}
	return 0;
}
