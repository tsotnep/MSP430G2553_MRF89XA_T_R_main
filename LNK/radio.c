/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include "system.h"
#include "network.h"
#include "radio.h"
#include "uart.h"
#include "spi.h"

/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
uint8 _Add_Address_To_Packet(uint8 *packet, uint8 length, uint8 destination);

/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
const unsigned char vcotune = 0;

const uint8 P_VALUE_CH_[] = {
		CHANNEL1_P_VALUE,  CHANNEL2_P_VALUE,  CHANNEL3_P_VALUE,  CHANNEL4_P_VALUE,  CHANNEL5_P_VALUE,  CHANNEL6_P_VALUE,
		CHANNEL7_P_VALUE,  CHANNEL8_P_VALUE,  CHANNEL9_P_VALUE,  CHANNEL10_P_VALUE, CHANNEL11_P_VALUE, CHANNEL12_P_VALUE,
		CHANNEL13_P_VALUE, CHANNEL14_P_VALUE, CHANNEL15_P_VALUE, CHANNEL16_P_VALUE, CHANNEL17_P_VALUE, CHANNEL18_P_VALUE,
		CHANNEL19_P_VALUE, CHANNEL20_P_VALUE, CHANNEL21_P_VALUE};

const uint8 R_VALUE_CH_[] = {
		CHANNEL1_R_VALUE,  CHANNEL2_R_VALUE,  CHANNEL3_R_VALUE,  CHANNEL4_R_VALUE,  CHANNEL5_R_VALUE,  CHANNEL6_R_VALUE,
		CHANNEL7_R_VALUE,  CHANNEL8_R_VALUE,  CHANNEL9_R_VALUE,  CHANNEL10_R_VALUE, CHANNEL11_R_VALUE, CHANNEL12_R_VALUE,
		CHANNEL13_R_VALUE, CHANNEL14_R_VALUE, CHANNEL15_R_VALUE, CHANNEL16_R_VALUE, CHANNEL17_R_VALUE, CHANNEL18_R_VALUE,
		CHANNEL19_R_VALUE, CHANNEL20_R_VALUE, CHANNEL21_R_VALUE};

const uint8 S_VALUE_CH_[] = {
		CHANNEL1_S_VALUE,  CHANNEL2_S_VALUE,  CHANNEL3_S_VALUE,  CHANNEL4_S_VALUE,  CHANNEL5_S_VALUE,  CHANNEL6_S_VALUE,
		CHANNEL7_S_VALUE,  CHANNEL8_S_VALUE,  CHANNEL9_S_VALUE,  CHANNEL10_S_VALUE, CHANNEL11_S_VALUE, CHANNEL12_S_VALUE,
		CHANNEL13_S_VALUE, CHANNEL14_S_VALUE, CHANNEL15_S_VALUE, CHANNEL16_S_VALUE, CHANNEL17_S_VALUE, CHANNEL18_S_VALUE,
		CHANNEL19_S_VALUE, CHANNEL20_S_VALUE, CHANNEL21_S_VALUE};


// *************************************************************************************************
// @fn          Radio_Init
// @brief       Initialize 868MHz MRF89XA RF Module
// @param       uint32 bps              Data transfer speed
//              uint8  power            TX Power in dBm, allowed range: -8..+13dBm
//              uint8  channel          Channel number, allowed range: 1..21
//              uint8  *error           Error code in case of error. Possible error codes are:
//                         ERR_RF_FREQ_FIX_TIMEOUT
// @return      uint8  EXIT_ERROR       In case of error (error code returned to error)
//                     EXIT_NO_ERROR    In case of success
// *************************************************************************************************
//uint8 Radio_Init(uint32 bps, uint8 power, uint8 channel, uint8* error) {
uint8 Radio_Init(uint32 bps, uint8 power, uint8 channel) {

	uint8 n;
	uint16 timeout = 0xFFFF;

	RF_RESET();		// Reset RF module

	// IRQ0 - RX:SYNC WORD / TX: not used
	// IRQ1 - RX:CRC OK / TX:TXDONE interrupt
	// Used to wake up MCU and start reading RSSI during RF data receiving

	// Set pin as input
	RF_IRQ0_IN();
	RF_IRQ1_IN();

	// Enable pull-up resistor
	RF_IRQ0_REN();
	RF_IRQ1_REN();

	// Set interrupt edge LOW to HIGH edge
	RF_IRQ0_IES();
	RF_IRQ1_IES();

	// Enable interrupt
	RF_IRQ0_IE();
	RF_IRQ1_IE();

	// Clear pending interrupts
	RF_IRQ0_IFG();
	RF_IRQ1_IFG();

	__delay_cycles(150000); // wait for MRF89XA POR for 10ms

	/******* Configure RF Module  *******/
	SPI_Conf_Write_Register(MRF89_REG_GCONREG, GCON_STANDBY | GCON_863_BAND | GCON_RPS_1);

	// FSK modulation, DMODE1 and DMODE0 enabled
	SPI_Conf_Write_Register(MRF89_REG_DMODREG, DMOD_FSK | DMOD_IF_0DB | DMOD_PACKET);

	// FIFO 64 bytes, treshold 1 byte
	SPI_Conf_Write_Register(MRF89_REG_FIFOCREG, FIFOC_64B | FIFOC_FTINT(0x3F));

	// Set radio channel
	SPI_Conf_Write_Register(MRF89_REG_R1CREG, R_VALUE_CH_[channel]);
	SPI_Conf_Write_Register(MRF89_REG_P1CREG, P_VALUE_CH_[channel]);
	SPI_Conf_Write_Register(MRF89_REG_S1CREG, S_VALUE_CH_[channel]);

	/* RX: IRQ0=SYNC, IRQ1=CRCOK TX: IRQ1=TXDONE */
	SPI_Conf_Write_Register(MRF89_REG_FTXRXIREG, FTXRXI_P_IRQ0RX_SYNC | FTXRXI_P_IRQ1RX_CRCOK | FTXRXI_P_IRQ1TX_TXDONE);

	/* based on table A-1, MRF89XA datasheet */

	if (bps == 2000) {
		SPI_Conf_Write_Register(MRF89_REG_FILCREG, FILC_PAS_157KHZ | FILC_BUT(BW_50KHZ));
		//SPI_Conf_Write_Register(MRF89_REG_FILCREG, FILC_PAS_157KHZ | FILC_BUT(BW_400KHZ));
		SPI_Conf_Write_Register(MRF89_REG_BRSREG, 99);   /* 12.8MHz / (64 * (99 + 1)) -> 2kbps */
		SPI_Conf_Write_Register(MRF89_REG_FDEVREG, 11);   /* 12.8MHz / (32 * (11 + 1)) -> 33.333KHz */

	} else if (bps == 10000) {
		//SPI_Conf_Write_Register(MRF89_REG_FILCREG, FILC_PAS_157KHZ | FILC_BUT(BW_50KHZ));
		SPI_Conf_Write_Register(MRF89_REG_FILCREG, FILC_PAS_157KHZ | FILC_BUT(BW_400KHZ));
		SPI_Conf_Write_Register(MRF89_REG_BRSREG, 19);   /* 12.8MHz / (64 * (19 + 1)) -> 10kbps */
		SPI_Conf_Write_Register(MRF89_REG_FDEVREG, 11);   /* 12.8MHz / (32 * (11 + 1)) -> 33.333KHz */

	} else if (bps == 33333) {
		//SPI_Conf_Write_Register(MRF89_REG_FILCREG, FILC_PAS_378KHZ | FILC_BUT(BW_125KHZ));
		SPI_Conf_Write_Register(MRF89_REG_FILCREG, FILC_PAS_378KHZ | FILC_BUT(BW_400KHZ));
		SPI_Conf_Write_Register(MRF89_REG_BRSREG, 5);			/* 12.8MHz / (64 * (5 + 1)) -> 33.333kbps */
		SPI_Conf_Write_Register(MRF89_REG_FDEVREG, 5);			/* 12.8MHz / (32 * (5 + 1)) -> 66.666KHz */

	} else if (bps == 100000) {
		SPI_Conf_Write_Register(MRF89_REG_FILCREG, FILC_PAS_987KHZ | FILC_BUT(BW_400KHZ));
		SPI_Conf_Write_Register(MRF89_REG_BRSREG, 1);			/* 12.8MHz / (64 * (1 + 1)) -> 100kbps */
		SPI_Conf_Write_Register(MRF89_REG_FDEVREG, 3);			/* 12.8MHz / (32 * (3 + 1)) -> 100KHz */
	} else {
		return ERR_UNKNOWN_RF_SPEED;
	}

	/* enable SYNC gen/rec, 32 bits, 0 errors accepted */
	SPI_Conf_Write_Register(MRF89_REG_SYNCREG, SYNC_SYNCREN | SYNC_32BIT | SYNC_0ERR);

	// SYNC values
	SPI_Conf_Write_Register(MRF89_REG_SYNCV31REG, NETWORK_ADDR1);
	SPI_Conf_Write_Register(MRF89_REG_SYNCV23REG, NETWORK_ADDR2);
	SPI_Conf_Write_Register(MRF89_REG_SYNCV15REG, NETWORK_ADDR3);
	SPI_Conf_Write_Register(MRF89_REG_SYNCV07REG, NETWORK_ADDR4);

	// Set TX Power
	//SPI_Conf_Write_Register(MRF89_REG_TXCONREG, TXCON_TXIPOLFV(BW_200KHZ) | power);
	Radio_Set_TxPower(power);

	// Clock output disabled
	SPI_Conf_Write_Register(MRF89_REG_CLKOUTREG, 0x00);


	//variable-length format, preamble=2, whiteon=1, crc_check_ena ,  address_filtering node & 00(broadcast)
	SPI_Conf_Write_Register(MRF89_REG_PLOADREG, 64); // maximum FIFO length in case of variable length packet
	SPI_Conf_Write_Register(MRF89_REG_PKTCREG, PKTC_VARIABLE | PKTC_PREAMBLE_4B | PKTC_WHITENING | PKTC_ADDR_NODE_OR_00 | PKTC_CRC_ENABLE);
	SPI_Conf_Write_Register(MRF89_REG_NADDSREG, ADDR_LOCAL);


	// Enable FIFO read access
	SPI_Conf_Write_Register(MRF89_REG_FCRCREG, FCRC_AUTO_CLEAR | FCRC_FIFO_STANDBY_READ);

	SPI_Conf_Write_Register(MRF89_REG_FTPRIREG, FTPRI_SBO | FTPRI_PLL_LOCKED | FTPRI_PLL_LOCK_ENABLE);

	// Set radio into freq synth mode
	SPI_Conf_Write_Register(MRF89_REG_GCONREG, (vcotune << 1) | GCON_FREQ_SYNTH | GCON_863_BAND | GCON_RPS_1);

	// Wait for frequency fixing
	for (;;) {
		if (!(timeout--)) {
			//*error = ERR_RF_FREQ_FIX_TIMEOUT;
			return ERR_RF_FREQ_FIX_TIMEOUT;
		}
		n = SPI_Conf_Read_Register(MRF89_REG_FTPRIREG);
		if (n & FTPRI_PLL_LOCKED) {
			break;
		}
	}

	// Put radio back to sleep
	SPI_Conf_Write_Register(MRF89_REG_GCONREG, GCON_STANDBY | GCON_863_BAND | GCON_RPS_1);

	return EXIT_NO_ERROR;
}


// *************************************************************************************************
// @fn          Radio_Tx
// @brief       Transmit packet data over RF
// @param       uint8 packet[] 			data array to be sent over RF.
//				uint8 length 			length of packet (up to 64)
//				uint8 destination		Address of receiving node
// @return		uint8 	EXIT_ERROR 		In case of error (error code returned to error argument)
//						EXIT_NO_ERROR 	In case of success
// *************************************************************************************************
uint8 Radio_Tx(uint8 packet[], uint8 length, uint8 destination) {
	uint8 n;
	uint8 cntr;				// Counter for Tx buffer cleaning
	uint32 timeout = 0xFFFF;
	//uint32 timeout = SYSTEM_SPEED_MHZ*10*timeout_ms;		// Timeout in ms

	// Get new length of packet
	length = _Add_Address_To_Packet(packet, length, destination);

#if (DEBUG_RF)
	uint8 var;
	UART_Send_Data("\r\nLNK Radio TX data: ");
	for (var = 0; var < length; ++var) {
		UART0_Send_ByteToChar(&(packet[var]));
	}
#endif

	SPI_Conf_Write_Register(MRF89_REG_GCONREG, (vcotune << 1) | GCON_TX | GCON_863_BAND | GCON_RPS_1);
	SPI_Conf_Write_Register(MRF89_REG_FTPRIREG,	FTPRI_SBO | FTPRI_PLL_LOCK_ENABLE | FTPRI_TX_ON_FIFO_NOT_EMPTY);

	SPI_Data_Write(&length, 1);
	SPI_Data_Write(packet, length);

	// Wait until TX Done inerrupt comes
	for (;;) {
		if(	!(timeout--)){
#if (DEBUG_ERR)
			UART_Send_Data("\r\nLNK ERR: TX Timeout!");
#endif
			//*error = ERR_TX_TIMEOUT;
			SPI_Conf_Write_Register(MRF89_REG_GCONREG, (vcotune << 1) | GCON_STANDBY | GCON_863_BAND | GCON_RPS_1); // Put radio to standby
			return ERR_TX_TIMEOUT;
		}
		n = SPI_Conf_Read_Register(MRF89_REG_FTPRIREG);

		// Check if TX_DONE
		if (n & FTPRI_TX_DONE) {
			break;
		}
	}

	// Put radio back to standby
	SPI_Conf_Write_Register(MRF89_REG_GCONREG, (vcotune << 1) | GCON_STANDBY | GCON_863_BAND | GCON_RPS_1);

	// Clear Tx packet buffer
	cntr = 0;
	for (cntr=RF_BUFFER_SIZE; cntr > 0; cntr--)
		TxPacket[cntr] = 0;


	return EXIT_NO_ERROR;
}



// *************************************************************************************************
// @fn          Radio_Rx
// @brief       Receive data over RF
// @param       uint8  packet[]			byte sequence of length length to be received over RF
//    			uint8  length 			length of packet[] (up to 64)
//				uint32 timeout_ms		RX Timeout in ms
//										Small values (under 10) add small overhead
//										Timeout is needed because otherwise progam may stay forever
//										inside RX while loop
// @return		uint8  *rssi			Signal strength during packet receiving
//				uint8 	EXIT_ERROR 		In case of error(error code returned to error argument)
//						EXIT_NO_ERROR 	In case of success
// *************************************************************************************************
//uint8 Radio_Rx(uint8 packet[], uint8 *length, uint16 timeout_ms, uint8 *rssi, uint8 *error) {
uint8 Radio_Rx(uint8 packet[], uint8 *length, uint16 timeout_ms, uint8 *rssi) {
	uint8 var;
	uint8 rssi_tmp;
	*rssi = 0;		// Reset previous rssi value
	uint32 timeout = SYSTEM_SPEED_MHZ*10*timeout_ms;		// Timeout in ms

	// TODO: Check if radio is already in RX, if not then turn on
	SPI_Conf_Write_Register(MRF89_REG_GCONREG, (vcotune << 1) | GCON_RX | GCON_863_BAND | GCON_RPS_1);

	// INTERRUPT PIN CHECK
	while (!(RF_IRQ_PORT_IN & RF_IRQ1_PIN)) {
		rssi_tmp = SPI_Conf_Read_Register(MRF89_REG_RSTSREG);		// Get RSSI value
		// Get MAX rssi value
		if (rssi_tmp > *rssi)
			*rssi = rssi_tmp;

		// Timeout functionality
		if (!(timeout--)) {					// timeout
			//SPI_Conf_Write_Register(MRF89_REG_GCONREG, (vcotune << 1) | GCON_STANDBY | GCON_863_BAND | GCON_RPS_1);
			//*error = EXIT_TIMEOUT;
			return ERR_RX_TIMEOUT;
		}

	}


#if (DEBUG_RF)
	UART_Send_Data("\r\nLNK RX RSSI:");
	UART0_Send_ByteToChar(rssi);
	UART_Send_Data("\r\nLNK Radio RX data: ");
#endif

	var = 0;
	while (RF_IRQ_PORT_IN & RF_IRQ1_PIN) { // while data last read from RF buffer
		packet[var] = SPI_Data_Read();

#if (DEBUG_RF)
		// Print out raw received data to UART0
		UART0_Send_ByteToChar(&(packet[var]));
#endif

		var++;
		if (var > RF_BUFFER_SIZE) {

#if (DEBUG_ERR)
			UART_Send_Data("\r\nLNK ERR: RX Wrong length! Overflow");
#endif

			//*error = ERR_RX_WRONG_LENGTH;
			return ERR_RX_WRONG_LENGTH;
		}
	}
	//
	if (var == packet[0]+1) {
		*length = var;
	} else {

#if (DEBUG_ERR)
		UART_Send_Data("\r\nLNK ERR: RX wrong length! Not Equal");
#endif

		//*error = ERR_RX_WRONG_LENGTH;
		return ERR_RX_WRONG_LENGTH;

	}
	return EXIT_NO_ERROR;
}


// *************************************************************************************************
// @fn          Radio_Set_Channel
// @brief       This function set the operating channel for the RF transceiver
//				This is the primary interface for the protocol layer to
// 				set the operating frequency of the RF transceiver. Valid channel
//  			number are from 1 to 21. The center frequency is calculated as
// 				(LowestFrequency + Channel * ChannelGap + offsetFreq)
// @param		uint8 channel		Channel number.
//									Supported channels are: 1 to 21
// @return      uint8 EXIT_NO_ERROR
// *************************************************************************************************
uint8 Radio_Set_Channel(uint8 channel) {
	if( channel >= RF_CHANNEL_MAX_NUM ) {
		return ERR_UNKNOWN_CH_NO;
	}

	//Program registers R, P, S and Synthesize the RF
	SPI_Conf_Write_Register(MRF89_REG_R1CREG, R_VALUE_CH_[channel]);
	SPI_Conf_Write_Register(MRF89_REG_P1CREG, P_VALUE_CH_[channel]);
	SPI_Conf_Write_Register(MRF89_REG_S1CREG, S_VALUE_CH_[channel]);

	// Put radio back to sleep
	SPI_Conf_Write_Register(MRF89_REG_GCONREG, GCON_STANDBY | GCON_863_BAND | GCON_RPS_1);

	// Set radio into freq synth mode
	SPI_Conf_Write_Register(MRF89_REG_GCONREG, (vcotune << 1) | GCON_FREQ_SYNTH | GCON_863_BAND | GCON_RPS_1);

	SPI_Conf_Write_Register(MRF89_REG_FTPRIREG, FTPRI_SBO | FTPRI_PLL_LOCKED | FTPRI_PLL_LOCK_ENABLE);
	__delay_cycles(60000);

	// Put radio back to sleep
	SPI_Conf_Write_Register(MRF89_REG_GCONREG, GCON_STANDBY | GCON_863_BAND | GCON_RPS_1);

	return EXIT_NO_ERROR;
}


// *************************************************************************************************
// @fn          Radio_Set_TxPower
// @brief       Set TX power of radio module
// @param		uint8 power				Set radio power. Supported modes are:
//										TXCON_N8DBM		-8dBm
//										TXCON_N5DBM		-5dBm
// 										TXCON_N2DBM		-2dBm
// 										TXCON_1DBM		+1dBm
// 										TXCON_4DBM		+4dBm
// 										TXCON_7DBM		+7dBm
// 										TXCON_10DBM		+10dBm
// 										TXCON_13DBM		+13dBm
// @return      uint8 EXIT_NO_ERROR
// *************************************************************************************************
uint8 Radio_Set_TxPower(uint8 power) {
	SPI_Conf_Write_Register(MRF89_REG_TXCONREG, TXCON_TXIPOLFV(BW_200KHZ) | power);
	return EXIT_NO_ERROR;
}


// *************************************************************************************************
// @fn          Radio_Set_Mode
// @brief       Switch radio between different modes
// @param		uint8 mode              Set radio mode. Supported modes are:
//                      RADIO_STANDBY   Set radio into standby mode
//                      RADIO_SLEEP     Set radio into sleep mode
//                      RADIO_RX        Set radio into receive mode
// @return      uint8 EXIT_NO_ERROR     In case of success
//                    EXIT_ERROR        In case of error (error code returned to error)
// *************************************************************************************************
uint8 Radio_Set_Mode(uint8 mode) {
	switch (mode) {
	case RADIO_STANDBY:
		SPI_Conf_Write_Register(MRF89_REG_GCONREG, GCON_STANDBY | GCON_863_BAND | GCON_RPS_1);
		break;
	case RADIO_SLEEP:
		SPI_Conf_Write_Register(MRF89_REG_GCONREG, GCON_SLEEP | GCON_863_BAND | GCON_RPS_1);
		break;
	case RADIO_RX:
		SPI_Conf_Write_Register(MRF89_REG_GCONREG, (vcotune << 1) | GCON_RX | GCON_863_BAND | GCON_RPS_1);
		break;
	}
return EXIT_NO_ERROR;
}


// *************************************************************************************************
// @fn          Radio_Get_RSSI
// @brief       Get RSSI value from radio when in RX mode
// @param		none
// @return      uint8 rssi				Received environment RSSI value. Values can be translated
//										into dBm using Radio_Calculate_RSSI() function. More
//										information in MRF89XA datasheet, section 3.4.7.3
// *************************************************************************************************
uint8 Radio_Get_RSSI(void) {
	uint8 rssi_count;
	uint8 rssi = 0;
	uint16 rssi_tmp = 0;

	SPI_Conf_Write_Register(MRF89_REG_GCONREG, (vcotune << 1) | GCON_RX | GCON_863_BAND | GCON_RPS_1);

	for (rssi_count = 4; rssi_count > 0; rssi_count--) {
		rssi = SPI_Conf_Read_Register(MRF89_REG_RSTSREG);
		rssi_tmp += rssi;
	}
	rssi = (rssi_tmp>>2);

	// Put radio back to sleep
	SPI_Conf_Write_Register(MRF89_REG_GCONREG, GCON_STANDBY | GCON_863_BAND | GCON_RPS_1);

	return rssi;
}


// *************************************************************************************************
// @fn          Radio_Calculate_RSSI
// @brief       Calculate Radio RX RSSI value into dBm. Negative dBm values are represented as
//				positive values.
// @param		uint8 rssi				RSSI value from radio
// @return      uint8 dbm				Received environment RSSI value in dBm.
// *************************************************************************************************
uint8 Radio_Calculate_RSSI(uint8 rssi) {
	uint8 dbm;
	dbm=(uint8)((-10/19*rssi)-2220/19);
	return dbm;
}


// *************************************************************************************************
// @fn          _Add_Address_To_Packet
// @brief       Reallocates *packet and adds destination address to packet and returns new *packet size
// @param       uint8 *packet 				original data packet, where new packet is returned (!)
//				uint8  length 				Length of the packet
//				uint8  destination 			Address to be added to the beginning of the packet
// @return		uint8  length 				New length in case of success
//				uint8 *packet 				Packet is reallocated and filled with new packet
// *************************************************************************************************
uint8 _Add_Address_To_Packet(uint8 *packet, uint8 length, uint8 destination) {
	uint8 var;
	uint8 payload_offset = 2;	// Number of bytes to add to the packet => size of header

	for (var=length; var > 0; var--) {
		packet[var+payload_offset-1] = packet[var-1];
	}
	packet[0] = destination;
	packet[1] = ADDR_LOCAL;

	return length+payload_offset;
}
