/*
 * vim: tabstop=8 : noexpandtab
 */
#ifndef _ATHEROS_H
#define _ATHEROS_H

#include <linux/compiler.h>
#include <linux/types.h>
#include <asm/addrspace.h>
// #include <linux/autoconf.h>

/*
 * Set everything to zero. The corresponding header will
 * undef and re-define the appropriate ones
 */
#define is_ar7100()	(0)

#define is_ar724x()	(0)
#define is_ar7240()	(0)
#define is_ar7241()	(0)
#define is_ar7242()	(0)

#define is_ar933x()	(0)
#define is_ar9330()	(0)

#define is_ar934x()	(0)
#define is_ar934x_1_0() (0)
#define is_ar934x_1_1() (0)
#define is_ar934x_1_2() (0)
#define is_ar9341()	(0)
#define is_ar9342()	(0)
#define is_ar9344()	(0)

#define is_wasp()	(0)

#define ATH_CONSOLE_BAUD	115200

#ifdef CONFIG_ATH_EMULATION
#define is_emu()	(1)
#else
#define is_emu()	(0)
#endif

#define ath_arch_init_irq() /* nothing */

extern uint32_t ath_cpu_freq, ath_ahb_freq, ath_ddr_freq,
		ath_ref_freq, ath_uart_freq;

void ath_sys_frequency(void);

typedef unsigned int ath_reg_t;

#define ath_reg_rd(_phys)	(*(volatile ath_reg_t *)KSEG1ADDR(_phys))

#define ath_reg_wr_nf(_phys, _val) \
	((*(volatile ath_reg_t *)KSEG1ADDR(_phys)) = (_val))

#define ath_reg_wr(_phys, _val) do {	\
	ath_reg_wr_nf(_phys, _val);	\
	ath_reg_rd(_phys);		\
} while(0)

#define ath_reg_rmw_set(_reg, _mask)	do {			\
	ath_reg_wr((_reg), (ath_reg_rd((_reg)) | (_mask)));	\
	ath_reg_rd((_reg));					\
} while(0)

#define ath_reg_rmw_clear(_reg, _mask) do {			\
	ath_reg_wr((_reg), (ath_reg_rd((_reg)) & ~(_mask)));	\
	ath_reg_rd((_reg));					\
} while(0)

#define MY_WRITE(y, z)		((*((volatile u32*)(y))) = z)

#define UART_READ(y)		ath_reg_rd((ATH_UART_BASE+y))
#define UART_WRITE(x, z)	ath_reg_wr((ATH_UART_BASE+x), z)

#define REG_OFFSET		4

#define OFS_RCV_BUFFER		(0 * REG_OFFSET)
#define OFS_TRANS_HOLD		(0 * REG_OFFSET)
#define OFS_SEND_BUFFER		(0 * REG_OFFSET)
#define OFS_INTR_ENABLE		(1 * REG_OFFSET)
#define OFS_INTR_ID		(2 * REG_OFFSET)
#define OFS_DATA_FORMAT		(3 * REG_OFFSET)
#define OFS_LINE_CONTROL	(3 * REG_OFFSET)
#define OFS_MODEM_CONTROL	(4 * REG_OFFSET)
#define OFS_RS232_OUTPUT	(4 * REG_OFFSET)
#define OFS_LINE_STATUS		(5 * REG_OFFSET)
#define OFS_MODEM_STATUS	(6 * REG_OFFSET)
#define OFS_RS232_INPUT		(6 * REG_OFFSET)
#define OFS_SCRATCH_PAD		(7 * REG_OFFSET)

#define OFS_DIVISOR_LSB		(0 * REG_OFFSET)
#define OFS_DIVISOR_MSB		(1 * REG_OFFSET)

/* #if defined(CONFIG_MACH_AR724x)
#	include <724x.h>
#elif defined(CONFIG_MACH_AR933x)
#	include <933x.h>
#elif defined(CONFIG_MACH_AR934x)
#	include <934x.h>
#else
#	error "Building atheros BSP for unknown device"
#endif
*/
/*
 * GPIO Access & Control
 */
void ath_gpio_init(void);
void ath_gpio_down(void);
void ath_gpio_up(void);

void ath_gpio_irq_init(int);
/*
 * GPIO Helper Functions
 */
void ath_gpio_enable_slic(void);

/* enable UART block, takes away GPIO 10 and 9 */
void ath_gpio_enable_uart(void);

/* enable STEREO block, takes away GPIO 11,8,7, and 6 */
void ath_gpio_enable_stereo(void);

/* allow CS0/CS1 to be controlled via SPI register, takes away GPIO0/GPIO1 */
void ath_gpio_enable_spi_cs1_cs0(void);

/* allow GPIO0/GPIO1 to be used as SCL/SDA for software based i2c */
void ath_gpio_enable_i2c_on_gpio_0_1(void);

/*
 * GPIO General Functions
 */
void ath_gpio_drive_low(unsigned int mask);
void ath_gpio_drive_high(unsigned int mask);

unsigned int ath_gpio_float_high_test(unsigned int mask);

/*
 * Software support of i2c on gpio 0/1
 */
int ath_i2c_raw_write_bytes_to_addr(int addr, unsigned char *buffer, int count);
int ath_i2c_raw_read_bytes_from_addr(int addr, unsigned char *buffer, int count);

/* Functions to access SPI through software. Example:
 *
 * ath_spi_down(); ---------------------- disable others from accessing SPI bus taking semaphore
 * ath_spi_enable_soft_access(); -------- disable HW control of SPI
 *
 * <board specific chip select routine>
 *
 * <read/write SPI using using custom routine or general purposeflash routines
 * Custom routine may use:
 *
 *	ath_spi_raw_output_u8(unsigned char)
 *	ath_spi_raw_output_u32(unsigned int)
 *	ath_spi_raw_input_u32()
 *
 * General purpose flash routines:
 *	ath_spi_flash_read_page(unsigned int addr, unsigned char *data, int len);
 *	ath_spi_flash_write_page(unsigned int addr, unsigned char *data, int len);
 *	ath_spi_flash_sector_erase(unsigned int addr);
 * >
 *
 * <board specific chip deselect routine>
 *
 * ath_spi_disable_soft_acess(); ------- enable HW control of SPI bus
 * ath_spi_up(); ----------------------- enable others to access SPI bus releasing semaphore
 */
/*void ath_spi_init(void);
void ath_spi_down(void);
void ath_spi_up(void);

static inline void
ath_spi_enable_soft_access(void)
{
	ath_reg_wr_nf(ATH_SPI_FS, 1);
}

static inline void
ath_spi_disable_soft_access(void)
{
	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_DIS);
	ath_reg_wr_nf(ATH_SPI_FS, 0);
}

void ath_spi_raw_output_u8(unsigned char val);
void ath_spi_raw_output_u32(unsigned int val);
unsigned int ath_spi_raw_input_u8(void);
unsigned int ath_spi_raw_input_u32(void);

void ath_spi_flash_read_page(unsigned int addr, unsigned char *data, int len);
void ath_spi_flash_write_page(unsigned int addr, unsigned char *data, int len);
void ath_spi_flash_sector_erase(unsigned int addr);
*/
/*
 * Allow access to cs0-2 when GPIO Function enables cs0-2 through SPI register.
 */
/*static inline void
ath_spi_enable_cs0(void)
{
	unsigned int cs;
	ath_spi_down();
	ath_spi_enable_soft_access();
	cs = ath_reg_rd(ATH_SPI_WRITE) & ~ATH_SPI_CS_DIS;
	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_ENABLE_0 | cs);
}

static inline void
ath_spi_enable_cs1(void)
{
	unsigned int cs;
#if defined(CONFIG_MACH_AR934x)
	ath_spi_down();
	ath_spi_init();
	ath_spi_enable_soft_access();
	cs = ath_reg_rd(ATH_SPI_WRITE) & ATH_SPI_CS_DIS;
	ath_reg_wr_nf(ATH_SPI_WRITE, cs | ATH_SPI_CLK_HIGH);
	cs = ath_reg_rd(ATH_SPI_WRITE) & ~ATH_SPI_CS_DIS;
	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_ENABLE_1 | cs | ATH_SPI_CLK_HIGH);
	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_ENABLE_1 | cs);
#else
	ath_spi_down();
	ath_spi_enable_soft_access();
	cs = ath_reg_rd(ATH_SPI_WRITE) & ~ATH_SPI_CS_DIS;
	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_ENABLE_1 | cs);
#endif
}

static inline void
ath_spi_disable_cs(void)
{
	unsigned int cs = ath_reg_rd(ATH_SPI_WRITE) | ATH_SPI_CS_DIS;
	ath_reg_wr_nf(ATH_SPI_WRITE, cs);
	ath_spi_disable_soft_access();
	ath_spi_up();
}
*/
/*
 * Example usage to access BOOT flash
 */
/*
static inline void
ath_spi_flash_cs0_sector_erase(unsigned int addr)
{
	ath_spi_enable_cs0();
	ath_spi_flash_sector_erase(addr);
	ath_spi_disable_cs();
}

static inline void
ath_spi_flash_cs0_write_page(unsigned int addr, unsigned char *data, int len)
{
	ath_spi_enable_cs0();
	ath_spi_flash_write_page(addr, data, len);
	ath_spi_disable_cs();
}
*/

/* Low-level routines */
void ath_dma_addr_wr(int chan, unsigned int val);
void ath_dma_config_wr(int chan, unsigned int val);
void ath_dma_update_wr(int chan, unsigned int val);

unsigned int ath_dma_addr_rd(int chan);
unsigned int ath_dma_config_rd(int chan);

/* Use this routine to configure DMA access. Example:
 *
 * ath_dma_config_buffer( ATH_DMA_CHAN_SLIC0_TX,
 *				< address of buffer >,
 *				ATH_DMA_BUF_SIZE_512X2
 */
void ath_dma_config_buffer(int chan, void *buffer, int sizeCfg);

/*
 * SLIC Helper Functions
 */
unsigned int ath_slic_status_rd(void);
unsigned int ath_slic_cntrl_rd(void);

void ath_slic_cntrl_wr(unsigned int val);
void ath_slic_0_slot_pos_wr(unsigned int val);
void ath_slic_1_slot_pos_wr(unsigned int val);
void ath_slic_freq_div_wr(unsigned int val);
void ath_slic_sample_pos_wr(unsigned int val);

void ath_slic_setup(int _sam, int _s0n, int _s1n);

/*
 * Audio data is little endian so 16b values must be swapped in the DMA buffers.
 */
static inline int
ath_stereo_sample_16b_cvt(unsigned int _v)
{
	return (((_v<<8)&0xff00)|((_v>>8)&0xff)) & 0xffff;
}

/* Low level read/write of configuration */
void ath_stereo_config_wr(unsigned int val);
void ath_stereo_volume_wr(unsigned int val);

unsigned int ath_stereo_config_rd(void);
unsigned int ath_stereo_volume_rd(void);


/* Routine sets up STEREO block for use. Use one of the predefined
 * configurations. Example:
 *
 * ath_stereo_config_setup(
 *	ATH_STEREO_CFG_MASTER_STEREO_FS32_48KHZ(ATH_STEREO_WS_16B))
 *
 */
void ath_stereo_config_setup(unsigned int cfg);

/* 48 kHz, 16 bit data & i2s 32fs */
static inline void
ath_setup_for_stereo_master(int ws)
{
	ath_stereo_config_setup(ATH_STEREO_CFG_MASTER_STEREO_FS32_48KHZ(ws));
}

/* 48 kHz, 16 bit data & 32fs i2s */
static inline void
ath_setup_for_stereo_slave(int ws)
{
	ath_stereo_config_setup(ATH_STEREO_CFG_SLAVE_STEREO_FS32_48KHZ(ws));
}

void ath_reset(unsigned int mask);
int ath_local_read_config(int where, int size, u32 *value);
int ath_local_write_config(int where, int size, u32 value);
int ath_check_error(int verbose);
unsigned char __ath_readb(const volatile void __iomem *p);
unsigned short __ath_readw(const volatile void __iomem *p);
void ap_usb_led_on(void);
void ap_usb_led_off(void);
/*
#ifdef CONFIG_PCI
void ath_pci_irq_init(int);
#endif
i*/
#endif /* _ATHEROS_H */
