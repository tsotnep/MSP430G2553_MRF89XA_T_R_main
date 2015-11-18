/*
 * gpio.h
 *
 *  Created on: 06.04.2015
 *      Author: mairo
 */

#ifndef MSP430G2553_MRF89XA_T_R_MAIN_DRIVERS_GPIO_H_
#define MSP430G2553_MRF89XA_T_R_MAIN_DRIVERS_GPIO_H_

/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/

/******** MRF89XA *********/
#define RF_IRQ_PORT_DIR		(P2DIR)
#define RF_IRQ_PORT_IN		(P2IN)
#define RF_IRQ_PORT_REN		(P2REN)
#define RF_IRQ_PORT_IE		(P2IE)
#define RF_IRQ_PORT_IES		(P2IES)
#define RF_IRQ_PORT_IFG		(P2IFG)

#define RF_IRQ0_PIN			BIT0	// P2.0
#define RF_IRQ1_PIN			BIT1	// P2.1

#define RF_RESET_PORT_DIR	(P2DIR)
#define RF_RESET_PORT_OUT	(P2OUT)
#define RF_RESET_PIN		BIT5	// P2.5


/********** LED ***********/
#define LED_PORT_DIR      	(P1DIR)
#define LED_PORT_OUT       	(P1OUT)

#define LED1  	     		BIT0	// P1.0 (RED)
//#define LED2       		BIT6	// P1.6 (GREEN); used by SPI


/********* SPI ************/
#define SPI_PORT_DIR      	(P1DIR)
#define SPI_PORT_SEL       	(P1SEL)
#define SPI_PORT_SEL2       (P1SEL2)
#define SPI_PORT_OUT       	(P1OUT)

#define SPI_MISO_PIN       	BIT6	// P1.6
#define SPI_MOSI_PIN       	BIT7	// P1.7
#define SPI_CLK_PIN        	BIT5	// P1.5
#define SPI_CS_DATA_PIN    	BIT3	// P1.3
#define SPI_CS_CONF_PIN    	BIT4	// P1.4


/******** UART *************/
#define UART_PORT_DIR		(P1DIR)
#define UART_PORT_SEL      	(P1SEL)
#define UART_PORT_SEL2      (P1SEL2)

#define UART_RXD			BIT1	// P1.1
#define UART_TXD			BIT2	// P1.2


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/

#endif 
