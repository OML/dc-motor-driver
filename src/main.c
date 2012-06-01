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

//volatile uint16_t addr;
//volatile uint16_t source;

volatile uint8_t rx_buffer[128];
volatile uint8_t rx_current = 0;
volatile uint8_t rx_flags = 0;
volatile uint8_t rx_length;

volatile uint8_t tx_buffer[128];
volatile uint8_t tx_current = 0;
volatile uint8_t tx_size = 0;

void incoming_event(struct bus_descriptor* bus, char* data, size_t len)
{
    struct bus_event_hdr* evhdr = get_bus_event_header(data);
	struct bus_set_motor_driver* setmotor = get_bus_set_motor_driver(data);

    switch(evhdr->type) 
	{
		if(evhdr->type == EV_SET_THROTTLES)
		{	
			device.motor[0].throttle = abs(setmotor->motors[0]);
			if(setmotor->motors[0] > 0)
				device.motor[0].flags = MOT_FORWARD;
			else if(setmotor->motors[0] < 0)
				device.motor[0].flags = MOT_BACKWARD;
			else
				device.motor[0].flags = MOT_BRAKE;


				device.motor[1].throttle = abs(setmotor->motors[0]);
			if(setmotor->motors[1] > 0)
				device.motor[1].flags = MOT_FORWARD;
			else if(setmotor->motors[1] < 0)
				device.motor[1].flags = MOT_BACKWARD;
			else
				device.motor[1].flags = MOT_BRAKE;

			motor_set_power(1,setmotor->motors[0]/10);
			motor_set_power(2,setmotor->motors[1]/10);
		}
	}	
}

int16_t main(void)
{
        /* Configure the oscillator for the device */
        ConfigureOscillator();
        
        bus_init(1, DT_DUAL_MOTOR);

        init_motors();
    
        while(1)
        {
                update_motors();
                read_sensors();
                
                bus_do_work();
        }
}
