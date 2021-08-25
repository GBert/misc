#include <linux/delay.h>

#define MASTER                      1
#define SPDIF
#undef  SPDIFIOCTL

#define SPDIF_CONFIG_CHANNEL(x)     ((3&x)<<20)
#define SPDIF_MODE_LEFT             1
#define SPDIF_MODE_RIGHT            2
#define SPDIF_CONFIG_SAMP_FREQ(x)   ((0xf&x)<<24)
#define SPDIF_SAMP_FREQ_48          2
#define SPDIF_SAMP_FREQ_44          0
#define SPDIF_CONFIG_ORG_FREQ(x)    ((0xf&x)<<4)
#define SPDIF_ORG_FREQ_48           0xd
#define SPDIF_ORG_FREQ_44           0xf
#define SPDIF_CONFIG_SAMP_SIZE(x)   (0xf&x)
#define SPDIF_S_8_16                2
#define SPDIF_S_24_32               0xb

#define PAUSE				        0x1
#define START				        0x2
#define RESUME				        0x4

#define STATUSMASK1			        (1ul << 31)
#define STATUSMASK			        (1ul << 4)
#define SHIFT_WIDTH			        11
#define ADDR_VALID			        1

#define I2S_RESET                   1
#define MBOX_RESET                  (1ul << 1)
#define MBOX_INTR_MASK              (1ul << 7)

#define MONO                    	(1ul << 14)
#define ATH_I2S_NUM_DESC            128
#define ATH_I2S_BUFF_SIZE           768

#define ATH_AUDIO_PLL_CFG_PWR_DWN   (1ul << 5)

#define ATH_AUDIO_DPLL2             0xb8116204
#define ATH_AUDIO_DPLL3             0xb8116208

#define ATH_AUDIO_DPLL2_RANGE           (1        << 31)
#define ATH_AUDIO_DPLL2_PATH_GAIN(x)    ((0xf&x)  << 26)
#define ATH_AUDIO_DPLL2_PROP_GAIN(x)    ((0x7f&x) << 19)
#define ATH_AUDIO_DPLL3_PHASE_SHIFT(x)  ((0x7f&x) << 23) 

#define ATH_AUD_DPLL2_RANGE_MSB			31
#define ATH_AUD_DPLL2_RANGE_LSB			31
#define ATH_AUD_DPLL2_RANGE_MASK		0x80000000
#define ATH_AUD_DPLL2_RANGE_GET(x)		(((x) & ATH_AUD_DPLL2_RANGE_MASK) >> ATH_AUD_DPLL2_RANGE_LSB)
#define ATH_AUD_DPLL2_RANGE_SET(x)		(((x) << ATH_AUD_DPLL2_RANGE_LSB) & ATH_AUD_DPLL2_RANGE_MASK)
#define ATH_AUD_DPLL2_RANGE_RESET		0x0 // 0
#define ATH_AUD_DPLL2_LOCAL_PLL_MSB		30
#define ATH_AUD_DPLL2_LOCAL_PLL_LSB		30
#define ATH_AUD_DPLL2_LOCAL_PLL_MASK		0x40000000
#define ATH_AUD_DPLL2_LOCAL_PLL_GET(x)		(((x) & ATH_AUD_DPLL2_LOCAL_PLL_MASK) >> ATH_AUD_DPLL2_LOCAL_PLL_LSB)
#define ATH_AUD_DPLL2_LOCAL_PLL_SET(x)		(((x) << ATH_AUD_DPLL2_LOCAL_PLL_LSB) & ATH_AUD_DPLL2_LOCAL_PLL_MASK)
#define ATH_AUD_DPLL2_LOCAL_PLL_RESET		0x0 // 0
#define ATH_AUD_DPLL2_KI_MSB			29
#define ATH_AUD_DPLL2_KI_LSB			26
#define ATH_AUD_DPLL2_KI_MASK			0x3c000000
#define ATH_AUD_DPLL2_KI_GET(x)			(((x) & ATH_AUD_DPLL2_KI_MASK) >> ATH_AUD_DPLL2_KI_LSB)
#define ATH_AUD_DPLL2_KI_SET(x)			(((x) << ATH_AUD_DPLL2_KI_LSB) & ATH_AUD_DPLL2_KI_MASK)
#define ATH_AUD_DPLL2_KI_RESET			0x6 // 6
#define ATH_AUD_DPLL2_KD_MSB			25
#define ATH_AUD_DPLL2_KD_LSB			19
#define ATH_AUD_DPLL2_KD_MASK			0x03f80000
#define ATH_AUD_DPLL2_KD_GET(x)			(((x) & ATH_AUD_DPLL2_KD_MASK) >> ATH_AUD_DPLL2_KD_LSB)
#define ATH_AUD_DPLL2_KD_SET(x)			(((x) << ATH_AUD_DPLL2_KD_LSB) & ATH_AUD_DPLL2_KD_MASK)
#define ATH_AUD_DPLL2_KD_RESET			0x7f // 127
#define ATH_AUD_DPLL2_EN_NEGTRIG_MSB		18
#define ATH_AUD_DPLL2_EN_NEGTRIG_LSB		18
#define ATH_AUD_DPLL2_EN_NEGTRIG_MASK		0x00040000
#define ATH_AUD_DPLL2_EN_NEGTRIG_GET(x)		(((x) & ATH_AUD_DPLL2_EN_NEGTRIG_MASK) >> ATH_AUD_DPLL2_EN_NEGTRIG_LSB)
#define ATH_AUD_DPLL2_EN_NEGTRIG_SET(x)		(((x) << ATH_AUD_DPLL2_EN_NEGTRIG_LSB) & ATH_AUD_DPLL2_EN_NEGTRIG_MASK)
#define ATH_AUD_DPLL2_EN_NEGTRIG_RESET		0x0 // 0
#define ATH_AUD_DPLL2_SEL_1SDM_MSB		17
#define ATH_AUD_DPLL2_SEL_1SDM_LSB		17
#define ATH_AUD_DPLL2_SEL_1SDM_MASK		0x00020000
#define ATH_AUD_DPLL2_SEL_1SDM_GET(x)		(((x) & ATH_AUD_DPLL2_SEL_1SDM_MASK) >> ATH_AUD_DPLL2_SEL_1SDM_LSB)
#define ATH_AUD_DPLL2_SEL_1SDM_SET(x)		(((x) << ATH_AUD_DPLL2_SEL_1SDM_LSB) & ATH_AUD_DPLL2_SEL_1SDM_MASK)
#define ATH_AUD_DPLL2_SEL_1SDM_RESET		0x0 // 0
#define ATH_AUD_DPLL2_PLL_PWD_MSB		16
#define ATH_AUD_DPLL2_PLL_PWD_LSB		16
#define ATH_AUD_DPLL2_PLL_PWD_MASK		0x00010000
#define ATH_AUD_DPLL2_PLL_PWD_GET(x)		(((x) & ATH_AUD_DPLL2_PLL_PWD_MASK) >> ATH_AUD_DPLL2_PLL_PWD_LSB)
#define ATH_AUD_DPLL2_PLL_PWD_SET(x)		(((x) << ATH_AUD_DPLL2_PLL_PWD_LSB) & ATH_AUD_DPLL2_PLL_PWD_MASK)
#define ATH_AUD_DPLL2_PLL_PWD_RESET		0x1 // 1
#define ATH_AUD_DPLL2_OUTDIV_MSB		15
#define ATH_AUD_DPLL2_OUTDIV_LSB		13
#define ATH_AUD_DPLL2_OUTDIV_MASK		0x0000e000
#define ATH_AUD_DPLL2_OUTDIV_GET(x)		(((x) & ATH_AUD_DPLL2_OUTDIV_MASK) >> ATH_AUD_DPLL2_OUTDIV_LSB)
#define ATH_AUD_DPLL2_OUTDIV_SET(x)		(((x) << ATH_AUD_DPLL2_OUTDIV_LSB) & ATH_AUD_DPLL2_OUTDIV_MASK)
#define ATH_AUD_DPLL2_OUTDIV_RESET		0x0 // 0
#define ATH_AUD_DPLL2_DELTA_MSB			12
#define ATH_AUD_DPLL2_DELTA_LSB			7
#define ATH_AUD_DPLL2_DELTA_MASK		0x00001f80
#define ATH_AUD_DPLL2_DELTA_GET(x)		(((x) & ATH_AUD_DPLL2_DELTA_MASK) >> ATH_AUD_DPLL2_DELTA_LSB)
#define ATH_AUD_DPLL2_DELTA_SET(x)		(((x) << ATH_AUD_DPLL2_DELTA_LSB) & ATH_AUD_DPLL2_DELTA_MASK)
#define ATH_AUD_DPLL2_DELTA_RESET		0x1e // 30
#define ATH_AUD_DPLL2_SPARE_MSB			6
#define ATH_AUD_DPLL2_SPARE_LSB			0
#define ATH_AUD_DPLL2_SPARE_MASK		0x0000007f
#define ATH_AUD_DPLL2_SPARE_GET(x)		(((x) & ATH_AUD_DPLL2_SPARE_MASK) >> ATH_AUD_DPLL2_SPARE_LSB)
#define ATH_AUD_DPLL2_SPARE_SET(x)		(((x) << ATH_AUD_DPLL2_SPARE_LSB) & ATH_AUD_DPLL2_SPARE_MASK)
#define ATH_AUD_DPLL2_SPARE_RESET		0x0 // 0
#define ATH_AUD_DPLL2_ADDRESS			0x18116204

#define ATH_AUD_DPLL3_MEAS_AT_TXON_MSB		31
#define ATH_AUD_DPLL3_MEAS_AT_TXON_LSB		31
#define ATH_AUD_DPLL3_MEAS_AT_TXON_MASK		0x80000000
#define ATH_AUD_DPLL3_MEAS_AT_TXON_GET(x)	(((x) & ATH_AUD_DPLL3_MEAS_AT_TXON_MASK) >> ATH_AUD_DPLL3_MEAS_AT_TXON_LSB)
#define ATH_AUD_DPLL3_MEAS_AT_TXON_SET(x)	(((x) << ATH_AUD_DPLL3_MEAS_AT_TXON_LSB) & ATH_AUD_DPLL3_MEAS_AT_TXON_MASK)
#define ATH_AUD_DPLL3_MEAS_AT_TXON_RESET	0x0 // 0
#define ATH_AUD_DPLL3_DO_MEAS_MSB		30
#define ATH_AUD_DPLL3_DO_MEAS_LSB		30
#define ATH_AUD_DPLL3_DO_MEAS_MASK		0x40000000
#define ATH_AUD_DPLL3_DO_MEAS_GET(x)		(((x) & ATH_AUD_DPLL3_DO_MEAS_MASK) >> ATH_AUD_DPLL3_DO_MEAS_LSB)
#define ATH_AUD_DPLL3_DO_MEAS_SET(x)		(((x) << ATH_AUD_DPLL3_DO_MEAS_LSB) & ATH_AUD_DPLL3_DO_MEAS_MASK)
#define ATH_AUD_DPLL3_DO_MEAS_RESET		0x0 // 0
#define ATH_AUD_DPLL3_PHASE_SHIFT_MSB		29
#define ATH_AUD_DPLL3_PHASE_SHIFT_LSB		23
#define ATH_AUD_DPLL3_PHASE_SHIFT_MASK		0x3f800000
#define ATH_AUD_DPLL3_PHASE_SHIFT_GET(x)	(((x) & ATH_AUD_DPLL3_PHASE_SHIFT_MASK) >> ATH_AUD_DPLL3_PHASE_SHIFT_LSB)
#define ATH_AUD_DPLL3_PHASE_SHIFT_SET(x)	(((x) << ATH_AUD_DPLL3_PHASE_SHIFT_LSB) & ATH_AUD_DPLL3_PHASE_SHIFT_MASK)
#define ATH_AUD_DPLL3_PHASE_SHIFT_RESET		0x0 // 0
#define ATH_AUD_DPLL3_SQSUM_DVC_MSB		22
#define ATH_AUD_DPLL3_SQSUM_DVC_LSB		3
#define ATH_AUD_DPLL3_SQSUM_DVC_MASK		0x007ffff8
#define ATH_AUD_DPLL3_SQSUM_DVC_GET(x)		(((x) & ATH_AUD_DPLL3_SQSUM_DVC_MASK) >> ATH_AUD_DPLL3_SQSUM_DVC_LSB)
#define ATH_AUD_DPLL3_SQSUM_DVC_SET(x)		(((x) << ATH_AUD_DPLL3_SQSUM_DVC_LSB) & ATH_AUD_DPLL3_SQSUM_DVC_MASK)
#define ATH_AUD_DPLL3_SQSUM_DVC_RESET		0x0 // 0
#define ATH_AUD_DPLL3_SPARE_MSB			2
#define ATH_AUD_DPLL3_SPARE_LSB			0
#define ATH_AUD_DPLL3_SPARE_MASK		0x00000007
#define ATH_AUD_DPLL3_SPARE_GET(x)		(((x) & ATH_AUD_DPLL3_SPARE_MASK) >> ATH_AUD_DPLL3_SPARE_LSB)
#define ATH_AUD_DPLL3_SPARE_SET(x)		(((x) << ATH_AUD_DPLL3_SPARE_LSB) & ATH_AUD_DPLL3_SPARE_MASK)
#define ATH_AUD_DPLL3_SPARE_RESET		0x0 // 0
#define ATH_AUD_DPLL3_ADDRESS			0x18116208

#define ATH_AUD_DPLL4_MEAN_DVC_MSB		31
#define ATH_AUD_DPLL4_MEAN_DVC_LSB		21
#define ATH_AUD_DPLL4_MEAN_DVC_MASK		0xffe00000
#define ATH_AUD_DPLL4_MEAN_DVC_GET(x)		(((x) & ATH_AUD_DPLL4_MEAN_DVC_MASK) >> ATH_AUD_DPLL4_MEAN_DVC_LSB)
#define ATH_AUD_DPLL4_MEAN_DVC_SET(x)		(((x) << ATH_AUD_DPLL4_MEAN_DVC_LSB) & ATH_AUD_DPLL4_MEAN_DVC_MASK)
#define ATH_AUD_DPLL4_MEAN_DVC_RESET		0x0 // 0
#define ATH_AUD_DPLL4_VC_MEAS0_MSB		20
#define ATH_AUD_DPLL4_VC_MEAS0_LSB		4
#define ATH_AUD_DPLL4_VC_MEAS0_MASK		0x001ffff0
#define ATH_AUD_DPLL4_VC_MEAS0_GET(x)		(((x) & ATH_AUD_DPLL4_VC_MEAS0_MASK) >> ATH_AUD_DPLL4_VC_MEAS0_LSB)
#define ATH_AUD_DPLL4_VC_MEAS0_SET(x)		(((x) << ATH_AUD_DPLL4_VC_MEAS0_LSB) & ATH_AUD_DPLL4_VC_MEAS0_MASK)
#define ATH_AUD_DPLL4_VC_MEAS0_RESET		0x0 // 0
#define ATH_AUD_DPLL4_MEAS_DONE_MSB		3
#define ATH_AUD_DPLL4_MEAS_DONE_LSB		3
#define ATH_AUD_DPLL4_MEAS_DONE_MASK		0x00000008
#define ATH_AUD_DPLL4_MEAS_DONE_GET(x)		(((x) & ATH_AUD_DPLL4_MEAS_DONE_MASK) >> ATH_AUD_DPLL4_MEAS_DONE_LSB)
#define ATH_AUD_DPLL4_MEAS_DONE_SET(x)		(((x) << ATH_AUD_DPLL4_MEAS_DONE_LSB) & ATH_AUD_DPLL4_MEAS_DONE_MASK)
#define ATH_AUD_DPLL4_MEAS_DONE_RESET		0x0 // 0
#define ATH_AUD_DPLL4_SPARE_MSB			2
#define ATH_AUD_DPLL4_SPARE_LSB			0
#define ATH_AUD_DPLL4_SPARE_MASK		0x00000007
#define ATH_AUD_DPLL4_SPARE_GET(x)		(((x) & ATH_AUD_DPLL4_SPARE_MASK) >> ATH_AUD_DPLL4_SPARE_LSB)
#define ATH_AUD_DPLL4_SPARE_SET(x)		(((x) << ATH_AUD_DPLL4_SPARE_LSB) & ATH_AUD_DPLL4_SPARE_MASK)
#define ATH_AUD_DPLL4_SPARE_RESET		0x0 // 0
#define ATH_AUD_DPLL4_ADDRESS			0x1811620c

#define ATH_AUD_DPLL3_KD_25			0x3d
#define ATH_AUD_DPLL3_KD_40			0x32

#define ATH_AUD_DPLL3_KI_25			0x4
#define ATH_AUD_DPLL3_KI_40			0x4

#define I2S_LOCK_INIT(_sc)		spin_lock_init(&(_sc)->i2s_lock)
#define I2S_LOCK_DESTROY(_sc)
#define I2S_LOCK(_sc)			spin_lock_irqsave(&(_sc)->i2s_lock, (_sc)->i2s_lockflags)
#define I2S_UNLOCK(_sc)			spin_unlock_irqrestore(&(_sc)->i2s_lock, (_sc)->i2s_lockflags)

#define I2S_VOLUME      _IOW('N', 0x20, int)
#define I2S_FREQ        _IOW('N', 0x21, int)
#define I2S_DSIZE       _IOW('N', 0x22, int)
#define I2S_MODE        _IOW('N', 0x23, int)
#define I2S_FINE        _IOW('N', 0x24, int)
#define I2S_COUNT       _IOWR('N', 0x25, int)
#define I2S_PAUSE       _IOWR('N', 0x26, int)
#define I2S_RESUME      _IOWR('N', 0x27, int)
#define I2S_MCLK        _IOW('N', 0x28, int)

typedef struct {
	unsigned int OWN		:  1,    /* bit 00 */
	             EOM		:  1,    /* bit 01 */
	             rsvd1	    :  6,    /* bit 07-02 */
	             size	    : 12,    /* bit 19-08 */
	             length	    : 12,    /* bit 31-20 */
	             rsvd2	    :  4,    /* bit 00 */
	             BufPtr	    : 28,    /* bit 00 */
	             rsvd3	    :  4,    /* bit 00 */
	             NextPtr	: 28;    /* bit 00 */
#ifdef SPDIF
    unsigned int Va[6];
    unsigned int Ua[6];
    unsigned int Ca[6];
    unsigned int Vb[6];
    unsigned int Ub[6];
    unsigned int Cb[6];
#endif
} ath_mbox_dma_desc;

/*
 * XXX : This is the interface between i2s and wlan
 *       When adding info, here please make sure that
 *       it is reflected in the wlan side also
 */
typedef struct i2s_stats {
    unsigned int write_fail;
    unsigned int rx_underflow;
} i2s_stats_t;

typedef struct i2s_buf {
    uint8_t *bf_vaddr;
    unsigned long bf_paddr;
} i2s_buf_t;

typedef struct i2s_dma_buf {
    ath_mbox_dma_desc *lastbuf;
    ath_mbox_dma_desc *db_desc;
    dma_addr_t db_desc_p;
    i2s_buf_t db_buf[ATH_I2S_NUM_DESC];
    int tail;
} i2s_dma_buf_t;

typedef struct ath_i2s_softc {
	int			ropened,
				popened,
				sc_irq,
				ft_value,
				ppause,
				rpause;
	char			*sc_pmall_buf,
				*sc_rmall_buf;
	i2s_dma_buf_t		sc_pbuf,
				sc_rbuf;
	wait_queue_head_t	wq_rx,
				wq_tx;
	spinlock_t		i2s_lock;
    unsigned long i2s_lockflags;
	struct timer_list	pll_lost_lock;
	uint32_t		pll_timer_inited,
				dsize,
				vol,
				freq;
} ath_i2s_softc_t;

ath_i2s_softc_t sc_buf_var;
i2s_stats_t stats;

