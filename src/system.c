/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "device.h"

#include <stdbool.h>

#include "system.h"     
     
_FOSCSEL(FNOSC_PRIPLL);
_FOSC(POSCMOD_HS & POSCFREQ_MS & FCKSM_CSECMD);
_FWDT(FWDTEN_OFF);
_FPOR(MCLRE_OFF);

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

