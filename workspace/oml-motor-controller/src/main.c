/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "device.h"

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */

#include "connectivity.h"
#include "motors.h"

static void init_interrupts(void)
{
//	IEC1bits.INT0IE = 0;
}

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

	init_interrupts();

    init_connectivity();
    //init_motors();
    
    while(1)
    {
        read_instructions();
        update_motors();
        read_sensors();
    }
}
