/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "analyzer.h"
#include "sniffer.h"
#include <libopencm3/stm32/timer.h>

static volatile uint32_t cc1if = 0, cc2if = 0;
extern volatile uint32_t milliseconds;

void timer_setup(void);
void gpio_setup(void);

void tim3_isr(void) {
    uint32_t sr = TIM_SR(TIM3);

    if (sr & TIM_SR_CC1IF) {
	cc1if = TIM_CCR1(TIM3) - cc2if;
	analyzer(milliseconds, cc1if);
    }
    if (sr & TIM_SR_CC2IF) {
	cc2if = TIM_CCR2(TIM3);
	analyzer(milliseconds, cc2if);
    }
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
