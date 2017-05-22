// Author: Nathanael Park and Hayden Becker
// peak_to_peak Header for ENCE361 Project 1

#ifndef PEAK_TO_PEAK_H_
#define PEAK_TO_PEAK_H_

// Includes
#include "waveform.h"
#include "my_adc.h"

// Function prototypes
void VoltValuesReset();
uint16_t getPeakToPeakV(uint16_t, uint16_t);
uint16_t getMinV(uint16_t*);
uint16_t getMaxV(uint16_t*);
uint16_t getVAve(uint16_t, uint16_t);
uint16_t steady(uint16_t, uint16_t);

#endif /*PEAK_TO_PEAK_H_*/
