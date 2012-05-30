#ifndef _PROT_MOTCON_H
#define _PROT_MOTCON_H

#include "device.h"

/*
 * The Motor conctroller communication protocol
 *
 */

#include <stdint.h>

/* Values for motor_info.flags */
#define MOT_FORWARD         (1 << 0)
#define MOT_BACKWARD        (1 << 1)
#define MOT_BRAKE			(0)

struct motor
{
    uint16_t                flags;
    uint16_t                current;        /* milliamps */
    uint16_t                voltage;        /* millivolts */
    uint16_t                temp;           /* millidegrees */
    uint16_t                throttle;       /* promille */
	uint16_t				throttle_actual;
} __attribute__((packed));

struct motcon_buffer
{
    uint16_t            flags;
    struct motor        motor[N_MOTORS];
} __attribute__((packed));


#endif /* prot_motcon.h */
