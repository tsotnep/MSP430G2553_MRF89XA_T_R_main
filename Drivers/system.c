/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include "system.h"
#include "network.h"
#include "uart.h"
#include "spi.h"
#include "radio.h"

/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/


// *************************************************************************************************
// @fn          Init_System
// @brief       Initialize system
// @param       none
// @return      none
// *************************************************************************************************
uint8 System_Init(void) {
	uint8 exit_code = 0;

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// Configure CPU clock
	System_Set_Speed(SYSTEM_SPEED_MHZ);

	// Ports
	LED_PORT_DIR |=  (LED1);				//Set LED1 pin as output
	LED_PORT_OUT &= ~(LED1);				// Set LED1 pin to LOW
	RF_RESET_OUT();							// Set RF module reset pin as output

	// Communication
	UART_Init();		//
	SPI_Init();			// RF module interface

	// RF Module

	if (exit_code = Radio_Init(RF_DATA_RATE, TX_POWER, RF_CHANNEL))		// Initialize RF module with 2kbps speed
		return exit_code;

	if (exit_code = Radio_Set_Channel(RF_CHANNEL))
		return exit_code;

	__enable_interrupt();

	return EXIT_NO_ERROR;
}


// *************************************************************************************************
// @fn          System_Set_Speed
// @brief       Initialize CPU with selected speed
// @param       uint8 mhz		MCU speed in MHz. Available options:
//									SYSTEM_SPEED_1MHZ
//									SYSTEM_SPEED_8MHZ
//									SYSTEM_SPEED_12MHZ
//									SYSTEM_SPEED_16MHZ
// @return      none
// *************************************************************************************************
void System_Set_Speed(uint8 mhz) {
	uint8 CALBC1_MHZ, CALDCO_MHZ;

	switch (mhz) {
	case SYSTEM_SPEED_1MHZ:
		CALBC1_MHZ = CALBC1_1MHZ;
		CALBC1_MHZ = CALBC1_1MHZ;
		CALDCO_MHZ = CALDCO_1MHZ;
		break;
	case SYSTEM_SPEED_8MHZ:
		CALBC1_MHZ = CALBC1_8MHZ;
		CALBC1_MHZ = CALBC1_8MHZ;
		CALDCO_MHZ = CALDCO_8MHZ;
		break;
	case SYSTEM_SPEED_12MHZ:
		CALBC1_MHZ = CALBC1_12MHZ;
		CALBC1_MHZ = CALBC1_12MHZ;
		CALDCO_MHZ = CALDCO_12MHZ;
		break;
	case SYSTEM_SPEED_16MHZ:
		CALBC1_MHZ = CALBC1_16MHZ;
		CALBC1_MHZ = CALBC1_16MHZ;
		CALDCO_MHZ = CALDCO_16MHZ;
		break;
	}

	// Configure CPU clock
	if (CALBC1_MHZ == 0xFF) {		    // If calibration constant erased
		while (1);                      // do not load, trap CPU
	}
	DCOCTL = 0;                         // Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_MHZ;              // Set DCO
	DCOCTL = CALDCO_MHZ;
}


// *************************************************************************************************
// @fn          cByteToHex
// @brief       Convert byte to char
// @param
// @return      uint8 result            Outputs char
// *************************************************************************************************
int cByteToHex(char input) {
  char result = 0;
  switch (input) {
    case 0x00:  result = '0';   break;
    case 0x01:  result = '1';   break;
    case 0x02:  result = '2';   break;
    case 0x03:  result = '3';   break;
    case 0x04:  result = '4';   break;
    case 0x05:  result = '5';   break;
    case 0x06:  result = '6';   break;
    case 0x07:  result = '7';   break;
    case 0x08:  result = '8';   break;
    case 0x09:  result = '9';   break;

    case 0x0A:  result = 'A';   break;
    case 0x0B:  result = 'B';   break;
    case 0x0C:  result = 'C';   break;
    case 0x0D:  result = 'D';   break;
    case 0x0E:  result = 'E';   break;
    case 0x0F:  result = 'F';   break;
  }
  return result;
}


// *************************************************************************************************
// @fn          PORT2_VECTOR
// @brief       Port2 interrupt vectors
// @param		none
// @return      none
// *************************************************************************************************
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {

	if (RF_IRQ_PORT_IN & RF_IRQ0_PIN) {
		RF_IRQ0_IFG();		// Clear interrupt flag; IRQ0TXST - transmit start with IRQ0 bit
		_NOP();
	}

	if (RF_IRQ_PORT_IN & RF_IRQ1_PIN) {
		RF_IRQ1_IFG();		// Clear interrupt flag; TXDONE
		_NOP();
	}

	// Wake up from low power mode
	__bic_SR_register_on_exit(LPM0_bits);
}
