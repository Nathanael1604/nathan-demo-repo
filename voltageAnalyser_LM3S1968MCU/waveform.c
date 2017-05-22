// Author: Nathanael Park and Hayden Becker
// Waveform Module for ENCE361 Project 1
// Finds the waveform type by calculating RMS voltages without DC offset
// for each waveform type then comparing to a manually calculated value

#include "waveform.h"

// Global variable declaration
uint16_t dcFlag = 1, waveFlag = DEFAULT_WAVE, currentSumEl = 0;
uint32_t sumNow = 0, sumFinal = 0, sampleCount = 0, sampleCountOut = 0;
uint32_t sampleRMS = 0, RMSSine = 0, RMSTri = 0, RMSSq = 0;

// Return the the type of waveform
uint16_t getWaveform(uint16_t peakToPeak, uint16_t* dataArray, uint16_t vAve, uint16_t peakCalib)
{
	// Checking for DC input
	if (peakToPeak <= 68) {
        dcFlag = 1;
    }
    else {
    	dcFlag = 0;
    }     	
    // Finding Area of the top half of the Wave
    uint16_t i = 0;
    for (i = 1; i <= DATA_BUFF_SIZE; i++)
    {
        currentSumEl = dataArray[i];
       	if (currentSumEl > vAve)
       	{
       		sumNow = sumNow + (currentSumEl - vAve);
        	sampleCount++;
        }
       	else 
       	{
       		sumNow = sumNow;
        }
		// Stop adding when the end of the array is reached and reset variables
        if (i == 100000 || i == 9999) {
        	sumFinal = sumNow;
        	sampleCountOut = sampleCount;
        	sumNow = 0;
        	sampleCount = 0;
        }
    }
    
    sampleRMS = sumFinal/sampleCountOut;
    // Calculating RMS values for each waveform
    RMSSine = (peakCalib*100)/isqrt(2*10000);
    RMSTri =  (peakCalib*100)/isqrt(3*10000);
    RMSSq = peakCalib;

    // Finding which waveform's RMS is close enough to the sample RMS and set that wave flag high
    if (dcFlag == 1)
    {
        waveFlag = DC_WAVE;
    }   	
    else if (((RMSSine - sampleRMS) <= TOLERANCE) || ((sampleRMS - RMSSine) <= TOLERANCE))
    {
    	waveFlag = SINE_WAVE;
    }
    else if (((RMSTri - sampleRMS) <= TOLERANCE) || ((sampleRMS - RMSTri) <= TOLERANCE))
    {
    	waveFlag = TRI_WAVE;
    }
    else if (((RMSSq - sampleRMS) <= TOLERANCE) || ((sampleRMS - RMSSq) <= TOLERANCE))
    {
    	waveFlag = SQ_WAVE;
    }
    else
    {
    	waveFlag = UNKNOWN_WAVE;
    }
    return waveFlag;
}
