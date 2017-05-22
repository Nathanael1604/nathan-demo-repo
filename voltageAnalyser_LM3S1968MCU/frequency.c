// Author: Nathanael Park and Hayden Becker
// Frequency Module for ENCE361 Project 1
// Calculates the frequency of the input wave using Hysteresis

#include "frequency.h"

// Global variable declaration
uint16_t frequency = 0;

uint16_t getFrequency(uint16_t* dataArray, uint16_t vAve)
{
	uint16_t stateCount = 0, waveInternalState = 0;
	uint16_t i = 0;
	// Count how many state changes there are in dataArray
	for (i = 1; i <= DATA_BUFF_SIZE; i++)
	{
		//If the value goes over vAve + (vAve/2), then the state is changed
		if ((waveInternalState == 0) && (dataArray[i] > (vAve + (vAve / 10))))
		{
			waveInternalState = 1;
		}
		// If the value goes under vAve - (vAve/2), then the state is changed
        else if ((waveInternalState == 1) && (dataArray[i] < (vAve - (vAve / 10))))
        {
        	stateCount++;
        	waveInternalState = 0;
        }
	}
	// The frequency is found by multiplying the number of
	// state changes by an empirical proportional constant
	frequency = (stateCount * 1247 / 250);
	return frequency;
}
