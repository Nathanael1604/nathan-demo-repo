// Author: Nathanael Park and Hayden Becker
// VRMS Module for ENCE361 Project 1
// Calculates the Root Mean Square voltage

#include "VRMS.h"

// Global variable declaration
uint16_t currentEl = 0;
uint32_t integralNow = 0, integralFinal = 0, sampleCounts = 0, sampleCountsOut = 0;
uint32_t sampRMS = 0;

// Return the Root Mean Square voltage
uint16_t getVRMS (uint16_t* dataArray) {
	uint16_t i = 0;
	for (i = 1; i <= DATA_BUFF_SIZE; i++) {
		currentEl = dataArray[i];
		integralNow = integralNow + (currentEl * currentEl);
		sampleCounts++;
		// Stop adding when the end of the array is reached and reset variables
		if (i == 10001 || i == 10000) {
			integralFinal = integralNow;
			sampleCountsOut = sampleCounts;
			integralNow = 0;
			sampleCounts = 0;
		}
	}
	sampRMS = isqrt(integralFinal/sampleCountsOut);
	return sampRMS;
}

