/*
 * main.h
 *
 *  Created  on: 06.02.2015
 *  Author: Mairo Leier, Maksim Gorev
 *
 *  Version: 0.3		15.10.2015
 *  Transmit
 */
/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/

#include "system.h"
#include "network.h"
#include "general.h"

// Drivers
#include "uart.h"
#include "spi.h"
#include "radio.h"

// Network
#include "nwk_security.h"
#include "nwk_radio.h"

/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
void Print_Error(uint8 error_code);

/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
uint8 exit_code = 0;		// Exit code number that is set after function exits
uint8 payload_length;


/***************************************************************************************************
 *         Main section                                                                            *
 ***************************************************************************************************/
void main_nwk_T(void) {
	uint8 cntr = 0;

	// Initialize system
	Print_Error(System_Init());

	while (1) {

		payload_length = 0;

		// Construct the packet
		// This is the place where you can put your own data to send
		TxPacket[payload_length++] = PKT_CTRL | PKT_CTRL_REQUEST;
		TxPacket[payload_length++] = PKT_TYPE_VOLTAGE;
		TxPacket[payload_length++] = 0x23;
		TxPacket[payload_length++] = 0x17;

		// Print out all data that we are goind to send
		UART_Send_Data("\r\nPayload length:\t");
		UART0_Send_ByteToChar(&payload_length);

		UART_Send_Data("\r\nMessage: ");
		for (cntr = 1; cntr < payload_length; cntr++) {
			UART0_Send_ByteToChar(&TxPacket[cntr]);
		}

		UART_Send_Data("\r\n");		// Insert new line to separate packets

		// Send data over RF and get the exit code to check for errors
		exit_code = Radio_Send_Data(TxPacket, payload_length, ADDR_REMOTE, PAYLOAD_ENC_ON, PCKT_ACK_ON);

		// Add some delay (around 2sec)
		__delay_cycles(5000000*SYSTEM_SPEED_MHZ);

	}
}		/* END: main */


// *************************************************************************************************
// @fn          Print_Error
// @brief       Print code that is set when function exits. If error code is 0 then nothin is prnted
//				out and packet is received or sent correctly
// @param       uint8 error_code		Error code number that is set by the function
// @return      none
// *************************************************************************************************
void Print_Error(uint8 error_code) {

	// Print out error code only if it is not 0
	if (error_code) {
		UART_Send_Data("Error code: ");
		UART0_Send_ByteToChar(&error_code);
		UART_Send_Data("\r\n");
		error_code = 0;
	}

	// If packet size has wrong length then reset radio module
	if (error_code == ERR_RX_WRONG_LENGTH) {
		Radio_Init(RF_DATA_RATE, TX_POWER, RF_CHANNEL);
	}

	exit_code = 0;
}

