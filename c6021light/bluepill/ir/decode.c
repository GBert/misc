#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>

#define MM_INPUT	GPIO3

#include "can.h"
#include "sniffer.h"
#include "decode.h"

volatile uint32_t old_timestamp = 0, new_timestamp = 0, pulse_duration = 0;

void exti_setup(void) {
    /* Enable GPIOB clock. */
    rcc_periph_clock_enable(RCC_GPIOB);

    /* Enable AFIO clock. */
    rcc_periph_clock_enable(RCC_AFIO);

    /* Enable EXTI3 interrupt. */
    nvic_enable_irq(NVIC_EXTI4_IRQ);

    /* Set MM Input (in GPIO port B) to 'input pull-down'. */
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON, 1);
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO4);
    gpio_set(GPIOB, GPIO4);

    /* Configure the EXTI subsystem. */
    exti_select_source(EXTI4, GPIOB);
    exti_set_trigger(EXTI4, EXTI_TRIGGER_BOTH);
    exti_enable_request(EXTI4);
}

void exti4_isr(void) {
    // gpio_set(GPIOA, GPIO0);
    exti_reset_request(EXTI4);
    new_timestamp = micros();
    pulse_duration = new_timestamp - old_timestamp;
    old_timestamp = new_timestamp;
    // gpio_clear(GPIOA, GPIO0);
}
