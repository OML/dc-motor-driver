#ifndef _MOTORS_H
#define _MOTORS_H

#include <stdint.h>

#define MOT1_CURRENT_PIN    15
#define MOT1_VOLTAGE_PIN    14
#define MOT1_TEMP_PIN       3
#define MOT2_CURRENT_PIN    13
#define MOT2_VOLTAGE_PIN    12
#define MOT2_TEMP_PIN       4

#define MOT1_CURRENT_CHAN   9
#define MOT1_VOLTAGE_CHAN   15
#define MOT1_TEMP_CHAN      5
#define MOT2_CURRENT_CHAN   11
#define MOT2_VOLTAGE_CHAN   12
#define MOT2_TEMP_CHAN      15

void init_motors(void);

/*
 * motor = 1 | 2
 * value = 0..100 (%)
 */
void motor_set_power(int motor, uint8_t value);
void update_motors(void);


void read_sensors(void);

#endif /* motors.h */
