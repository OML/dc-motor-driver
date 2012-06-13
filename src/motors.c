#include "motors.h"
#include "connectivity.h"

#include "device.h"


struct motcon_buffer device;

#define PWM_PERIOD 6400

#define THROTTLE_TDELTA 0xFFFF
#define THROTTLE_INCREMENT 4

int demo = 0;
#define DEMO_INCREMENT 1
int demo_up = 0;

void __attribute__((interrupt,auto_psv)) _T1Interrupt(void)
{
	struct motor* m;
    int i;

    for(i = 0; i < 2; i++) {
        m = &device.motor[i];
            if(m->throttle == 0)
              	m->throttle_actual = 0;
            else if(m->throttle_actual < m->throttle)
                m->throttle_actual += THROTTLE_INCREMENT;
            else if(m->throttle_actual > m->throttle)
              	m->throttle_actual -= THROTTLE_INCREMENT;
                }
        
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
        demo = 0;
        
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


void update_motors(void)
{
        motor_set_power(1, 100);//device.motor[0].throttle_actual/10);
		motor_set_power(2, 200);//device.motor[1].throttle_actual/10);
        device.motor[0].flags = MOT_FORWARD;
		device.motor[1].flags = MOT_FORWARD;

        struct motor* m;

        m = &device.motor[0];
        if(!m->flags & (MOT_FORWARD | MOT_BACKWARD))
                m->throttle_actual = 0;
        else {
                if(m->flags & MOT_FORWARD)
                        MOT1_FWD_PORT |= MOT1_FWD_PIN;
                else
                        MOT1_FWD_PORT &= ~MOT1_FWD_PIN;
                if(m->flags & MOT_BACKWARD)
                        MOT1_BACK_PORT |= MOT1_BACK_PIN;
                else
                        MOT1_BACK_PORT &= ~MOT1_BACK_PIN;

        }
        if(m->flags & MOT_BRAKE)
                m->throttle_actual = 0;

     


        m = &device.motor[1];

        if(!m->flags & (MOT_FORWARD | MOT_BACKWARD))
                 m->throttle_actual = 0;
        else {
                if(m->flags & MOT_FORWARD)
                        MOT2_FWD_PORT |= MOT2_FWD_PIN;
                else
                        MOT2_FWD_PORT &= ~MOT2_FWD_PIN;
        
                if(m->flags & MOT_BACKWARD)
                        MOT2_BACK_PORT |= MOT2_BACK_PIN;
                else
                        MOT2_BACK_PORT &= ~MOT2_BACK_PIN;
        }
        
        if(m->flags & MOT_BRAKE)
                m->throttle_actual = 0;



        

        motor_set_power(1, 50);//device.motor[0].throttle_actual/10);
		motor_set_power(2, 50);//device.motor[1].throttle_actual/10);
        device.motor[0].flags = MOT_FORWARD;
		device.motor[1].flags = MOT_FORWARD;
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
#warning "adval_to_temp not implemented"
    return val;
}

static uint16_t adval_to_current(uint16_t val)
{
        return ((uint32_t)(9900L*val))/0x3FFL;
}

static uint16_t adval_to_voltage(uint16_t val)
{
#warning "adval_to_voltage not implemented"
    return val;
}

void read_sensors(void)
{
    device.motor[0].temp = adval_to_temp(read_adc(MOT1_TEMP_CHAN));
    device.motor[0].current = adval_to_current(read_adc(MOT1_CURRENT_CHAN));
    device.motor[0].voltage = adval_to_voltage(read_adc(MOT1_VOLTAGE_CHAN));
    device.motor[1].temp = adval_to_temp(read_adc(MOT2_TEMP_CHAN));
    device.motor[1].current = adval_to_current(read_adc(MOT2_CURRENT_CHAN));
    device.motor[1].voltage = adval_to_voltage(read_adc(MOT2_VOLTAGE_CHAN));
}
