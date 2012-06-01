/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "device.h"

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */

#include "connectivity.h"
#include "motors.h"

#include <bus.h>
#include <offsets.h>

void incoming_event(struct bus_descriptor* bus, char* data, size_t len)
{
        struct bus_event_hdr* evhdr = get_bus_event_header(data);
        switch(evhdr->type) {
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
