#include "motors.h"
#include "connectivity.h"

#include "device.h"

temperature_t temps[2];
voltage_t voltages[2];
current_t currents[2];

#define PWM_PERIOD 6400

#define THROTTLE_TDELTA 0xFFFF
#define THROTTLE_INCREMENT 4

void __attribute__((interrupt,auto_psv)) _T1Interrupt(void)
{
        IFS0bits.T1IF = 0;
}

static void init_adc(void)
{
        // Configure ports for analog input
        ANSA = 0;
        ANSB =    (1 << MOT1_TEMP_PIN) | (1 << MOT1_CURRENT_PIN)
                | (1 << MOT1_VOLTAGE_PIN)
                | (1 << MOT2_TEMP_PIN) | (1 << MOT2_CURRENT_PIN)
                | (1 << MOT2_VOLTAGE_PIN);


        AD1CON1 = 0x0070;
        AD1CON2 = 0x0800;
        AD1CON3 = 0x1F01;

        AD1CON1bits.ADON = 1;       // Enable
}

void init_pwm(void)
{
		// Configure PWM
		// OC1
		TRISAbits.TRISA6 = 0;
	
		OC1CON1bits.OCM = 0; 
		OC1CON1bits.OCTSEL = 0x7;	// System clock source
		OC1CON2bits.SYNCSEL = 0x1F;	// Sync source = self
	
		OC1R = 0;
		OC1RS = PWM_PERIOD;
	
		OC1CON1bits.OCM = 0x6;		// Enable

	
		// OC2
		TRISBbits.TRISB11 = 0;
		
		OC2CON1bits.OCM = 0;
		OC2CON1bits.OCTSEL = 0x7;	// Idem
		OC2CON2bits.SYNCSEL = 0x1F;	// Idem
	
		OC2R = 0;
		OC2RS = PWM_PERIOD;
	
		OC2CON1bits.OCM = 0x6;		// Enable
}

void init_throttle(void)
{
        IFS0bits.T1IF = 0;
        IEC0bits.T1IE = 1;
        
        PR1 = THROTTLE_TDELTA;
        T1CONbits.TON = 1;
}

void init_motors(void)
{
		init_adc();
        init_pwm();
        
        init_throttle();

        TRISAbits.TRISA7 = 0;
        TRISBbits.TRISB9 = 0;
        TRISBbits.TRISB8 = 0;
        TRISBbits.TRISB6 = 0;
}

void motor_set_power(int motor, uint8_t value)
{
	value = (value > 100) ? 100 : value;

	switch(motor) {
		case 1:
			OC2R = (value * PWM_PERIOD)/100;
			break;
		case 2:
			OC1R = (value * PWM_PERIOD)/100;
			break;
		default:
			break;
	}
}

static uint16_t read_adc(int channel)
{
    AD1CHSbits.CH0SA = channel;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE);
    return *((&ADC1BUF0) + channel);
}




static uint16_t adval_to_temp(uint16_t val)
{
        return ((uint32_t)((12900L*val)/10000L));
}

static uint16_t adval_to_current(uint16_t val)
{
        return ((uint32_t)(9900L*val))/0x3FFL;
}

static uint16_t adval_to_voltage(uint16_t val)
{

        return ((uint32_t)(val/31L))
}

void read_sensors(void)
{
    temps[0] = adval_to_temp(read_adc(MOT1_TEMP_CHAN));
    currents[0] = adval_to_current(read_adc(MOT1_CURRENT_CHAN));
    voltages[0] = adval_to_voltage(read_adc(MOT1_VOLTAGE_CHAN));
    temps[1] = adval_to_temp(read_adc(MOT2_TEMP_CHAN));
    currents[1] = adval_to_current(read_adc(MOT2_CURRENT_CHAN));
    voltages[1] = adval_to_voltage(read_adc(MOT2_VOLTAGE_CHAN));
}
