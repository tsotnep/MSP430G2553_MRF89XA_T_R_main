/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include <msp430.h>
#include "system.h"

/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/

#define SPI_SEL()      		{ SPI_PORT_SEL  |=  SPI_MISO_PIN | SPI_MOSI_PIN | SPI_CLK_PIN; }
#define SPI_SEL2()     		{ SPI_PORT_SEL2 |=  SPI_MISO_PIN | SPI_MOSI_PIN | SPI_CLK_PIN; }
#define SPI_CLK_OUT()  		{ SPI_PORT_DIR  |=  SPI_CLK_PIN; }
#define SPI_MOSI_OUT() 		{ SPI_PORT_DIR  |=  SPI_MOSI_PIN; }
#define SPI_MISO_IN()  		{ SPI_PORT_DIR  &= ~SPI_MISO_PIN; }

#define SPI_CS_DATA_OUT()   { SPI_PORT_DIR |=  SPI_CS_DATA_PIN; }
#define SPI_CS_DATA_HI()    { SPI_PORT_OUT |=  SPI_CS_DATA_PIN; }
#define SPI_CS_DATA_LO()    { SPI_PORT_OUT &= ~SPI_CS_DATA_PIN; }

#define SPI_CS_CONF_OUT()   { SPI_PORT_DIR |=  SPI_CS_CONF_PIN; }
#define SPI_CS_CONF_HI()    { SPI_PORT_OUT |=  SPI_CS_CONF_PIN; }
#define SPI_CS_CONF_LO()    { SPI_PORT_OUT &= ~SPI_CS_CONF_PIN; }

#define SPI_READ_CMD           0x40  // Read command (first bit is high)
#define SPI_WRITE_CMD          0x00  // Write command

/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
void SPI_Init(void);
unsigned char Spi_Send(const unsigned char _data);

// Functions for register configurations
unsigned char SPI_Conf_Read_Register(unsigned char address);
unsigned char SPI_Conf_Write_Register(unsigned char address, unsigned char data);

// Functiond data read or write data
unsigned char SPI_Data_Read(void);
unsigned char SPI_Data_Write(unsigned char *packet, unsigned int len);

/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/

