#ifndef _CONNECTIVITY_H
#define _CONNECTIVITY_H

#include "prot_motcon.h"
#include <bus.h>

extern volatile uint8_t buffer[];

void init_connectivity(void);

void do_communication_tasks(void);

extern struct uart_descriptor* main_uart;

#endif /* connectivity.h */
