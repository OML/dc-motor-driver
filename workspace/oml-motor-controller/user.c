/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "device.h"

#include <stdint.h>          /* For uint32_t definition */
#include <stdbool.h>         /* For true/false definition */

#include "user.h"            /* variables/params used by user.c */

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

/* TODO Initialize User Ports/Peripherals/Project here */

void InitApp(void)
{
    /* Setup analog functionality and port direction */

    /* Initialize peripherals */

    // Configure UART1
    U1MODE = 0;
    U1BRG = (((FOSC/2) / BAUD)/6)-1;
    U1MODEbits.ABAUD = 1;

    U1STA = 0;
    U1STAbits.URXISEL = 0b00;   // per-byte interrupt

    U1MODEbits.UARTEN = 1;

    U1STAbits.UTXEN = 1;
}

