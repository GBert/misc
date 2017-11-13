// a20hw.c : Routinen zur direkten Hardware-Ansteuerung eines A20-SOC
// C 2017 Rainer Müller 
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).
// frei nach bcm2835.c von Mike McCauley
//

#include <stdio.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include "a20hw.h"

// ***************** #define A20_TEST
#define BLOCK_SIZE      4096

#define GPIO_ADRESS     0x01C20000
#define GPIO_CFG_OFFSET 0x0800
#define GPIO_DAT_OFFSET 0x0810
#define GPIO_DRV_OFFSET 0x0814
#define GPIO_PUL_OFFSET 0x081C


// Base of the GPIO registers, available after a20_init has been called.
volatile uint32_t *a20_gpio  =  (volatile uint32_t *)MAP_FAILED;


// Reads 32 bit value from a peripheral address
uint32_t a20_peri_read(volatile uint32_t* paddr)
{
    __sync_synchronize();
	uint32_t ret = *paddr;
    __sync_synchronize();
	return ret;
}

// Writes 32 bit value from a peripheral address
void a20_peri_write(volatile uint32_t* paddr, uint32_t value)
{
    __sync_synchronize();
	*paddr = value;
    __sync_synchronize();
}

// Alters a number of bits in a 32 peripheral regsiter.
void a20_peri_set_bits(volatile uint32_t* paddr, uint32_t value, uint32_t mask)
{
    uint32_t v = a20_peri_read(paddr);
    v = (v & ~mask) | (value & mask);
    a20_peri_write(paddr, v);
}

// Sets the specified pin output to HIGH.
void a20_gpio_set(uint16_t pin)
{
	if (pin >= 288) return;
	volatile uint32_t* paddr = a20_gpio + ((GPIO_DAT_OFFSET +(pin/32) * 0x24) >> 2);
	uint8_t   shift = pin % 32;
	uint32_t  mask = 0x01 << shift;
	uint32_t  value = 1 << shift;
	a20_peri_set_bits(paddr, value, mask);
}

// Sets the specified pin output to LOW.
void a20_gpio_clr(uint16_t pin)
{
	if (pin >= 288) return;
	volatile uint32_t* paddr = a20_gpio + ((GPIO_DAT_OFFSET +(pin/32) * 0x24) >> 2);
	uint8_t   shift = pin % 32;
	uint32_t  mask = 0x01 << shift;
	uint32_t  value = 0 << shift;
	a20_peri_set_bits(paddr, value, mask);
}

// Sets the output state of the specified pin to HIGH or LOW.
void a20_gpio_write(uint16_t pin, uint8_t on)
{
    if (on) a20_gpio_set(pin);
    else    a20_gpio_clr(pin);
}

// Reads the current level on the specified pin and returns either HIGH or LOW.
// Works whether or not the pin is an input or an output.
uint8_t a20_gpio_lev(uint16_t pin)
{
	if (pin >= 288) return 0;
	volatile uint32_t* paddr = a20_gpio + ((GPIO_DAT_OFFSET +(pin/32) * 0x24) >> 2);
	uint8_t shift = pin % 32;
	uint32_t value = a20_peri_read(paddr);
	return (value & (1 << shift)) ? HIGH : LOW;
}

// Sets the Function Select register for the given pin, which configures
// the pin as Input, Output or one of the 6 alternate functions.
void a20_gpio_set_fsel(uint16_t pin, uint8_t mode)
{
	if (pin >= 288) return;
	volatile uint32_t* paddr = a20_gpio+((GPIO_CFG_OFFSET+(pin/32)*0x24+((pin%32)/8)*0x04) >> 2);
	uint8_t   shift = (pin % 8) * 4;
	uint32_t  mask = A20_GPIO_FSEL_MASK << shift;
	uint32_t  value = mode << shift;
	a20_peri_set_bits(paddr, value, mask);
}

// Reads the Function Select register for the given pin, which configures
// the pin as Input, Output or one of the 6 alternate functions.
uint8_t a20_gpio_fsel(uint16_t pin)
{
	if (pin >= 288) return 0;
	volatile uint32_t* paddr = a20_gpio+((GPIO_CFG_OFFSET+(pin/32)*0x24+((pin%32)/8)*0x04) >> 2);
	uint8_t  shift = (pin % 8) * 4;
	uint32_t value = a20_peri_read(paddr);
    return (value >> shift) & A20_GPIO_FSEL_MASK;
}

// Sets the Pull-up/down mode for the specified pin. 
void a20_gpio_set_pud(uint16_t pin, uint8_t pud)
{
	if (pin >= 288) return;
	volatile uint32_t* paddr = a20_gpio+((GPIO_PUL_OFFSET+(pin/32)*0x24+((pin%32)/16)*0x04) >> 2);
	uint8_t  shift = (pin % 16) * 2;
	uint32_t  mask = A20_GPIO_PUD_MASK << shift;
	uint32_t  value = pud << shift;
	a20_peri_set_bits(paddr, value, mask);
}

// Reads the Pull-up/down mode for the specified pin. 
uint8_t a20_gpio_pud(uint16_t pin)
{
	if (pin >= 288) return 0;
	volatile uint32_t* paddr = a20_gpio+((GPIO_PUL_OFFSET+(pin/32)*0x24+((pin%32)/16)*0x04) >> 2);
	uint8_t  shift = (pin % 16) * 2;
	uint32_t value = a20_peri_read(paddr);
    return (value >> shift) & A20_GPIO_PUD_MASK;
}

// Sets the Pad Control for the given GPIO pin.
void a20_gpio_set_pad(uint16_t pin, uint8_t control)
{
	if (pin >= 288) return;
	volatile uint32_t* paddr = a20_gpio+((GPIO_DRV_OFFSET+(pin/32)*0x24+((pin%32)/16)*0x04) >> 2);
	uint8_t  shift = (pin % 16) * 2;
	uint32_t  mask = A20_GPIO_PUD_MASK << shift;
	uint32_t  value = control << shift;
	a20_peri_set_bits(paddr, value, mask);
}

// Reads and returns the Pad Control for the given GPIO pin.
uint8_t a20_gpio_pad(uint16_t pin)
{
	if (pin >= 288) return 0;
	volatile uint32_t* paddr = a20_gpio+((GPIO_DRV_OFFSET+(pin/32)*0x24+((pin%32)/16)*0x04) >> 2);
	uint8_t  shift = (pin % 16) * 2;
	uint32_t value = a20_peri_read(paddr);
    return (value >> shift) & A20_GPIO_PUD_MASK;
}

// Initialise the library by opening /dev/mem and getting pointers to the 
// internal memory for A20 device registers. You must call this (successfully)
// before calling any other functions in this library. 
// return 1 if successful else 0
int a20_init(void)
{
	int memDev = open("/dev/mem", O_RDWR|O_SYNC);
	if (memDev < 0) {
        fprintf(stderr, "a20_init: Unable to open /dev/mem: %s\n",
		      strerror(errno)) ;
        return 0;
    }

	a20_gpio = (uint32_t *)mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, 
        MAP_SHARED, memDev, GPIO_ADRESS);
	if (a20_gpio == MAP_FAILED) {
		fprintf(stderr, "a20_init: GPIO mmap failed: %s\n", strerror(errno));
		return 0;
	}	

    return 1;
}

// Close the library, deallocating any allocated memory and closing /dev/mem
// return 1 if successful else 0
int a20_close(void)
{
 	if(a20_gpio == MAP_FAILED) return 1;
	else  {	
		munmap((void *)a20_gpio, BLOCK_SIZE);
		a20_gpio = MAP_FAILED;
	}
	return 1;
}

    // das braucht vermutlich niemand !
#if 0
    /// Gets the base of a register
    /// \param[in] regbase You can use one of the common values A20_REGBASE_*
    /// in \ref a20RegisterBase
    /// \return the register base
    /// \sa Physical Addresses
    extern uint32_t* a20_regbase(uint8_t regbase);

    /// Reads 32 bit value from a peripheral address without the read barrier
    /// You should only use this when your code has previously called a20_peri_read()
    /// within the same peripheral, and no other peripheral access has occurred since.
    /// \param[in] paddr Physical address to read from. See A20_GPIO_BASE etc.
    /// \return the value read from the 32 bit register
    /// \sa Physical Addresses
    extern uint32_t a20_peri_read_nb(volatile uint32_t* paddr);

    /// Writes 32 bit value from a peripheral address without the write barrier
    /// You should only use this when your code has previously called a20_peri_write()
    /// within the same peripheral, and no other peripheral access has occurred since.
    /// \param[in] paddr Physical address to read from. See A20_GPIO_BASE etc.
    /// \param[in] value The 32 bit value to write
    /// \sa Physical Addresses
    extern void a20_peri_write_nb(volatile uint32_t* paddr, uint32_t value);

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

    /// Sets any of the first 32 GPIO output pins specified in the mask to the state given by on
    /// \param[in] mask Mask of pins to affect. Use eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    /// \param[in] on HIGH sets the output to HIGH and LOW to LOW.
    extern void a20_gpio_write_multi(uint32_t mask, uint8_t on);

    /// Sets the first 32 GPIO output pins specified in the mask to the value given by value
    /// \param[in] value values required for each bit masked in by mask, eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    /// \param[in] mask Mask of pins to affect. Use eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    extern void a20_gpio_write_mask(uint32_t value, uint32_t mask);

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
#endif


// Test program

#ifdef A20_TEST
// this is a simple test program that prints out what it will do rather than 
// actually doing it
int main(int argc, char **argv)
{
    int i;

    if (!a20_init()) return 1;

    for (i=0; i<32; i++) {
    	printf ("Bit %2d  FSEL %d  DATA %d  PULL %d PAD %d\n", i,
                a20_gpio_fsel(PORTHPIN + i), a20_gpio_lev(PORTHPIN + i),
                a20_gpio_pud(PORTHPIN + i), a20_gpio_pad(PORTHPIN + i) );
    }

#if 0
    // Blink
    while (1)
    {
	// Turn it on
	bcm2835_gpio_write(RPI_GPIO_P1_11, HIGH);
	
	// wait a bit
	bcm2835_delay(500);
	
	// turn it off
	bcm2835_gpio_write(RPI_GPIO_P1_11, LOW);
	
	// wait a bit
	bcm2835_delay(500);
    }
#endif

#if 0
    // Read input
    while (1)
    {
	// Read some data
	uint8_t value = bcm2835_gpio_lev(RPI_GPIO_P1_15);
	printf("read from pin 15: %d\n", value);
	
	// wait a bit
	bcm2835_delay(500);
    }
#endif

    if (!a20_close()) return 1;
    return 0;
}
#endif



