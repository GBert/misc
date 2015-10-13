/******************************************************************************
 * @file time.c
 *
 * @brief This module handles any time specifics.
 *
 * @details A time with a resolution of 1 us is provided by using the SysTick
 *          timer.
 *
 * @author Alex
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "time.h"
#include "interrupt.h"
#include "communication.h"

/******************************************************************************
 * Defines and Macros
 *****************************************************************************/

/** SysTick timer interrupt (overflow) frequency in Hz */
#define SYSTICK_INTERRUPT_FREQUENCY             8

/** Setup SysTick Timer (24 bit) for 0.125 s interrupts (4 Hz blink signal)
 *  72 MHz / DIV = 8 Hz ==> DIV = 9000000 */
#define SYSTICK_RELOAD_VALUE                    (SystemFrequency / SYSTICK_INTERRUPT_FREQUENCY)

/** Bit mask for COUNTLFAG bit in SysTick Control and Status Register */
#define SYSTICK_COUNTFLAG_BIT_MASK              0x00010000UL

/******************************************************************************
 * Global variables
 *****************************************************************************/

/** Upper 32 bits of board time, the lower 32 bits are directly derived from
 *  SysTick timer (incremented each 1/8 s) */
static uint32_t g_SysTickOverflowCntr;

/******************************************************************************
 * Function definitions
 *****************************************************************************/

/******************************************************************************
 * @brief Initializes heart-beat LED and system clock.
 *****************************************************************************/
void Time_initialize(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Setup peripheral clock for GPIO_C (LED port) */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure LED Pin as output push-pull (used for heart-beat signal) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Setup SysTick Timer (24 bit) for 0.125 s interrupts (4 Hz blink signal)
     * 72 MHz / DIV = 8 Hz ==> DIV = 9000000 */
    SysTick_Config(SYSTICK_RELOAD_VALUE);

    g_SysTickOverflowCntr = 0;
}

/******************************************************************************
 * @brief Calculates board time in microseconds (starting with 0 after reset).
 *****************************************************************************/
uint64_t Time_getUs(void)
{
    const uint32_t ticksPerUs = SystemFrequency / 1000000UL;
    const uint64_t overflowDurationUs = 1000000UL / SYSTICK_INTERRUPT_FREQUENCY;
    InterruptStatus_t status;
    uint32_t ticks;
    uint32_t timeUs;
    uint32_t overflowCntr;

    /* Begin of critical section */
    status = Interrupt_saveAndDisable();

    overflowCntr = g_SysTickOverflowCntr;
    ticks = SysTick->VAL;

    /* A timer overflow between reading g_BoardTime and SysTick->VAL invalidates
     * the result. In this case an incremented g_BoardTime is used and
     * SysTick->VAL is read again to make sure to have a valid time. */
    if (0 != (SysTick->CTRL & SYSTICK_COUNTFLAG_BIT_MASK)) {
	overflowCntr++;
	ticks = SysTick->VAL;
    }

    /* End of critical section */
    Interrupt_restore(status);

    /* The timer counts from reload value downwards, this makes it count upwards */
    ticks = SYSTICK_RELOAD_VALUE - ticks;

    /* 1 SysTick (24 bit) equals 1 / 72 MHz
     * We want the final time to be in microseconds */
    timeUs = ticks / ticksPerUs;

    /* Now put ISR overflow counter (1 bit == 0.125 seconds) and time
     * (1 bit == 1 us) together */
    return ((uint64_t) timeUs + ((uint64_t) overflowCntr * overflowDurationUs));
}

/******************************************************************************
 * @brief Resets the board time to 0.
 *****************************************************************************/
void Time_reset(void)
{
    InterruptStatus_t status;

    /* Begin of critical section */
    status = Interrupt_saveAndDisable();

    SysTick->VAL = 0;
    g_SysTickOverflowCntr = 0;

    /* End of critical section */
    Interrupt_restore(status);
}

/******************************************************************************
 * @brief Interrupt service routine for SysTick timer.
 *****************************************************************************/
void SysTick_Handler(void)
{
    const uint32_t maxOverflows = 0xFFFFFFFFUL;
    static uint8_t State = 0;

    /* Clear counter reached zero flag */
    SysTick->CTRL &= ~SYSTICK_COUNTFLAG_BIT_MASK;

    /* Toggle LED (heart-beat) */
    if (1 == State) {
	/* Turn on LD1 */
	GPIO_SetBits(GPIOC, GPIO_Pin_12);
    } else {
	/* Turn off LD1 */
	GPIO_ResetBits(GPIOC, GPIO_Pin_12);
    }

    if (maxOverflows == g_SysTickOverflowCntr) {
	Communication_transmitStatusMessage(StatusMessage_Warning, "HW: Timestamp overflow (305.4 h elapsed)");
    }

    /* Toggle state and increment time */
    State ^= 1;
    g_SysTickOverflowCntr++;
}
