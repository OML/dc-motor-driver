#include "connectivity.h"

#include "busprot.h"
#include "device.h"
#include "motors.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>



#define RX_DONE (1 << 0)
#define RX_TEXT (1 << 1)

volatile uint16_t addr;
volatile uint16_t source;

volatile uint8_t rx_buffer[128];
volatile uint8_t rx_current = 0;
volatile uint8_t rx_flags = 0;
volatile uint8_t rx_length;

volatile uint8_t tx_buffer[128];
volatile uint8_t tx_current = 0;
volatile uint8_t tx_size = 0;

void init_connectivity(void)
{
        memset(&device, 0, sizeof(struct motcon_buffer));

	TRISBbits.TRISB2 = 1;
	ANSBbits.ANSB2 = 0;

	TRISBbits.TRISB7 = 0;

	U1BRG = 51;
	U1MODE = 0x8000;
	U1STA = 0x0400;

	IFS0bits.U1RXIF = 0;
	IPC2bits.U1RXIP = 0x7;		// Highest
	IEC0bits.U1RXIE = 1;
}


void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
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
	


	/*if(rx_current == 1) {
		if(rx_buffer[0] == 'U')
                        rx_flags &= ~RX_TEXT;
		else if(rx_buffer[0] >= 'a' && rx_buffer[0] < 'z')
			rx_flags |= RX_TEXT;
		else
			rx_current = 0;
	}*/

	if(rx_current >= 2 && rx_flags & RX_TEXT
		&& rx_buffer[rx_current-2] == 0x0D 
		&& rx_buffer[rx_current-1] == 0x0A) {
		rx_buffer[rx_current] = '\0';
		rx_current = 0;
		rx_flags |= RX_DONE;
	}
	IFS0bits.U1RXIF = 0;
}

void transmit(const char* data, unsigned int len)
{
        if(len == 0)
                len = strlen(data);

	memcpy((char*)tx_buffer, data, len);
	tx_size = len;
	tx_current = 0;
        
        while(tx_current < tx_size) {
                while(!U1STAbits.TRMT);
                U1TXREG = tx_buffer[tx_current++];
        }
}



void do_communication_tasks(void)
{

}
