// bcm2835.h
//
// C and C++ support for Broadcom BCM 2835 as used in Raspberry Pi
//
// Author: Mike McCauley
// Copyright (C) 2011-2013 Mike McCauley
// $Id: bcm2835.h,v 1.14 2014/08/21 01:26:42 mikem Exp mikem $
//

// Defines for A20
#ifndef A20HW_H
#define A20HW_H

#include <stdint.h>

// pin states
#define LOW  0
#define HIGH 1

// pin numbers
#define PORTAPIN    0
#define PORTBPIN    32
#define PORTCPIN    64
#define PORTDPIN    96
#define PORTEPIN    128
#define PORTFPIN    160
#define PORTGPIN    192
#define PORTHPIN    224
#define PORTIPIN    256




#if 0
// Physical addresses for various peripheral register sets
/// Base Physical Address of the BCM 2835 peripheral registers
#define A20_PERI_BASE               0x20000000
/// Base Physical Address of the System Timer registers
#define A20_ST_BASE			(A20_PERI_BASE + 0x3000)
/// Base Physical Address of the Pads registers
#define A20_GPIO_PADS               (A20_PERI_BASE + 0x100000)
/// Base Physical Address of the Clock/timer registers
#define A20_CLOCK_BASE              (A20_PERI_BASE + 0x101000)
/// Base Physical Address of the GPIO registers
#define A20_GPIO_BASE               (A20_PERI_BASE + 0x200000)
/// Base Physical Address of the SPI0 registers
#define A20_SPI0_BASE               (A20_PERI_BASE + 0x204000)
/// Base Physical Address of the BSC0 registers
#define A20_BSC0_BASE 		(A20_PERI_BASE + 0x205000)
/// Base Physical Address of the PWM registers
#define A20_GPIO_PWM                (A20_PERI_BASE + 0x20C000)
 /// Base Physical Address of the BSC1 registers
#define A20_BSC1_BASE		(A20_PERI_BASE + 0x804000)

/// Base of the GPIO registers.
/// Available after a20_init has been called
extern volatile uint32_t *sunxi_gpio;

/// Base of the PWM registers.
/// Available after a20_init has been called
extern volatile uint32_t *sunxi_pwm;

/// Base of the SPI0 registers.
/// Available after a20_init has been called
extern volatile uint32_t *sunxi_spi0;

/// Base of the iic2 registers.
/// Available after a20_init has been called
extern volatile uint32_t *sunxi_iic2;

/// \brief a20RegisterBase
/// Register bases for a20_regbase()
typedef enum
{
    A20_REGBASE_ST   = 1, ///< Base of the ST (System Timer) registers.
    A20_REGBASE_GPIO = 2, ///< Base of the GPIO registers.
    A20_REGBASE_PWM  = 3, ///< Base of the PWM registers.
    A20_REGBASE_CLK  = 4, ///< Base of the CLK registers.
    A20_REGBASE_PADS = 5, ///< Base of the PADS registers.
    A20_REGBASE_SPI0 = 6, ///< Base of the SPI0 registers.
    A20_REGBASE_BSC0 = 7, ///< Base of the BSC0 registers.
    A20_REGBASE_BSC1 = 8, ///< Base of the BSC1 registers.
    A20_REGBASE_IIC2  = 9 /*Add for BananaPro by LeMaker Team*/
} a20RegisterBase;

/// Size of memory page on RPi
#define A20_PAGE_SIZE               (4*1024)
/// Size of memory block on RPi
#define A20_BLOCK_SIZE              (4*1024)

#endif

/// \brief a20PortFunction
/// Port function select modes for a20_gpio_fsel()
typedef enum
{
    A20_GPIO_FSEL_INPT  = 0b000,   ///< Input
    A20_GPIO_FSEL_OUTP  = 0b001,   ///< Output
    A20_GPIO_FSEL_ALT0  = 0b100,   ///< Alternate function 0
    A20_GPIO_FSEL_ALT1  = 0b101,   ///< Alternate function 1
    A20_GPIO_FSEL_ALT2  = 0b110,   ///< Alternate function 2
    A20_GPIO_FSEL_ALT3  = 0b111,   ///< Alternate function 3
    A20_GPIO_FSEL_ALT4  = 0b011,   ///< Alternate function 4
    A20_GPIO_FSEL_ALT5  = 0b010,   ///< Alternate function 5
    A20_GPIO_FSEL_MASK  = 0b111    ///< Function select bits mask
} a20FunctionSelect;

/// \brief a20PUDControl
/// Pullup/Pulldown defines for a20_gpio_pud()
typedef enum
{
    A20_GPIO_PUD_OFF   = 0b00,  // Off = disable pull-up/down
    A20_GPIO_PUD_DOWN  = 0b10,  // Enable Pull Down control 
    A20_GPIO_PUD_UP    = 0b01,  // Enable Pull Up control 
    A20_GPIO_PUD_MASK  = 0b11  
} a20PUDControl;

#if 0
/*Add for BananaPro by LeMaker Team*/
#define SUNXI_PERI_BASE         0x01C20000
/// Base Physical Address of the System Timer registers
#define SUNXI_ST_BASE			(SUNXI_PERI_BASE + 0x3000)
/// Base Physical Address of the Pads registers
#define SUNXI_GPIO_PADS         (SUNXI_PERI_BASE + 0x800)
/// Base Physical Address of the Clock/timer registers
#define SUNXI_CLOCK_BASE        (SUNXI_PERI_BASE + 0x101000)
/// Base Physical Address of the GPIO registers
#define SUNXI_GPIO_BASE         (SUNXI_PERI_BASE + 0x800)
/// Offset Address of the GPIO CFG registers
#define SUNXI_GPIO_CFG_OFFSET	(0x00)
/// Offset Address of the GPIO DAT registers
#define SUNXI_GPIO_DAT_OFFSET	(0x10)
/// Offset Address of the GPIO PUL registers
#define SUNXI_GPIO_PUL_OFFSET	(0x1C)
/// Base Physical Address of the SPI0 registers
#define SUNXI_SPI0_BASE         (0x01C05000)
/// Base Physical Address of the BSC0 registers
#define SUNXI_BSC0_BASE 		(SUNXI_PERI_BASE + 0x205000)
/// Base Physical Address of the PWM registers
#define SUNXI_GPIO_PWM          (SUNXI_PERI_BASE + 0xC00)
/// Offset Address of the PWM CFG registers
#define SUNXI_PWM_CTRL_OFFSET	 (0x200)
/// Offset Address of the PWM CH0 registers
#define SUNXI_PWM_CH0_OFFSET	 (0x204)
/// Offset Address of the PWM CH1 registers
#define SUNXI_PWM_CH1_OFFSET	 (0x208)
 /// Base Physical Address of the BSC1 registers
#define SUNXI_BSC1_BASE		(SUNXI_PERI_BASE + 0x804000)
#define SUNXI_IIC2_BASE			(SUNXI_PERI_BASE + 0xB400)
#define I2C2_CLK_OFFSET			 0x14
#define SUNXI_TIMER_BASE 		(0x01C20C00)
#define MAP_SIZE	(4096)
#define MAP_MASK	(MAP_SIZE - 1)

#define SUNXI_PWM_CLKDIV_MASK (0xF<<15)

#define SUNXI_SPI0_CLK                     0x1C ///< SPI Master Clock Divider
#define SUNXI_SPI0_RXDATA		      0x00//RX data reg
#define SUNXI_SPI0_TXDATA		      0x04//TX data reg
#define SUNXI_SPI0_CTRL                    0x08//control reg 
#define SUNXI_SPI0_INT_CTL		      0x0C//int control reg
#define SUNXI_SPI0_STAT                    0x10//status reg 
#define SUNXI_SPI0_DMA_CTRL                   0x14//DMA control reg 
#define SUNXI_SPI0_BC                   0x20//burst counter reg 
#define SUNXI_SPI0_TC                   0x24//transmit counter reg 
#define SUNXI_SPI0_FIFO_STA        (0x28)

#define SUNXI_SPI0_CS_CS                   0x3000 ///< Chip Select
#define SUNXI_SPIO_CS_CTRL                0x10000
#define SUNXI_SPIO_CS_POL                  0x10
#define SUNXI_SPI0_MASTER_MASK                   0x2 ///master or slave
#define SUNXI_SPI0_MSBFIRST_MASK                   0x40 ///MSB
#define SUNXI_SPI0_STAT_MASK       (0x7f|(0x7f<<8)|(0x3<<16))
#define SUNXI_SPI0_DRQEN_MASK      (0x1f|(0x1f<<8))
#define SUNXI_SPI0_FIFORST_MASK      (3<<8)
#define SUNXI_SPI0_CTL_XCH_MASK         (0x1 << 10) 

#define SUNXI_SPI0_INTEN_RO        (0x1 << 5)  //rxFIFO Overflow Interrupt Enable ---used for error detect 
#define SUNXI_SPI0_INTEN_RU        (0x1 << 6)  // rxFIFO Underrun Interrupt Enable ---used for error detect 

#define SUNXI_SPI0_INTEN_TO        (0x1 << 13) // txFIFO Overflow Interrupt Enable
#define SUNXI_SPI0_INTEN_TC        (0x1 << 16) // Transfer Completed Interrupt Enable
#define SUNXI_SPI0_INTEN_ERR       (SUNXI_SPI0_INTEN_TO|SUNXI_SPI0_INTEN_RU|SUNXI_SPI0_INTEN_RO) //NO txFIFO underrun
#define SUNXI_SPI0_INTEN_MASK      (0x7f|(0x7f<<8)|(0x3<<16))
#define SUNXI_SPI0_FIFO_TXCNT      (0x7F << 16)// txFIFO counter
#define SUNXI_SPI0_TXCNT_BIT_POS   (16)
#define SUNXI_SPI0_FIFO_RXCNT      (0x7F << 0)     // rxFIFO counter

#endif

#ifdef __cplusplus
extern "C" {
#endif

    /// Initialise the library by opening /dev/mem and getting pointers to the 
    /// internal memory for BCM 2835 device registers. You must call this (successfully)
    /// before calling any other 
    /// functions in this library (except a20_set_debug). 
    /// If a20_init() fails by returning 0, 
    /// calling any other function may result in crashes or other failures.
    /// Prints messages to stderr in case of errors.
    /// \return 1 if successful else 0
    extern int a20_init(void);

    /// Close the library, deallocating any allocated memory and closing /dev/mem
    /// \return 1 if successful else 0
    extern int a20_close(void);

    /// Gets the base of a register
    /// \param[in] regbase You can use one of the common values A20_REGBASE_*
    /// in \ref a20RegisterBase
    /// \return the register base
    /// \sa Physical Addresses
    extern uint32_t* a20_regbase(uint8_t regbase);

    /// Reads 32 bit value from a peripheral address
    /// The read is done twice, and is therefore always safe in terms of 
    /// manual section 1.3 Peripheral access precautions for correct memory ordering
    /// \param[in] paddr Physical address to read from. See A20_GPIO_BASE etc.
    /// \return the value read from the 32 bit register
    /// \sa Physical Addresses
    extern uint32_t a20_peri_read(volatile uint32_t* paddr);


    /// Reads 32 bit value from a peripheral address without the read barrier
    /// You should only use this when your code has previously called a20_peri_read()
    /// within the same peripheral, and no other peripheral access has occurred since.
    /// \param[in] paddr Physical address to read from. See A20_GPIO_BASE etc.
    /// \return the value read from the 32 bit register
    /// \sa Physical Addresses
    extern uint32_t a20_peri_read_nb(volatile uint32_t* paddr);


    /// Writes 32 bit value from a peripheral address
    /// The write is done twice, and is therefore always safe in terms of 
    /// manual section 1.3 Peripheral access precautions for correct memory ordering
    /// \param[in] paddr Physical address to read from. See A20_GPIO_BASE etc.
    /// \param[in] value The 32 bit value to write
    /// \sa Physical Addresses
    extern void a20_peri_write(volatile uint32_t* paddr, uint32_t value);

    /// Writes 32 bit value from a peripheral address without the write barrier
    /// You should only use this when your code has previously called a20_peri_write()
    /// within the same peripheral, and no other peripheral access has occurred since.
    /// \param[in] paddr Physical address to read from. See A20_GPIO_BASE etc.
    /// \param[in] value The 32 bit value to write
    /// \sa Physical Addresses
    extern void a20_peri_write_nb(volatile uint32_t* paddr, uint32_t value);

    /// Alters a number of bits in a 32 peripheral regsiter.
    /// It reads the current valu and then alters the bits deines as 1 in mask, 
    /// according to the bit value in value. 
    /// All other bits that are 0 in the mask are unaffected.
    /// Use this to alter a subset of the bits in a register.
    /// The write is done twice, and is therefore always safe in terms of 
    /// manual section 1.3 Peripheral access precautions for correct memory ordering
    /// \param[in] paddr Physical address to read from. See A20_GPIO_BASE etc.
    /// \param[in] value The 32 bit value to write, masked in by mask.
    /// \param[in] mask Bitmask that defines the bits that will be altered in the register.
    /// \sa Physical Addresses
    extern void a20_peri_set_bits(volatile uint32_t* paddr, uint32_t value, uint32_t mask);

    /// Sets the Function Select register for the given pin, which configures
    /// the pin as Input, Output or one of the 6 alternate functions.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \param[in] mode Mode to set the pin to, one of A20_GPIO_FSEL_* from \ref a20FunctionSelect
    extern void a20_gpio_set_fsel(uint16_t pin, uint8_t mode);

    // Reads the Function Select register for the given pin, which configures
    // the pin as Input, Output or one of the 6 alternate functions.
    extern uint8_t a20_gpio_fsel(uint16_t pin);

    /// Sets the specified pin output to 
    /// HIGH.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \sa a20_gpio_write()
    extern void a20_gpio_set(uint16_t pin);

    /// Sets the specified pin output to 
    /// LOW.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \sa a20_gpio_write()
    extern void a20_gpio_clr(uint16_t pin);

    /// Sets any of the first 32 GPIO output pins specified in the mask to 
    /// HIGH.
    /// \param[in] mask Mask of pins to affect. Use eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    /// \sa a20_gpio_write_multi()
    extern void a20_gpio_set_multi(uint32_t mask);

    /// Sets any of the first 32 GPIO output pins specified in the mask to 
    /// LOW.
    /// \param[in] mask Mask of pins to affect. Use eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    /// \sa a20_gpio_write_multi()
    extern void a20_gpio_clr_multi(uint32_t mask);

    /// Reads the current level on the specified 
    /// pin and returns either HIGH or LOW. Works whether or not the pin
    /// is an input or an output.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \return the current level  either HIGH or LOW
    extern uint8_t a20_gpio_lev(uint16_t pin);

    /// Event Detect Status.
    /// Tests whether the specified pin has detected a level or edge
    /// as requested by a20_gpio_ren(), a20_gpio_fen(), a20_gpio_hen(), 
    /// a20_gpio_len(), a20_gpio_aren(), a20_gpio_afen().
    /// Clear the flag for a given pin by calling a20_gpio_set_eds(pin);
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \return HIGH if the event detect status for the given pin is true.
    extern uint8_t a20_gpio_eds(uint16_t pin);

    /// Sets the Event Detect Status register for a given pin to 1, 
    /// which has the effect of clearing the flag. Use this afer seeing
    /// an Event Detect Status on the pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_set_eds(uint16_t pin);

    /// Enable Rising Edge Detect Enable for the specified pin.
    /// When a rising edge is detected, sets the appropriate pin in Event Detect Status.
    /// The GPRENn registers use
    /// synchronous edge detection. This means the input signal is sampled using the
    /// system clock and then it is looking for a ?011? pattern on the sampled signal. This
    /// has the effect of suppressing glitches.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_ren(uint16_t pin);

    /// Disable Rising Edge Detect Enable for the specified pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_clr_ren(uint16_t pin);

    /// Enable Falling Edge Detect Enable for the specified pin.
    /// When a falling edge is detected, sets the appropriate pin in Event Detect Status.
    /// The GPRENn registers use
    /// synchronous edge detection. This means the input signal is sampled using the
    /// system clock and then it is looking for a ?100? pattern on the sampled signal. This
    /// has the effect of suppressing glitches.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_fen(uint16_t pin);

    /// Disable Falling Edge Detect Enable for the specified pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_clr_fen(uint16_t pin);

    /// Enable High Detect Enable for the specified pin.
    /// When a HIGH level is detected on the pin, sets the appropriate pin in Event Detect Status.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_hen(uint16_t pin);

    /// Disable High Detect Enable for the specified pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_clr_hen(uint16_t pin);

    /// Enable Low Detect Enable for the specified pin.
    /// When a LOW level is detected on the pin, sets the appropriate pin in Event Detect Status.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_len(uint16_t pin);

    /// Disable Low Detect Enable for the specified pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_clr_len(uint16_t pin);

    /// Enable Asynchronous Rising Edge Detect Enable for the specified pin.
    /// When a rising edge is detected, sets the appropriate pin in Event Detect Status.
    /// Asynchronous means the incoming signal is not sampled by the system clock. As such
    /// rising edges of very short duration can be detected.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_aren(uint16_t pin);

    /// Disable Asynchronous Rising Edge Detect Enable for the specified pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_clr_aren(uint16_t pin);

    /// Enable Asynchronous Falling Edge Detect Enable for the specified pin.
    /// When a falling edge is detected, sets the appropriate pin in Event Detect Status.
    /// Asynchronous means the incoming signal is not sampled by the system clock. As such
    /// falling edges of very short duration can be detected.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_afen(uint16_t pin);

    /// Disable Asynchronous Falling Edge Detect Enable for the specified pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void a20_gpio_clr_afen(uint16_t pin);

    /// Sets the Pull-up/down register for the given pin. This is
    /// used with a20_gpio_pudclk() to set the  Pull-up/down resistor for the given pin.
    /// However, it is usually more convenient to use a20_gpio_set_pud().
    /// \param[in] pud The desired Pull-up/down mode. One of A20_GPIO_PUD_* from a20PUDControl
    /// \sa a20_gpio_set_pud()
    extern void a20_gpio_set_pud(uint16_t pin, uint8_t pud);

    // Reads the Pull-up/down mode for the specified pin. 
    extern uint8_t a20_gpio_pud(uint16_t pin);

    /// Clocks the Pull-up/down value set earlier by a20_gpio_pud() into the pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \param[in] on HIGH to clock the value from a20_gpio_pud() into the pin. 
    /// LOW to remove the clock. 
    /// \sa a20_gpio_set_pud()
    extern void a20_gpio_pudclk(uint16_t pin, uint8_t on);

    /// Reads and returns the Pad Control for the given GPIO group.
    /// \param[in] group The GPIO pad group number, one of A20_PAD_GROUP_GPIO_*
    /// \return Mask of bits from A20_PAD_* from \ref a20PadGroup
    extern uint8_t a20_gpio_pad(uint16_t pin);

    /// Sets the Pad Control for the given GPIO group.
    /// \param[in] group The GPIO pad group number, one of A20_PAD_GROUP_GPIO_*
    /// \param[in] control Mask of bits from A20_PAD_* from \ref a20PadGroup. Note 
    /// that it is not necessary to include A20_PAD_PASSWRD in the mask as this
    /// is automatically included.
    extern void a20_gpio_set_pada20_gpio_set_pad(uint16_t pin, uint8_t control);

    /// Delays for the specified number of milliseconds.
    /// Uses nanosleep(), and therefore does not use CPU until the time is up.
    /// However, you are at the mercy of nanosleep(). From the manual for nanosleep():
    /// If the interval specified in req is not an exact multiple of the granularity  
    /// underlying  clock  (see  time(7)),  then the interval will be
    /// rounded up to the next multiple. Furthermore, after the sleep completes, 
    /// there may still be a delay before the CPU becomes free to once
    /// again execute the calling thread.
    /// \param[in] millis Delay in milliseconds
    extern void a20_delay (unsigned int millis);

    /// Delays for the specified number of microseconds.
    /// Uses a combination of nanosleep() and a busy wait loop on the A20 system timers,
    /// However, you are at the mercy of nanosleep(). From the manual for nanosleep():
    /// If the interval specified in req is not an exact multiple of the granularity  
    /// underlying  clock  (see  time(7)),  then the interval will be
    /// rounded up to the next multiple. Furthermore, after the sleep completes, 
    /// there may still be a delay before the CPU becomes free to once
    /// again execute the calling thread.
    /// For times less than about 450 microseconds, uses a busy wait on the System Timer.
    /// It is reported that a delay of 0 microseconds on RaspberryPi will in fact
    /// result in a delay of about 80 microseconds. Your mileage may vary.
    /// \param[in] micros Delay in microseconds
    extern void a20_delayMicroseconds (uint64_t micros);

    /// Sets the output state of the specified pin
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \param[in] on HIGH sets the output to HIGH and LOW to LOW.
    extern void a20_gpio_write(uint16_t pin, uint8_t on);

    /// Sets any of the first 32 GPIO output pins specified in the mask to the state given by on
    /// \param[in] mask Mask of pins to affect. Use eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    /// \param[in] on HIGH sets the output to HIGH and LOW to LOW.
    extern void a20_gpio_write_multi(uint32_t mask, uint8_t on);

    /// Sets the first 32 GPIO output pins specified in the mask to the value given by value
    /// \param[in] value values required for each bit masked in by mask, eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    /// \param[in] mask Mask of pins to affect. Use eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    extern void a20_gpio_write_mask(uint32_t value, uint32_t mask);

    /// Sets the Pull-up/down mode for the specified pin. This is more convenient than
    /// clocking the mode in with a20_gpio_pud() and a20_gpio_pudclk().
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \param[in] pud The desired Pull-up/down mode. One of A20_GPIO_PUD_* from a20PUDControl
    extern void a20_gpio_set_pud(uint16_t pin, uint8_t pud);

    /// Sets the PWM clock divisor, 
    /// to control the basic PWM pulse widths.
    /// \param[in] divisor Divides the basic 19.2MHz PWM clock. You can use one of the common
    /// values A20_PWM_CLOCK_DIVIDER_* in \ref a20PWMClockDivider
    extern void a20_pwm_set_clock(uint32_t divisor);
    
    /// Sets the mode of the given PWM channel,
    /// allowing you to control the PWM mode and enable/disable that channel
    /// \param[in] channel The PWM channel. 0 or 1.
    /// \param[in] markspace Set true if you want Mark-Space mode. 0 for Balanced mode.
    /// \param[in] enabled Set true to enable this channel and produce PWM pulses.
    extern void a20_pwm_set_mode(uint8_t channel, uint8_t markspace, uint8_t enabled);

    /// Sets the maximum range of the PWM output.
    /// The data value can vary between 0 and this range to control PWM output
    /// \param[in] channel The PWM channel. 0 or 1.
    /// \param[in] range The maximum value permitted for DATA.
    extern void a20_pwm_set_range(uint8_t channel, uint32_t range);
    
    /// Sets the PWM pulse ratio to emit to DATA/RANGE, 
    /// where RANGE is set by a20_pwm_set_range().
    /// \param[in] channel The PWM channel. 0 or 1.
    /// \param[in] data Controls the PWM output ratio as a fraction of the range. 
    ///  Can vary from 0 to RANGE.
    extern void a20_pwm_set_data(uint8_t channel, uint32_t data);

    /// @} 
#ifdef __cplusplus
}
#endif

#endif // A20HW_H
