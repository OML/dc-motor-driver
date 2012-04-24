/*
===============================================================================
 Name        : main.c
 Author      : Wouter Dijkstra & Leon Colijn
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

#include <stdio.h>


void init(void)
{
	/*
	 * Initialize clock
	 */
	LPC_SYSCON->SYSPLLCLKSEL = 0x01;	/* External clock  */
	LPC_SYSCON->SYSPLLCLKUEN = 0;
	LPC_SYSCON->SYSPLLCLKUEN = 0x01;	/* Update PLL clock source  */

	LPC_SYSCON->SYSPLLCTRL = 0x01;		/* Feedback divider = 2,
										   Post divider = 1  */
	while(LPC_SYSCON->SYSPLLSTAT == 0);	/* Wait for PLL lock  */

	LPC_SYSCON->MAINCLKSEL = 0x3;		/* PLL  */
	LPC_SYSCON->MAINCLKUEN = 0;
	LPC_SYSCON->MAINCLKUEN = 0x01;		/* Update clock source  */

	/*
	 * Initialize UART
	 * 	First configure the multiplexer before enabling the clock.
	 */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16); /* Enable IO configuration block */
	LPC_IOCON->PIO2_7 = 0x02 | (0x01 << 4); /* PIO2.7 = RXD with
											   pull-down resistor */
	LPC_IOCON->PIO2_8 = 0x02 | (0x01 << 4); /* PIO2.8 = TXD with
											   pull-down resistor */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12);	/* Enable clock  */
	LPC_SYSCON->UARTCLKDIV = 1;


}



int main(void) {
	init();
	printf("Hello World\n");
	
	// Enter an infinite loop, just incrementing a counter
	volatile static int i = 0 ;
	while(1) {
		i++ ;
	}
	return 0 ;
}
