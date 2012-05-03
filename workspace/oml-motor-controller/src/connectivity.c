#include "connectivity.h"

#include "device.h"

#include <string.h>

struct motcon_buffer buffer;

void init_connectivity(void)
{
    memset(&buffer, 0, sizeof(struct motcon_buffer));

    U1MODE = 0;
    U1BRG = (((FOSC/2) / BAUD)/6)-1;
    U1MODEbits.ABAUD = 1;

    U1STA = 0;
    U1STAbits.URXISEL = 0b00;   // per-byte interrupt

    U1MODEbits.UARTEN = 1;

    U1STAbits.UTXEN = 1;

}

void read_instructions(void)
{
    int pos = 0;

    do {
        while(!U1STAbits.URXDA);
    } while(U1RXREG != 0x55);

    for(pos = 0; pos < sizeof(struct motcon_buffer); pos++) {
        while(!U1STAbits.URXDA);
        ((char*)(&buffer))[pos] = U1RXREG;
    }
}

void send_response(void)
{
    
}
