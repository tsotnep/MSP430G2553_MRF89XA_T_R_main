/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include "uart.h"

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
// @fn          Init_UART
// @brief       Initialize UART
//									Pinout: TXD = P1.1		RXD = P1.2
// @param       none
// @return      none
// *************************************************************************************************
void UART_Init(void) {
	// Define UART ports
	UART_PORT_SEL = UART_RXD + UART_TXD;   		// P1.1 = RXD, P1.2=TXD
	UART_PORT_SEL2 = UART_RXD + UART_TXD;     	// P1.1 = RXD, P1.2=TXD

	// Configure UART
	UCA0CTL1 |= UCSSEL_2;                   	// SMCLK

	// Set UART speed settings based on system speed
	switch (SYSTEM_SPEED_MHZ) {
	case SYSTEM_SPEED_1MHZ:
		UCA0BR0 = 104;
		UCA0BR1 = 0;
		UCA0MCTL = UCBRS0;
		break;
	case SYSTEM_SPEED_8MHZ:
		UCA0BR0 = 65;
		UCA0BR1 = 3;
		UCA0MCTL = UCBRS1 + UCBRS2;
		break;
	case SYSTEM_SPEED_12MHZ:
		UCA0BR0 = 226;
		UCA0BR1 = 4;
		UCA0MCTL = UCBRS1 + UCBRS2;
		break;
	case SYSTEM_SPEED_16MHZ:
		UCA0BR0 = 130;
		UCA0BR1 = 6;
		UCA0MCTL = UCBRS1 + UCBRS2;
		break;
	}

	UCA0CTL1 &= ~UCSWRST;               		// **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          	// Enable USCI_A0 RX interrupt

	uart_buf_size = 0;
}



// *************************************************************************************************
// @fn          UART_Send_Byte
// @brief       Send char over UART
// @param       uint8 c				Char to send
// @return      none
// *************************************************************************************************
void UART_Send_Byte(unsigned char c) {
	while(!(IFG2 & UCA0TXIFG));
	UCA0TXBUF = c;
	while(UCA0STAT & UCBUSY);                 // Wait until the last byte is completely sent
	_NOP();
}


// *************************************************************************************************
// @fn          UART_Send_Data
// @brief       Send string over UART
// @param       const char str			String to send
// @return      none
// *************************************************************************************************
void UART_Send_Data(const char *str) {
	while (*str)
		UART_Send_Byte(*str++);
}


// *************************************************************************************************
// @fn          sendByteToChar
// @brief
// @param
// @return      none
// *************************************************************************************************
void UART0_Send_ByteToChar(unsigned char bytes[]) {
  char hexValue;
  char charValue;

  hexValue = (unsigned char)((bytes[0] & 0xF0)) >> 4;   // Get 4 bits of byte
  charValue = cByteToHex(hexValue);         			// Convert byte to char
  while(!(IFG2 & UCA0TXIFG));
  UCA0TXBUF = charValue;	            				// write string to transfer buffer
  while(UCA0STAT & UCBUSY);                 			// Wait until the last byte is completely sent

  hexValue = (unsigned char)(bytes[0] & 0x0F);  		// Get 4 bits of byte
  charValue = cByteToHex(hexValue);         			// Convert byte to char
  while(!(IFG2 & UCA0TXIFG));
  UCA0TXBUF = charValue;	            				// write string to transfer buffer
  while(UCA0STAT & UCBUSY);                 			// Wait until the last byte is completely sent
}


#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
	uart_rcv_buf[uart_buf_size++] = UCA0RXBUF;
	_NOP();
}



