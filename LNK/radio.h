#ifndef RADIO_H_
#define RADIO_H_

/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include "mrf89xa.h"
#include "system.h"


/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/
#define RF_BUFFER_SIZE			64
#define PAYLOAD_MAX_SIZE		60
#define PAYLOAD_MAX_ENC_SIZE	16		//Encrypted payload can be up to 3x16 = 48 bytes but currently
										// automatic packet cutting is not yet implemented


#define RF_DATA_RATE_2			2000
#define RF_DATA_RATE_10			10000
#define RF_DATA_RATE_33			33333
#define RF_DATA_RATE_100		100000


// RF Channels
#define RF_CHANNEL_MAX_NUM		21

// IRQ definitions
#define RF_IRQ0_IN()  		{ RF_IRQ_PORT_DIR  &= ~RF_IRQ0_PIN; }
#define RF_IRQ1_IN()  		{ RF_IRQ_PORT_DIR  &= ~RF_IRQ1_PIN; }

// Enable interrupt
#define RF_IRQ0_IE()  		{ RF_IRQ_PORT_IE  |= RF_IRQ0_PIN; }
#define RF_IRQ1_IE()  		{ RF_IRQ_PORT_IE  |= RF_IRQ1_PIN; }

// Low to high edge
#define RF_IRQ0_IES()  		{ RF_IRQ_PORT_IES  &= ~RF_IRQ0_PIN; }
#define RF_IRQ1_IES()  		{ RF_IRQ_PORT_IES  &= ~RF_IRQ1_PIN; }

// Clear interrupt
#define RF_IRQ0_IFG()  		{ RF_IRQ_PORT_IFG  &= ~RF_IRQ0_PIN; }
#define RF_IRQ1_IFG()  		{ RF_IRQ_PORT_IFG  &= ~RF_IRQ1_PIN; }

// Pulldown resistor
#define RF_IRQ0_REN()  		{ RF_IRQ_PORT_REN  &= ~RF_IRQ0_PIN; }
#define RF_IRQ1_REN()  		{ RF_IRQ_PORT_REN  &= ~RF_IRQ1_PIN; }

// RF Module RESET pin
#define RF_RESET_OUT() 		{ RF_RESET_PORT_DIR |= RF_RESET_PIN; }
#define RF_RESET() 			{ RF_RESET_PORT_OUT |= RF_RESET_PIN; \
								__delay_cycles(20000); \
								RF_RESET_PORT_OUT &= ~RF_RESET_PIN;}
							// Keep RESET pin high for at least 100us


// Radio modes
#define RADIO_STANDBY		1
#define RADIO_SLEEP			2
#define RADIO_RX			3

/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
//uint8 Radio_Init(uint32 bps, uint8 power, uint8 channel, uint8* error);
uint8 Radio_Init(uint32 bps, uint8 power, uint8 channel);
//uint8 Radio_Tx(uint8 packet[], uint8 length, uint8 destination, uint8 *error);
uint8 Radio_Tx(uint8 packet[], uint8 length, uint8 destination);
//uint8 Radio_Rx(uint8 packet[], uint8 *length,  uint16 timeout_ms, uint8 *rssi, uint8 *error);
uint8 Radio_Rx(uint8 packet[], uint8 *length,  uint16 timeout_ms, uint8 *rssi);
uint8 Radio_Set_Channel(uint8 channel);
uint8 Radio_Set_TxPower(uint8 power);
uint8 Radio_Set_Mode(uint8 mode);

uint8 Radio_Get_RSSI(void);
uint8 Radio_Calculate_RSSI(uint8 rssi);


/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
uint8 TxPacket[RF_BUFFER_SIZE];
uint8 RxPacket[RF_BUFFER_SIZE];
uint8 rssi_rx;


#endif /* RADIO_H_ */
