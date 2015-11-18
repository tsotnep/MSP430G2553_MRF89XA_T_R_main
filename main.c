/*
 * main.c
 *
 *  Created on: Nov 18, 2015
 *      Author: Tsotne
 */

#include <msp430.h>
#include <../main_nwk_T.h>
#include <../main_nwk_R.h>

int main(void) {
	main_nwk_T();
	main_nwk_R();

	return 0;
}
