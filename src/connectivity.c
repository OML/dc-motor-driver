#include "connectivity.h"

#include "device.h"
#include "motors.h"

#include <string.h>

#include <bus.h>

struct uart_descriptor* main_uart;

void uart_init(struct uart_descriptor* uart, size_t uid)
{
        main_uart = uart;
        memset(&device, 0, sizeof(struct motcon_buffer));

	TRISBbits.TRISB2 = 1;
	ANSBbits.ANSB2 = 0;

	TRISBbits.TRISB7 = 0;

	U1BRG = 25;
	U1MODE = 0x8000;
	U1STA = 0x0400;

	IFS0bits.U1RXIF = 0;
	IPC2bits.U1RXIP = 0x7;		// Highest
	IEC0bits.U1RXIE = 1;

        uart->rxreg = &U1RXREG;
        uart->txreg = &U1TXREG;
        uart->stareg = &U1STA;
}


void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
        uart_has_byte_available(main_uart);
	IFS0bits.U1RXIF = 0;
}
