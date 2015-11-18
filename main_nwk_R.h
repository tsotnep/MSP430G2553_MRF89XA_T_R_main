/*
 * main.h
 *
 *  Created  on: 06.02.2015
 *  Author: Mairo Leier, Maksim Gorev
 *
 *  Version: 0.3		15.10.2015
 *  Receive
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
//uint8 exit_code = 0;		// Exit code number that is set after function exits
uint8 payload_length;


/***************************************************************************************************
 *         Main section                                                                            *
 ***************************************************************************************************/
void main_nwk_R(void) {
	uint8 cntr, len, var;
	uint8 rssi_env, rssi_rx;

	// Initialize system
	Print_Error(System_Init());

	while (1) {

		// Clear received packet buffer
		for (var = RF_BUFFER_SIZE-1; var > 0; var--)
				RxPacket[var] = 0;

		// Set radio into RX mode
		Radio_Set_Mode(RADIO_RX);

		// Enter to LPM0 w/ interrupts enabled
		// Module stays in low power mode until interrupt is received from radio and wakes up
		__bis_SR_register(LPM0_bits + GIE);

		// Receive data, wait until 100ms until timeout and continue with other tasks
		if (exit_code = Radio_Receive_Data(RxPacket, &len, 100, &rssi_rx)) {
			Print_Error(exit_code);
		} else {

			// Toggle LED1 when data is received
			LED1_TOGGLE();

			UART_Send_Data("\r\nPacket length: ");
			UART0_Send_ByteToChar(&(RxPacket[0]));

			UART_Send_Data("\r\nDestination address: ");
			UART0_Send_ByteToChar(&(RxPacket[1]));

			UART_Send_Data("\r\nSource address: ");
			UART0_Send_ByteToChar(&(RxPacket[2]));

			UART_Send_Data("\r\nMessage: ");
			for (var = 5; var < len; ++var) {
				UART0_Send_ByteToChar(&(RxPacket[var]));
			}

			// Print the RECEIVED_SIGNAL_SRENGTH
			UART_Send_Data("\r\nSignal dtrength:");
			UART0_Send_ByteToChar(&(rssi_rx));

			// Get environemnt RSSI value (noise level)
			rssi_env = Radio_Get_RSSI();
			// Print the NOISE LEVEL
			UART_Send_Data("\r\nEnvironment noise level:");
			UART0_Send_ByteToChar(&(rssi_env));

			UART_Send_Data("\r\n");		// Insert new line to separate packets
		}
	}
}		/* END: main */



// *************************************************************************************************
// @fn          Print_Error
// @brief       Print code that is set when function exits. If error code is 0 then nothin is prnted
//				out and packet is received or sent correctly
// @param       uint8 error_code		Error code number that is set by the function
// @return      none
// *************************************************************************************************
//void Print_Error(uint8 error_code) {
//
//	// Print out error code only if it is not 0
//	if (error_code) {
//		UART_Send_Data("Error code: ");
//		UART0_Send_ByteToChar(&error_code);
//		UART_Send_Data("\r\n");
//		error_code = 0;
//	}
//
//	// If packet size has wrong length then reset radio module
//	if (error_code == ERR_RX_WRONG_LENGTH) {
//		Radio_Init(RF_DATA_RATE, TX_POWER, RF_CHANNEL);
//	}
//
//	exit_code = 0;
//}

