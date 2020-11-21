/*
 * PC13		LED
 * PA0		Osci pin
 * PA9		TxD USART 115200
 * PB4		TIM3.CH1 in (5-volt tolerant)
 */

#include <string.h>
#include <stdio.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

#include "usart.h"

/* Use GPIO A0 as debug pin */
#define OSCI_PIN_TOGGLE GPIOA_ODR ^= 1
#define OSCI_PIN_ON     GPIOA_BSRR = 1
#define OSCI_PIN_OFF    GPIOA_BSRR = 1 << 16

static volatile uint32_t counter = 0, cc1if = 0, cc2if = 0, c1count = 0, c2count = 0;

void timer_setup(void);
void gpio_setup(void);

void tim3_isr(void) {
    uint32_t sr = TIM_SR(TIM3);

    OSCI_PIN_ON;
    if (sr & TIM_SR_CC1IF) {
	cc1if = TIM_CCR1(TIM3);
	++c1count;
	timer_clear_flag(TIM3, TIM_SR_CC1IF);
    }
    if (sr & TIM_SR_CC2IF) {
	cc2if = TIM_CCR2(TIM3);
	++c2count;
	timer_clear_flag(TIM3, TIM_SR_CC2IF);
    }
    OSCI_PIN_OFF;
}

static void systick_setup(void) {
    /* 72MHz / 8 => 9000000 counts per second */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    /* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
    /* SysTick interrupt every N clock pulses: set reload to N-1 */
    systick_set_reload(8999);

    systick_interrupt_enable();

    /* Start counting */
    systick_counter_enable();
}

void sys_tick_handler(void) {

    /* We call this handler every 1ms so every 1ms = 0.001s */
    counter++;
    if (counter == 500) {
	counter = 0;
	gpio_toggle(GPIOC, GPIO13);	/* toggle onboard LED */
    }
}

void gpio_setup(void) {

    rcc_periph_clock_enable(RCC_GPIOA);	// Need GPIOA clock
    rcc_periph_clock_enable(RCC_GPIOC);	// Need GPIOC clock
    /* Configure LED&Osci pins */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_clear(GPIOC, GPIO13);	// LED off

}

void timer_setup(void) {

    rcc_periph_clock_enable(RCC_TIM3);	// Need TIM3 clock

    rcc_periph_clock_enable(RCC_AFIO);	// Need AFIO clock
    /* Disable JATG and set PB4 TIM3.CH1 */
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON, AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP);

    // PB4 == TIM3.CH1
    //gpio_set_af(GPIOB, GPIO_TIM3_PR_CH1, GPIO4);

    // TIM3:
    timer_disable_counter(TIM3);
    rcc_periph_reset_pulse(RST_TIM3);
    //nvic_set_priority(NVIC_DMA1_CHANNEL3_IRQ, 2);
    nvic_enable_irq(NVIC_TIM3_IRQ);
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    // set 1us counter
    timer_set_prescaler(TIM3, 72 - 1);
    timer_ic_set_input(TIM3, TIM_IC1, TIM_IC_IN_TI1);
    timer_ic_set_input(TIM3, TIM_IC2, TIM_IC_IN_TI1);
    timer_ic_set_filter(TIM3, TIM_IC_IN_TI1, TIM_IC_CK_INT_N_2);
    timer_ic_set_prescaler(TIM3, TIM_IC1, TIM_IC_PSC_OFF);
    timer_slave_set_mode(TIM3, TIM_SMCR_SMS_RM);
    timer_slave_set_trigger(TIM3, TIM_SMCR_TS_TI1FP1);
    TIM_CCER(TIM3) &= ~(TIM_CCER_CC2P | TIM_CCER_CC2E | TIM_CCER_CC1P | TIM_CCER_CC1E);
    TIM_CCER(TIM3) |= TIM_CCER_CC2P | TIM_CCER_CC2E | TIM_CCER_CC1E;
    timer_ic_enable(TIM3, TIM_IC1);
    timer_ic_enable(TIM3, TIM_IC2);
    timer_enable_irq(TIM3, TIM_DIER_CC1IE | TIM_DIER_CC2IE);
    timer_enable_counter(TIM3);
}

int main(void) {

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    systick_setup();
    gpio_setup();
    usart_setup();
    timer_setup();

    /* endless loop */
    while (1) {
	printf("%lu %lu\r", cc1if, cc2if);
    }
}
