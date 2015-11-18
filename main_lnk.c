/*
 * main.h
 *
 *  Created on: 06.02.2015
 *  Modifiedon: 10.04.2015
 *  Author: Mairo Leier, Maksim Gorev
 *
 *  Version: 0.2.1		09.10.2015
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
uint8 error;
uint8 payload_length;
uint8 exit_code = 0;		// Exit code number that is set after function exits


/***************************************************************************************************
 *         Main section                                                                            *
 ***************************************************************************************************/
void main(void) {
	error = 0;
	uint8 cntr;


	// Initialize system speed, communication interfaces and RF radio module
	exit_code = System_Init();
	Print_Error(exit_code);

	while (1) {

		payload_length = 0;

		// Construct the packet
		TxPacket[payload_length++] = 0x01;
		TxPacket[payload_length++] = 0x02;
		TxPacket[payload_length++] = 0x03;

		// Send data over radio
		exit_code = Radio_Tx(TxPacket, payload_length, ADDR_REMOTE);

		// Print out packet that was sent
		UART_Send_Data("\r\nSending:");
		for (cntr = 0; cntr < payload_length; cntr++) {
			UART0_Send_ByteToChar(&TxPacket[cntr]);
		}

		// Add some delay (around 1sec)
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

