// Author: Nathanael Park and Hayden Becker
// f_sysTick Header for ENCE361 Project 2

#ifndef SYSTICK_H_
#define SYSTICK_H_

// Includes
#include "f_main.h"

// Function prototypes
void myTimerInit(void);
void myTimerIntHandler(void);
void sysTickInit(void);
void sysTickIntHandler(void);
uint16_t getSysTickCounter(void);
float* getWheelSpeeds(void);

// Constant definitions
#define SYSTICK_RATE_HZ 100000ul
#define PULSE_DELAY 20000
#define pi 3.14

#endif
