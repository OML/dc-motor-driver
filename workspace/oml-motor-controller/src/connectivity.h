#ifndef _CONNECTIVITY_H
#define _CONNECTIVITY_H

#include "prot_motcon.h"

extern struct motcon_buffer device;
extern volatile uint8_t buffer[];

void init_connectivity(void);

void read_instructions(void);

void send_buffer(void);

#endif /* connectivity.h */
