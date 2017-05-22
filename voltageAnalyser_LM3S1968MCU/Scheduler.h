// Author: Nathanael Park and Hayden Becker
// Scheduler header for ENCE361 Project 1

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

// Includes
#include "my_adc.h"
#include "display.h"
#include "Buttons.h"
#include "peak_to_peak.h"
#include "frequency.h"

// Function prototypes
void schedulerStart();
uint16_t schedulerRun();
void schedulerTask(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);

#endif /*SCHEDULER_H_*/
