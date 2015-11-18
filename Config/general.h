#ifndef MSP430G2553_MRF89XA_T_R_MAIN_CONFIG_GENERAL_H_
#define MSP430G2553_MRF89XA_T_R_MAIN_CONFIG_GENERAL_H_

/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/

/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/

/********* DEBUG **********/
#define DEBUG				0		// Send all information to the UART
#define DEBUG_RF			0		// Send all RF related data to the UART
#define DEBUG_ERR			0		// Send only error information to the UART


/********* SYSTEM **********/

/* System speed settings. Available options:
 * SYSTEM_SPEED_1MHZ
 * SYSTEM_SPEED_8MHZ
 * SYSTEM_SPEED_12MHZ
 * SYSTEM_SPEED_16MHZ */
#define SYSTEM_SPEED_MHZ	SYSTEM_SPEED_8MHZ		// MCU speed in MHz. Can be between 1 to 16 MHz


#endif 
