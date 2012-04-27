#ifndef _CONNECTIVITY_H
#define _CONNECTIVITY_H

#include "prot_motcon.h"

extern struct motcon_buffer buffer;

void init_connectivity(void);

void read_instructions(void);
void send_response(void);

#endif /* connectivity.h */
