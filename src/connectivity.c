#include "connectivity.h"

#include "device.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "motors.h"

#define RX_DONE (1 << 0)
#define RX_TEXT (1 << 1)

static void eval_cmd(const char* c);

volatile uint8_t rx_buffer[128];
volatile uint8_t rx_current = 0;
volatile uint8_t rx_flags = 0;

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
	if(rx_current == 1) {
		if(rx_buffer[0] == 'U')
                        rx_flags &= ~RX_TEXT;
		else if(rx_buffer[0] >= 'a' && rx_buffer[0] < 'z')
			rx_flags |= RX_TEXT;
		else
			rx_current = 0;
	}
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




inline const char* skip_space(const char* c)
{
	while(isspace(*c))
		c++;
	return c;
}

enum {
        MOTCON,
        MOTOR
};

enum {
        OP_WRITE,
        OP_READ
};

static void eval_cmd(const char* cmd)
{
	unsigned int len = strlen(cmd);
	const char* pos = cmd;
        const char* start;
	const char* end = (char*)(cmd + len);
        int level = MOTCON;
        int operation = OP_READ;

        if(strncmp(cmd, "demo", 4) == 0) {
                demo = 1;     
                return;
        }
        if(strncmp(cmd, "stop", 4) == 0) {
                demo = 0;
                return;
        }

        uint8_t* property = (uint8_t*)&device;
        
        while(pos < end) {
                pos = start = skip_space(pos);
                while(pos < end && isalnum(*pos))
                        pos++;
                       
                if(*start == '=') {
                        operation = OP_WRITE;
                        pos++;
                        break;
                }


                if(strncmp(start, "flags", pos-start) == 0) {
                        switch(level) {
                                case MOTCON:
                                        property += offsetof(struct motcon_buffer, flags);
                                        break;
                                case MOTOR:
                                        property += offsetof(struct motor, flags);
                                        break;
                        }
                } else if(strncmp(start, "motor", pos-start) == 0) {
                        int index;

                        if(level != MOTCON) {
                                transmit("Structure parsing error\n", 0);
                                return;
                        }
                        property += offsetof(struct motcon_buffer, motor);
                        start = pos = skip_space(pos);
                        if(*pos != '[') {
                                transmit("Syntactic error, expected `[`\n", 0);
                                return;
                        }
                        start = ++pos;
                        
                        index = strtol(start, (char**)&pos, 10);
                        if(index >= N_MOTORS) {
                                transmit("Invalid motor number.\n", 0);
                                return;
                        }
                     
                        if(*pos != ']') {
                                transmit("Syntactic error, expected `]`.", 0);
                                return;
                        }
                     
                        property += index * sizeof(struct motor);
                        pos++;


                        start = pos = skip_space(pos);
                        if(*pos != '.') {
                                transmit("Syntactic error, expected `.`\n", 0);
                                return;
                        }
                        pos++;
                } else if(strncmp(start, "throttle", pos-start) == 0) {
                        property += offsetof(struct motor, throttle);
                } else if(strncmp(start, "throttle_actual", pos-start) == 0) {
                        property += offsetof(struct motor, throttle_actual);
                } else if(strncmp(start, "current", pos-start) == 0) {
                        property += offsetof(struct motor, current);
                } else if(strncmp(start, "voltage", pos-start) == 0) {
                        property += offsetof(struct motor, voltage);
                } else if(strncmp(start, "temp", pos-start) == 0) {
                        property += offsetof(struct motor, temp);
                } else {
                        transmit("Unknown property.\n", 0);
                        transmit(start, 5);
                        transmit("\n", 1);
                        return;
                }
        }
        
        if(operation == OP_WRITE) {
                start = pos = skip_space(pos);

                if(isdigit(*pos)) {
                        __builtin_nop();
                        *((uint16_t*)property) = strtol(start, (char**)&pos, 10);
                } else {
                        while(pos < end && isalnum(*pos))
                                pos++;
                        if(strncmp(start, "forward", pos-start) == 0)
                                *((uint16_t*)property) = MOT_FORWARD;
                        else if(strncmp(start, "backward", pos-start) == 0) 
                                *((uint16_t*)property) = MOT_BACKWARD;
                        else if(strncmp(start, "brake", pos-start) == 0) 
                                *((uint16_t*)property) = MOT_BRAKE;
                        else {
                                transmit("Invalid value\n", 0);
                                return;
                        }
                }
                transmit("Ok\n", 0);
        } else {
                char response[16];
                uint16_t val = *((uint16_t*)property);
                sprintf(response, "%u\n", val);
                transmit(response, 0);
        }
}


void do_communication_tasks(void)
{
        if(rx_flags & RX_DONE && rx_flags & RX_TEXT) {
                eval_cmd((char*)rx_buffer);
                rx_flags &= ~RX_DONE;
        }
}
