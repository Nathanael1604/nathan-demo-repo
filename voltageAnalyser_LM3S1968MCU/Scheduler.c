// Author: Nathanael Park and Hayden Becker
// Scheduler Module for ENCE361 Project 1
// Controller to handle events and display

#include "Scheduler.h"

// Global variable declaration
uint16_t buttonEvent = 0;

// Starting the program
void schedulerStart()
{
	displayStartup();
}
// Running state; get button input and display the menu
uint16_t schedulerRun()
{
	buttonEvent = getButtonEvent(buttonEvent);
	displayMenu();
	return buttonEvent;
}

// Scheduling the tasks to perform and menu to display depending on button events,
// also making sure the correct information is sent to the display functions
void schedulerTask(uint16_t vrmsVal1, uint16_t peakToPeakSteadied1, uint16_t frequencyVal1, uint16_t waveFlag1,
		uint16_t vrmsVal2, uint16_t peakToPeakSteadied2, uint16_t frequencyVal2, uint16_t waveFlag2, uint16_t MSEVal)
{
	displayClear();
	// The menu is always displayed regardless of task
	displayMenu();

	if (buttonEvent == 1)
    {
     	displayVrms(vrmsVal1, vrmsVal2, waveFlag1, waveFlag2);
    }
    else if (buttonEvent == 2)
    {
    	displayP2P(peakToPeakSteadied1, peakToPeakSteadied2, waveFlag1, waveFlag2);
    }
    else if (buttonEvent == 3)
    {
    	displayFrequency(frequencyVal1, frequencyVal2, waveFlag1, waveFlag2);
    }
    else if (buttonEvent == 4)
    {
    	displayMSE(MSEVal, waveFlag1, waveFlag2);
    }
}
