#ifndef _DEVICE_H
#define _DEVICE_H

/* Device header file */
#if defined(__PIC24E__)
	#include <p24Exxxx.h>
#elif defined (__PIC24F__)
	#include <p24Fxxxx.h>
#elif defined(__PIC24H__)
	#include <p24Hxxxx.h>
#endif
#include <p24F16KA302.h>

#define BAUD 9600
#define FOSC 16000000L

#define N_MOTORS 2

#endif /* device.h */
