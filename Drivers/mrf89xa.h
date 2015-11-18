#ifndef MRF89XA_H_
#define MRF89XA_H_

// Register addresses
#define MRF89_REG_GCONREG		0x00		// General configuration register
#define MRF89_REG_DMODREG		0x01		// Data and Modulation Configuration
#define MRF89_REG_FDEVREG		0x02		// Frequency deviation control register
#define MRF89_REG_BRSREG		0x03		// Bit rate set register
#define MRF89_REG_FLTHREG		0x04		//
#define MRF89_REG_FIFOCREG		0x05
#define MRF89_REG_R1CREG		0x06
#define MRF89_REG_P1CREG		0x07
#define MRF89_REG_S1CREG		0x08
#define MRF89_REG_R2CREG		0x09
#define MRF89_REG_P2CREG		0x0A
#define MRF89_REG_S2CREG		0x0B
#define MRF89_REG_PACREG		0x0C
#define MRF89_REG_FTXRXIREG		0x0D
#define MRF89_REG_FTPRIREG		0x0E
#define MRF89_REG_RSTHIREG		0x0F
#define MRF89_REG_FILCREG		0x10
#define MRF89_REG_PFCREG		0x11
#define MRF89_REG_SYNCREG		0x12
#define MRF89_REG_RESVREG		0x13
#define MRF89_REG_RSTSREG		0x14
#define MRF89_REG_OOKCREG		0x15
#define MRF89_REG_SYNCV31REG	0x16
#define MRF89_REG_SYNCV23REG	0x17
#define MRF89_REG_SYNCV15REG	0x18
#define MRF89_REG_SYNCV07REG	0x19
#define MRF89_REG_TXCONREG		0x1A
#define MRF89_REG_CLKOUTREG		0x1B
#define MRF89_REG_PLOADREG		0x1C
#define MRF89_REG_NADDSREG		0x1D
#define MRF89_REG_PKTCREG		0x1E
#define MRF89_REG_FCRCREG		0x1F

//********* REGISTER SETTINGS **************/

#define GCON_TX         (4 << 5)
#define GCON_RX         (3 << 5)
#define GCON_FREQ_SYNTH (2 << 5)
#define GCON_STANDBY    (1 << 5)
#define GCON_SLEEP      (0 << 5)
#define GCON_950_BAND   (2 << 3)
#define GCON_863_BAND   (2 << 3)
#define GCON_915_BAND   (1 << 3)
#define GCON_902_BAND   (0 << 3)
#define GCON_RPS_1      (0 << 0) /* use R1/P1/S1 */
#define GCON_RPS_2      (1 << 0) /* use R2/P2/S2 */


#define DMOD_FSK        (2 << 6) /* modulation: FSK */
#define DMOD_OOK        (1 << 6) /* modulation: OOK */
#define DMOD_OOK_AVG    (2 << 3)
#define DMOD_OOK_PEAK   (1 << 3)
#define DMOD_OOK_FIXED  (0 << 3)
#define DMOD_IF_N13_5DB (3 << 0) /* IF gain: -13.5dB */
#define DMOD_IF_N9DB    (2 << 0) /* IF gain: -9dB */
#define DMOD_IF_N4_5DB  (1 << 0) /* IF gain: -4.5dB */
#define DMOD_IF_0DB     (0 << 0) /* IF gain: 0dB */
#define DMOD_CONTINUOUS (0x00) /* data mode: continuous */
#define DMOD_BUFFER     (0x20) /* data mode: buffer */
#define DMOD_PACKET     (0x04) /* data mode: packet */

/* fDev = fXtal / (32 * (FDEVREG + 1))    FDEVREG = 0..255 */
/* bitrate = fXtal / (64 * (BRSREG + 1))  BRSREG = 0..127  */

#define FIFOC_64B       (3 << 6) /* fifo size: 64 bytes */
#define FIFOC_48B       (2 << 6) /* fifo size: 48 bytes */
#define FIFOC_32B       (1 << 6) /* fifo size: 32 bytes */
#define FIFOC_16B       (0 << 6) /* fifo size: 16 bytes */
#define FIFOC_FTINT(n)  ((n) & 0x3F) /* fifo int threshold */

#define PACREG_23US     (0x58) /* OOK TX PA ramp: 23 uS */
#define PACREG_15US     (0x50)
#define PACREG_8_5US    (0x48)
#define PACREG_3US      (0x40)

#define FTXRXI_P_IRQ0RX_SYNC            (3 << 6)
#define FTXRXI_P_IRQ0RX_NFIFOEMPTY      (2 << 6)
#define FTXRXI_P_IRQ0RX_WRITEBYTE       (1 << 6)
#define FTXRXI_P_IRQ0RX_PLREADY         (0 << 6)
#define FTXRXI_P_IRQ1RX_FIFO_THRESHOLD  (3 << 4)
#define FTXRXI_P_IRQ1RX_RSSI            (2 << 4)
#define FTXRXI_P_IRQ1RX_FIFOFULL        (1 << 4)
#define FTXRXI_P_IRQ1RX_CRCOK           (0 << 4)
#define FTXRXI_P_IRQ1TX_TXDONE          (1 << 3)
#define FTXRXI_P_IRQ1TX_FIFOFULL        (0 << 3)
#define FTXRXI_FIFO_FULL                (1 << 2)
#define FTXRXI_FIFO_EMPTY               (1 << 1)
#define FTXRXI_FIFO_OVERRUN             (1 << 0)

#define FTPRI_TX_DONE                   (1 << 5)
#define FTPRI_TX_ON_FIFO_NOT_EMPTY      (1 << 4)
#define FTPRI_TX_ON_FIFO_THRESHOLD      (0 << 4)
#define FTPRI_SBO                       (1 << 3) /* must be one */
#define FTPRI_RSSI_ABOVE_THRESHOLD      (1 << 2)
#define FTPRI_PLL_LOCKED                (1 << 1)
#define FTPRI_PLL_LOCK_ENABLE           (1 << 0)

#define FILC_PAS_987KHZ                 (15 << 4)
#define FILC_PAS_676KHZ                 (14 << 4)
#define FILC_PAS_514KHZ                 (13 << 4)
#define FILC_PAS_458KHZ                 (12 << 4)
#define FILC_PAS_414KHZ                 (11 << 4)
#define FILC_PAS_378KHZ                 (10 << 4)
#define FILC_PAS_321KHZ                 (9 << 4)
#define FILC_PAS_262KHZ                 (8 << 4)
#define FILC_PAS_234KHZ                 (7 << 4)
#define FILC_PAS_211KHZ                 (6 << 4)
#define FILC_PAS_184KHZ                 (5 << 4)
#define FILC_PAS_157KHZ                 (4 << 4)
#define FILC_PAS_137KHZ                 (3 << 4)
#define FILC_PAS_109KHZ                 (2 << 4)
#define FILC_PAS_82KHZ                  (1 << 4)
#define FILC_PAS_65KHZ                  (0 << 4)
#define FILC_BUT(n)                     ((n) & 0xF)

#define SYNC_POLFILEN                   (1 << 7)
#define SYNC_BSYNCEN                    (1 << 6)
#define SYNC_SYNCREN                    (1 << 5)
#define SYNC_32BIT                      (3 << 3)
#define SYNC_24BIT                      (2 << 3)
#define SYNC_16BIT                      (1 << 3)
#define SYNC_8BIT                       (0 << 3)
#define SYNC_3ERR                       (3 << 1)
#define SYNC_2ERR                       (2 << 1)
#define SYNC_1ERR                       (1 << 1)
#define SYNC_0ERR                       (0 << 1)

#define TXCON_TXIPOLFV(n)               (((n) & 15) << 4)
#define TXCON_N8DBM                     (7 << 1) /* -8dBm */
#define TXCON_N5DBM                     (6 << 1) /* -5dBm */
#define TXCON_N2DBM                     (5 << 1) /* -2dBm */
#define TXCON_1DBM                      (4 << 1) /*  1dBm */
#define TXCON_4DBM                      (3 << 1) /*  4dBm */
#define TXCON_7DBM                      (2 << 1) /*  7dBm */
#define TXCON_10DBM                     (1 << 1) /* 10dBm */
#define TXCON_13DBM                     (0 << 1) /* 13dBm */

#define PLOAD_MCHSTREN                  (1 << 7)

#define PKTC_VARIABLE                   (1 << 7)
#define PKTC_FIXED                      (0 << 7)
#define PKTC_PREAMBLE_4B                (3 << 5)
#define PKTC_PREAMBLE_3B                (2 << 5)
#define PKTC_PREAMBLE_2B                (1 << 5)
#define PKTC_PREAMBLE_1B                (0 << 5)
#define PKTC_WHITENING                  (1 << 4)
#define PKTC_CRC_ENABLE                 (1 << 3)
#define PKTC_ADDR_NODE_OR_00_OR_FF      (3 << 1)
#define PKTC_ADDR_NODE_OR_00            (2 << 1)
#define PKTC_ADDR_NODE                  (1 << 1)
#define PKTC_ADDR_OFF                   (0 << 1)
#define PKTC_CRC_OK                     (1 << 0)

#define FCRC_AUTO_CLEAR                 (1 << 7) /* clear if CRC bad */
#define FCRC_FIFO_STANDBY_READ          (1 << 6)
#define FCRC_FIFO_STANDBY_WRITE         (0 << 5)

// Channel spacing ???
/* common freq values for 12.8 MHz crystal */
/* freq = 200kHz * (fXtal / 12.8 MHz) * ((val + 1) / 8) */
#define BW_400KHZ                         (0xF)
#define BW_375KHZ                         (0xE)
#define BW_350KHZ                         (0xD)
#define BW_325KHZ                         (0xC)
#define BW_300KHZ                         (0xB)
#define BW_275KHZ                         (0xA)
#define BW_250KHZ                         (0x9)
#define BW_225KHZ                         (0x8)
#define BW_200KHZ                         (0x7)
#define BW_175KHZ                         (0x6)
#define BW_150KHZ                         (0x5)
#define BW_125KHZ                         (0x4)
#define BW_100KHZ                         (0x3)
#define BW_75KHZ                          (0x2)
#define BW_50KHZ                          (0x1)
#define BW_25KHZ                          (0x0)


//Define the R, P, S sets for different channels
#define CHANNEL1_P_VALUE			104
#define CHANNEL2_P_VALUE			93
#define CHANNEL3_P_VALUE			123
#define CHANNEL4_P_VALUE			112
#define CHANNEL5_P_VALUE			116
#define CHANNEL6_P_VALUE			105
#define CHANNEL7_P_VALUE			109
#define CHANNEL8_P_VALUE			98
#define CHANNEL9_P_VALUE			102
#define CHANNEL10_P_VALUE			106
#define CHANNEL11_P_VALUE			110
#define CHANNEL12_P_VALUE			95
#define CHANNEL13_P_VALUE			122
#define CHANNEL14_P_VALUE			126
#define CHANNEL15_P_VALUE			88
#define CHANNEL16_P_VALUE			96
#define CHANNEL17_P_VALUE			123
#define CHANNEL18_P_VALUE			104
#define CHANNEL19_P_VALUE			112
#define CHANNEL20_P_VALUE			120
#define CHANNEL21_P_VALUE			97


#define CHANNEL1_R_VALUE			131
#define CHANNEL2_R_VALUE			117
#define CHANNEL3_R_VALUE			155
#define CHANNEL4_R_VALUE			141
#define CHANNEL5_R_VALUE			146
#define CHANNEL6_R_VALUE			132
#define CHANNEL7_R_VALUE			137
#define CHANNEL8_R_VALUE			123
#define CHANNEL9_R_VALUE			128
#define CHANNEL10_R_VALUE			133
#define CHANNEL11_R_VALUE			138
#define CHANNEL12_R_VALUE			119
#define CHANNEL13_R_VALUE			153
#define CHANNEL14_R_VALUE			158
#define CHANNEL15_R_VALUE			110
#define CHANNEL16_R_VALUE			117
#define CHANNEL17_R_VALUE			154
#define CHANNEL18_R_VALUE			130
#define CHANNEL19_R_VALUE			140
#define CHANNEL20_R_VALUE			150
#define CHANNEL21_R_VALUE			121

#define CHANNEL1_S_VALUE			28
#define CHANNEL2_S_VALUE			17
#define CHANNEL3_S_VALUE			47
#define CHANNEL4_S_VALUE			36
#define CHANNEL5_S_VALUE			40
#define CHANNEL6_S_VALUE			29
#define CHANNEL7_S_VALUE			33
#define CHANNEL8_S_VALUE			22
#define CHANNEL9_S_VALUE			26
#define CHANNEL10_S_VALUE			30
#define CHANNEL11_S_VALUE			34
#define CHANNEL12_S_VALUE			19
#define CHANNEL13_S_VALUE			46
#define CHANNEL14_S_VALUE			50
#define CHANNEL15_S_VALUE			12
#define CHANNEL16_S_VALUE			20
#define CHANNEL17_S_VALUE			47
#define CHANNEL18_S_VALUE			28
#define CHANNEL19_S_VALUE			36
#define CHANNEL20_S_VALUE			44
#define CHANNEL21_S_VALUE			21



#endif /* MRF89XA_H_ */
