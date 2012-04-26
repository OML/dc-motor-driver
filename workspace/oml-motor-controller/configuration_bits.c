/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "device.h"

/******************************************************************************/
/* Configuration Bits                                                         */
/*                                                                            */
/* This is not all available configuration bits for all PIC24 devices.        */
/* Refer to the PIC24 device specific .h file in the compiler                 */
/* support\PIC24x\h (x=F,H,E) directory for complete options specific to the  */
/* selected device.  For additional information about what the hardware       */
/* configurations mean in terms of device operation, refer to the device      */
/* datasheet 'Special Features' chapter.                                      */
/*                                                                            */
/******************************************************************************/

/* TODO Fill in your configuration bits here.  The general style is below:    */

_FOSCSEL(FNOSC_PRI);
_FOSC(POSCMOD_HS)
_FWDT(FWDTEN_OFF);

#if 0

/* Selects internal oscillator with no switching */
_FOSCSEL(FNOSC_FRC & IESO_OFF);

/* Disables clock switching and selects pri osc of HS with OSCIO clock output */
_FOSC(FCKSM_CSECME & OSCIOFNC_ON & POSCMD_HS);

/* Turns off JTAG and selects debug channel */
_FICD(JTAGEN_OFF & ICS_PGD1);

#endif
