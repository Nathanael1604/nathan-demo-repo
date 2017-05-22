// Author: Nathanael Park and Hayden Becker
// peak_to_peak Module for ENCE361 Project 1
// Calculates the maximum, minimum, average
// and peak-to-peak voltage values

#include "peak_to_peak.h"

// Global variable declaration
uint16_t vAve = 0, peakToPeak = 0;

// Checking for minor changes in the voltage to avoid printing
// fluctuating values on the display
uint16_t steady(uint16_t realtimeValue, uint16_t displayValue)
{
	if ((realtimeValue > (displayValue + TOLERANCE)) ||
		 (realtimeValue < (displayValue - TOLERANCE)))
	{
		displayValue = realtimeValue;
	}
	return displayValue;
}

// Calculates the maximum voltage value stored in the data buffer
uint16_t getMaxV(uint16_t *dataArray)
{
	uint16_t i = 0;
	uint16_t maxV = 0, prevMaxV = 0;
	for (i = 0; i < DATA_BUFF_SIZE; i++)
	{
		prevMaxV = maxV;
    	if (dataArray[i] > (prevMaxV + TOLERANCE))
       	{
       		maxV = dataArray[i];
       	}
       	else {
       		maxV = prevMaxV;
       	}
	}
    return maxV;
}

// Calculates the minimum voltage
uint16_t getMinV(uint16_t *dataArray)
{
    uint16_t i = 0;
    uint16_t minV = 1023, prevMinV = 1023;
	for (i = 0; i < DATA_BUFF_SIZE; i++)
	{
		prevMinV = minV;
    	if ((dataArray[i] < (prevMinV - TOLERANCE)) && (dataArray[i] >= TOLERANCE))
       	{
       		minV = dataArray[i];
       	}
       	else {
       		minV = prevMinV;
       	}
       		
	}
   	return minV;
}



// Calculate the peak to peak voltage by using the maxV and minV values
uint16_t getPeakToPeakV(uint16_t maxV, uint16_t minV)
{
    peakToPeak = maxV - minV;  //maxV - minV;
    return peakToPeak;
}

// Calculates the average voltage
uint16_t getVAve(uint16_t peakToPeak, uint16_t minV)
{
	vAve = (peakToPeak/2) + minV;
	return vAve;
}
