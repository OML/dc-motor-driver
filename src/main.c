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
volatile uint16_t source;

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

    switch(evhdr->type) 
	{
		rx_buffer[rx_current] = U1RXREG;
		rx_current++;

		struct bus_hdr* header;
		struct bus_event_hdr* event_hdr;
		struct bus_set_motor_driver* set_driver;

		if(rx_current == 2)
		{
			memcpy((uint8_t*)&rx_length, (uint8_t*)(rx_buffer), 2); // put the first 2 bytes into rx_length
		}

		if(rx_current == rx_length)
		{
			header = (struct bus_hdr*)(rx_buffer + sizeof(uint16_t));
			if(header->opcode.op == BUSOP_HELLO)
			{
				memcpy((uint16_t*)&addr, (uint16_t*)header->daddr,2);
				memcpy((uint16_t*)&source, (uint16_t*)header->saddr,2);
			}

			if(header->opcode.op == BUSOP_EVENT && header->daddr == addr)
			{
				event_hdr = (struct bus_event_hdr*)((char*)header + sizeof(header));

				if(event_hdr->type == EV_SET_THROTTLES)
				{
					set_driver = (struct bus_set_motor_driver*)((char*)event_hdr + sizeof(event_hdr));
	
					device.motor[0].throttle = abs(set_driver->motors[0]);
					if(set_driver->motors[0] > 0)
						device.motor[0].flags = MOT_FORWARD;
					else if(set_driver->motors[0] < 0)
						device.motor[0].flags = MOT_BACKWARD;
					else
						device.motor[0].flags = MOT_BRAKE;


					device.motor[1].throttle = abs(set_driver->motors[0]);
					if(set_driver->motors[1] > 0)
						device.motor[1].flags = MOT_FORWARD;
					else if(set_driver->motors[1] < 0)
						device.motor[1].flags = MOT_BACKWARD;
					else
						device.motor[1].flags = MOT_BRAKE;

					motor_set_power(1,set_driver->motors[0]/10);
					motor_set_power(2,set_driver->motors[1]/10);
				}
			}

			if(header->opcode.op == BUSOP_DONE)
			{
				//nothing?
			}
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
