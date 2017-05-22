// Author: Nathanael Park and Hayden Becker
// Steering/Wheels Header for ENCE361 Project 2
// Module to control functions regarding the steering wheel and wheels


#ifndef F_STEERINGWHEELS_H_
#define F_STEERINGWHEELS_H_

// Includes
#include "f_main.h"
#include <math.h>

// Function prototypes
void GPIOInputsInit(void);
void PortFIntHandler(void);
void calcExpectedSpeeds(void);
float* getRadius(void);
float* getExpectedSpeeds(void);
float getAngle(void);
float getRadians(void);
uint16_t getSteeringStatus (void);
uint16_t getSteeringInputHigh(void);

// Constant definitions
#define EXPECTED_CALC_TOLERANCE 2
#define ANGLE_MAX 29.1
#define ANGLE_MAX_RAD 0.50789
#define pi 3.14


#endif /* F_STEERINGWHEELS_H_ */
