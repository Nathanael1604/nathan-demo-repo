// Author: Nathanael Park and Hayden Becker
// f_main Header for ENCE361 Project 2

#ifndef MAIN_H_
#define MAIN_H_

// Type definitions
#define uint16_t unsigned short
#define uint32_t unsigned long

// Includes
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/lm3s1968.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "drivers/rit128x96x4.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "stdio.h"
#include "ustdlib.h"
#include "sine.h"
#include <math.h>

// Include Modules
#include "f_buttons.h"
#include "f_display.h"
#include "f_sysTick.h"
#include "f_scheduler.h"
#include "f_systemABS.h"
#include "f_Brakes.h"
#include "f_steeringWheels.h"

// Function prototypes
void clockInit(void);
void main(void);


// Global (extern) variables
extern uint16_t steeringCounter;
extern uint16_t wheelCounter[];
extern uint16_t brakeCounter;
extern uint16_t brakeSignalCounter;
extern uint16_t slipping;

// Constant definitions
#define pi 3.14
#define AUDIO_FREQUENCY 196 // G one octave below middle
#define EXPECTED_CALC_TOLERANCE 2
#define ANGLE_MAX 29.1
#define ANGLE_MAX_RAD 0.50789

#endif
