#include "motors.h"
#include "connectivity.h"

#include "device.h"

static void init_adc(void)
{
    // Configure ports for analog input
    ANSA = 0;
    ANSB =    (1 << MOT1_TEMP_PIN) | (1 << MOT1_CURRENT_PIN)
            | (1 << MOT1_VOLTAGE_PIN)
            | (1 << MOT2_TEMP_PIN) | (1 << MOT2_CURRENT_PIN)
            | (1 << MOT2_VOLTAGE_PIN);

    AD1CON1 = 0;
    AD1CON1bits.SSRC = 0b0111;  // Start conversion after sampling
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CON3bits.SAMC = 16;      // Sample time = 16 * Tad
    AD1CON3bits.ADCS = 32-1;    // Tad = 32 * Tcy
    AD1CON5 = 0;

    AD1CHS = 0;

    AD1CON1bits.ADON = 1;       // Enable
}

void init_motors(void)
{
    init_adc();
}

void update_motors(void)
{
    
}

static uint16_t read_adc(int channel)
{
    AD1CHSbits.CH0SB = channel;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE);
    return *((&ADC1BUF0) + channel);
}




static uint16_t adval_to_temp(uint16_t val)
{
#warning "adval_to_temp not implemented"
    return val;
}

static uint16_t adval_to_current(uint16_t val)
{
#warning "adval_to_current not implemented"
    return val;
}

static uint16_t adval_to_voltage(uint16_t val)
{
#warning "adval_to_voltage not implemented"
    return val;
}

void read_sensors(void)
{
    buffer.motor[0].temp = adval_to_temp(read_adc(MOT1_TEMP_CHAN));
    buffer.motor[0].current = adval_to_current(read_adc(MOT1_CURRENT_CHAN));
    buffer.motor[0].voltage = adval_to_voltage(read_adc(MOT1_VOLTAGE_CHAN));
    buffer.motor[1].temp = adval_to_temp(read_adc(MOT2_TEMP_CHAN));
    buffer.motor[1].current = adval_to_current(read_adc(MOT2_CURRENT_CHAN));
    buffer.motor[1].voltage = adval_to_voltage(read_adc(MOT2_VOLTAGE_CHAN));
}
