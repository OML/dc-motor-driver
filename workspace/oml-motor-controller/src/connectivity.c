#include "connectivity.h"

#include "device.h"

#include <string.h>
#include <stdlib.h>


struct motcon_buffer device;

static void eval_cmd(const char* c);

volatile uint8_t rx_buffer[128];
volatile uint8_t rx_current = 0;
volatile uint8_t rx_txt = 0;

volatile uint8_t tx_buffer[128];
volatile uint8_t tx_current = 0;

void init_connectivity(void)
{
    memset(&device, 0, sizeof(struct motcon_buffer));

	TRISBbits.TRISB2 = 1;
	ANSA = ANSB = 0;

	TRISBbits.TRISB7 = 0;

	U1BRG = 51;
	U1MODE = 0x8000;
	U1STA = 0x8400;


	IFS0bits.U1TXIF = 0;
	IPC3bits.U1TXIP = 0x1;		// Lowest
	IEC0bits.U1TXIE = 0;
	
	IFS0bits.U1RXIF = 0;
	IPC2bits.U1RXIP = 0x7;		// Highest
	IEC0bits.U1RXIE = 1;
}

inline void enable_tx_interrupt(int enable)
{
	IEC0bits.U1TXIE = (enable != 0) ? 1 : 0;
}

void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
{
	while(1);
}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
	rx_buffer[rx_current] = U1RXREG;
	rx_current++;	
	if(rx_current == 1) {
		if(rx_buffer[0] == 'U')
			rx_txt = 0; 
		else if(rx_buffer[0] >= 'a' && rx_buffer[0] < 'z')
			rx_txt = 1;
		else
			rx_current = 0;
	}
	if(rx_current >= 2 && rx_txt
		&& rx_buffer[rx_current-2] == 0x0D 
		&& rx_buffer[rx_current-1] == 0x0A) {
		rx_buffer[rx_current] = '\0';
		rx_current = 0;
		eval_cmd((char*)rx_buffer);
	}
	IFS0bits.U1RXIF = 0;
}

static void write_byte(uint8_t b)
{
	while(U1STAbits.UTXBF == 1);
	U1TXREG = b;
}

uint8_t tmp;
void read_instructions(void)
{
	while(1);
}



static void eval_cmd(const char* c)
{
}

