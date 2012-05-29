/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "device.h"

#include <stdint.h>          /* For uint32_t definition */
#include <stdbool.h>         /* For true/false definition */

#include "system.h"          /* variables/params used by system.c */

/******************************************************************************/
/* System Level Functions                                                     */
/*                                                                            */
/* Custom oscillator configuration funtions, reset source evaluation          */
/* functions, and other non-peripheral microcontroller initialization         */
/* functions get placed in system.c                                           */
/*                                                                            */
/******************************************************************************/

/* Refer to the device Family Reference Manual Oscillator section for
information about available oscillator configurations.  Typically
this would involve configuring the oscillator tuning register or clock
switching useing the compiler's __builtin_write_OSCCON functions.
Refer to the C Compiler for PIC24 MCUs and dsPIC DSCs User Guide in the
compiler installation directory /doc folder for documentation on the
__builtin functions. */

_FOSCSEL(FNOSC_PRI);
_FOSC(POSCMOD_HS & POSCFREQ_MS & FCKSM_CSECMD);
_FWDT(FWDTEN_OFF);

/* TODO Add clock switching code if appropriate.  An example stub is below.   */
void ConfigureOscillator(void)
{
/*	asm(
		"mov #0x3, w0\n"		// PRI + PLL
		// Unlock OSCCONH
		"mov #OSCCONH, w1\n"
		"mov #0x78, w2\n"
		"mov #0x9A, w3\n"
		"mov.b w2, [w1]\n"
		"mov.b w3, [w1]\n"

		// Set NOSC
		"mov.b w0, [w1]\n"
		
		// Unlock OSCCONL
		"mov #OSCCONL, w1\n"
		"mov #0x46, w2\n"
		"mov #0x57, w3\n"
		"mov.b w2, [w1]\n"
		"mov.b w3, [w1]\n"

		// Start clock switch
		"bset OSCCONL, #0\n"
		:
		:
		:"w0", "w1", "w2", "w3");

	while(!OSCCONbits.LOCK);	// Wait for lock	
*/
}

