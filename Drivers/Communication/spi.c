/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include "system.h"
#include "spi.h"

/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
unsigned char dummy;

// *************************************************************************************************
// @fn          Init_SPI
// @brief       Initialize UART
//				Pinout: 	CLK  = P1.5 (OUT)
//							SOMI = P1.6 (IN)
//							SIMO = P1.7 (OUT)
// 							CS   = P1.3 (OUT)
// @param       none
// @return      none
// *************************************************************************************************
void SPI_Init(void) {
	// Configure SPI pins
	SPI_SEL();
	SPI_SEL2();
	SPI_CLK_OUT();
	SPI_MOSI_OUT();
	SPI_MISO_IN();

	SPI_CS_DATA_OUT();
	SPI_CS_DATA_HI();
	SPI_CS_CONF_OUT();
	SPI_CS_CONF_HI();


	// Configure SPI
	UCB0CTL1 = UCSWRST;										// Enable SW reset
	UCB0CTL0 |= UCMODE_0 | UCMSB | UCSYNC | UCMST | UCCKPH ;// 4-pin, 8-bit SPI master
	UCB0CTL1 |= UCSSEL_2;                    	 			// SMCLK
	UCB0BR0 |= SYSTEM_SPEED_MHZ;              				// Divider: SMCLK/SYS_SPEED = 1MHz
	UCB0BR1 = 0;                              				//
	UCB0CTL1 &= ~UCSWRST;                   				// **Initialize USCI state machine**
	IE2 |= 0;                          						// Disable USCI0 interrupts
}


// *************************************************************************************************
// @fn          Spi_Send
// @brief       Send data over SPI
// @param       const unsigned char _data
// @return      unsigned char
// *************************************************************************************************
unsigned char Spi_Send(const unsigned char _data) {
	UCB0TXBUF = _data; 				// setting TXBUF clears the TXIFG flag
	while (!(IFG2 & UCB0TXIFG));
	// Add some delay for MRF89XA
	if (SYSTEM_SPEED_MHZ > SYSTEM_SPEED_1MHZ)
		__delay_cycles(120);			// Minimum req. delay for MRF89 module is 100 cycles
	else
		__delay_cycles(10);
	return UCB0RXBUF; 				// reading clears RXIFG flag
}


// *************************************************************************************************
// @fn          SPI_Conf_Read_Register
// @brief       Initialize system
// @param       none
// @return      none
// *************************************************************************************************
unsigned char SPI_Conf_Read_Register(unsigned char address) {
	unsigned char Rx_buf;

	SPI_CS_CONF_LO();

	dummy = Spi_Send((address << 1) | SPI_READ_CMD);
	Rx_buf = Spi_Send(0x00);

	SPI_CS_CONF_HI();

	return Rx_buf;
}


// *************************************************************************************************
// @fn          SPI_Data_Read_Register
// @brief       Initialize system
// @param       unsigned char data
// @return      none
// *************************************************************************************************
unsigned char SPI_Data_Read(void) {
	unsigned char Rx_buf;

	SPI_CS_DATA_LO();
	Rx_buf = Spi_Send(0x00);
	SPI_CS_DATA_HI();

	return Rx_buf;
}


// *************************************************************************************************
// @fn          SPI_Conf_Write_Register
// @brief       Write to the register
// @param       uint8 address       Register address
//              uint8 data          Configuration data
// @return      uint8 Rx_buf        Result of configuration
// *************************************************************************************************
unsigned char SPI_Conf_Write_Register(unsigned char address, unsigned char data) {
	unsigned char Rx_buf;

	SPI_CS_CONF_LO();

	dummy = Spi_Send((address << 1) | SPI_WRITE_CMD);
	Rx_buf = Spi_Send(data);

	SPI_CS_CONF_HI();

	return Rx_buf;
}


// *************************************************************************************************
// @fn          SPI_Data_Write_Register
// @brief       Write to the register
// @param       uint8 data          Configuration data
// @return      uint8 Rx_buf        Result of configuration
// *************************************************************************************************
unsigned char SPI_Data_Write(unsigned char *packet, unsigned int len) {
	unsigned char Rx_buf;

	while (len-- > 0) {
		SPI_CS_DATA_LO();
		Rx_buf = Spi_Send(*packet++);
		SPI_CS_DATA_HI();
	}

	return Rx_buf;
}
