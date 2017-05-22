// Author: Nathanael Park and Hayden Becker
// Scheduler header for ENCE361 Project 2

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

// Includes
#include "f_main.h"
#include "f_display.h"
#include "f_buttons.h"

// Function prototypes
uint16_t schedulerRun(void);
void schedulerTask(float*, float, float, uint16_t, uint16_t, float*, uint16_t, uint16_t, float*);

#endif /*SCHEDULER_H_*/
