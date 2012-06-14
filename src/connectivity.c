#include "connectivity.h"

#include "device.h"
#include "motors.h"

#include <string.h>

#include <bus.h>

struct uart_descriptor* main_uart;
volatile uint8_t do_once = 0;

void uart_init(struct uart_descriptor* uart, size_t uid)
{
        main_uart = uart;

		TRISBbits.TRISB2 = 1;
		ANSBbits.ANSB2 = 0;

		TRISBbits.TRISB7 = 0;

		U1BRG = 25;
		U1MODE = 0x8000;
		U1STA = 0x0420;

		IFS0bits.U1RXIF = 0;
		IPC2bits.U1RXIP = 0x7;		// Highest
		IEC0bits.U1RXIE = 1;

        uart->rxreg = &U1RXREG;
        uart->txreg = &U1TXREG;
        uart->stareg = &U1STA;
}

volatile uint32_t prev = 0;
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
		if(rt_clock() - prev > 20) {
			main_uart->rx_ep.pos = 0;
			main_uart->rx_ep.len = 0;
		}		

		uart_has_byte_available(main_uart);
		prev = rt_clock();

		IFS0bits.U1RXIF = 0;
}
