/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "device.h"

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <stdlib.h>
#include <string.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */

#include "connectivity.h"
#include "motors.h"

#include <bus.h>
#include <offsets.h>

void incoming_event(struct bus_descriptor* bus, char* data, size_t len)
{
        struct bus_event_hdr* evhdr = get_bus_event_header(data);
	struct bus_set_motor_driver* setmotor = get_bus_set_motor_driver(data);

        switch(evhdr->type) 
	{
		case EV_SET_THROTTLES:
		{	
			motor_set_power(1, abs(setmotor->motors[0]/10));
			if(setmotor->motors[0] > 0) {
				MOT1_FWD_LATCH |= MOT1_FWD_PIN;
				MOT1_BACK_LATCH &= ~MOT1_BACK_PIN;
			} else if(setmotor->motors[0] < 0) {
				MOT1_FWD_LATCH &= ~MOT1_FWD_PIN;
				MOT1_BACK_LATCH |= MOT1_BACK_PIN;
			} else {
				MOT1_FWD_LATCH |= MOT1_FWD_PIN;
				MOT1_BACK_LATCH |= MOT1_BACK_PIN;
			}

			motor_set_power(2, abs(setmotor->motors[1]/10));
			if(setmotor->motors[1] > 0) {
				MOT2_FWD_LATCH |= MOT2_FWD_PIN;
				MOT2_BACK_LATCH &= ~MOT2_BACK_PIN;
			} else if(setmotor->motors[1] < 0) {
				MOT2_FWD_LATCH &= ~MOT2_FWD_PIN;
				MOT2_BACK_LATCH |= MOT2_BACK_PIN;
			} else {
				MOT2_FWD_LATCH |= MOT2_FWD_PIN;
				MOT2_BACK_LATCH |= MOT2_BACK_PIN;
			}
                        break;
		}
	}	
}

int16_t main(void)
{
        /* Configure the oscillator for the device */
        ConfigureOscillator();

        clock_start();
        
        bus_init(1, DT_DUAL_MOTOR_BACK);

        init_motors();

        while(1)
        {	
                read_sensors();      
                bus_do_work();
        }
}
