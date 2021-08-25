/* Copyright 2016, Gerhard Bertelsmann
 *
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Siegfried Lohberg wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 */

/*
 *	  GPIO Usage
 *  J12 Pin3 	NC 	IO-7		PH5
 *  J12 Pin4 	RXD 	UART7_RX 	PI21
 *  J12 Pin5 	NC 	IO-8		PH3
 *  J12 Pin6 	TXD 	UART7_TX 	PI20
 */

#include "allwinner.h"

#define BPI_PH5		(7*32+5)
#define BPI_PI21	(8*32+21)
#define BPI_PH3		(7*32+3)
#define BPI_PI20	(8*32+20)

#define DATA_PIN	BPI_PI20
#define CLOCK_PIN	BPI_PH3
#define LOAD_PIN	BPI_PI21
#define RESET_PIN	BPI_PH5

