// Author: Nathanael Park and Hayden Becker
// f_ Brakes Header for ENCE361 Project 2

#ifndef F_BRAKES_H_
#define F_BRAKES_H_

// Includes
#include "f_main.h"

// Function prototypes
void PortBIntHandler();
void changeBrakeDuty();
void checkBrakeNoSignal();
uint16_t getBrakeStatus();
uint16_t getBrakeInputHigh();


#endif /* F_BRAKES_H_ */
