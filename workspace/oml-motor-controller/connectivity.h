#ifndef _CONNECTIVITY_H
#define _CONNECTIVITY_H

#include <stdint.h>

/* Values for motor_info.flags */
#define MOT_FORWARD         (1 << 0)
#define MOT_BACKWARD        (1 << 1)

struct motor
{
    uint16_t                flags;
    uint16_t                current;        /* milliamps */
    uint16_t                voltage;        /* millivolts */
    uint16_t                temp;           /* millidegrees */
    uint16_t                throttle;       /* promille */
} __attribute__((packed));

struct
{
    union
    {
        unsigned char           b[];
        struct
        {
            uint16_t            flags;
            struct motor        motors[2];
        } __attribute__((packed));
    };
} buffer;

void init_buffer(void);

void read_instructions(void);

#endif /* connectivity.h */
